#include <stdio.h>

void PAUSE(void)
{
	printf("Press any key to continue... ...");
	getchar();
}
int main(int argc, char **argv)
{
	PAUSE();
}
