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

static char * DEV_NAME = "Zero";
static dev_t dev_number; // Device Number
static struct cdev c_dev; // character device structure
static struct class *cl; 	//device class
static atomic_t open_count = ATOMIC_INIT(-1);
static char * hello_word = "Hello World";

static int my_open(struct inode *i, struct file *f)
{
	if((atomic_inc_and_test(&open_count)) != 0)
	{
		printk(KERN_INFO "%sDriver: open()\n", DEV_NAME);
		return 0;
	}
	printk(KERN_INFO "%sDriver in use\n", DEV_NAME);
	return -EIO;
	
}

static int my_close(struct inode *i, struct file *f)
{
	atomic_dec(&open_count);
	printk(KERN_INFO "%sDriver: close()\n", DEV_NAME);
	return 0;
}
static ssize_t my_read(struct file *f, char __user *buf, size_t count, loff_t *off)
{
	int not_copied, to_copy;
	int minor = iminor( f->f_dentry->d_inode );

	printk(KERN_INFO "%sDriver: read()\n", DEV_NAME);
	printk(KERN_INFO "Minornumber: %d\n", minor);
	to_copy = strlen(hello_word)+1;
	if(to_copy > count)
	{
		to_copy = count;
	}
	not_copied = copy_to_user(buf, hello_word, to_copy);

	return to_copy - not_copied;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t count, loff_t *off)
{
	printk(KERN_INFO "%sDriver: write()\n", DEV_NAME);
	return count;
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
	printk(KERN_INFO "%sDriver: Registering...\n", DEV_NAME);
	if (alloc_chrdev_region(&dev_number,0,2, "MyZeroDriver") < 0) 
	{
		printk(KERN_ALERT "%sDriver: Reserving Devicenumber failed!\n", DEV_NAME);
		return -EIO;
	}
	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		printk(KERN_ALERT "%sDriver: Creating Device Class failed", DEV_NAME);
		unregister_chrdev_region(dev_number, 2);
		return -EIO;
	}
	if (device_create(cl, NULL, dev_number, NULL, "myzero") == NULL)
	{
		printk(KERN_ALERT "%sDriver: Populating Device Class failed", DEV_NAME);
		class_destroy(cl);
		unregister_chrdev_region(dev_number, 2);
		return -EIO;
	}
	
	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, dev_number, 2) == -1)
	{
		printk(KERN_ALERT "%sDriver: Driver registering failed", DEV_NAME);
		device_destroy(cl, dev_number);
		class_destroy(cl);
		unregister_chrdev_region(dev_number, 2);
		return -EIO;
	}
	
	printk(KERN_INFO "%sDriver: Registered\n", DEV_NAME);
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return 0;
	
}
 
static void __exit ModExit(void)
{
	printk(KERN_INFO "%sDriver: Deregistering...\n", DEV_NAME);
	device_destroy(cl, dev_number);
	class_destroy(cl);
	unregister_chrdev_region(dev_number, 2);
	printk(KERN_INFO "%sDriver: Deregistered\n", DEV_NAME);
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return;	
}
 
module_init(ModInit);
module_exit(ModExit);
