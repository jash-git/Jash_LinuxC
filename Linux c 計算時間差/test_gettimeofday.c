/*
	Linux c 計算時間差
	功能:可計算毫秒和微秒的時間差
	資料來源:http://blog.jeoygin.org/2012/03/c-get-time-millisecond.html

*/
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>// sleep(3);
#include <sys/timeb.h>//timeb
 long long getSystemTime() {
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}
int main() {
    struct timeval start, end;
    gettimeofday( &start, NULL );
    sleep(3);
    gettimeofday( &end, NULL );
    int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    printf("time: %d us\n", timeuse);
////////////////////////////////////////////////////////////
    long long start1=getSystemTime();
    sleep(3);
    long long end1=getSystemTime();
 
    printf("time: %lld ms\n", end1-start1);
    return 0;
}
