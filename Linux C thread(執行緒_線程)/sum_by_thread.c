#include <stdio.h>
#include <pthread.h>
void *runner(void *param);
int sum;
int main(int argc, char *argv[])
{
	sum=0;
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid,&attr,runner,argv[1]);
	pthread_join(tid,NULL);
	printf("sum=%d\n",sum);//主程序抓線程變數
}
void *runner(void *param){
	int i;
	int upper=atoi(param);
	if(upper>0)
	{
		for(i=0;i<=upper;i++)
		{
			sum+=i;
		}
	}
	pthread_exit(0);
}
/*
example.c:20:1: error: stray ‘\343’ in program
 　　 for(i=0;i<=upper;i++)
->表示前面有特殊空白所導致
*/