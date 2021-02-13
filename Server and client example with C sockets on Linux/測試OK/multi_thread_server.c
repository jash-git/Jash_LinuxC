/*
    C socket server example, handles multiple clients using threads
	
	gcc multi_thread_server.c -o multi_thread_server -pthread
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 
//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("multi_thread_server: Could not create socket");
    }
    puts("multi_thread_server: Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("multi_thread_server: bind done,port=8888");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("multi_thread_server: Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("multi_thread_server: Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("multi_thread_server: 1 Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("multi_thread_server: could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    char *p='\0'; 
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));
     
    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while(1)
    {
        //Send the message back to client
		memset(client_message , '\0' , sizeof(client_message));//clean buffer
		read_size= recv(sock , client_message , 2000 , 0);
		if(read_size > 0) 
		{
			p=strstr(client_message,"bye2");
			if((p-client_message)>=0)
			{
				message = "Good Bye Client...\n";
				write(sock , message , strlen(message));
				break;
			}
			else
			{
				p='\0';
				printf("client send: %s\n",client_message);
				strcat(client_message,"-form Server\n");
				write(sock , client_message , strlen(client_message));
				memset(client_message , '\0' , sizeof(client_message));//clean buffer
			}
		}
    }
     
    if(read_size == 0)
    {
        puts("multi_thread_server: Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}