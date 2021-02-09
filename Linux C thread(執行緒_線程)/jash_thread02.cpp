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
	printf("thread %d's pid=%ld:�D��Ʀb���ڧ������ȶܡH\n",value,count);
	pthread_exit(NULL);
}
void thread_create(void)
{
	int temp;
	int j[4];//�n�ǻ�����Ƴ��ݭn���W���ܼ�
	memset(&thread, 0, sizeof(thread));          //comment1
	/*�Ыؽu�{*/
	for(int i=0;i<4;i++)
	{
		j[i]=i+1;
		if((temp = pthread_create(&thread[i], NULL, thread_fun, &j[i]))!= 0)       //comment2
		{
			printf("�u�{%d�Ыإ���!\n",i);
		}
		else
		{
			printf("�u�{%d�Q�Ы�\n",i);
		}
		sleep(1);//����u�{�إ߳t��,�_�h�L�k���T�ǻ��ܼ�
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
/*
gcc example.cpp -o example -pthread
�ڬO�D��Ʈ@�A�ڥ��b�Ыؽu�{�A����
�u�{0�Q�Ы�
thread 1's pid=140730539345664: number = 1
�u�{1�Q�Ы�
thread 1's pid=140730539345664: number = 2
thread 2's pid=140730530952960: number = 3
�u�{2�Q�Ы�
thread 1's pid=140730539345664: number = 4
thread 2's pid=140730530952960: number = 5
thread 3's pid=140730522560256: number = 6
�u�{3�Q�Ы�
thread 1's pid=140730539345664: number = 7
thread 2's pid=140730530952960: number = 8
thread 3's pid=140730522560256: number = 9
thread 4's pid=140730514167552: number = 10
�ڬO�D��Ʈ@�A�ڥ��b���ݽu�{�������Ȫ��A����
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
thread 4's pid=140730514167552:�D��Ʀb���ڧ������ȶܡH
thread 1's pid=140730539345664:�D��Ʀb���ڧ������ȶܡH
thread 2's pid=140730530952960:�D��Ʀb���ڧ������ȶܡH
�u�{0�w�g����
�u�{1�w�g����
thread 3's pid=140730522560256:�D��Ʀb���ڧ������ȶܡH
�u�{2�w�g����
�u�{3�w�g����
*/