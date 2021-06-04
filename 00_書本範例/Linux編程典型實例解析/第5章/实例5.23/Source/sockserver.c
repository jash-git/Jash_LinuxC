/*服务进程*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

#define SERVER "/tmp/serversocket"
#define MAXMSG 512

int main()
{
	int sock;
	char message[MAXMSG];
	struct sockaddr_un servername, clientname;
	size_t serversize, clientsize;
	int nbytes;

	unlink(SERVER);

	sock = socket (PF_LOCAL, SOCK_DGRAM, 0);//创建服务套接字

	servername.sun_family = AF_LOCAL;//服务套接字地址格式
	strncpy (servername.sun_path, SERVER, sizeof (servername.sun_path));
	serversize = SUN_LEN (&servername);

	bind (sock, (struct sockaddr *) &servername, serversize);//服务进程套接字绑定到name
	while(1)
	{
		clientsize = sizeof (clientname);
		nbytes = recvfrom (sock, message, MAXMSG, 0,
                              (struct sockaddr *) &clientname, &clientsize);//接收数据，客户进程套接字地址被存储在（*clientname）中
		
		
		fprintf (stdout, "Server: received message: %s\n", message);
		sendto (sock, message, nbytes, 0,
                            (struct sockaddr *) & clientname, clientsize);//向客户进程返回地址，字节数nbytes
		fprintf (stdout, "Server: sentback message: %s\n", message);
		
	}
}
