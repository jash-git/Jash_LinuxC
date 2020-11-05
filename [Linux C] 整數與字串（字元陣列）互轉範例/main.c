#include <stdlib.h>
#include <stdio.h>
int main(void)
{
  int n;
  char buf[30];
  char *str = "12345.67";
  n = atoi(str);
  printf("string = %s integer =%d\n", str, n);

  sprintf(buf,"%d",n);
  printf("string = %s integer =%d\n", buf, n);
  return 0;
}