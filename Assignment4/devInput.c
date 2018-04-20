#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "devInput"
#define CLASS_NAME  "devI"
#define SIZE 1024
#define REPL_STRING_SIZE 38

MODULE_LICENSE("GPL");

static struct device* devCharDevice = NULL;
static struct class*  devCharClass  = NULL;

static int majorNumber;
extern char text[SIZE + 1] = {0};
EXPORT_SYMBOL(text);
static DEFINE_MUTEX(inputMutex);

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .release = dev_release,
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

	mutex_init(&inputMutex);
	printk(KERN_INFO "devChar: device created successfully\n");
	return 0;
}

static void __exit devChar_exit(void)
{
	device_destroy(devCharClass, MKDEV(majorNumber, 0));
	class_unregister(devCharClass);
	class_destroy(devCharClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	mutex_unlock(&inputMutex);
	printk(KERN_INFO "devChar: Exiting\n");
}

static int dev_open(struct inode *inoded, struct file *filep)
{
	if(mutex_trylock(&inputMutex))
	{
		printk(KERN_INFO "Device is opened\n");
   		return 0;
	}

	printk(KERN_ALERT "charDriver is currently being used by another process");
	return -EBUSY;
   
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Device is closed\n");
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	int bytesUsed = strlen(text);
	int bytesFree = SIZE - bytesUsed;
	int i = 0, minLength = 0;
	int bufferSize = strlen(buffer);
	char replacementString[REPL_STRING_SIZE + 1];
	strcpy(replacementString, "Undefeated 2018 National Champions UCF");

	if(bufferSize < bytesFree) 
		minLength = bufferSize;
	else
		minLength = bytesFree;

	for (i = 0; i < minLength; i++)
	{
		int written = 0;
		if (i + 1 < bufferSize && i + 2 < bufferSize && buffer[i] == 'U' && buffer[i+1] == 'C' && buffer[i+2] == 'F')
		{	// found
			printk(KERN_INFO "UCF detected\n");
			i = i + 3;
			written = sprintf(text + bytesUsed, "%s", replacementString);
			if(written > 0) bytesUsed += written;
		}

		text[bytesUsed] = buffer[i];
		bytesUsed++;
	}

	printk(KERN_INFO "devChar: received %zu characters from the user\n", len);
	return len;
}

module_init(devChar_init);
module_exit(devChar_exit);

