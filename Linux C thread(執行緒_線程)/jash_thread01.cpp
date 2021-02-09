#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#define MAX 20

pthread_t thread[4];
pthread_mutex_t mut;
int number;
int i;
void *thread_fun(void *p)
{
	pthread_t pid = pthread_self();
	long count=pid;
	for (i = 0; i < MAX; i++)
	{
		printf("thread %ld: %d\n",count,i);
		pthread_mutex_lock(&mut);
			if(i<MAX)
			{
				number++;
				printf("thread %ld: number = %d\n",count,number);
			}
		pthread_mutex_unlock(&mut);
		sleep(1);
	}
	printf("thread %ld:主函數在等我完成任務嗎？\n",count);
	pthread_exit(NULL);
}
void thread_create(void)
{
	int temp;
	memset(&thread, 0, sizeof(thread));          //comment1
	/*創建線程*/
	for(int i=0;i<4;i++)
	{
		if((temp = pthread_create(&thread[i], NULL, thread_fun, NULL))!= 0)       //comment2
		{
			printf("線程%d創建失敗!\n",i);
		}
		else
		{
			printf("線程%d被創建\n",i);
		}
	}
}
void thread_wait(void)
{
	/*等待線程結束*/
	for(int i=0;i<4;i++)
	{
		if(thread[i] !=0)
		{//comment4
			pthread_join(thread[i],NULL);
			printf("線程%d已經結束\n",i);
		}
	}
}
int main()
{
		number=0;
        /*用默認屬性初始化互斥鎖*/
        pthread_mutex_init(&mut,NULL);
        printf("我是主函數哦，我正在創建線程，呵呵\n");
        thread_create();
        printf("我是主函數哦，我正在等待線程完成任務阿，呵呵\n");
        thread_wait();
        return 0;
}