#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "devChar"
#define CLASS_NAME  "dev"
#define SIZE 1024

MODULE_LICENSE("GPL");

static struct device* devCharDevice = NULL;
static struct class*  devCharClass  = NULL;

static int majorNumber;
static int bytesUsed = 0;
static char text[SIZE + 1] = {0}

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);

static struct file_operations fops =
{
   .open = dev_open,
   .release = dev_release,
   .read = dev_read,
   .write = dev_write   
};

static int dev_open(struct inode *inoded, struct file *filep)
{
   printk(KERN_INFO "Device is opened\n");
   return 0;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Device is closed\n");
   return 0;
}