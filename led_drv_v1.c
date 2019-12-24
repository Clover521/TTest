#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>

#include <asm/io.h>
#include <asm/uaccess.h>

struct s5pv210_led{
	int dev_major;
	struct class * led_cls;
	struct device *led_dev;
	int value;
};

struct s5pv210_led *led_dev;

unsigned long *gpc0_conf;
unsigned long *gpc0_data;

int led_open(struct inode *inode, struct file *filp)
{
	printk("------------%s------------\n",__FUNCTION__);

	*gpc0_conf  &= ~(0xf << 12);
	*gpc0_conf  |=  0x01 << 12;

	
		
	return 0;
}
ssize_t led_write(struct file *filp, const char __user *buf, size_t size, loff_t *flag)
{
	int ret;
	printk("------------%s------------\n",__FUNCTION__);
	ret = copy_from_user(&led_dev->value,buf , size);
	if(ret > 0){
		printk("copy_from_user error\n");
		return -EFAULT;
	}

	if(led_dev->value){
	
	*gpc0_data  |= (0x01 << 3);
		
	}else{
	
		*gpc0_data &= ~(0x01 << 3);
	}
	
	return size;
}
int led_close(struct inode *inode, struct file *filp)
{
	printk("------------%s------------\n",__FUNCTION__);
	//ÃðµÆ
	*gpc0_data &= ~(0x01 << 3);

	return 0;
}
struct file_operations fops = {
	.open = led_open,
	.write = led_write,
	.release = led_close,
};


static int __init led_init(void)
{
	int ret;
	printk("------------%s------------\n",__FUNCTION__);

	led_dev = kzalloc(sizeof(struct s5pv210_led), GFP_KERNEL);
	if(led_dev == NULL){
		printk("kzalloc  error\n");
		return -ENOMEM;
	}

	
	led_dev->dev_major = register_chrdev(0, "led_drv", &fops);
	if(led_dev->dev_major < 0){
		printk("dev_major error\n");
		ret =  -EINVAL;
		goto err_free;
	}



	led_dev->led_cls = class_create(THIS_MODULE, "led_class");
	if(IS_ERR(led_dev->led_cls)){
		printk("class_create error\n");
		ret = PTR_ERR(led_dev->led_cls);
		goto err_unregister_chrdev;
	}
	
	led_dev->led_dev = device_create(led_dev->led_cls, NULL, MKDEV(led_dev->dev_major,0), NULL,"led");
	if(IS_ERR(led_dev->led_dev)){
		printk("device_create error\n");
		ret = PTR_ERR(led_dev->led_dev);
		goto err_class_destroy;
	}


	gpc0_conf = ioremap(0x01C208B4, 4);
	gpc0_data = ioremap(0x01C208C4, 4);


	
	return 0;

err_class_destroy:
	class_destroy(led_dev->led_cls);
err_unregister_chrdev:
	unregister_chrdev(led_dev->dev_major,  "led_drv");
err_free:
	kfree(led_dev);
	return ret;
}

//Çý¶¯Ä£¿éÐ¶ÔØº¯Êý
static void __exit led_exit(void)
{
	printk("------------%s------------\n",__FUNCTION__);
	iounmap(gpc0_conf);
	device_destroy(led_dev->led_cls, MKDEV(led_dev->dev_major,0));
	class_destroy(led_dev->led_cls);
	unregister_chrdev(led_dev->dev_major,  "led_drv");
	kfree(led_dev);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");


