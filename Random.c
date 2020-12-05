#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/random.h>

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static unsigned int randomResult; //Store random result

static int my_open(struct inode *i, struct file *f)
{
 printk(KERN_INFO "Driver: open()\n");
 return 0;
}

 static int my_close(struct inode *i, struct file *f)
{
 printk(KERN_INFO "Driver: close()\n");
 return 0;
}

 static ssize_t my_read(struct file *f, char __user *buf, size_t
 len, loff_t *off)
{
 printk(KERN_INFO "Driver: read()\n");
 
 get_random_bytes(&randomResult, sizeof(randomResult));

 if (copy_to_user(buf, &randomResult, sizeof(randomResult))) { //Failed
  return -EFAULT;
  }
 return sizeof(randomResult);
}

 static ssize_t my_write(struct file *f, const char __user *buf,
 size_t len, loff_t *off)
{
 printk(KERN_INFO "Driver: write()\n");
 return len;
}

 static struct file_operations pugs_fops =
{
 .owner = THIS_MODULE,
 .open = my_open,
 .release = my_close,
 .read = my_read,
 .write = my_write
};

static int __init ofcd_init(void) /* Constructor */
{
 printk(KERN_INFO "Hello: ofcd registered");
 if (alloc_chrdev_region(&first, 0, 1, "RandomDevice") < 0)
 {
 printk(KERN_ERR "\nERROR: alloc_chrdev_region\n");
 return -1;
 }
 if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
 {
 printk(KERN_ERR "\nERROR: class_create\n");
 unregister_chrdev_region(first, 1);
 return -1;
 }
 if (device_create(cl, NULL, first, NULL, "mydev1") == NULL)
 {
 printk(KERN_ERR "\nERROR: device_create\n"); 
 class_destroy(cl);
 unregister_chrdev_region(first, 1);
 return -1;
 }
 cdev_init(&c_dev, &pugs_fops);
 if (cdev_add(&c_dev, first, 1) == -1)
 {
 printk(KERN_ERR "\nERROR: cdev_add\n");
 device_destroy(cl, first);
 class_destroy(cl);
 unregister_chrdev_region(first, 1);
 return -1;
 }
  printk(KERN_INFO "\nDevice is created\n");
 return 0;
}

static void __exit ofcd_exit(void) /* Destructor */
{
 cdev_del(&c_dev);
 device_destroy(cl, first);
 class_destroy(cl);
 unregister_chrdev_region(first, 1);
 printk(KERN_INFO "ofcd unregistered");
 printk(KERN_INFO "\nDevice is closed\n");
}

module_init(ofcd_init);
module_exit(ofcd_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Trieu Trang Tong - Nguyen Duc Minh Tri");
MODULE_DESCRIPTION("Our First Character Driver");
