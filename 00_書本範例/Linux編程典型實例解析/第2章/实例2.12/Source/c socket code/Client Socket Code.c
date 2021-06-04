#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	address.sin_port = 30000;
	len = sizeof(address);
	
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1) {
		perror("client connect failure");
		exit(1);
	}
	
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("\nchar read from server is %c\n", ch);
	close(sockfd);
	exit(0);
}
