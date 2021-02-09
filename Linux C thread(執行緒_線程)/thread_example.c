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
	printf("thread1 :�D��Ʀb���ڧ������ȶܡH\n");
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
	printf("thread2 :�D��Ʀb���ڧ������ȶܡH\n");
	pthread_exit(NULL);
}
void thread_create(void)
{
	int temp;
	memset(&thread, 0, sizeof(thread));          //comment1
	/*�Ыؽu�{*/
	if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)       //comment2
	{
		printf("�u�{1�Ыإ���!\n");
	}
	else
	{
		printf("�u�{1�Q�Ы�\n");
	}
	if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)  //comment3
	{	
		printf("�u�{2�Ыإ���");
	}
	else
	{
		printf("�u�{2�Q�Ы�\n");
	}
}
void thread_wait(void)
{
	/*���ݽu�{����*/
	if(thread[0] !=0)
	{//comment4
		pthread_join(thread[0],NULL);
		printf("�u�{1�w�g����\n");
	}
	if(thread[1] !=0)
	{                //comment5
		pthread_join(thread[1],NULL);
		printf("�u�{2�w�g����\n");
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

