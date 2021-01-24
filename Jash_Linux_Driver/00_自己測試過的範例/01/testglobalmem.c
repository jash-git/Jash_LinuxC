#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define MEM_CLEAR 0x1  /*�M0�����O����*/
#define GLOBALMEM_SIZE	0x1000/*�����O�����̤j4K�줸��*/

void main()
{
	int file;
	char setdata[]="testglobalmen data!";
	char getdata[4096];
	file=open("/dev/globalmem",O_RDWR);//���}�]��
	if(file<0)
	{
		printf("open /dev/globalmem error\n");
	}
	else
	{		
		printf("clean Data\n");
		ioctl(file,MEM_CLEAR,"");//�M�ŰO����
		
		lseek(file,0,SEEK_SET);
		read(file,getdata,sizeof(setdata));//Ū�X����
		printf("read Data=%s \n",getdata);			
		
		lseek(file,0,SEEK_SET);		
		write(file,setdata,sizeof(setdata));//�g�J����
		printf("write Data=%s \n",setdata);
		
		lseek(file,0,SEEK_SET);
		read(file,getdata,sizeof(setdata));//Ū�X����
		printf("read Data=%s \n",getdata);

		//printf("clean Data\n");
		//ioctl(file,MEM_CLEAR,"");//�M�ŰO����				
		close(file);//��]��			
	}

}