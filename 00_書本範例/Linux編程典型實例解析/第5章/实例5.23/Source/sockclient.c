/*�ͻ�����*/
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

	sock = socket (PF_LOCAL, SOCK_DGRAM, 0);//�����׽���

	nameclient.sun_family = AF_LOCAL;
	strncpy (nameclient.sun_path, CLIENT, sizeof (nameclient.sun_path));
	sizeclient = SUN_LEN (&nameclient);//Ϊ�ͻ�sockaddr_in���ݽṹ��ֵ

	bind (sock, (struct sockaddr *) &nameclient, sizeclient);//����ַ�󶨵��ͻ������׽���

	nameserver.sun_family = AF_LOCAL;
	strcpy (nameserver.sun_path, SERVER);
	sizeserver = SUN_LEN (&nameserver);//Ϊ������sockaddr_in���ݽṹ��ֵ

	sendto (sock, MESSAGE, strlen (MESSAGE) + 1, 0,
                        (struct sockaddr *) &nameserver, sizeserver);//�������̷������ݣ�MESSAGE��
	fprintf (stdout, "Client: sent message: %s\n", MESSAGE);

	recvfrom (sock, message, MAXMSG, 0, NULL, 0);//�ȴ����������̷��ص���Ϣ

	fprintf (stdout, "Client: got message: %s\n", message);//���ն˷�����Ϣ

	remove (CLIENT);//����
	close (sock);//�ر��׽���
}
