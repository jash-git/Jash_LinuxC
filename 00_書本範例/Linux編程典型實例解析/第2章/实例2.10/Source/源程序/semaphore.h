//Semaphore.h
#include "unistd.h"
#include "pthread.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/sem.h>
//定义IPC键
#define TDS_S_KEY                (ftok(".", 's'))	
#define TDS_S_LOCK_INIT_VALUE       1
#define TDS_S_OPERATION_LOCK        (-1)
#define TDS_S_OPERATION_UNLOCK      1
//定义信号灯标志符，由semget创建，由semop和semctl使用
int iSemSetID;	
//定义semun联合
union semun{
   int val;
   struct semid_ds* buf;
   unsigned short *array;
   };
/*功能：	创建System V信号灯集					 */	
/*参数：	LockNums 输入参数，信号灯集中信号灯的数目	 */
/*返回值：成功返回1，失败返回0					 */
int CreateLocks(int LockNums){
   int i;
   union semun SemOperations;
   //调用semget创建System V信号灯	 		
   if(-1==(iSemSetID = semget ( TDS_S_KEY, LockNums, IPC_CREAT|0666 ))) {
      printf("该信号灯集已经存在。创建System V信号灯失败!\n");
      return 0;
   }
   //为新创建的信号灯集中的每个信号灯赋初值（－1）
   SemOperations.val = TDS_S_LOCK_INIT_VALUE;
   for(i=0; i<LockNums;++i)
      semctl(iSemSetID, i, SETVAL, SemOperations);

   return 1;
}
/*功能：	锁定指定的资源									*/
/*参数：	iLock 输入参数，锁定的资源序号，代表一个要锁定的资源	*/
/*返回值：成功返回1，失败返回0							*/
int LockResource(int iLock){
   struct sembuf SemLock;

   //锁定指定的资源
   SemLock.sem_num=iLock;
   SemLock.sem_op=TDS_S_OPERATION_LOCK;
   //设定该标志，线程异常退出时，资源会自动释放
SemLock.sem_flg=SEM_UNDO;//0; 
   if ((semop (iSemSetID, &SemLock, 1 )) == -1){
      printf("加锁失败\n");
      return 0;
   }
   return 1;
}
/*功能：	释放指定的资源								*/
/*参数：	iLock  输入参数，欲解锁的资源序号，代表要解锁的资源	*/
/*返回值：成功返回1，失败返回0						*/
int UnlockResource(int iLock){

   struct sembuf SemUnlock;
   int iLockValue;
   // 判定资源是否曾被锁定，如果没有被锁定过，则返回
   if(TDS_S_LOCK_INIT_VALUE==(iLockValue = GetLockValue(iLock))){
      printf("资源未被锁定过，不必释放\n");
      return 0;
   }
   //尝试解锁指定的资源
   SemUnlock.sem_num=iLock;
   SemUnlock.sem_op=TDS_S_OPERATION_UNLOCK;
   SemUnlock.sem_flg=SEM_UNDO;//0
   if ((semop (iSemSetID, &SemUnlock, 1)) == -1){
      printf("解锁失败\n");
      return 0;
   }
   return 1;
}
/*功能：	获取信号灯集中指定信号灯的值		*/
/*输入：	iLock 输入参数，信号灯的序号			*/
/*返回值：	信号灯的值，该值必须大于或等于0	*/
int GetLockValue (int iLock){
   return semctl (iSemSetID, iLock, GETVAL, 0);
}
/*功能：	删除System V信号灯集	*/
void RemoveLocks (void ){
   semctl ( iSemSetID, 0, IPC_RMID, 0 );
   printf ("System V信号灯被从系统中删除\n");
}
