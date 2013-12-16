#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/types.h> //defines dev_t
#include <linux/fs.h> // for registering!
#include <linux/kdev_t.h> //makros for major and minor number
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <asm/uaccess.h> //copy_to/from_user
#include <linux/sched.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Fröhlich");
MODULE_DESCRIPTION("My first driver... oh yeah");

#define BUFFER_LIMIT 1024
#define BUFFER_SIZE 5
#define READ_POSSIBLE (pos > 0)
#define WRITE_POSSIBLE (pos < BUFFER_SIZE)


char k_buffer[BUFFER_SIZE][BUFFER_LIMIT];
int pos =0; //buffer position


//queue for buffer
static wait_queue_head_t write_queue; 
static wait_queue_head_t read_queue; 


ssize_t buf_read(struct file *instance, char __user *buf, size_t len, loff_t *off)
{
	size_t to_copy, not_copied;
	printk(KERN_DEBUG "buf: Driver read()");

	if(!READ_POSSIBLE && instance->f_flags&O_NONBLOCK)
	{
		printk(KERN_DEBUG " -> Nonblock!\n");
		return -EAGAIN;
	} 
		
	if(wait_event_interruptible(read_queue, READ_POSSIBLE) )
	{
		printk(KERN_DEBUG " some error occured -> RESTART!\n");
		return -ERESTART;
	}

	to_copy = min(len,(size_t)BUFFER_LIMIT);
	not_copied = copy_to_user(buf, k_buffer[--pos], to_copy);
	printk(KERN_DEBUG "buf: read.. pos=%d\n", pos);
	wake_up_interruptible(&write_queue);

	return to_copy - not_copied;
}

ssize_t buf_write(struct file *instance, char __user *buf, size_t len, loff_t *off)
{
	size_t to_copy = len, not_copied = len;
	printk(KERN_DEBUG "buf: Driver write()");

	if(!WRITE_POSSIBLE && instance->f_flags&O_NONBLOCK) 
		return -EAGAIN;
	if(wait_event_interruptible(write_queue, WRITE_POSSIBLE)) 
		return -ERESTART;

	to_copy = min(len,(size_t)BUFFER_LIMIT);
	not_copied = copy_from_user(k_buffer[pos++], buf, to_copy);
	
	printk(KERN_DEBUG "buffer: %s", buf);
	printk(KERN_DEBUG " pos=%d\n", pos);
	wake_up_interruptible(&read_queue);

	return to_copy-not_copied;
}

int buf_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "buf: Driver open()\n");
	return 0;
}

int buf_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "buf: Driver close()\n");
	return 0;
}

static struct file_operations buf_fops =
{
	.owner = THIS_MODULE,
	.open = buf_open,
	.release = buf_close,
	.read = buf_read,
	.write = buf_write
};

static dev_t first; // var for first device number
static struct cdev c_dev; //global chardevice struct
static struct class *cl;	//global device class


int __init buf_init(void)
{
	if(alloc_chrdev_region(&first, 0, 3, "buf") < 0)
	{
		printk(KERN_ERR "buf: could not register driver!\n");
		return -1;
	}
	if ((cl = class_create(THIS_MODULE, "buf")) == NULL)
   {
    	unregister_chrdev_region(first, 1);
    	return -1;
   }
   if (device_create(cl, NULL, first, NULL, "buf") == NULL)
	{
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
   cdev_init(&c_dev, &buf_fops);
   if (cdev_add(&c_dev, first, 1) == -1)
   {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
   }
	init_waitqueue_head( &write_queue );
	init_waitqueue_head( &read_queue );
	
  	printk(KERN_INFO "buf <%d, %d>: driver registered\n", MAJOR(first), MINOR(first));
	return 0;
}

void __exit buf_exit(void)
{
	cdev_del(&c_dev);
  	device_destroy(cl, first);
  	class_destroy(cl);
  	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "buf: driver unregistered\n");
}



module_init(buf_init);
module_exit(buf_exit);

