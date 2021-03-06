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
static char text[SIZE + 1] = {0};

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .release = dev_release,
   .read = dev_read,
   .write = dev_write,   
};

static int __init devChar_init(void)
{
	printk(KERN_INFO "devChar: Initializing the module\n");

	// dynamically allocate major number
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0)	// registration failed
	{
		printk(KERN_ALERT "devChar failed to register a major number.\n");
		return majorNumber;
	}
	printk(KERN_INFO "devChar: registered with major number %d\n", majorNumber);
	
	// register the device class
	devCharClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(devCharClass))
	{
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class");
		return PTR_ERR(devCharClass);
	}
	printk(KERN_INFO "devChar: device class registered\n");

	// register device driver
	devCharDevice = device_create(devCharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(devCharDevice))
	{
		class_destroy(devCharClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device.\n");
		return PTR_ERR(devCharDevice);
	}
	printk(KERN_INFO "devChar: device created successfully\n");
	return 0;
}

static void __exit devChar_exit(void)
{
	device_destroy(devCharClass, MKDEV(majorNumber, 0));
	class_unregister(devCharClass);
	class_destroy(devCharClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_INFO "devChar: Exiting\n");
}

static int dev_open(struct inode *inoded, struct file *filep)
{
   printk(KERN_INFO "Device is opened\n");
   return 0;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Device is closed\n");
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
	int bytesRead = 0;
	int i = 0;
	
	// If the entire text stored doesn't need to be read out
	if(len <= bytesUsed)
	{
		// Reads out the length asked
		error_count = copy_to_user(buffer, text, len);		
		for(i = len; i < bytesUsed; i++)
			text[i - len] = text[i];
		
		bytesRead = len;
		text[i - len] = '\0';
		// Updates the count of bytes stored
		bytesUsed -= bytesRead;
	}
	else
	{
		// Reads the whole text so now bytes stored is set to zero
		error_count = copy_to_user(buffer, text, bytesUsed);
		buffer[bytesUsed] = '\0';
		bytesRead = bytesUsed;
		bytesUsed = 0;
		text[0] = '\0';
	} 

	if (error_count == 0)
	{
		printk(KERN_INFO "devChar: Sent %d characters to the user\n", bytesRead);
		return (0);
	}
	else
	{
		printk(KERN_INFO "devChar: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;			// return bad address text
	}
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	int bytesFree = SIZE - bytesUsed;
	int i = 0;

	// Stores the buffer if text can hold it
	if(len < bytesFree)
	{
		sprintf(text + bytesUsed, "%s", buffer);
		bytesUsed += len;
	}
	else
	{	
		// Can't store the whole write request, so only stored up to the available amount 	
		for(i = 0; i < bytesFree; i++)
			text[bytesUsed + i] = buffer[i];

		bytesUsed += bytesFree;
		text[bytesUsed] = '\0';

	}

	printk(KERN_INFO "devChar: received %zu characters from the user\n", len);
	return len;
}

module_init(devChar_init);
module_exit(devChar_exit);

