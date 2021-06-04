#include "semaphore.h"
//������Դ������Դ���źŵƼ��е�һ���źŵ����Ӧ
#define TDS_RESOURCE_1 1	
//���ܺ���1
void* func_1 (void* data)
{
  int i;
  int threadNO=*((int*)data); 
  LockResource (TDS_RESOURCE_1); 	//����Դ����
  for(i=0;i<5;i++)	//��ӡ5����Ϣ
  {
    sleep (1);	//�úķѵ�ʱ�����ִ��ĳ����ҵ
    printf("thread %d' -Current Value- '%d'\n", threadNO,i);
  }
  UnlockResource (TDS_RESOURCE_1);	//����Դ����
  pthread_exit(NULL);
}
//���ܺ���2
void* func_2 (void* data)
{
  int i;
  int threadNO =*((int*)data);
  LockResource(TDS_RESOURCE_1);		//����Դ����
  for(i=0;i<5;i++)	//��ӡ5����Ϣ
  {
    sleep (1);	//�úķѵ�ʱ�����ִ��ĳ����ҵ
    printf ("thread %d' -Current Value- '%d'\n", threadNO,i);
  }
  UnlockResource (TDS_RESOURCE_1);	//����Դ����
  pthread_exit (NULL);
}

int main (void)
{
  pthread_t p_thread1,p_thread2;
  int a=1;
  int b=2;
  //����System V�źŵ�,ָ���źŵƼ�����16���źŵ�
  if (CreateLocks (16)==0)     return 0;
  //����ִ�����߳�
  pthread_create (&p_thread1, NULL, (void*)&func_1, (void*)&a);
  pthread_create (&p_thread2, NULL, (void*)&func_2, (void*)&b);
  sleep (3); //�������Ϊ�����߳�1��2����һ���

  //���ĳ����������,���Ҹ�����ָ���߳�1��Ӧ���ټ�������,��ɱ���߳�1
  if(1)
    pthread_cancel (p_thread1);

  //�������е������ǵȴ�p_thread1��p_thread2ִ�����main�������˳�
  pthread_join (p_thread1,NULL); 
  pthread_join (p_thread2,NULL);

  //�ͷ�System V�źŵ�  
  RemoveLocks ();
  return 1;
}
