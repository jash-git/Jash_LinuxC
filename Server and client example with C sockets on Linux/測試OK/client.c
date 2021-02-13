/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
	char *p='\0'; 
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
	else
	{
		puts("Connected\n");
        //Receive a reply from the server
		memset(server_reply , '\0' , sizeof(server_reply));//clean buffer
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
        }
        else
		{
			puts("Server reply :");
			puts(server_reply);
			puts("\n");
		}
	}
	
    //keep communicating with server
    while(1)
    {
		
        printf("Enter message : ");
        scanf("%s" , message);
         
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
		memset(server_reply , '\0' , sizeof(server_reply));//clean buffer
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
		puts("\n");
		p=strstr(server_reply,"Good Bye");
		if((p-server_reply)>=0)
		{
			break;
		}
    }
     
	close(sock);
	puts("client close.");
	return 0;
}