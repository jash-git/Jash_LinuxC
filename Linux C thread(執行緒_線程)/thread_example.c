/*thread_example.c :  c multiple thread programming in linux
  *author : falcon
  *E-mail : tunzhj03@st.lzu.edu.cn
  *Web URL : http://zhuwenlong.blog.51cto.com/blog/209020/40339
*/
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#define MAX 10

pthread_t thread[2];
pthread_mutex_t mut;
int number;
int i;
void *thread1()
{
	printf ("thread1 : I'm thread 1\n");
	for (i = 0; i < MAX; i++)
	{
		printf("thread1 : number = %d\n",number);
		pthread_mutex_lock(&mut);
			number++;
		pthread_mutex_unlock(&mut);
		sleep(2);
	}
	printf("thread1 :主函數在等我完成任務嗎？\n");
	pthread_exit(NULL);
}
void *thread2()
{
	printf("thread2 : I'm thread 2\n");
	for (i = 0; i < MAX; i++)
	{
		printf("thread2 : number = %d\n",number);
		pthread_mutex_lock(&mut);
			number++;
		pthread_mutex_unlock(&mut);
		sleep(3);
	}
	printf("thread2 :主函數在等我完成任務嗎？\n");
	pthread_exit(NULL);
}
void thread_create(void)
{
	int temp;
	memset(&thread, 0, sizeof(thread));          //comment1
	/*創建線程*/
	if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)       //comment2
	{
		printf("線程1創建失敗!\n");
	}
	else
	{
		printf("線程1被創建\n");
	}
	if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)  //comment3
	{	
		printf("線程2創建失敗");
	}
	else
	{
		printf("線程2被創建\n");
	}
}
void thread_wait(void)
{
	/*等待線程結束*/
	if(thread[0] !=0)
	{//comment4
		pthread_join(thread[0],NULL);
		printf("線程1已經結束\n");
	}
	if(thread[1] !=0)
	{                //comment5
		pthread_join(thread[1],NULL);
		printf("線程2已經結束\n");
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

