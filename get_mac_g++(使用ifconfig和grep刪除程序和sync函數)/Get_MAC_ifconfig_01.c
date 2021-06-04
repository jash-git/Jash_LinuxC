#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char **argv)
{
	FILE *pf='\0';
	int intIsGetMAC=0;//紀錄在第幾次取得MAC
	char chrMAC[20];
	system("rm -rf /home/jash/MAC.txt");//刪除檔案
	//sleep(1);
	if((pf=fopen("/home/jash/MAC.txt","r"))=='\0')
	{
		printf("can't find /home/jash/MAC.txt %d\n",intIsGetMAC);
		intIsGetMAC++;
	}	
	system("ifconfig |grep HW |awk '{print $5}' > /home/jash/MAC.txt &");//紀錄MAC
	//sleep(1);
	do
	{
		sync();//system("sync");
		if((pf=fopen("/home/jash/MAC.txt","r"))=='\0')
		{
			printf("can't find /home/jash/MAC.txt %d\n",intIsGetMAC);
			intIsGetMAC++;
		}
		else
		{
			memset(chrMAC, 'F' , 20 * sizeof(chrMAC[0]));
			printf("Clear MAC Buffer= %s\n",chrMAC);
			fscanf(pf,"%s",chrMAC);
			fclose(pf);
			pf='\0';
			printf("MAC Buffer= %s\n",chrMAC);
			intIsGetMAC=-1;
				
		}
	}while(intIsGetMAC!=-1);
	return 0;
}
