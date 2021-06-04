#include "test.h"

int testvoid(void)
{
  printf("void\n");
  return 0;
}

int teststring(char * szTest)
{
  //strcat(szTest,"hi");
  printf("The test string is :%s\n",szTest);
  strcat(szTest,"hi");
  return 0;
}
