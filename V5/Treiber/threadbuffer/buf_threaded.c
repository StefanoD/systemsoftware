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

//driver variables
static dev_t DEV_NUMBER; // var for first device number
static struct cdev c_dev; //global chardevice struct
static struct class *cl;	//global device class

static wait_queue_head_t write_queue;
static wait_queue_head_t read_queue;

DEFINE_MUTEX( mutex );
DECLARE_COMPLETION( on_exit );
#define DEV_NAME "buf"
#define DEV_CLASS "buf_class"
#define DEV_DRIVER "buf_driver"

#define BUFFER_SIZE 10
#define READ_POSSIBLE (atomic_read(&buffer_count) > 0)
#define WRITE_POSSIBLE (atomic_read(&buffer_count) < BUFFER_SIZE)

struct _instance_data {
	char *data;
	int length;
	int count;
};
static atomic_t buffer_count = ATOMIC_INIT(0);
static char *Buffer[BUFFER_SIZE];
static struct task_struct *thread_id;


static int readthread( void * data);
static int writethread(  void * length);
static ssize_t buf_read(struct file *instance, char __user *buf, size_t len, loff_t *off);
static ssize_t buf_write(struct file *instance, const char __user *buf, size_t len, loff_t *off);
static int buf_open(struct inode *i, struct file *instance);
static int buf_close(struct inode *i, struct file *instance);

static ssize_t buf_read(struct file *instance, char __user *buf, size_t len, loff_t *off)
{
	int count, to_copy, not_copied;
	printk(KERN_DEBUG "%s: (read)\n", DEV_DRIVER);
	if(wait_event_interruptible(read_queue, READ_POSSIBLE))
		return -ERESTARTSYS;

	count = atomic_read(&buffer_count)-1;
	
	printk(KERN_DEBUG "%s: (read after completion) %d\n", DEV_DRIVER, count);
	to_copy = min(len, sizeof(Buffer[count])); 
	not_copied = copy_to_user(buf, Buffer[count], to_copy);
	printk(KERN_INFO "%s: Aus dem Buffer gelesen: %s\n", DEV_DRIVER, Buffer[count]);
	
	printk(KERN_DEBUG "%s: (create ReadThread)\n", DEV_DRIVER);
	
	thread_id = kthread_create(readthread,(void *)count, "readthread");
	if( thread_id == 0)
		return -EIO;
	wake_up_process(thread_id);
	wait_for_completion(&on_exit);
	printk(KERN_DEBUG "%s: (after completion)\n", DEV_DRIVER);
	
	atomic_dec(&buffer_count);
	wake_up_interruptible(&write_queue);

	return not_copied;
}

static int readthread(void *data)
{
	int count;
	printk(KERN_DEBUG "%s: (im ReadThread)\n", DEV_DRIVER);
	count = (int)data;
	printk(KERN_DEBUG "%s: count:%d \n", DEV_DRIVER, count);
	kfree(Buffer[count]);
	printk(KERN_DEBUG "%s: (Buffer befreit!)\n", DEV_DRIVER);
	complete_and_exit(&on_exit,0);
}


static ssize_t buf_write(struct file *instance, const char __user *buf, size_t len, loff_t *off)
{
	struct _instance_data *iptr;
	printk(KERN_DEBUG "%s: (write)\n", DEV_DRIVER);
	iptr = (struct _instance_data*) kmalloc (sizeof(struct _instance_data), GFP_KERNEL);
	if( iptr ==0 ) {
		printk(KERN_ERR "Not enough Kernel Memory\n");
		return -ENOMEM;
	}

	if(wait_event_interruptible(write_queue, WRITE_POSSIBLE))
		return -ERESTARTSYS;

	iptr->length = len;
	instance->private_data = (void *) iptr;

	if( mutex_lock_interruptible(&mutex) == -EINTR )
	{
		printk(KERN_ERR "%s(write): Mutex interrupted!", DEV_DRIVER);
		return -EINTR;
	}
	iptr->count = atomic_read(&buffer_count);
	atomic_inc(&buffer_count);
	mutex_unlock(&mutex);

	printk(KERN_DEBUG "%s: (create Thread)\n", DEV_DRIVER);
	thread_id = kthread_create(writethread,(void *) instance, "writethread");
	if( thread_id == 0)
		return -EIO;
	wake_up_process(thread_id);
	
	wait_for_completion(&on_exit);

	printk(KERN_DEBUG "%s: (after completion)\n", DEV_DRIVER);
	Buffer[iptr->count]=iptr->data;
	strlcpy(Buffer[iptr->count], buf, len);

	wake_up_interruptible(&read_queue);
	printk(KERN_INFO "%s: In den Buffer geschrieben: %s Pos:%d\n", DEV_DRIVER, Buffer[iptr->count], iptr->count);
	return len;
}

static int writethread(void *data)
{
	struct _instance_data *iptr;
	struct file * instance;
	allow_signal(SIGTERM);
	instance = (struct file *) data;
 	iptr = (struct _instance_data *) instance->private_data;
	
	iptr->data = kmalloc(iptr->length, GFP_KERNEL);

	complete_and_exit(&on_exit,0);
}

static int buf_open(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: open()\n", DEV_DRIVER);
	return 0;
}


static int buf_close(struct inode *i, struct file *instance)
{
	printk(KERN_INFO "%s: close()\n", DEV_DRIVER);
	if( instance->private_data ) 
	{
		kfree( instance->private_data );
	}
	return 0;
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
	};

	init_waitqueue_head( &write_queue );
	init_waitqueue_head( &read_queue );
    printk(KERN_INFO "%s: registered\n", DEV_DRIVER);
	printk(KERN_INFO "major, minor: %d, %d \n", MAJOR(DEV_NUMBER), MINOR(DEV_NUMBER));
	return 0;
}

void __exit buf_exit(void)
{	
	printk(KERN_INFO "Kill write thread\n");
	if(thread_id > 0)
	{
		kill_pid(task_pid(thread_id), SIGTERM, 1);
		//wait_for_completion(&on_exit);
	}
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
