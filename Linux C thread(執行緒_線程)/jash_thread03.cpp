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
        /*�άO�{�ݩʪ�l�Ƥ�����*/
        i=0;
        pthread_mutex_init(&mut,NULL);
        printf("�ڬO�D��Ʈ@�A�ڥ��b�Ыؽu�{�A����\n");
        thread_create();
        printf("�ڬO�D��Ʈ@�A�ڥ��b���ݽu�{�������ȬO�A����\n");
        thread_wait();
        return 0;
}
/*
http://www.tutorialspoint.com/compile_c_online.php
sh-4.2# gcc -o main *.c -pthread -std=c99            
sh-4.2# main                                         
�ڬO�D��Ʈ@�A�ڥ��b�Ыؽu�{�A����                                  
�u�{0�Q�Ы�                                               
thread 1's i=0: number = 1  
thread 1's i=1: number = 2                           
�u�{1�Q�Ы�                                               
thread 2's i=2: number = 3                           
thread 1's i=3: number = 4                           
thread 2's i=4: number = 5                           
thread 3's i=5: number = 6                           
�u�{2�Q�Ы�                                               
thread 1's i=6: number = 7                           
thread 3's i=7: number = 8                           
thread 2's i=8: number = 9                           
�u�{3�Q�Ы�                                               
thread 4's i=9: number = 10                          
thread 1's i=10: number = 11                         
thread 3's i=11: number = 12                         
thread 2's i=12: number = 13                         
�ڬO�D��Ʈ@�A�ڥ��b���ݽu�{�������ȬO�A����                             
thread 4's i=13: number = 14                         
thread 1's i=14: number = 15                         
thread 3's i=15: number = 16                         
thread 2's i=16: number = 17                         
thread 4's i=17: number = 18                         
thread 1's i=18: number = 19                         
thread 3's i=19: number = 20                         
thread 2's pid=140404842510080:�D��Ʀb���ڧ������ȶܡH         
thread 4's pid=140404825724672:�D��Ʀb���ڧ������ȶܡH         
thread 1's pid=140404850902784:�D��Ʀb���ڧ������ȶܡH         
thread 3's pid=140404834117376:�D��Ʀb���ڧ������ȶܡH         
�u�{0�w�g����                                              
�u�{1�w�g����                                              
�u�{2�w�g����                                              
�u�{3�w�g���� 
*/