#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/io.h>

#include <asm/uaccess.h>



//定义结构体对象
struct a23_Light{

	int dev_major;
	struct class *led_cls;
	struct device *led_Dev;
	int value;
};
struct a23_Light *led_dev;

//虚拟地址变量
unsigned long *a23_Led_conf;
unsigned long *a23_Led_data;


void a23_Led_init(void)
{
	//配置PF3引脚为输出引脚
	*a23_Led_conf &= ~(0xf << 12);
	*a23_Led_conf |= 0x01 << 12;
	//配置初始状态为关闭状态
	*a23_Led_data = (0x00 << 3);
}

void a23_LedOn(void)
{
	//开灯
	*a23_Led_data = (0x01 << 3);
}

void a23_LedOff(void)
{
	//开灯
	*a23_Led_data = (0x00 << 3);
}





int a23_led_open(struct inode *ind, struct file *filp)
{

	//需要加一个优先级
	   printk(KERN_EMERG "----------%s----------\n",__FUNCTION__);

	//初始化LED灯
	 a23_Led_init();

	return 0;
}

ssize_t a23_led_write(struct file *filp, const char __user *buf, size_t size, loff_t *flag)
{
	int ret;
	//需要加一个优先级
	 printk(KERN_EMERG "----------%s----------\n",__FUNCTION__);
	//从应用层获取数据，第一个参数就是获得的值
	 
	
	ret = copy_from_user(&led_dev->value, buf,size); 
	 printk(KERN_EMERG "----------%d----------\n",led_dev->value);
	if(ret > 0)
	{
		 printk(KERN_EMERG "copy_from_user error\n");
		 return -EFAULT;
	}

	if(led_dev->value)
	{
		a23_LedOn();    //开灯
	}else{

		a23_LedOff();  //灭灯
	}
	


	return size;
}

int a23_led_close(struct inode *inode, struct file *filp)
{
	//需要加一个优先级
	 printk(KERN_EMERG "----------%s----------\n",__FUNCTION__);

	a23_LedOff();  //灭灯
	return 0;
}




struct file_operations fops = {

	.open = a23_led_open,
	.write = a23_led_write,
	.release = a23_led_close,

};




static int __init Led_init(void)
{
    int ret;
    //需要加一个优先级
	printk(KERN_EMERG "----------%s----------\n",__FUNCTION__);

	//申请结构体空间
	led_dev = kzalloc(sizeof(struct a23_Light), GFP_KERNEL);
	if(led_dev == NULL)
	{
		printk(KERN_EMERG "kzalloc error\n");

		return -ENOMEM;
	}

	//1、申请设备号
	led_dev->dev_major = register_chrdev(0, "led_drv", &fops);
	if(led_dev->dev_major < 0)
	{
		printk(KERN_EMERG "register_chrdev error\n");

		ret = -EINVAL;
		goto err_free;
	}


	//2、创建设备节点
	led_dev->led_cls = class_create(THIS_MODULE, "led_class");
	if(IS_ERR(led_dev->led_cls))
	{
		printk(KERN_EMERG "class_create error\n");
		ret = PTR_ERR(led_dev->led_cls);
		goto err_unregister_chrdev;
	}

	led_dev->led_Dev = device_create(led_dev->led_cls, NULL, MKDEV(led_dev->dev_major,0), NULL, "a23_led");
	if(IS_ERR(led_dev->led_Dev))
	{
		printk(KERN_EMERG "device_create error\n");
		ret = PTR_ERR(led_dev->led_Dev);
		goto err_class_destroy;
	}

	//3、初始化硬件 
	a23_Led_conf = ioremap(0x01C208B4, 4);
	a23_Led_data = ioremap(0x01C208C4, 4);


#if 0
	//点灯,测试没问题

	*a23_Led_conf &= ~(0xf << 12);
	*a23_Led_conf |= 0x01 << 12;

	
	*a23_Led_data = (0x00 << 3);
	
#endif
	

	return 0;

err_class_destroy:
	class_destroy(led_dev->led_cls);

err_unregister_chrdev:
	unregister_chrdev(led_dev->dev_major, "led_drv");
err_free:
	kfree(led_dev);

}



static void __exit Led_exit(void)
{
	//需要加一个优先级
	printk(KERN_EMERG "----------%s----------\n",__FUNCTION__);


	device_destroy(led_dev->led_cls, MKDEV(led_dev->dev_major,0));
	class_destroy(led_dev->led_cls);
	unregister_chrdev(led_dev->dev_major, "led_drv");
	kfree(led_dev);


}




//驱动加载和卸载模块的声明
module_init(Led_init);
module_exit(Led_exit);


//许可证的声明
MODULE_LICENSE("GPL");
























