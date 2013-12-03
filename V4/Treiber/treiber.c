#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
 
 
MODULE_LICENSE("GPL");
 
static int major;
static struct file_operations fops;

static int __init ModInit(void)
{
	//majornumber 0 for dynamic majornumber
	if((major=register_chrdev(0,"TestDriver", &fops))==0) {
		printk(KERN_ALERT "Register driver failed!\n");
		return -EIO;
	}
	printk(KERN_ALERT "Register driver successful!\n");
	return 0;
}
 
static void __exit ModExit(void)
{
	unregister_chrdev(major, "TestDriver");
	printk(KERN_ALERT "Deregister driver successful!\n");
}
 
module_init(ModInit);
module_exit(ModExit);
