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
	while(i<MAX)
	{
		pthread_mutex_lock(&mut);
			if(i<MAX)
			{
				number++;
			}
		    i++;
		pthread_mutex_unlock(&mut);
		printf("thread %d's i=%d: number = %d\n",value,(i-1),number);
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
        /*用是認屬性初始化互斥鎖*/
        i=0;
        pthread_mutex_init(&mut,NULL);
        printf("我是主函數哦，我正在創建線程，呵呵\n");
        thread_create();
        printf("我是主函數哦，我正在等待線程完成任務是，呵呵\n");
        thread_wait();
        return 0;
}
/*
http://www.tutorialspoint.com/compile_c_online.php
sh-4.2# gcc -o main *.c -pthread -std=c99            
sh-4.2# main                                         
我是主函數哦，我正在創建線程，呵呵                                  
線程0被創建                                               
thread 1's i=0: number = 1  
thread 1's i=1: number = 2                           
線程1被創建                                               
thread 2's i=2: number = 3                           
thread 1's i=3: number = 4                           
thread 2's i=4: number = 5                           
thread 3's i=5: number = 6                           
線程2被創建                                               
thread 1's i=6: number = 7                           
thread 3's i=7: number = 8                           
thread 2's i=8: number = 9                           
線程3被創建                                               
thread 4's i=9: number = 10                          
thread 1's i=10: number = 11                         
thread 3's i=11: number = 12                         
thread 2's i=12: number = 13                         
我是主函數哦，我正在等待線程完成任務是，呵呵                             
thread 4's i=13: number = 14                         
thread 1's i=14: number = 15                         
thread 3's i=15: number = 16                         
thread 2's i=16: number = 17                         
thread 4's i=17: number = 18                         
thread 1's i=18: number = 19                         
thread 3's i=19: number = 20                         
thread 2's pid=140404842510080:主函數在等我完成任務嗎？         
thread 4's pid=140404825724672:主函數在等我完成任務嗎？         
thread 1's pid=140404850902784:主函數在等我完成任務嗎？         
thread 3's pid=140404834117376:主函數在等我完成任務嗎？         
線程0已經結束                                              
線程1已經結束                                              
線程2已經結束                                              
線程3已經結束 
*/