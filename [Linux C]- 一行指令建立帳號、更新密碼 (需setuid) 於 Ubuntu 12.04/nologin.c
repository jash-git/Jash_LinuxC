#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#define CMD_ADDUSER "sudo adduser --quiet --gecos \"\" --disabled-login --no-create-home --shell \"/usr/sbin/nologin\" "
#define CMD_CHECK_USER_EXISTS "getent passwd"
#define MAX_BUFFER_LINE 1024
int main(int argc, char *argv[])
{
   char buf[MAX_BUFFER_LINE+1], pass[MAX_BUFFER_LINE+1], *cmd;
   FILE *fp;
   setuid(0); // use root
   //system("id");
   if( argc < 3 )
   {
      fprintf( stderr, "Usage> %s username [ - | password]\n\t%s username -\t\t(read password from stdin)\n\t%s username password\n\n\tothers: sudo chown root %s && sudo chmod 4755 %s\n" , argv[0], argv[0], argv[0] , argv[0], argv[0] );
      exit(1);
   }

   memset( buf, 0, MAX_BUFFER_LINE + 1);
   memset( pass, 0, MAX_BUFFER_LINE + 1);
   if( argv[2][0] != '-' )
   {
      strncpy( pass, argv[2], MAX_BUFFER_LINE );
      pass[MAX_BUFFER_LINE] = '\0';
   }
   else if( !feof( stdin ) && fgets( pass, MAX_BUFFER_LINE, stdin ) > 0 )
   {
      //printf("Pass:[%s]\n",pass);
   }

   if( strlen(pass) < 1 )
   {
      fprintf( stderr, "Error @ Init: password is empty\n" );
      exit(1);
   }

   // check account exists
   cmd = buf;
   snprintf( buf, MAX_BUFFER_LINE, "%s %s", CMD_CHECK_USER_EXISTS, argv[1] );
   fp = popen( cmd , "r" );

   memset( buf, 0, MAX_BUFFER_LINE + 1);
   fgets( buf, MAX_BUFFER_LINE, fp );
   pclose(fp);

   if( !strlen(buf) ) // create the account if not exists
   {
      // add account
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      cmd = buf;
      snprintf( buf, MAX_BUFFER_LINE, "%s %s", CMD_ADDUSER, argv[1] );
      pclose( popen( cmd , "r" ) );

      // query the account
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      cmd = buf;
      snprintf( buf, MAX_BUFFER_LINE, "%s %s", CMD_CHECK_USER_EXISTS, argv[1] );
      fp = popen( cmd , "r" );
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      fgets( buf, MAX_BUFFER_LINE, fp );
      pclose(fp);

      if( !strstr( buf, argv[1] ) ) // user is not created
      {
         printf("Error @ create an account: user cannot be created\n");
         exit(1);
      }

      // change password
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      cmd = buf;
      snprintf( buf, MAX_BUFFER_LINE, "echo \"%s:%s\" | chpasswd ", argv[1] , pass );
      fp = popen( cmd , "r" );
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      fgets( buf, MAX_BUFFER_LINE, fp );
      pclose(fp);
      if( !strlen(buf) )
         printf("OK\n");
   }
   else if( strstr( buf, "/bin/false" ) || strstr( buf, "/nologin" ) ) // change password
   {
      // change password
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      cmd = buf;
      snprintf( buf, MAX_BUFFER_LINE, "echo \"%s:%s\" | chpasswd ", argv[1] , pass );
      fp = popen( cmd , "r" );
      memset( buf, 0, MAX_BUFFER_LINE + 1);
      fgets( buf, MAX_BUFFER_LINE, fp );
      pclose(fp);
      if( !strlen(buf) )
         printf("OK\n");
   }
   else
   {
      printf("SKIP @ cannot change the password for '%s'.\n", argv[1]);
   }
   return 0;
}
