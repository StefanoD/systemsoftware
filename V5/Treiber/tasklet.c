#include <linux/module.h>
#include <linux/interrupt.h>

struct tasklet_struct tl_descr;

static void tasklet_func( unsigned long data)
{
	printk(KERN_INFO "Ich bin ein Tasklet\n");
}

DECLARE_TASKLET( tl_descr, tasklet_func, 0L); 

static int __init mod_init(void)
{
	printk(KERN_INFO "mod init called\n");
	tasklet_schedule( &tl_descr );	/* sobald möglich ausfuehren */
	return 0;
}

static void __exit	mod_exit(void)
{
	printk(KERN_INFO "mod exit called\n");
	tasklet_kill( &tl_descr );
}

module_init( mod_init );
module_exit( mod_exit );

MODULE_LICENSE("GPL");
