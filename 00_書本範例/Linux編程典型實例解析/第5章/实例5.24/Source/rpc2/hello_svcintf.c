#include        "hello.h"

static   char* msg ;

char * RequestMessage( int ) ;

char ** requestmessage_1_svc(int *arg , struct svc_req * arg2 )
{
        int index ;

        index = (int)(*arg) ;

        msg = RequestMessage( index ) ;

        return  &msg ;
}
