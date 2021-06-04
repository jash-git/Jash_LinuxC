#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[],char *env[])
{
	int i=0;
	int j=0;
	//execl("/cpp/cube_fun/cube_fun","cube_fun",(char*)0);//不會執行迴圈
	//execle("/cpp/cube_fun/cube_fun","cube_fun",0,env);//不會執行迴圈
	system("/cpp/cube_fun/cube_fun");//會執行迴圈
	for(i=0;i<5;i++)
	{
		for(j=0;j<10;j++)
			printf("value=%d\t",j);
		printf("\n");
	}
}
