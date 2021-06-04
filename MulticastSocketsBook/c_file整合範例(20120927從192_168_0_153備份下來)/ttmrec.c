#include <sys/types.h>  /* for type definitions */
#include <sys/socket.h> /* for socket API calls */
#include <netinet/in.h> /* for address structs */
#include <arpa/inet.h>  /* for sockaddr_in */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for strlen() */
#include <unistd.h>     /* for close() */

#define MAX_LEN  2048  /* maximum receive string size */
#define MIN_PORT 1024   /* minimum port allowed */
#define MAX_PORT 65535  /* maximum port allowed */

int main(int argc, char *argv[]) {

  char* source_server_ip;	   /* source server ip */
  int socks;                   /* Send socket descriptor */
  struct sockaddr_in mc_addrs; /* Send socket address structure */
  char* mc_addr_strs;          /* Send multicast IP address */
  unsigned short mc_ports;     /* Send multicast port */
  unsigned char mc_ttl=10;     /* Send time to live (hop count) */

  int sock;                     /* Recv socket descriptor */
  int flag_on = 1;              /* Recv socket option flag */
  struct sockaddr_in mc_addr;   /* Recv socket address structure */
  char recv_str[MAX_LEN];       /* Recv buffer to receive string */
  int recv_len;                 /* Recv length of string received */
  struct ip_mreq mc_req;        /* Recv multicast request structure */
  char* mc_addr_str;            /* Recv multicast IP address */
  unsigned short mc_port;       /* Recv multicast port */
  struct sockaddr_in from_addr; /* Recv packet source */
  unsigned int from_len;        /* Recv source addr length */

  /* validate number of arguments */
  if (argc != 6) {
    fprintf(stderr,
            "Usage: %s <Source_Server_IP> <Recv Multicast IP> <Recv Multicast Port> <Send Multicast IP> <Send Multicast Port>\n",
            argv[0]);
    exit(1);
  }
  
  source_server_ip = argv[1]; /* arg 1: Source Server ip address */
  mc_addr_str = argv[2];      /* arg 2: multicast ip address */
  mc_port = atoi(argv[3]);    /* arg 3: multicast port number */
  mc_addr_strs = argv[4];     /* arg 4: multicast IP address */
  mc_ports = atoi(argv[5]);   /* arg 5: multicast port number */

  /* validate the port range */
  if ((mc_port < MIN_PORT) || (mc_port > MAX_PORT)) {
    fprintf(stderr, "Recv Invalid port number argument %d.\n", mc_port);
    fprintf(stderr, "Recv Valid range is between %d and %d.\n", MIN_PORT, MAX_PORT);
    exit(1);
  }

   if ((mc_ports < MIN_PORT) || (mc_ports > MAX_PORT)) {
    fprintf(stderr, "Send Invalid port number argument %d.\n", mc_ports);
    fprintf(stderr, "Send Valid range is between %d and %d.\n", MIN_PORT, MAX_PORT);
    exit(1);
  }

  /* create socket to join multicast group on */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("Rec socket() failed");
    exit(1);
  }

   /* create a socket for sending to the multicast address */
  if ((socks = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket() failed");
    exit(1);
  }

  /* set reuse port to on to allow multiple binds per host */
  if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag_on, sizeof(flag_on))) < 0) {
    perror("setsockopt() failed");
    exit(1);
  }

   /* set the TTL (time to live/hop count) for the send */
   if ((setsockopt(socks, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &mc_ttl, sizeof(mc_ttl))) < 0) {
    perror("setsockopt() failed");
    exit(1);
  }
/*                            */
  /* construct a multicast address structure */
  memset(&mc_addr, 0, sizeof(mc_addr));
  mc_addr.sin_family      = AF_INET;
  mc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  mc_addr.sin_port        = htons(mc_port);

 /* construct a multicast address structure for send  */
  memset(&mc_addrs, 0, sizeof(mc_addrs));
  mc_addrs.sin_family      = AF_INET;
  mc_addrs.sin_addr.s_addr = inet_addr(mc_addr_strs);
  mc_addrs.sin_port        = htons(mc_ports);

   printf("From %s prot %d \n", mc_addr_str,mc_port);
   printf("to %s prot %d \n", mc_addr_strs,mc_ports);
   printf("ctrl-C to quit\n");

  /* bind to multicast address to socket rec */
  if ((bind(sock, (struct sockaddr *) &mc_addr, sizeof(mc_addr))) < 0) {
    perror("bind() failed");
    exit(1);
  }
  
  /* construct an IGMP join request structure Recv */
  mc_req.imr_multiaddr.s_addr = inet_addr(mc_addr_str);
  mc_req.imr_interface.s_addr = htonl(INADDR_ANY);

  /* send an ADD MEMBERSHIP message via setsockopt Recv */
  if ((setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void*) &mc_req, sizeof(mc_req))) < 0) {
    perror("setsockopt() failed");
    exit(1);
  }

  for (;;) {          /* loop forever */

    /* clear the receive buffers & structs */
    memset(recv_str, 0, sizeof(recv_str));
    from_len = sizeof(from_addr);
    memset(&from_addr, 0, from_len);

    /* block waiting to receive a packet */
       if ((recv_len = recvfrom(sock, recv_str, MAX_LEN, 0,(struct sockaddr *)&from_addr, &from_len)) < 0) {
         perror("recvfrom() failed");
        exit(1);
       }
      
 	  if(!strcmp( inet_ntoa(from_addr.sin_addr),source_server_ip )) {
         /* send data to multicast address */
        if ((sendto(socks, recv_str, recv_len, 0, (struct sockaddr *) &mc_addrs, sizeof(mc_addrs))) != recv_len) {
          perror("sendto() send incorrect number of bytes");
         exit(1);
        }
      }
	}
  /* send a DROP MEMBERSHIP message via setsockopt for recv*/
  if ((setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,(void*) &mc_req, sizeof(mc_req))) < 0) {
    perror("setsockopt() failed");
    exit(1);
  }
  close(sock);
}

