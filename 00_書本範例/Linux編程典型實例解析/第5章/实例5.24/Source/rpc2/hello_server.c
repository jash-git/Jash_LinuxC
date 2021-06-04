#include        <rpc/rpc.h>
#include        <stdlib.h>
#include        <stdio.h>
#include        "hello.h"

char  * stringarray[ ] = { "Hello!" ,"This" , "is" , "a", "RPC" ,"Application" };

char  * RequestMessage( int index )
{
     return stringarray[ index ] ;
}
