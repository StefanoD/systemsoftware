#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
 
 
MODULE_LICENSE("GPL");
 
static dev_t dev_number;

static int __init ModInit(void)
{
	printk(KERN_INFO "TestDriver: Registering...\n");
	if(alloc_chrdev_region(&dev_number,0,1, "TestDriver") < 0) 
	{
		printk(KERN_ALERT "TestDriver: Registering failed!\n");
		return -EIO;
	}
	printk(KERN_INFO "TestDriver: Registered\n");
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return 0;
	
}
 
static void __exit ModExit(void)
{
	printk(KERN_INFO "TestDriver: Deregistering...\n");
	unregister_chrdev_region( dev_number, 1 );
	printk(KERN_INFO "TestDriver: Deregistered\n");
	printk(KERN_INFO "Major, Minor: %d, %d \n", MAJOR(dev_number), MINOR(dev_number));
	return;	
}
 
module_init(ModInit);
module_exit(ModExit);
