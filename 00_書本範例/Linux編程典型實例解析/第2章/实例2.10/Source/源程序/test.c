#include "semaphore.h"
//定义资源，该资源与信号灯集中的一个信号灯相对应
#define TDS_RESOURCE_1 1	
//功能函数1
void* func_1 (void* data)
{
  int i;
  int threadNO=*((int*)data); 
  LockResource (TDS_RESOURCE_1); 	//对资源加锁
  for(i=0;i<5;i++)	//打印5行信息
  {
    sleep (1);	//用耗费的时间代表执行某种作业
    printf("thread %d' -Current Value- '%d'\n", threadNO,i);
  }
  UnlockResource (TDS_RESOURCE_1);	//对资源解锁
  pthread_exit(NULL);
}
//功能函数2
void* func_2 (void* data)
{
  int i;
  int threadNO =*((int*)data);
  LockResource(TDS_RESOURCE_1);		//对资源加锁
  for(i=0;i<5;i++)	//打印5行信息
  {
    sleep (1);	//用耗费的时间代表执行某种作业
    printf ("thread %d' -Current Value- '%d'\n", threadNO,i);
  }
  UnlockResource (TDS_RESOURCE_1);	//对资源解锁
  pthread_exit (NULL);
}

int main (void)
{
  pthread_t p_thread1,p_thread2;
  int a=1;
  int b=2;
  //创建System V信号灯,指定信号灯集中有16个信号灯
  if (CreateLocks (16)==0)     return 0;
  //创建执行者线程
  pthread_create (&p_thread1, NULL, (void*)&func_1, (void*)&a);
  pthread_create (&p_thread2, NULL, (void*)&func_2, (void*)&b);
  sleep (3); //该语句是为了让线程1和2运行一会儿

  //如果某种条件成立,并且该条件指明线程1不应当再继续运行,则杀死线程1
  if(1)
    pthread_cancel (p_thread1);

  //下面两行的作用是等待p_thread1和p_thread2执行完后main函数才退出
  pthread_join (p_thread1,NULL); 
  pthread_join (p_thread2,NULL);

  //释放System V信号灯  
  RemoveLocks ();
  return 1;
}
