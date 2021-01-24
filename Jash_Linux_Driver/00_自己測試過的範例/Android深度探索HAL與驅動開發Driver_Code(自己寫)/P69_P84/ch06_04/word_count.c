#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

//  定义设备文件名
#define DEVICE_NAME "wordcount"
static unsigned char mem[10000]; // 保存向设备文件写入的数据
static char read_flag = 'y'; //y:設備已讀取 n:設備未讀取
static int written_count = 0;
//當設備文件讀取時會被呼叫的函數
//file：指向設備文件 buf：保存可讀取的資訊 count：可讀取的字數 ppos：讀取的偏移量
static ssize_t word_count_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)
{
	if( read_flag == 'n')
	{
		copy_to_user(buf,(void*)mem,written_count);
		printk("read count %d\n",(int) written_count);
		read_flag = 'y';
		return written_count;
	}
	//讀取過一次不能再讀取
	else
	{
		return 0;
	}
}
//當設備文件寫入時會被呼叫的函數
//file：指向設備文件 buf：保存可讀取的資訊 count：可讀取的字數 ppos：讀取的偏移量
static ssize_t word_count_write(struct file *file, const char __user *buf,size_t count, loff_t *ppos)
{
	copy_from_user(mem, buf, count);
	read_flag = 'n';
	written_count = count;
	printk("write:written count:%d\n", (int) written_count);
	return written_count;

}

//  描述与设备文件触发的事件对应的回调函数指针
// owner 指定整個模組都是驅動程式
//read API對應函數設定
//write API對應函數設定
static struct file_operations dev_fops =
{ .owner = THIS_MODULE, .read = word_count_read, .write = word_count_write };

//  描述设备文件的信息   
//name 指定驅動程式名稱
//minor 動態版號
static struct miscdevice misc =
{ .minor = MISC_DYNAMIC_MINOR, .name = DEVICE_NAME, .fops = &dev_fops };

//  初始化Linux驱动
static int __init word_count_init(void)
{
	int ret;
	//  建立设备文件
	ret = misc_register(&misc);
	//  输出日志信息
	printk("word_count_init_success\n");
	return ret;
}

// 卸载Linux驱动
static void __exit word_count_exit(void)
{
	//  删除设备文件  
	misc_deregister(&misc);
	//  输出日志信息
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

