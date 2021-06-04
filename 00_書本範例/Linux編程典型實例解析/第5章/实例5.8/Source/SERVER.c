#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#define MYPORT 13
#define  BACKLOG 5
main()
{
  int sockfd,new_fd;
  struct sockaddr_in srvaddr;
  struct sockaddr_in cliaddr;
  int sin_size;
  time_t tick;
  char buf[1024];
  pid_t childpid;
if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
{
  perror("socket error");
  exit(1);
} 
bzero(&srvaddr,sizeof(srvaddr));
srvaddr.sin_family=AF_INET;
srvaddr.sin_port=htons(MYPORT);
srvaddr.sin_addr.s_addr=htonl(INADDR_ANY);
if(bind(sockfd,(struct sockaddr *)&srvaddr,sizeof(struct sockaddr))==-1)
{
  perror("bind error");
  exit(1);
}
if(listen(sockfd,BACKLOG)==-1){
  perror("listen error");
  exit(1);
}
for(;;)
{
  new_fd=accept(sockfd,(struct sockaddr *)NULL,NULL);
  if(new_fd<0)
     perror("accept error");
  if(fork()==0)
  {
     close(sockfd);
     tick=time(NULL);
     snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&tick));
     write(new_fd,buf,strlen(buf));
     exit(0);  
   }
   close(new_fd);
 }
}
