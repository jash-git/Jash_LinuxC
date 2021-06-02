#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
struct configset{
	char chrpf[50];
	char chrurl[255];
};
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
int configcount()
{
	FILE *pfconfig;
	int intcount=0;
	char buf[255];
	pfconfig=fopen("config.set","r");
	while(fgets(buf,512,pfconfig) != NULL)
	{
		intcount++;
	}
	fclose(pfconfig);
	return intcount;
}
void createSHFile(char *data)
{
	struct stat statbuf;
	FILE *pfsh;
	pfsh='\0';
	pfsh=fopen("test.sh","w");
	fprintf(pfsh,"%s",data);
	fclose(pfsh);
	stat("test.sh", &statbuf);
	chmod("test.sh",(((statbuf.st_mode | S_IXUSR)|S_IXGRP)|S_IXOTH));
	pfsh='\0';
}
int main()
{
	init_daemon();//使歷程成為Daemon
	//以下可以添加功能代碼
	char chrpath[255];//char *chrpath="/home/jash/WowzaDisconnectAnalytics/log/";
	char chrdatacmd[1000];
	char bufAll[255];
	char chrpf[50];
	char chrurl[205];	
	char chrdataAll[500];
	char chrdatatime[25];
	char chrdataVar[450];
	char chrdataIp[25];

	char *chrfind;
	//cat wowzastreamingengine_access.log|grep \"	disconnect\"|awk -F\"	\" '{print $2\",\"$16\",\"$17}'|awk '{if (($1>\"01:10:10\") && ($1<\"01:10:50\")) print}'
	//cat wowzastreamingengine_access.log|grep "	disconnect"|awk -F"	" '{print $2","$16","$17}'|awk '{if (($1>"01:10:10") && ($1<"01:10:50")) print}'
	//cat wowzastreamingengine_access.log|grep "      disconnect"|awk -F"     " '{print $2","$16","$17}'|awk '{if (($1>"01:10:10") && ($1<"01:10:50")) print}'
	FILE *pflog;
	FILE *pfdata;
	FILE *pfconfig;
	configset *configset01;
	int intconfigcount=0;
	int inthr=0;
	int intmin=0;
	int intsec=0;
	int i=0;
	int j=0;
	int intNAIndex=-1;

	intconfigcount=configcount();
	configset01=new configset[intconfigcount];

	pfconfig=fopen("config.set","r");
	while(fgets(bufAll,255,pfconfig) != NULL)
	{
		sscanf(bufAll,"%[^,],%[^,],",chrurl,chrpf);
		strcpy((configset01+i)->chrpf,chrpf);
		strcpy((configset01+i)->chrurl,chrurl);
		//printf("%s\t%s\t\n",(configset01+i)->chrpf,(configset01+i)->chrurl);
		chrfind='\0';
		chrfind=strstr((configset01+i)->chrpf,"pf=N/A");
		if(chrfind !='\0')
		{
			intNAIndex=i;
		}
		i++;
	}
	fclose(pfconfig);
	pfconfig='\0';
	chrfind='\0';

	//printf("config count=%d\n",intconfigcount);
	//printf("NAIndex=%d\n",intNAIndex);
	while(1)
	{
		time_t t = time(0);   // get time now 
		struct tm * now = localtime( & t );
		sprintf(chrpath, "/home/jash/WowzaDisconnectAnalytics/log/%d-%d-%d.log",(now->tm_year + 1900),(now->tm_mon + 1),now->tm_mday);
		chrdatacmd[0]='\0';		
		sprintf(chrdatacmd,"cat /var/log/wowzalog/log-119/wowzastreamingengine_access.log|grep \"	disconnect\"|awk -F\"	\" '{print $2\",\"$16\",\"$17}'|awk '{if (($1>\"%d%d:%d%d:%d%d\") && ($1<\"%d%d:%d%d:%d%d\")) print}' > data.txt;sync;sync;cat /var/log/wowzalog/log-120/wowzastreamingengine_access.log|grep \"	disconnect\"|awk -F\"	\" '{print $2\",\"$16\",\"$17}'|awk '{if (($1>\"%d%d:%d%d:%d%d\") && ($1<\"%d%d:%d%d:%d%d\")) print}' >> data.txt;sync;sync;",(inthr/10),(inthr%10),(intmin/10),(intmin%10),(intsec/10),(intsec%10),(now->tm_hour/10),(now->tm_hour%10),(now->tm_min/10),(now->tm_min%10),(now->tm_sec/10),(now->tm_sec%10),(inthr/10),(inthr%10),(intmin/10),(intmin%10),(intsec/10),(intsec%10),(now->tm_hour/10),(now->tm_hour%10),(now->tm_min/10),(now->tm_min%10),(now->tm_sec/10),(now->tm_sec%10));
		createSHFile(chrdatacmd);
		inthr=now->tm_hour;
		intmin=now->tm_min;
		intsec=now->tm_sec;   
		pflog=fopen(chrpath,"a");
		fprintf(pflog,"Now Time: %d-%d-%d %d:%d:%d\n",(now->tm_year + 1900),(now->tm_mon + 1),now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		fprintf(pflog,"%s\n",chrdatacmd);
		fclose(pflog);
		system("./test.sh");
		//printf("%s\n",chrdatacmd);
		pflog='\0';
		
		pfdata=fopen("data.txt","r");
		while(fgets(chrdataAll,500,pfdata) != NULL)
		{
			sscanf(chrdataAll,"%[^?]?%[^,],%[^,]",chrdatatime,chrdataVar,chrdataIp);
			j=intNAIndex;
			for(i=0;i<intconfigcount;i++)
			{
				chrfind='\0';
				chrfind=strstr((configset01+i)->chrpf,chrdataVar);
				if(chrfind !='\0')
				{
					j=i;
					break;
				}
			}
			//printf("%s\t%s\t%s\n",chrdataIp,chrdataVar,(configset01+j)->chrpf);
			chrdatacmd[0]='\0';
			sprintf(chrdatacmd,"wget %s?%s&init=0&ip=%s -O getweb.html",(configset01+j)->chrurl,chrdataVar,chrdataIp);
			createSHFile(chrdatacmd);
			system("./test.sh");
			//printf("%s\n",chrdatacmd);
		}
		fclose(pfdata);
		pfdata='\0';
		sleep(1);//睡眠1S
	}
	return 0;
}
