#include <linux/init.h>    //
#include <linux/module.h>  //
#include <linux/version.h> //
#include <linux/fs.h>      // for registering!
#include <linux/kernel.h>  //
#include <linux/device.h>  //
#include <linux/kdev_t.h>  // makros for major and minor number
#include <linux/types.h>   // defines dev_t
#include <linux/cdev.h>    //
#include <linux/delay.h>   // for msleep!

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Froehlich");
MODULE_DESCRIPTION("test for semaphores and stuff");

#define DEV_NAME "lock"
#define DEV_CLASS_NAME "c_lock"
#define DEV_DRIVER "lock_driver"

#define SEMAPHORE_COUNT 1
#define TIME_TO_WAIT 3000
#define TIME_WAIT_STEP 200

static dev_t DEV_NUMBER;  // var for first device number
static struct cdev c_dev; // global chardevice struct
static struct class *cl;  // global device class
static struct semaphore sem;    


static int lock_open(struct inode *i, struct file *instance)
{
	
	printk(KERN_INFO "%s: open()\n", DEV_DRIVER);
	printk(KERN_DEBUG "%s: try to get lock", DEV_DRIVER);
	while(down_trylock(&sem)!= 0)
	{
		printk(KERN_DEBUG ".");
		msleep(TIME_WAIT_STEP);
	}
	printk(KERN_DEBUG "\n%s: wait %dseconds\n", DEV_DRIVER, TIME_TO_WAIT/1000);
	msleep(TIME_TO_WAIT);
	return 0;
}


static int lock_close(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: close()\n", DEV_DRIVER);
 	up(&sem);
	return 0;
}


static ssize_t lock_read(struct file *instance, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "%s: read()\n", DEV_DRIVER);
	return 0;
}

static ssize_t lock_write(struct file *instance, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "%s: write()\n", DEV_DRIVER);
	return 0;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = lock_open,
	.release = lock_close,
	.read = lock_read,
	.write = lock_write	
};

static int __init lock_init(void)
{
	printk(KERN_INFO "%s: init()\n", DEV_NAME);
	if (alloc_chrdev_region(&DEV_NUMBER,0,1, DEV_DRIVER) < 0) 
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
		printk(KERN_ALERT "%s: registering failed", DEV_DRIVER);
		device_destroy(cl, DEV_NUMBER);
		class_destroy(cl);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	sema_init(&sem, SEMAPHORE_COUNT);
	printk(KERN_INFO "%s: registered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return 0;
}

static void __exit lock_exit(void)
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

module_init(lock_init);
module_exit(lock_exit);
