#include <linux/init.h>    //
#include <linux/module.h>  //
#include <linux/version.h> //
#include <linux/fs.h>      // for registering!
#include <linux/kernel.h>  //
#include <linux/device.h>  //
#include <linux/kdev_t.h>  // makros for major and minor number
#include <linux/types.h>   // defines dev_t
#include <linux/cdev.h>    //
#include <linux/wait.h>    //
#include <asm/uaccess.h>   // copy_to/from_user
#include <linux/sched.h>   // scheduling (waitqueue)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Froehlich");
MODULE_DESCRIPTION("a simple buffer driver");

#define DEV_NAME "buf"
#define DEV_CLASS "buf_class"
#define DEV_DRIVER "buf_driver"

static dev_t DEV_NUMBER; // var for first device number
static struct cdev c_dev; //global chardevice struct
static struct class *cl;	//global device class

#define BUFFER_LIMIT 100
#define BUFFER_SIZE 5
#define READ_POSSIBLE (pos > 0)
#define WRITE_POSSIBLE (pos < BUFFER_SIZE)

int pos =0; //buffer position
char k_buffer[BUFFER_SIZE][BUFFER_LIMIT];
static wait_queue_head_t write_queue;
static wait_queue_head_t read_queue;


static int buf_open(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: open()\n", DEV_DRIVER);
	return 0;
}


static int buf_close(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: close()\n", DEV_DRIVER);
	return 0;
}


static ssize_t buf_read(struct file *instance, char __user *buf, size_t len, loff_t *off)
{
	size_t to_copy, not_copied;
	printk(KERN_INFO "%s: read()\n", DEV_DRIVER);

	if(!READ_POSSIBLE && instance->f_flags&O_NONBLOCK)
	{
		printk(KERN_DEBUG "%s: some error occured -> EAGAIN\n", DEV_DRIVER);
		return -EAGAIN;
	} 
		
	if(wait_event_interruptible(read_queue, READ_POSSIBLE) )
	{
		printk(KERN_DEBUG "%s: some error occured -> ERESTART\n", DEV_DRIVER);
		return -ERESTART;
	}

	to_copy = min(len,(size_t)BUFFER_LIMIT-1);
	not_copied = copy_to_user(buf, k_buffer[--pos], to_copy);
	k_buffer[pos][to_copy]=(int)"/0";
	
	printk(KERN_DEBUG "%s: read.. pos=%d\n",DEV_DRIVER, pos);
	wake_up_interruptible(&write_queue);

	return (ssize_t)to_copy - not_copied;
}

static ssize_t buf_write(struct file *instance, const char __user *buf, size_t len, loff_t *off)
{
	size_t to_copy = len, not_copied = len;
	printk(KERN_INFO "%s: write()\n", DEV_DRIVER);

	if(!WRITE_POSSIBLE && instance->f_flags&O_NONBLOCK)
	{
		printk(KERN_DEBUG "%s: O_NONBLOCK -> EAGAIN\n", DEV_DRIVER);
		return -EAGAIN;
	}
		
	if(wait_event_interruptible(write_queue, WRITE_POSSIBLE)) 
	{
		printk(KERN_DEBUG "%s: wait_event interrupted -> ERESTART\n", DEV_DRIVER);
		return -ERESTART;
	}
	to_copy = min(len,(size_t)BUFFER_LIMIT-1);
	not_copied = copy_from_user(k_buffer[pos++], buf, to_copy);
	
	printk(KERN_DEBUG "%s: buffer=%s", DEV_DRIVER, (char *)buf);
	printk(KERN_DEBUG " pos=%d\n", pos);
	wake_up_interruptible(&read_queue);

	return (ssize_t)to_copy-not_copied;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = buf_open,
	.release = buf_close,
	.read = buf_read,
	.write = buf_write
};

int __init buf_init(void)
{
	printk(KERN_INFO "%s: init()\n", DEV_DRIVER);
	if (alloc_chrdev_region(&DEV_NUMBER,0,1, DEV_DRIVER) < 0) 
	{
		printk(KERN_ALERT "%s: reserving device number failed!\n", DEV_DRIVER);
		return -EIO;
	}
	if ((cl = class_create(THIS_MODULE, DEV_CLASS)) == NULL)
	{
		printk(KERN_ALERT "%s: creating device class failed", DEV_DRIVER);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	if (device_create(cl, NULL, DEV_NUMBER, NULL, DEV_NAME) == NULL)
	{
		printk(KERN_ALERT "%s: populating device class failed", DEV_DRIVER);
		class_destroy(cl);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, DEV_NUMBER, 1) == -1)
	{
		printk(KERN_ALERT "%s_driver: registering failed", DEV_DRIVER);
		device_destroy(cl, DEV_NUMBER);
		class_destroy(cl);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	init_waitqueue_head( &write_queue );
	init_waitqueue_head( &read_queue );
	
   printk(KERN_INFO "%s: registered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return 0;
}

void __exit buf_exit(void)
{
	printk(KERN_INFO "%s: exit()\n", DEV_DRIVER);
	device_destroy(cl, DEV_NUMBER);
	class_destroy(cl);
	cdev_del(&c_dev);
	unregister_chrdev_region(DEV_NUMBER, 1);
	printk(KERN_INFO "%s: deregistered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return;	
}



module_init(buf_init);
module_exit(buf_exit);

