#include <stdio.h>
#include <time.h>
int main(int argc,char*argv[])
{
	while(1)
	{
		int inthr,intmin,intsec;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		if(argc>3)
		{
			inthr=atoi(argv[1]);
			intmin=atoi(argv[2]);
			intsec=atoi(argv[2]);
			if((inthr==tm.tm_hour)&&(tm.tm_min==intmin)&&(intsec==tm.tm_sec))
			{
				printf("Run APP\n");
				break;
			}
		}
		else
		{
			printf("NO par\n");
			break;
		}
	}
	
}