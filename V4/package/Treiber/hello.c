#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/atomic.h>
 
 
MODULE_LICENSE("GPL");
 
static dev_t dev_number; // Device Number
static struct cdev c_dev; // character device structure
static struct class *cl; 	//device class
static atomic_t open_count = ATOMIC_INIT(-1);
static char * hello_word = "Hello World";

static int my_open(struct inode *i, struct file *f)
{
	if((atomic_inc_and_test(&open_count)) != 0)
	{
		printk(KERN_INFO "Driver: open()\n");
		return 0;
	}
	printk(KERN_INFO "Driver in use\n");
	return -EIO;
	
}

static int my_close(struct inode *i, struct file *f)
{
	atomic_dec(&open_count);
	printk(KERN_INFO "Driver: close()\n");
	return 0;
}
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Driver: read()\n");
	if(copy_to_user(buf, &hello_word, 1) != 0)
	{
		return -EFAULT;
	}
	return 1;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Driver: write()\n");
	return len;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write
};

static int __init ModInit(void)
{
	printk(KERN_INFO "TestDriver: Registering...\n");
	if (alloc_chrdev_region(&dev_number,0,1, "TestDriver") < 0) 
	{
		printk(KERN_ALERT "TestDriver: Reserving Devicenumber failed!\n");
		return -EIO;
	}
	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		printk(KERN_ALERT "TestDriver: Creating Device Class failed");
		unregister_chrdev_region(dev_number, 1);
		return -EIO;
	}
	if (device_create(cl, NULL, dev_number, NULL, "mydevice") == NULL)
	{
		printk(KERN_ALERT "TestDriver: Populating Device Class failed");
		class_destroy(cl);
		unregister_chrdev_region(dev_number, 1);
		return -EIO;
	}
	
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, dev_number, 1) == -1)
	{
		printk(KERN_ALERT "TestDriver: Driver registering failed");
		device_destroy(cl, dev_number);
		class_destroy(cl);
		unregister_chrdev_region(dev_number, 1);
		return -EIO;
	}
	
	printk(KERN_INFO "TestDriver: Registered\n");
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return 0;
	
}
 
static void __exit ModExit(void)
{
	printk(KERN_INFO "TestDriver: Deregistering...\n");
	device_destroy(cl, dev_number);
	class_destroy(cl);
	unregister_chrdev_region(dev_number, 1);
	printk(KERN_INFO "TestDriver: Deregistered\n");
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return;	
}
 
module_init(ModInit);
module_exit(ModExit);
