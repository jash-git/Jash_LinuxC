/*
	http://webcache.googleusercontent.com/search?q=cache:6_N6So_JZgMJ:blog.163.com/ky_147258/blog/static/14288004920118685640778/+&cd=9&hl=zh-TW&ct=clnk&lr=lang_zh-CN%7Clang_zh-TW
	linux 獲取進程ＩＤ以及popen用法等
	掃描ps命令的輸出: 
*/
/*
pidof :查找運行程式的進程ID/通過進程名查看進程ID

語 法
pidof[必要參數][選擇參數][程式]
功 能
pidof 命令：可以顯示正在運行的進程ID
參數
必要參數
-s 單次選中
-c 只返回進程ID
-x 同時返回腳本

選擇參數
-o<進程號>
範例2： 每次只返回一個進程號 

root@snail-hnlinux:~# pidof sshd -s
31327
*/
#include <stdio.h>  
#include <string.h>  
#include <sys/types.h>  
   
#define PS_COMMAND "ps -ax"  
#define SCAN(buf) (sscanf(buf,"%d %s %s %s %s",pid,s[0],s[1],s[2],cmd) == 5)  
   
/* Returns number of pid matching process name, or -1 on error */  
int lookup_pid(char *name, pid_t *pids, int n)  
{  
  FILE *fp;  
  char buf[256], cmd[200], s[3][32];  
  pid_t *pid;  
   
  if ((fp = popen(PS_COMMAND, "r")) == NULL)  
    return -1;  
   
  pid = pids;  
  while (n > 0 && fgets(buf, sizeof(buf), fp))  
    if (SCAN(buf) && strstr(cmd, name)) /* improve on this simple name check */  
      pid++, n--;  
   
  fclose(fp);  
  return pid - pids;  
}  
   
int main(int argc, char **argv)  
{  
  char *process;  
  pid_t pids[8];  
  int n, i;  
   
  if (argc != 2) {  
    printf("Usage: %s process_name  ", argv[0]);  
    return 1;  
  }  
  process = argv[1];  
   
  if ((n = lookup_pid(process, pids, 8)) == -1) {  
    printf("Error reading pid of %s  ", process);  
    return 2;  
  }  
   
  printf("%d %s  ", n, process);  
  for (i = 0; i < n; i++)  
    printf("%d  ", pids[ i]);  
  return 0;  
}
