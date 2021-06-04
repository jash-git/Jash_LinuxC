#include "test.h"

static int ret;

int* testvoid_1_svc(void* v,struct svc_req* s)
{
  ret=testvoid();
  return (&ret);
}

int* teststring_1_svc(char** word,struct svc_req* s)
{
  ret=teststring(*word);
  strcat(*word,"hehe");
  return (&ret);
}


