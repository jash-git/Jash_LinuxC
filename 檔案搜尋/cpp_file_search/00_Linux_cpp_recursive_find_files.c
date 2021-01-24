#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
/*
http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
http://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1046380353&id=1044780608
*/
int walker( char *result ) {
  DIR           *d;
  struct dirent *dir;
  d = opendir( "." );
  if( d == NULL ) {
    return 1;
  }
  while( ( dir = readdir( d ) ) ) 
  {
	if( strcmp( dir->d_name, "." ) == 0 || strcmp( dir->d_name, ".." ) == 0 ) 
	{
		continue;
    }
    if( dir->d_type == DT_DIR )
	{
		chdir( dir->d_name );
		walker( result );
		chdir( ".." );
    } 
	else
	{
		int  len;
        getcwd( result, MAXPATHLEN );
        len = strlen( result );
        snprintf( result + len, MAXPATHLEN - len, "/%s", dir->d_name );
		printf( "Found: %s\n", result );
    }
  }
  closedir( d );
  return *result == 0;
}

int main( ) {
  char buf[MAXPATHLEN] = { 0 };
  if( walker(buf) == 0 ) {
    printf( "last Found: %s\n", buf );
  } else {
    puts( "Not found" );
  }
  return 0;
}
