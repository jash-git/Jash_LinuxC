//Semaphore.h
#include "unistd.h"
#include "pthread.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/sem.h>
//����IPC��
#define TDS_S_KEY                (ftok(".", 's'))	
#define TDS_S_LOCK_INIT_VALUE       1
#define TDS_S_OPERATION_LOCK        (-1)
#define TDS_S_OPERATION_UNLOCK      1
//�����źŵƱ�־������semget��������semop��semctlʹ��
int iSemSetID;	
//����semun����
union semun{
   int val;
   struct semid_ds* buf;
   unsigned short *array;
   };
/*���ܣ�	����System V�źŵƼ�					 */	
/*������	LockNums ����������źŵƼ����źŵƵ���Ŀ	 */
/*����ֵ���ɹ�����1��ʧ�ܷ���0					 */
int CreateLocks(int LockNums){
   int i;
   union semun SemOperations;
   //����semget����System V�źŵ�	 		
   if(-1==(iSemSetID = semget ( TDS_S_KEY, LockNums, IPC_CREAT|0666 ))) {
      printf("���źŵƼ��Ѿ����ڡ�����System V�źŵ�ʧ��!\n");
      return 0;
   }
   //Ϊ�´������źŵƼ��е�ÿ���źŵƸ���ֵ����1��
   SemOperations.val = TDS_S_LOCK_INIT_VALUE;
   for(i=0; i<LockNums;++i)
      semctl(iSemSetID, i, SETVAL, SemOperations);

   return 1;
}
/*���ܣ�	����ָ������Դ									*/
/*������	iLock �����������������Դ��ţ�����һ��Ҫ��������Դ	*/
/*����ֵ���ɹ�����1��ʧ�ܷ���0							*/
int LockResource(int iLock){
   struct sembuf SemLock;

   //����ָ������Դ
   SemLock.sem_num=iLock;
   SemLock.sem_op=TDS_S_OPERATION_LOCK;
   //�趨�ñ�־���߳��쳣�˳�ʱ����Դ���Զ��ͷ�
SemLock.sem_flg=SEM_UNDO;//0; 
   if ((semop (iSemSetID, &SemLock, 1 )) == -1){
      printf("����ʧ��\n");
      return 0;
   }
   return 1;
}
/*���ܣ�	�ͷ�ָ������Դ								*/
/*������	iLock  �������������������Դ��ţ�����Ҫ��������Դ	*/
/*����ֵ���ɹ�����1��ʧ�ܷ���0						*/
int UnlockResource(int iLock){

   struct sembuf SemUnlock;
   int iLockValue;
   // �ж���Դ�Ƿ��������������û�б����������򷵻�
   if(TDS_S_LOCK_INIT_VALUE==(iLockValue = GetLockValue(iLock))){
      printf("��Դδ���������������ͷ�\n");
      return 0;
   }
   //���Խ���ָ������Դ
   SemUnlock.sem_num=iLock;
   SemUnlock.sem_op=TDS_S_OPERATION_UNLOCK;
   SemUnlock.sem_flg=SEM_UNDO;//0
   if ((semop (iSemSetID, &SemUnlock, 1)) == -1){
      printf("����ʧ��\n");
      return 0;
   }
   return 1;
}
/*���ܣ�	��ȡ�źŵƼ���ָ���źŵƵ�ֵ		*/
/*���룺	iLock ����������źŵƵ����			*/
/*����ֵ��	�źŵƵ�ֵ����ֵ������ڻ����0	*/
int GetLockValue (int iLock){
   return semctl (iSemSetID, iLock, GETVAL, 0);
}
/*���ܣ�	ɾ��System V�źŵƼ�	*/
void RemoveLocks (void ){
   semctl ( iSemSetID, 0, IPC_RMID, 0 );
   printf ("System V�źŵƱ���ϵͳ��ɾ��\n");
}
