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
	printf("thread %ld:�D��Ʀb���ڧ������ȶܡH\n",count);
	pthread_exit(NULL);
}
void thread_create(void)
{
	int temp;
	memset(&thread, 0, sizeof(thread));          //comment1
	/*�Ыؽu�{*/
	for(int i=0;i<4;i++)
	{
		if((temp = pthread_create(&thread[i], NULL, thread_fun, NULL))!= 0)       //comment2
		{
			printf("�u�{%d�Ыإ���!\n",i);
		}
		else
		{
			printf("�u�{%d�Q�Ы�\n",i);
		}
	}
}
void thread_wait(void)
{
	/*���ݽu�{����*/
	for(int i=0;i<4;i++)
	{
		if(thread[i] !=0)
		{//comment4
			pthread_join(thread[i],NULL);
			printf("�u�{%d�w�g����\n",i);
		}
	}
}
int main()
{
		number=0;
        /*���q�{�ݩʪ�l�Ƥ�����*/
        pthread_mutex_init(&mut,NULL);
        printf("�ڬO�D��Ʈ@�A�ڥ��b�Ыؽu�{�A����\n");
        thread_create();
        printf("�ڬO�D��Ʈ@�A�ڥ��b���ݽu�{�������Ȫ��A����\n");
        thread_wait();
        return 0;
}