#include <linux/init.h>    //
#include <linux/module.h>  //
#include <linux/version.h> //
#include <linux/fs.h>      // for registering!
#include <linux/kernel.h>  //
#include <linux/device.h>  //
#include <linux/kdev_t.h>  // makros for major and minor number
#include <linux/types.h>   // defines dev_t
#include <linux/cdev.h>    //
#include <linux/wait.h>    //
#include <asm/uaccess.h>   // copy_to/from_user
#include <linux/sched.h>   // scheduling (waitqueue)
#include <linux/string.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <asm/atomic.h>
#include <asm/io.h>
#include <linux/random.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Froehlich");
MODULE_DESCRIPTION("a simple buffer driver");

#define DEV_NAME "buf"
#define DEV_CLASS "buf_class"
#define DEV_DRIVER "buf_driver"

static dev_t DEV_NUMBER; // var for first device number
static struct cdev c_dev; //global chardevice struct
static struct class *cl;	//global device class


#define READ_POSSIBLE (atomic_read(&count) > 0)
#define WRITE_POSSIBLE (atomic_read(&count) < 100)

static atomic_t count = ATOMIC_INIT(0); //Listenelemente
static wait_queue_head_t write_queue;
static wait_queue_head_t read_queue;
static DECLARE_COMPLETION( on_exit );
static struct rt_mutex mut;
struct task_struct *thread_id_write, *thread_id_read;

typedef struct _liste {
	struct _liste *next;
	char *element;
} liste;

static liste *root = NULL;

static void liste_add(char *new_element)
{
	liste *lptr;
	
	if(root == NULL)
	{
		return;
	}
	printk(KERN_INFO "Thread %d ListeAdd Starting Index:%d\n", current->pid, atomic_read(&count));
	for( lptr = root; lptr->next != NULL; lptr=lptr->next );
	lptr->next = (liste*) kmalloc(sizeof(liste), GFP_ATOMIC);
	lptr->next->next = NULL;
	lptr->next->element = (void *) kmalloc(sizeof(new_element), GFP_ATOMIC);
	memcpy(lptr->next->element, new_element, strlen(new_element));
	atomic_inc(&count);
	printk(KERN_INFO "Thread %d ListeAdd FinishedIndex:%d\n", current->pid, atomic_read(&count));
}

static liste *liste_remove(void)
{
	liste *lptr, *tail;
	
	if(root == NULL || root->next == NULL)
	{
		return NULL;
	}
	printk(KERN_INFO "Thread %d ListeRemove Starting Index:%d\n", current->pid, atomic_read(&count));
	for( lptr = root; lptr->next->next != NULL; lptr=lptr->next );
	tail = lptr->next;
	lptr->next = NULL;
	atomic_dec(&count);
	printk(KERN_INFO "Thread %d ListeRemove Finished Index:%d\n", current->pid, atomic_read(&count));
	return tail;
}

static void liste_clear(void)
{
	liste *lptr;
	if(root == NULL)
	{
		return;
	}
	while( root->next != NULL)
	{
		for( lptr = root; lptr->next->next != NULL; lptr=lptr->next );
		kfree(lptr->next);
		lptr->next = NULL;
	}
	kfree(root);
}

static int readthread( void * data)
{
	liste *t_buffer = NULL;
	unsigned int i, sleeptime;
	struct sched_param schedpar;
	allow_signal(SIGTERM);

	schedpar.sched_priority = 50;
	sched_setscheduler(current, SCHED_FIFO, &schedpar);
	printk(KERN_INFO "ReadThread %d start with Priority: %d\n", current->pid, current->rt_priority);
	if(wait_event_interruptible(read_queue, READ_POSSIBLE) )
	{
		printk(KERN_DEBUG "%s: some error occured -> ERESTART\n", DEV_DRIVER);
		return -ERESTART;
	}

	if( rt_mutex_lock_interruptible(&mut,0) <0)
	{
		printk(KERN_INFO "INTERRUPT while rt_mutex_lock_interruptible\n");
	} else {
		printk(KERN_INFO "ReadThread %d in critical section\n", current->pid);
		t_buffer = liste_remove();
	
		printk(KERN_INFO "ReadThread %d read %s\n", current->pid, t_buffer->element);
	}
	printk(KERN_INFO "ReadThread %d leaving critical section\n", current->pid);

	if(t_buffer != NULL) 
	{
	 	kfree(t_buffer->element);
		kfree(t_buffer);
	}
	get_random_bytes(&i, sizeof(i));
	sleeptime = i % 5;
	printk(KERN_INFO "ReadThread Sleeptime %d", sleeptime);
	ssleep(sleeptime);
	rt_mutex_unlock(&mut);
	printk(KERN_INFO "WriteThread %d finished\n",current->pid);
	complete_and_exit(&on_exit,0);
	wake_up_interruptible(&write_queue);
	
}

