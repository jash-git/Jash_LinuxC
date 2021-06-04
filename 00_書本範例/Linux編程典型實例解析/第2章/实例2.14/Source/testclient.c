#include "test.h"

#define RMACHINE "192.210.100.7"
CLIENT* handle;

int testvoid();
int teststring(char* szTest);

int main(int argc,char* argv)
{
  char szTest[100];
  strcpy(szTest,"hello");
  handle=clnt_create(RMACHINE,RTEST,RTESTVER,"tcp");
  if(handle==0)
  {
    printf("error to create handle\n");
    return 1;
  }

  testvoid();
  teststring(szTest);
  printf("%s\n",szTest);
  return 0;
}

int teststring(char* szTest)
{
  char** pszArg;
  int* ret;
  
  pszArg=&szTest;
  ret=teststring_1(pszArg,handle);
  return (ret==0?0:*ret);
}

int testvoid(void)
{
  int* ret;
  ret=testvoid_1(0,handle);
  return (ret==0?0:*ret);
}
