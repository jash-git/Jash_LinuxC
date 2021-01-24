#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
void init_daemon(void)
{
	int pid;
	int i;
	if(pid=fork())
		exit(0);
	else if(pid<0)
		exit(1);
	//是第一子歷程
	setsid();//第一子歷程成為新的會話組長和歷程組長, 並與節制終端分手
	if(pid=fork())
		exit(0);//竣事第一子歷程
	else if(pid< 0)
	exit(1);
	//第二子歷程不再是會話組長
	for(i=0;i< NOFILE;++i)//封鎖打開的文件描述符
	close(i);
	chdir("/");//改變工作目錄到/
	umask(0);//重設文件建樹掩模
	return;
}
int main()
{
	init_daemon();//使歷程成為Daemon
	//以下可以添加功能代碼
	char *chrpath="/home/jash/cpp_service/log/cpp_service.log";
	FILE *pf;   
	while(1)
	{
		time_t t = time(0);   // get time now 
		struct tm * now = localtime( & t );
		pf=fopen(chrpath,"a");
		fprintf(pf,"Now Time: %d-%d-%d %d:%d:%d\n",(now->tm_year + 1900),(now->tm_mon + 1),now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		fclose(pf);
		pf='\0';
		sleep(10);//睡眠一分鐘
	}
	return 0;
}

