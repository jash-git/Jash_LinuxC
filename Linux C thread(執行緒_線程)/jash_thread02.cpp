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
void *thread_fun(void *arg)
{
	pthread_t pid = pthread_self();
	int value = *(int *)arg;
	long count=pid;
	for (i = 0; i < MAX; i++)
	{
		//printf("thread %ld: %d\n",count,i);
		pthread_mutex_lock(&mut);
			if(i<MAX)
			{
				number++;
				printf("thread %d's pid=%ld: number = %d\n",value,count,number);
			}
		pthread_mutex_unlock(&mut);
		sleep(1);
	}
	printf("thread %d's pid=%ld:主函數在等我完成任務嗎？\n",value,count);
	pthread_exit(NULL);
}
void thread_create(void)
{
	int temp;
	int j[4];//要傳遞的資料閉需要式獨立變數
	memset(&thread, 0, sizeof(thread));          //comment1
	/*創建線程*/
	for(int i=0;i<4;i++)
	{
		j[i]=i+1;
		if((temp = pthread_create(&thread[i], NULL, thread_fun, &j[i]))!= 0)       //comment2
		{
			printf("線程%d創建失敗!\n",i);
		}
		else
		{
			printf("線程%d被創建\n",i);
		}
		sleep(1);//延遲線程建立速度,否則無法正確傳遞變數
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
/*
gcc example.cpp -o example -pthread
我是主函數哦，我正在創建線程，呵呵
線程0被創建
thread 1's pid=140730539345664: number = 1
線程1被創建
thread 1's pid=140730539345664: number = 2
thread 2's pid=140730530952960: number = 3
線程2被創建
thread 1's pid=140730539345664: number = 4
thread 2's pid=140730530952960: number = 5
thread 3's pid=140730522560256: number = 6
線程3被創建
thread 1's pid=140730539345664: number = 7
thread 2's pid=140730530952960: number = 8
thread 3's pid=140730522560256: number = 9
thread 4's pid=140730514167552: number = 10
我是主函數哦，我正在等待線程完成任務阿，呵呵
thread 1's pid=140730539345664: number = 11
thread 2's pid=140730530952960: number = 12
thread 3's pid=140730522560256: number = 13
thread 4's pid=140730514167552: number = 14
thread 1's pid=140730539345664: number = 15
thread 2's pid=140730530952960: number = 16
thread 3's pid=140730522560256: number = 17
thread 4's pid=140730514167552: number = 18
thread 1's pid=140730539345664: number = 19
thread 2's pid=140730530952960: number = 20
thread 3's pid=140730522560256: number = 21
thread 4's pid=140730514167552: number = 22
thread 1's pid=140730539345664: number = 23
thread 2's pid=140730530952960: number = 24
thread 3's pid=140730522560256: number = 25
thread 4's pid=140730514167552: number = 26
thread 1's pid=140730539345664: number = 27
thread 2's pid=140730530952960: number = 28
thread 3's pid=140730522560256: number = 29
thread 4's pid=140730514167552:主函數在等我完成任務嗎？
thread 1's pid=140730539345664:主函數在等我完成任務嗎？
thread 2's pid=140730530952960:主函數在等我完成任務嗎？
線程0已經結束
線程1已經結束
thread 3's pid=140730522560256:主函數在等我完成任務嗎？
線程2已經結束
線程3已經結束
*/