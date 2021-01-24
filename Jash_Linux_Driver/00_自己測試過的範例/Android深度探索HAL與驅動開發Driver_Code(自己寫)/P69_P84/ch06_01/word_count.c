#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

//  初始化Linux驱动
static int __init word_count_init(void)
{
	//  输出日志信息
	printk("word_count_init_success\n");
	return 0;
}

// 卸载Linux驱动
static void __exit word_count_exit(void)
{
	//  输出日志信息
	printk("word_count_init_exit_success\n");
} 
//  注册初始化Linux驱动的函数
module_init( word_count_init);
//  注册卸载Linux驱动的函数
module_exit( word_count_exit);

