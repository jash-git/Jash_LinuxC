#include <stdio.h>
#include <stdlib.h>
#define BRIGHT 1
#define RED 31
#define BG_BLACK 40
void print_info_line(char *data)
{
	printf("%s---%d\t%s\n",__FILE__, __LINE__,data);
}
int main(void)
{
	char *data="jash test";
	printf("%c[%d;%d;%dmHello World\n", 0x1B, BRIGHT,RED,BG_BLACK);
	print_info_line(data);
	return 0;
}
