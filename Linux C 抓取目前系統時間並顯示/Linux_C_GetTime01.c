#include <stdio.h>
#include <time.h>
int main(void)
{
	time_t t = time(NULL);//epoch time
	t+=28800;//+8hr---->台灣時間
	struct tm tm = *gmtime(&t);//epoch time---->UTC
	printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}