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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Fröhlich");
MODULE_DESCRIPTION("My first driver... oh yeah");

const int BUFFER_LIMIT=1024;
char char_buffer[1024];
int pos =0;


static wait_queue_head_t wq; //queue for buffer

ssize_t buf_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "buf: Driver read()\n");
	if(wait_event_interruptible(wq,(pos>0)) ) 
		return -ERESTART;
	
	if(copy_to_user(buf, char_buffer, len))
		return -EFAULT;
	
	return len;
}

ssize_t buf_write(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	
	printk(KERN_INFO "buf: Driver write()\n");
	
	int nret = copy_from_user(char_buffer, buf, len);
	printk(KERN_DEBUG "buffer: %s", buf);
	pos +=len - nret;
	wake_up_interruptible(&wq);
		
	return len;
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
	init_waitqueue_head( &wq );
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

