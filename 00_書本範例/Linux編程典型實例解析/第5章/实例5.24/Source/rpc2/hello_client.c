#include        "rpc/rpc.h"
#include        "stdlib.h"
#include        "stdio.h"
#include        "hello.h"

#define			STRINGNUM		6
#define         REMOTEMACHINE   "localhost"

CLIENT          *handle ;

char  * RequestMessage( int ) ;

int   main( )
{
        int     i ;

        char    *str1 ;
        
        handle = clnt_create( REMOTEMACHINE , HELLOWORLDPROG , HELLOWORLDVERS , "tcp" ) ;

        if( handle == 0 )
        {
                printf( "can not connect to the remote procedure\n" ) ;

                exit( 1 ) ;
        }


		for( i = 0 ; i < STRINGNUM ; i ++)
        {
			str1 = RequestMessage( i ) ;

			printf( "%s " , str1 )  ;
		}
	
		printf( ".\n" )  ;

        return 1 ;
}
