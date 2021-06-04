
本目录下有3个目录：

1、源程序：
这里存放的是System V信号灯的源代码程序，有两个文件test.c和semaphore.h。
本目录下的文件经编译连接后会生成测试1目录下的可执行文件。


2、测试1： 
这里存放的是测试被锁定的资源自动释放的可执行程序test。

3、测试2： 
这里存放的是测试锁定资源的线程被突然杀死后，程序处于阻塞状态的可执行程序test。
要生成该可执行程序，要修改semaphore.h中的LockResource和UnlockResource的代码，
	   
分别将	SemLock.sem_flg=SEM_UNDO;	和	SemUnlock.sem_flg=SEM_UNDO;	
改为：
	SemLock.sem_flg=0;		和	SemUnlock.sem_flg=0;
然后重新编译连接程序。

编译连接的环境：本程序在RedHat7.0下编译连接通过。
连接命令行为:> gcc -o test test.c -lpthread
运行该程序:>   ./test








