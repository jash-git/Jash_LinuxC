/*�������*/
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

	sock = socket (PF_LOCAL, SOCK_DGRAM, 0);//���������׽���

	servername.sun_family = AF_LOCAL;//�����׽��ֵ�ַ��ʽ
	strncpy (servername.sun_path, SERVER, sizeof (servername.sun_path));
	serversize = SUN_LEN (&servername);

	bind (sock, (struct sockaddr *) &servername, serversize);//��������׽��ְ󶨵�name
	while(1)
	{
		clientsize = sizeof (clientname);
		nbytes = recvfrom (sock, message, MAXMSG, 0,
                              (struct sockaddr *) &clientname, &clientsize);//�������ݣ��ͻ������׽��ֵ�ַ���洢�ڣ�*clientname����
		
		
		fprintf (stdout, "Server: received message: %s\n", message);
		sendto (sock, message, nbytes, 0,
                            (struct sockaddr *) & clientname, clientsize);//��ͻ����̷��ص�ַ���ֽ���nbytes
		fprintf (stdout, "Server: sentback message: %s\n", message);
		
	}
}
