#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <asm/atomic.h>
 
 
MODULE_LICENSE("GPL");

static char * DEV_NAME = "Hello2";
static dev_t dev_number; // Device Number
static struct cdev driver_object; // character device structure
static struct class *cl; 	//device class
static atomic_t open_count = ATOMIC_INIT(-1);
static char * hello_world = "Hello World\n";

struct _instance_data {
	int counter;
};

static int my_open(struct inode *i, struct file *instance)
{
	struct _instance_data *iptr;

	if((atomic_inc_and_test(&open_count)) != 0)
	{
		iptr = (struct _instance_data *) kmalloc(sizeof(struct _instance_data),GFP_KERNEL);
		if( iptr==0 )
		{
			printk(KERN_ALERT "%sDriver: kmalloc", DEV_NAME);
			return -ENOMEM;
		}
		iptr->counter = strlen(hello_world)+1;
		instance->private_data = (void *) iptr;
		printk(KERN_INFO "%sDriver: open()\n", DEV_NAME);
		return 0;
	}
	atomic_dec(&open_count);
	printk(KERN_INFO "%sDriver in u se\n", DEV_NAME);
	return -EIO;
	
}

static int my_close(struct inode *i, struct file *instance)
{
	atomic_dec(&open_count);
	if( instance->private_data ) 
	{
		kfree( instance->private_data );
	}
	printk(KERN_INFO "%sDriver: close()\n", DEV_NAME);
	return 0;
}
static ssize_t my_read(struct file *instance, char __user *buf, size_t count, loff_t *off)
{
	int not_copied, to_copy;
	struct _instance_data *iptr = instance->private_data;

	if(iptr->counter == 0) {
		return 0; /*EOF*/
	}
	printk(KERN_INFO "%sDriver: read()\n", DEV_NAME);
	
	to_copy = strlen(hello_world)+1;
	if(to_copy > count)
	{
		to_copy = count;
	}
	not_copied = copy_to_user(buf, hello_world, to_copy);
	iptr->counter -= to_copy - not_copied;
	return to_copy - not_copied;
}
static ssize_t my_write(struct file *instance, const char __user *buf, size_t count, loff_t *off)
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
	if (alloc_chrdev_region(&dev_number,0,1, "hello_driver") < 0) 
	{
		printk(KERN_ALERT "%sDriver: Reserving Devicenumber failed!\n", DEV_NAME);
		return -EIO;
	}
	if ((cl = class_create(THIS_MODULE, "chardrv_hello2")) == NULL)
	{
		printk(KERN_ALERT "%sDriver: Creating Device Class failed", DEV_NAME);
		unregister_chrdev_region(dev_number, 1);
		return -EIO;
	}
	if (device_create(cl, NULL, dev_number, NULL, "hello2") == NULL)
	{
		printk(KERN_ALERT "%sDriver: Populating Device Class failed", DEV_NAME);
		class_destroy(cl);
		unregister_chrdev_region(dev_number, 1);
		return -EIO;
	}
	
	cdev_init(&driver_object, &fops);
	if (cdev_add(&driver_object, dev_number, 1) == -1)
	{
		printk(KERN_ALERT "%sDriver: Driver registering failed", DEV_NAME);
		device_destroy(cl, dev_number);
		class_destroy(cl);
		unregister_chrdev_region(dev_number, 1);
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
	cdev_del(&driver_object);
	unregister_chrdev_region(dev_number, 1);
	printk(KERN_INFO "%sDriver: Deregistered\n", DEV_NAME);
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return;	
}
 
module_init(ModInit);
module_exit(ModExit);
