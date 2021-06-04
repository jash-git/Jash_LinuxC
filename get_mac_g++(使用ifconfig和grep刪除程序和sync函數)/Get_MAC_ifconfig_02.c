#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char **argv)
{
	FILE *pf='\0';
	int intIsGetMAC=0;
	char chrMAC[100];
	system("rm -rf /home/jash/MAC.txt");
	system("rm -rf /home/jash/ping.txt");
	system("ping 8.8.8.8 -c 1000 > /home/jash/ping.txt &");

	if((pf=fopen("/home/jash/MAC.txt","r"))=='\0')
	{
		printf("can't find /home/jash/MAC.txt %d\n",intIsGetMAC);
	}	
	system("ifconfig |grep HW |awk '{print $5}' > /home/jash/MAC.txt &");//紀錄MAC

	do
	{
		sync();//system("sync");
		if((pf=fopen("/home/jash/MAC.txt","r"))=='\0')
		{
			intIsGetMAC++;			
			printf("can't find /home/jash/MAC.txt %d\n",intIsGetMAC);
		}
		else
		{
			chrMAC[0]='\0';
			fscanf(pf,"%s",chrMAC);
			fclose(pf);
			pf='\0';
			if(chrMAC[0]!='\0')
				printf("MAC= %s\n",chrMAC);
			intIsGetMAC=-1;
			//system("rm /home/jash/MAC.txt");
			///////////////////////////////////////
			//刪除ping 程序
			system("kill -9 `ps -ef|grep ping|grep 8.8.8.8|awk '{print $2}'`");
		}
	}while(intIsGetMAC!=-1);
	system("rm -rf /home/jash/MAC.txt");
	system("rm -rf /home/jash/ping.txt");
	return 0;
}
