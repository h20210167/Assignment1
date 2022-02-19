#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include <linux/device.h>

#define magic_no 684
#define select_channel _IOR(magic_no, 1, int *)
#define select_allign _IOR(magic_no, 2, char *)
#define DEVICE_FILE_NAME "/dev/adc-dev"




static dev_t adc_16;       // initializing device no.
static struct cdev c_dev;  // initializing cdev struct
static struct class *cls;

static uint16_t channel = 0;  
static char allign = 'r';

uint16_t rgenerate(void);   // func to generate random no


static int adc_open(struct inode *i,struct file *f)

	{
		
		printk(KERN_INFO "Driver: open\n");
		return 0;
	}


static int adc_close(struct inode *i,struct file *f)
	
	{
		
		printk(KERN_INFO "Driver: close\n");
		return 0;

	}


static ssize_t adc_read(struct file *f, char __user *buf, size_t len, loff_t *off)
		
		{
    		
    		int k_user;
    		uint16_t a;
    		a = rgenerate();
		printk(KERN_INFO " random number is %d",a);
		
		if(allign == 'l' || allign = 'L')
		{									
			a = a * 16;	// shifting the no. left 4 times			
		}
		
		
		if(allign == 'r' || allign == 'R')
		{									
			a = a / 16;	// shifting the no. right 4 times			
		}
	
    		k_user = copy_to_user(buf,&a,sizeof(a));
    	
    	
    		if(k_user!=0)
    		{
    		printk(KERN_INFO "Failed to copy %d bytes to the userspace ",k_user);
    		}
		
		return sizeof(a);

	}


long adc_ioctl(struct file *file,unsigned int ioctl_num,unsigned long ioctl_param)
		{

                switch(ioctl_num)
                	{
                  	case select_channel:
				channel = ioctl_param;
                    		printk(KERN_INFO "channel: %d",channel);
				break;

                 	 case select_allign:
                    		allign = (char)ioctl_param;
                    		printk(KERN_INFO "allignment: %c",allign);
				break;
               	 }
              return 0;
  }

uint16_t rgenerate(void)    // generating 16bits of random no.
  {
    unsigned int r;
    
    get_random_bytes(&r,sizeof(r));
    
    r &= 0x0FFF;
    
    return (uint16_t)r;
  }

static struct file_operations fops =
					{
					.owner = THIS_MODULE,
					.open = adc_open,
					.release = adc_close,
                                        .unlocked_ioctl = adc_ioctl,
					.read = adc_read
					};

static int __init adc_16_init(void)
{
	//  1: dynamically allocating major & minor no.

		if(alloc_chrdev_region(&adc_16,0,1,"adc")<0)
	{
		return -1;
	}
	printk(KERN_INFO "<major , minor>:<%d,%d>\n",MAJOR(adc_16),MINOR(adc_16));

	
	//  2: creating class & device automatically
	
		if((cls=class_create(THIS_MODULE,"myclass"))==NULL)
	{
			unregister_chrdev_region(adc_16,1);
			return -1;
	}
		if((device_create(cls,NULL,adc_16,NULL,"adc-dev"))==NULL)
		/*here adc-dev is the name of device file in dev directory*/
	{
			class_destroy(cls);
			unregister_chrdev_region(adc_16,1);
			return -1;
	}
	
	
	// 3: linking fops to cdev

	cdev_init(&c_dev,&fops);
	//after cdev_add, driver is now alive
		
		if(cdev_add(&c_dev,adc_16, 1)==-1)
	{
		device_destroy(cls,adc_16);
		class_destroy(cls);
		unregister_chrdev_region(adc_16,1);
		return -1;
	}
	return 0;
}


static void __exit adc_16_exit(void)

{
	cdev_del(&c_dev);
	device_destroy(cls,adc_16);
	class_destroy(cls);
	unregister_chrdev_region(adc_16,1);
	printk(KERN_INFO "Driver unregistered");
}


module_init(adc_16_init);
module_exit(adc_16_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ADC CHAR DRIVER"); //these info is compiled and available in obj file
MODULE_AUTHOR("Kishansinh Chuauhan");
MODULE_INFO(ChipSupport, "RYZEN 5600U");



  
