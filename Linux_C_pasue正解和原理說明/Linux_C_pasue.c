#include <stdio.h>

void main(void)
{
    int c;

    printf("Press any key to continue...\n");
    system("stty raw -echo");
    c = getchar();
    system("stty cooked echo");
}