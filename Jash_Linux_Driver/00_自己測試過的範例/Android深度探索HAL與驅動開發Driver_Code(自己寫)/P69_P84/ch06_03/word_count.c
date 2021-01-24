#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

//  定义设备文件名
#define DEVICE_NAME "wordcount"

//  描述与设备文件触发的事件对应的回调函数指针
// owner 指定整個模組都是驅動程式
static struct file_operations dev_fops =
{ .owner = THIS_MODULE};

//  描述设备文件的信息   
//name 指定驅動程式名稱
//minor 動態版號
static struct miscdevice misc =
{ .minor = MISC_DYNAMIC_MINOR, .name = DEVICE_NAME, .fops = &dev_fops };

//  初始化Linux驱动
static int __init word_count_init(void)
{
	//  输出日志信息
	int ret;
	//  建立设备文件
	ret = misc_register(&misc);
	printk("word_count_init_success\n");
	return ret;
}

// 卸载Linux驱动
static void __exit word_count_exit(void)
{
	//  输出日志信息
	//  删除设备文件  
	misc_deregister(&misc);
	printk("word_count_init_exit_success\n");
} 
//  注册初始化Linux驱动的函数
module_init( word_count_init);
//  注册卸载Linux驱动的函数
module_exit( word_count_exit);

MODULE_AUTHOR("jash.liao");
MODULE_DESCRIPTION("statistics of word count.");
MODULE_ALIAS("word count module.");
MODULE_LICENSE("GPL");

