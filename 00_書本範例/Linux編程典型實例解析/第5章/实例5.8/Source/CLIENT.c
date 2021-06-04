#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 13
#define  MAXDATASIZE 512
int main(int argc,char *argv[])
{
  int sockfd,nbytes;
  char buf[1024];
  struct hostent *he;
  struct sockaddr_in srvaddr;

if(argc!=2){
  perror("usage:client hostname\n");
  exit(1);
}
if((he=gethostbyname(argv[1]))==NULL){
  perror("gethostbyname");
  exit(1);
}

if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
{
  perror("create socket error");
  exit(1);
} 
bzero(&srvaddr,sizeof(srvaddr));
srvaddr.sin_family=AF_INET;
srvaddr.sin_port=htons(PORT);
srvaddr.sin_addr=*((struct in_addr *)he->h_addr);

if(connect(sockfd,(struct sockaddr *)&srvaddr,sizeof(struct sockaddr))==-1)
{
  perror("connect error");
  exit(1);
}
if((nbytes=read(sockfd,buf,MAXDATASIZE))==-1){

  perror("read error");
  exit(1);
}
buf[nbytes]='\0';
printf("\nread: %s\n",buf);
close(sockfd);
}
