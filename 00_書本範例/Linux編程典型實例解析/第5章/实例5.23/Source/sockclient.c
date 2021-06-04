/*客户进程*/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#define CLIENT  "/tmp/clientsocket"
#define MESSAGE "Hello World!"
#define MAXMSG  512
#define SERVER  "/tmp/serversocket"

int main()
{
	struct sockaddr_un nameclient, nameserver;
	int sock;
	size_t sizeclient, sizeserver;
	char message[MAXMSG];

	sock = socket (PF_LOCAL, SOCK_DGRAM, 0);//创建套接字

	nameclient.sun_family = AF_LOCAL;
	strncpy (nameclient.sun_path, CLIENT, sizeof (nameclient.sun_path));
	sizeclient = SUN_LEN (&nameclient);//为客户sockaddr_in数据结构赋值

	bind (sock, (struct sockaddr *) &nameclient, sizeclient);//将地址绑定到客户进程套接字

	nameserver.sun_family = AF_LOCAL;
	strcpy (nameserver.sun_path, SERVER);
	sizeserver = SUN_LEN (&nameserver);//为服务器sockaddr_in数据结构赋值

	sendto (sock, MESSAGE, strlen (MESSAGE) + 1, 0,
                        (struct sockaddr *) &nameserver, sizeserver);//向服务进程发送数据（MESSAGE）
	fprintf (stdout, "Client: sent message: %s\n", MESSAGE);

	recvfrom (sock, message, MAXMSG, 0, NULL, 0);//等待服务器进程返回的信息

	fprintf (stdout, "Client: got message: %s\n", message);//向终端反馈信息

	remove (CLIENT);//清理
	close (sock);//关闭套接字
}
