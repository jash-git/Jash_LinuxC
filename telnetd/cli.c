
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define MY_PORT_ID 6089
#define SERVER_PORT_ID 6090
#define SERV_HOST_ADDR "127.0.0.1"

main()
{
	int sockid, retcode;
	struct sockaddr_in my_addr, server_addr;
	char msg[30];

	printf("Client: creating socket\n");
	if ( (sockid = socket(AF_INET, SOCK_DGRAM, 0)) <0)
	{
		printf("Client: socket failed: %d\n",errno);
		exit(0);
	}

	printf("Client: binding my local socket\n");
	bzero((char *) &my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(MY_PORT_ID);
	if ( (bind(sockid, (struct sockaddr *) &my_addr, sizeof(my_addr)) <0) )
	{
		printf("Client: bind fail: %d\n",errno);
		exit(0);
	}

	printf("Client: create addr structure for server\n");
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	server_addr.sin_port = htons(SERVER_PORT_ID);

	printf("Client: initializing message and sending\n");
	sprintf(msg, "SCO OpenServer(TM) Release 5");
	retcode = sendto(sockid,msg,30,0,(struct sockaddr *) &server_addr, sizeof(server_addr));
	if (retcode <= -1)
	{
		printf("Client: sendto failed; %d\n",errno);
		exit(0);
	}

	close(sockid);

}