#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

//  定义设备文件名
#define DEVICE_NAME "wordcount"
static unsigned char mem[10000]; // 保存向设备文件写入的数据
static int word_count = 0;
#define TRUE 255
#define FALSE 0

//  判断指定字符是否为空格（包括空格符、制表符、回车符和换行符）
static unsigned char is_spacewhite(char c)
{
	if (c == 32 || c == 9 || c == 13 || c == 10)
		return TRUE;
	else
		return FALSE;
}

static int get_word_count(const char *buf)
{
	int n = 1;
	int i = 0;
	char c = ' ';

	char flag = 0; // 处理多个空格分隔的情况，0：正常情况，1：已遇到一个空格
	if (*buf == '\0')
		return 0;
	//  第1个字符是空格，从0开始计数
	if (is_spacewhite(*buf) == TRUE)
		n--;

	//  扫描字符串中的每一个字符
	for (; (c = *(buf + i)) != '\0'; i++)
	{
		//  只由一个空格分隔单词的情况
		if (flag == 1 && is_spacewhite(c) == FALSE)
		{

			flag = 0;
		}
		//  由多个空格分隔单词的情况，忽略多余的空格
		else if (flag == 1 && is_spacewhite(c) == TRUE)
		{

			continue;
		}
		//  当前字符为空格是单词数加1
		if (is_spacewhite(c) == TRUE)
		{
			n++;
			flag = 1;
		}
	}
	//  如果字符串以一个或多个空格结尾，不计数（单词数减1）
	if (is_spacewhite(*(buf + i - 1)) == TRUE)
		n--;
	return n;
}

//當設備文件讀取時會被呼叫的函數
//file：指向設備文件 buf：保存可讀取的資訊 count：可讀取的字數 ppos：讀取的偏移量
static ssize_t word_count_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)
{
	unsigned char temp[4];

	temp[0] = word_count >> 24;
	temp[1] = word_count >> 16;   
	temp[2] = word_count >> 8;
	temp[3] = word_count;
	if (copy_to_user(buf, (void*) temp, 4))
	{
		return -EINVAL;
	}
	printk("read:word count:%d", (int) count);

	return count;
}
//當設備文件寫入時會被呼叫的函數
//file：指向設備文件 buf：保存可讀取的資訊 count：可讀取的字數 ppos：讀取的偏移量
static ssize_t word_count_write(struct file *file, const char __user *buf,size_t count, loff_t *ppos)
{
	ssize_t written = count;

	if (copy_from_user(mem, buf, count))
	{
		return -EINVAL;
	}
	mem[count] = '\0';
	word_count = get_word_count(mem);
	printk("write:word count:%d\n", (int) word_count);

	return written;
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