static int writethread( void *data)
{
	struct sched_param schedpar;
	unsigned int i, sleeptime;
	allow_signal(SIGTERM);

	schedpar.sched_priority = 70;
	sched_setscheduler(current, SCHED_FIFO, &schedpar);
	printk(KERN_INFO "WriteThread %d start with Priority:%d\n", current->pid, current->rt_priority);
	if(wait_event_interruptible(write_queue, WRITE_POSSIBLE)) 
	{
		printk(KERN_DEBUG "%s: wait_event interrupted -> ERESTART\n", DEV_DRIVER);
		return -ERESTART;
	}
	if( rt_mutex_lock_interruptible(&mut,0) <0)
	{
		printk(KERN_INFO "INTERRUPT while rt_mutex_lock_interruptible\n");
	} else {
		printk(KERN_INFO "WriteThread %d in critical section\n", current->pid);
		liste_add((char *) data);
		printk(KERN_INFO "WriteThread %d wrote %s\n", current->pid, (char*)data);
	}
	printk(KERN_INFO "WriteThread %d leaving critical section\n", current->pid);
	get_random_bytes(&i, sizeof(i));
	sleeptime = i % 5;
	printk(KERN_INFO "WriteThread Sleeptime %d\n", sleeptime);
	ssleep(sleeptime);
	rt_mutex_unlock(&mut);
	printk(KERN_INFO "WriteThread %d finished\n",current->pid);
	complete_and_exit(&on_exit,0);
	wake_up_interruptible(&read_queue);


}
static int buf_open(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: open()\n", DEV_DRIVER);
	return 0;
}


static int buf_close(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: close()\n", DEV_DRIVER);
	return 0;
}


static ssize_t buf_read(struct file *instance, char __user *buf, size_t len, loff_t *off)
{	
	printk(KERN_INFO "%s: read()\n", DEV_DRIVER);

	if(!READ_POSSIBLE && instance->f_flags&O_NONBLOCK)
	{
		printk(KERN_DEBUG "%s: some error occured -> EAGAIN\n", DEV_DRIVER);
		return -EAGAIN;
	} 
		
	thread_id_read = kthread_create(readthread, NULL, "readthread");
	if( thread_id_read == 0)
	{
		return -EIO;
	}
	wake_up_process(thread_id_read); /*starts thread*/

	wait_for_completion(&on_exit);
	return 0;
}



static ssize_t buf_write(struct file *instance, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "%s: write()\n", DEV_DRIVER);
	
	if(!WRITE_POSSIBLE && instance->f_flags&O_NONBLOCK)
	{
		printk(KERN_DEBUG "%s: O_NONBLOCK -> EAGAIN\n", DEV_DRIVER);
		return -EAGAIN;
	}

	thread_id_write = kthread_create(writethread,(void *) buf, "writethread");
	if( thread_id_write == 0)
	{
		return -EIO;
	}
	wake_up_process(thread_id_write); /*starts thread*/
	wait_for_completion(&on_exit);
	return len;
}


static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = buf_open,
	.release = buf_close,
	.read = buf_read,
	.write = buf_write
};

int __init buf_init(void)
{
	printk(KERN_INFO "%s: init()\n", DEV_DRIVER);
	if (alloc_chrdev_region(&DEV_NUMBER,0,1, DEV_DRIVER) < 0) 
	{
		printk(KERN_ALERT "%s: reserving device number failed!\n", DEV_DRIVER);
		return -EIO;
	}
	if ((cl = class_create(THIS_MODULE, DEV_CLASS)) == NULL)
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
		printk(KERN_ALERT "%s_driver: registering failed", DEV_DRIVER);
		device_destroy(cl, DEV_NUMBER);
		class_destroy(cl);
		unregister_chrdev_region(DEV_NUMBER, 1);
		return -EIO;
	}
	init_waitqueue_head( &write_queue );
	init_waitqueue_head( &read_queue );
	rt_mutex_init(&mut);
	root = (liste*) kmalloc(sizeof(liste), GFP_ATOMIC);
	root->next = NULL;
	
    printk(KERN_INFO "%s: registered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return 0;
}

void __exit buf_exit(void)
{
	kill_pid(task_pid(thread_id_write), SIGTERM, 1);
	wait_for_completion(&on_exit);
	kill_pid(task_pid(thread_id_read), SIGTERM, 1);
	wait_for_completion(&on_exit);
	liste_clear();
	printk(KERN_INFO "%s: exit()\n", DEV_DRIVER);
	device_destroy(cl, DEV_NUMBER);
	class_destroy(cl);
	cdev_del(&c_dev);
	unregister_chrdev_region(DEV_NUMBER, 1);
	printk(KERN_INFO "%s: deregistered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return;	
}



module_init(buf_init);
module_exit(buf_exit);

