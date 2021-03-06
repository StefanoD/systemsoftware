#include <linux/init.h>    //
#include <linux/module.h>  //
#include <linux/version.h> //
#include <linux/fs.h>      // for registering!
#include <linux/kernel.h>  //
#include <linux/device.h>  //
#include <linux/kdev_t.h>  // makros for major and minor number
#include <linux/types.h>   // defines dev_t
#include <linux/cdev.h>    //


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Froehlich");
MODULE_DESCRIPTION("my first driver... oh yeah");

#define DEV_NAME "flosd"
#define DEV_CLASS_NAME "c_flosd"
#define DEV_DRIVER "flosd_driver"

static dev_t DEV_NUMBER;  // var for first device number
static struct cdev c_dev; // global chardevice struct
static struct class *cl;  // global device class


static int flosd_open(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: open()\n", DEV_DRIVER);
	return 0;
}


static int flosd_close(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: close()\n", DEV_DRIVER);
	return 0;
}


static ssize_t flosd_read(struct file *instance, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "%s: read()\n", DEV_DRIVER);
	return len;
}

static ssize_t flosd_write(struct file *instance, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "%s: write()\n", DEV_DRIVER);
	return len;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = flosd_open,
	.release = flosd_close,
	.read = flosd_read,
	.write = flosd_write	
};

static int __init flosd_init(void)
{
	printk(KERN_INFO "%s: init()\n", DEV_DRIVER);
	if (alloc_chrdev_region(&DEV_NUMBER,0,1, DEV_NAME) < 0) 
	{
		printk(KERN_ALERT "%s: reserving device number failed!\n", DEV_DRIVER);
		return -EIO;
	}
	if ((cl = class_create(THIS_MODULE, DEV_CLASS_NAME)) == NULL)
	{
		printk(KERN_ALERT "%s: creating device class failed", DEV_DRIVER);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	if (device_create(cl, NULL, DEV_NUMBER, NULL, DEV_DRIVER) == NULL)
	{
		printk(KERN_ALERT "%s: populating device class failed", DEV_DRIVER);
		class_destroy(cl);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, DEV_NUMBER, 1) == -1)
	{
		printk(KERN_ALERT "%s: registering failed", DEV_DRIVER);
		device_destroy(cl, DEV_NUMBER);
		class_destroy(cl);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	
	printk(KERN_INFO "%s: registered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return 0;
}

static void __exit flosd_exit(void)
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

module_init(flosd_init);
module_exit(flosd_exit);
