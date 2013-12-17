#include <linux/module.h>
#include <linux/kthread.h>

static struct task_struct *thread_id;
static wait_queue_head_t wq;
static DECLARE_COMPLETION( on_exit );

static int mythread( void *data )
{
	unsigned long timeout;
	int i;

	allow_signal( SIGTERM );	/* immer erster aufruf */
	
	for( i = 0;(kthread_should_stop()==0); i++)
	{
		timeout= 2*HZ;
		timeout= wait_event_interruptible_timeout(wq, (timeout==0), timeout);
		printk(KERN_INFO "Thread woke up: %lds\n", timeout);
		if( timeout==-ERESTARTSYS )
		{
			printk("got signal, break\n");
			break;
		}
	}
	complete_and_exit( &on_exit, 0 );
}

static int __init kthread_init(void)
{
	init_waitqueue_head(&wq);
	thread_id=kthread_create(mythread, NULL, "mykthread");
	if( thread_id==0 )
	{
		return -EIO;
	}
	wake_up_process(thread_id);
	return 0;
}

static void __exit kthread_exit(void)
{
	kill_pid(task_pid(thread_id), SIGTERM, 1);
	wait_for_completion(&on_exit);
}

module_init(kthread_init);
module_exit(kthread_exit);

MODULE_LICENSE("GPL");

