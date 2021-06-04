#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <netdb.h>

void	srvfunc(int fd);
int	errexit(const char* format, ...);
int	passivesock(const char* service, const char *transport , int qlen);

int
main( int argc , char* argv[] )
{
	pthread_t	th;
	pthread_attr_t	ta;
	int msock , ssock ;		//master , slave socket
	int i;
	msock = passivesock( "10000", "tcp" , 32 );
	for( i = 0; i< 2 ; i++ ){
  		(void) pthread_attr_init(&ta);
		if( pthread_create( &th, &ta, (void*(*)(void *))srvfunc , (void*)msock) < 0 )
			errexit("pthread_create(prstats): %s\n", strerror(errno));
		printf("New thread %u is created!\n", th);
	}
	while(1){

	}
}

void
srvfunc(int fd)
{
	char		buf[1024];
	int		cc;
	struct sockaddr_in fsin;
	unsigned int		alen;
	int ssock;
	alen = sizeof(fsin);
	while(1){
		ssock = accept(fd, (struct sockaddr*)&fsin, &alen);
		if(ssock < 0){
			errexit("accept: %s\n", strerror(errno));}
		printf("I'm thread %u, I'm serving client at %s:%d!\n",pthread_self(), inet_ntoa(fsin.sin_addr), ntohs(fsin.sin_port) );
		while(cc = read(ssock, buf, sizeof(buf))){
			if( cc <= 0 )
				errexit("echo read: %s\n", strerror(errno));
			if( write(ssock, buf, cc) < 0 )
				errexit("echo write: %s\n", strerror(errno));
		}
		(void)close(ssock);
		printf("I am thread %ul, I finished service for %s:%d!\n\n",pthread_self(),inet_ntoa(fsin.sin_addr),ntohs(fsin.sin_port) );
	}
}

int
passivesock(const char *service, const char *transport, int qlen)
/*
 * Arguments:
 *      service   - service associated with the desired port
 *      transport - transport protocol to use ("tcp" or "udp")
 *      qlen      - maximum server request queue length
 */
{
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = PF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
	if ( pse = getservbyname(service, transport) )
		sin.sin_port = htons(ntohs((unsigned short)pse->s_port));
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
		errexit("can't get \"%s\" service entry\n", service);

    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(transport)) == 0)
		errexit("can't get \"%s\" protocol entry\n", transport);

    /* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		errexit("can't create socket: %s\n", strerror(errno));

    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		errexit("can't bind to %s port: %s\n", service,
			strerror(errno));
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		errexit("can't listen on %s port: %s\n", service,
			strerror(errno));
	return s;
}
int
errexit(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}

