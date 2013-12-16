#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/types.h> //defines dev_t
#include <linux/fs.h> // for registering!
#include <linux/kdev_t.h> //makros for major and minor number
#include <linux/device.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Fröhlich");
MODULE_DESCRIPTION("My first driver... oh yeah");

static dev_t first; // var for first device number
static struct cdev c_dev; //global chardevice struct
static struct class *cl;	//global device class


static int flosd_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "flosd: Driver open()\n");
	return 0;
}


static int flosd_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "flosd: Driver close()\n");
	return 0;
}


static int flosd_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "flosd: Driver read()\n");
	return 0;
}

static int flosd_write(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "flosd: Driver write()\n");
	return len;
}

static struct file_operations flosd_fops =
{
	.owner = THIS_MODULE,
	.open = flosd_open,
	.release = flosd_close,
	.read = flosd_read,
	.write = flosd_write
};

static int __init flosd_init(void)
{
	if(alloc_chrdev_region(&first, 0, 3, "flosd") < 0)
	{
		printk(KERN_ERR "flosd: could not register driver!\n");
		return -1;
	}
	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
   {
    	unregister_chrdev_region(first, 1);
    	return -1;
   }
   if (device_create(cl, NULL, first, NULL, "flosd") == NULL)
	{
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
   cdev_init(&c_dev, &flosd_fops);
   if (cdev_add(&c_dev, first, 1) == -1)
   {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
   }
  	printk(KERN_INFO "flosd <%d, %d>: driver registered\n", MAJOR(first), MINOR(first));
	return 0;
}

static void __exit flosd_exit(void)
{
	cdev_del(&c_dev);
  	device_destroy(cl, first);
  	class_destroy(cl);
  	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "flosd: driver unregistered\n");
}

module_init(flosd_init);
module_exit(flosd_exit);
