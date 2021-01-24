#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define MEM_CLEAR 0x1   /*清0全局内存*/
#define GLOBALMEM_SIZE	0x1000/*全局内存最大4K字节*/

void main()
{
	int file;
	char setdata[]="testglobalmen data!";
	char getdata[4096];
	file=open("/dev/globalmem",O_RDWR); //以读写的方式打开
	if(file<0)
	{
		printf("open /dev/globalmem error\n");
	}
	else
	{		
		printf("clean Data\n");
		ioctl(file,MEM_CLEAR,"");//清除字符
		
		lseek(file,0,SEEK_SET);//定位为相对文件开头0处
		read(file,getdata,sizeof(setdata));//读取字符
		printf("read Data=%s \n",getdata);			
		
		lseek(file,0,SEEK_SET);//定位为相对文件开头0处
		write(file,setdata,sizeof(setdata));//写入字符
		printf("write Data=%s \n",setdata);
		
		lseek(file,0,SEEK_SET);//定位为相对文件开头0处
		read(file,getdata,sizeof(setdata));//读取字符
		printf("read Data=%s \n",getdata);

		//printf("clean Data\n");
		//ioctl(file,MEM_CLEAR,"");//清除字符				
		close(file);	
	}

}