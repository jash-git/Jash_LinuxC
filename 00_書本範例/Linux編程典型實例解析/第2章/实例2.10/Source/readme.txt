
��Ŀ¼����3��Ŀ¼��

1��Դ����
�����ŵ���System V�źŵƵ�Դ��������������ļ�test.c��semaphore.h��
��Ŀ¼�µ��ļ����������Ӻ�����ɲ���1Ŀ¼�µĿ�ִ���ļ���


2������1�� 
�����ŵ��ǲ��Ա���������Դ�Զ��ͷŵĿ�ִ�г���test��

3������2�� 
�����ŵ��ǲ���������Դ���̱߳�ͻȻɱ���󣬳���������״̬�Ŀ�ִ�г���test��
Ҫ���ɸÿ�ִ�г���Ҫ�޸�semaphore.h�е�LockResource��UnlockResource�Ĵ��룬
	   
�ֱ�	SemLock.sem_flg=SEM_UNDO;	��	SemUnlock.sem_flg=SEM_UNDO;	
��Ϊ��
	SemLock.sem_flg=0;		��	SemUnlock.sem_flg=0;
Ȼ�����±������ӳ���

�������ӵĻ�������������RedHat7.0�±�������ͨ����
����������Ϊ:> gcc -o test test.c -lpthread
���иó���:>   ./test








