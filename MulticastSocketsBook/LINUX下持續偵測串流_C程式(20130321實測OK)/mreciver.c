#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_LEN  1400


int main(int argc, char *argv[])
{
        int sock;
        int flag_on = 1;
        struct sockaddr_in multicast_addr;
        char message_received[MAX_LEN+1];
        int msgrecv_len;
        struct ip_mreq mc_req;
        char* multicast_ip;
        unsigned short multicast_port;
        struct sockaddr_in from_addr;
        unsigned int from_len;


        if (argc != 3)
        {
                fprintf(stderr, "Usage: %s Multicast_IP Multicast_Port\n", argv[0]);
                exit(1);
        }


        multicast_ip = argv[1];      /* arg 1: multicast ip address */
        multicast_port = atoi(argv[2]);    /* arg 2: multicast port number */


        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        {
                perror("socket() failed");
                exit(1);
        }


        /* set reuse port to on to allow multiple binds per host */
        if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag_on,
        sizeof(flag_on))) < 0)
        {
                perror("setsockopt() failed");
                exit(1);
        }


        /* construct a multicast address structure */
        memset(&multicast_addr, 0, sizeof(multicast_addr));
        multicast_addr.sin_family      = AF_INET;
        multicast_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        multicast_addr.sin_port        = htons(multicast_port);


        /* bind to multicast address to socket */
        if ((bind(sock, (struct sockaddr *) &multicast_addr, sizeof(multicast_addr))) < 0)
        {
                perror("bind() failed");
                exit(1);
        }


        /* construct an IGMP join request structure */
        mc_req.imr_multiaddr.s_addr = inet_addr(multicast_ip);
        mc_req.imr_interface.s_addr = htonl(INADDR_ANY);


        /* send an ADD MEMBERSHIP message via setsockopt */
        if ((setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &mc_req, sizeof(mc_req))) < 0)
        {
                perror("setsockopt() failed");
                exit(1);
        }


        while(1)
        {
                memset(message_received, 0, sizeof(message_received));
                from_len = sizeof(from_addr);
                memset(&from_addr, 0, from_len);


                /* block waiting to receive a packet */
                if ((msgrecv_len = recvfrom(sock, message_received, MAX_LEN, 0, (struct sockaddr*)&from_addr, &from_len)) < 0)
                {
                        perror("recvfrom() failed");
                        break;
                }


                printf("Received %d bytes from : %s  Multicast_IP : %s  Multicast_Port : %s  \n", msgrecv_len,
                inet_ntoa(from_addr.sin_addr) , argv[1] , argv[2] );
/*              printf("%s", message_received)    */;
        }


        /* send a DROP MEMBERSHIP message via setsockopt */
        if ((setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*) &mc_req, sizeof(mc_req))) < 0)
        {
                perror("setsockopt() failed");
                exit(1);
        }
        close(sock);
}

