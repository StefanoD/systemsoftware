#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
 
 
MODULE_LICENSE("GPL");
 
static int major;
static dev_t template_dev_number;
static struct file_operations &fops;
struct cdev * driver_object

static int __init ModInit(void)
{
	if(alloc_chrdev_region(&template_dev_number,0,1, "TestDriver") < 0) 
	{
		return -EIO;
	}
	driver_object = cdev_alloc();
	if ( driver_object==NULL) 
	{
		goto free_device_number;
	}
	driver_object->owner = THIS_MODULE;
	driver_object->ops = &fops;
	if (cdev_add(driver_object, template_dev_number, 1) )
	{
		goto free_cdev;
	}
	template_class = class_create( THIS_MODULE, MY_CLASS_NAME );
	device_create( template_class, NULL, template_template_dev_number, NULL, "%s", MY_DEVICE_FILE_NAME );
	return 0;
	
	free_cdev:
			kobject_put( &driver_object->kobj );
	free_device_number:
			unregister_chrdev_region( template_template_dev_number, 1);
			return -EIO;
}
 
static void __exit ModExit(void)
{
	device_destroy( template_class, template_dev_number );
	class_destroy( template_class );
	/* abmelden des treibers */
	cdev_del( driver_object );
	unregister_chrdev_region( template_dev_number, 1);
	return;	
}
 
module_init(ModInit);
module_exit(ModExit);
