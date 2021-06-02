view plaincopy to clipboardprint?
/* 
 
  
 
Filename    : Linux_timer.cpp 
 
Compiler    : gcc 4.1.0 on Fedora Core 5 
 
Description : setitimer() set the interval to run function 
 
Synopsis    : #include <sys/time.h> 
 
              int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue); 
 
              struct itimerval { 
 
                struct timerval it_interval; 
 
                struct timerval it_value; 
 
              }; 
 
  
 
              struct timeval { 
 
                long tv_sec; 
 
                long tv_usec; 
 
              }             
 
Release     : 11/25/2006 

setitimer()為Linux的API，並非C語言的Standard Library，setitimer()有兩個功能，一是指定一段時間後，才執行某個function，二是每間格一段時間就執行某個function，以下程式demo如何使用setitimer()。

當setitimer()所執行的timer時間到了，會呼叫SIGALRM signal，所以在第30行用signal()將要執行的function指定給SIGALRM。 

在第43行呼叫setitimer()設定timer，但setitimer()第二個參數是sturct，負責設定timeout時間，所以第36行到第 40行設定此struct。itimerval.it_value設定第一次執行function所延遲的秒數， itimerval.it_interval設定以後每幾秒執行function，所以若只想延遲一段時間執行function，只要設定 itimerval.it_value即可，若要設定間格一段時間就執行function，則it_value和it_interval都要設定，否則 funtion的第一次無法執行，就別說以後的間隔執行了。 第36行和第39行的tv_sec為sec，第37行和40行為micro sec(0.001 sec)。

第43行的第一個參數ITIMER_REAL，表示以real-time方式減少timer，在timeout時會送出SIGALRM signal。第三個參數會存放舊的timeout值，如果不需要的話，指定NULL即可。 第47 行的pause()，命令系統進入sleep狀態，等待任何signal，一定要用while(1)無窮迴圈執行pause()，如此才能一直接收 SIGALRM signal以間隔執行function，若拿掉while(1)，則function只會執行一次而已。

*/  
  
#include <stdio.h>    // for printf()  
 
#include <unistd.h>   // for pause()  
  
#include <signal.h>   // for signal()  
 
#include <string.h>   // for memset()  
  
#include <sys/time.h> // struct itimeral. setitimer()  
  
  
  
void printMsg(int);  
  
  
  
int main() {  
  
  // Get system call result to determine successful or failed  
  
  int res = 0;  
  
  // Register printMsg to SIGALRM  
  
  signal(SIGALRM, printMsg);  
  
    
  
  struct itimerval tick;  
  
  // Initialize struct  
  
  memset(&tick, 0, sizeof(tick));  
  
  // Timeout to run function first time  
  
  tick.it_value.tv_sec = 1;  // sec  
  
  tick.it_value.tv_usec = 0; // micro sec.  
  
  // Interval time to run function  
  
  tick.it_interval.tv_sec = 1;  
  
  tick.it_interval.tv_usec = 0;  
  
  // Set timer, ITIMER_REAL : real-time to decrease timer,  
  
  //                          send SIGALRM when timeout  
  
  res = setitimer(ITIMER_REAL, &tick, NULL);  
  
  if (res) {  
  
    printf("Set timer failed!!\n");  
  
  }  
  
  
  
  // Always sleep to catch SIGALRM signal  
  
  while(1) {  
  
    pause();  
  
  }  
  
  
  
  return 0;    
  
}  
  
  
  
void printMsg(int num) {  
  
  printf("%s","Hello World!!\n");  
  
}  