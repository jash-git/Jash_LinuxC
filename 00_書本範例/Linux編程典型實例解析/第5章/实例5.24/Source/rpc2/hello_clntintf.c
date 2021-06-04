#include        "rpc/rpc.h"
#include        "stdio.h"
#include        "hello.h"

extern  CLIENT	* handle ;

static  char	** mesg ;

char * RequestMessage( int index )
{
        int  * arg ;

        arg = & index ;

        mesg = requestmessage_1( arg , handle ) ;

        if( mesg == NULL )

                return NULL ;
        else
                return *mesg ;
}
