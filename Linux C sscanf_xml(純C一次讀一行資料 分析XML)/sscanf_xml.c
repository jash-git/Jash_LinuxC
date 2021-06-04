#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    printf("Hello world!\n");
    FILE *pf;
    int i=0;
    char buf[513];
    char buf0[100],buf1[100],buf2[100],buf3[100];
    pf=fopen("456.xml","r");
    while(fgets(buf,512,pf) != NULL)
    {
        i++;
        switch(i)
        {
            case 3:
				//printf("%d\t%s\n",i,buf);
				//printf("%d\n",(strstr(buf,">")-buf+1));
				//printf("%d\n",(strstr(buf,"</Account>")-buf+1));
				strncpy(buf0,(buf+(strstr(buf,">")-buf+1)),(strstr(buf,"</Account>")-buf+1)-(strstr(buf,">")-buf+1)-1);
				buf0[(strstr(buf,"</Account>")-buf+1)-(strstr(buf,">")-buf+1)-1]='\0';
				printf("%d\t%s\n",i,buf0);
                break;
            case 4:
				strncpy(buf1,(buf+(strstr(buf,">")-buf+1)),(strstr(buf,"</Password>")-buf+1)-(strstr(buf,">")-buf+1)-1);
				buf1[(strstr(buf,"</Password>")-buf+1)-(strstr(buf,">")-buf+1)-1]='\0';
				printf("%d\t%s\n",i,buf1);
                break;
            break;
            case 5:
				strncpy(buf2,(buf+(strstr(buf,">")-buf+1)),(strstr(buf,"</ChannelName>")-buf+1)-(strstr(buf,">")-buf+1)-1);
				buf2[(strstr(buf,"</ChannelName>")-buf+1)-(strstr(buf,">")-buf+1)-1]='\0';
				printf("%d\t%s\n",i,buf2);
                break;
            break;
            case 6:
				strncpy(buf3,(buf+(strstr(buf,">")-buf+1)),(strstr(buf,"</Url>")-buf+1)-(strstr(buf,">")-buf+1)-1);
				buf3[(strstr(buf,"</Url>")-buf+1)-(strstr(buf,">")-buf+1)-1]='\0';
				printf("%d\t%s\n",i,buf3);
                break;
            break;
        }
		//printf("%d\t%s\n",i,buf);
    }
    fclose(pf);
    return 0;
}

