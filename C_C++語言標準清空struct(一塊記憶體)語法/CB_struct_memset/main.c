#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _Data{
	int i;
	int j;
	char strArray[10];
}Data;
int main()
{
	Data *d1,d2;
	d1=(Data*)malloc(sizeof(Data));
		printf("%d\t%d\t%s\n",d1[0].i,d1[0].j,d1[0].strArray);//���M��
		printf("%d\t%d\t%s\n",d2.i,d2.j,d2.strArray);//���M��
	memset(d1,0,sizeof(d1[0]));//C�y���зǲM��struct(�@���O����)�y�k
	memset(&d2,0,sizeof(Data));//C�y���зǲM��struct(�@���O����)�y�k
		printf("%d\t%d\t%s\n",d1[0].i,d1[0].j,d1[0].strArray);
		printf("%d\t%d\t%s\n",d2.i,d2.j,d2.strArray);
	memset(d1,'A',sizeof(d1[0]));//�M�����~
	memset(&d2,'A',sizeof(Data));//�M�����~
		printf("%d\t%d\t%s\n",d1[0].i,d1[0].j,d1[0].strArray);
		printf("%d\t%d\t%s\n",d2.i,d2.j,d2.strArray);
    free(d1);
	system("pause");
    return 0;
}
