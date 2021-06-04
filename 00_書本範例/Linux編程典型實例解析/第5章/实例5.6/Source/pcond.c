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


int client_array[10];	/*socket descriptors for client connection waiting to handle*/
int iput, iget;		/*index for main thread put and sub-thread get*/
pthread_cond_t	cli_cond;
pthread_mutex_t	cli_mutex;
int	passivesock(const char *service, const char *transport,	int qlen);
int	errexit(const char *format, ...);	

void srvfunc(int fd)
{
	char	buf[4096];
	int	cc;
	struct sockaddr_in cin;
	
	int alen = sizeof(cin);
	if(getpeername(fd, (struct sockaddr *)&cin, &alen)<0)
		errexit("accept: %s\n", strerror(errno));
	printf("I'm thread %u, I'm serving  client at address: %s : %d \n",pthread_self(), inet_ntoa(cin.sin_addr), ntohs(cin.sin_port));
	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			errexit("echo read: %s\n", strerror(errno));
		if (write(fd, buf, cc) < 0)
			errexit("echo write: %s\n", strerror(errno));
	}
	(void) close(fd);
}
void thread_main(void)
{
	for(;;){
		pthread_mutex_lock(&cli_mutex);
		while(iget == iput)
			pthread_cond_wait(&cli_cond, &cli_mutex);
		int fd = client_array[iget];
		if(++iget == 10)
			iget=0;
		pthread_mutex_unlock(&cli_mutex);
		srvfunc(fd);
	}
}
int main(int argc, char *argv[])
{
	struct	sockaddr_in fsin;		
	unsigned int	alen;			
	int	msock,ssock;
	pthread_t th;
	pthread_attr_t	ta;			

	msock = passivesock("10000", "tcp", 32);;
	(void) pthread_attr_init(&ta);
	int i=0;
	for( i=0; i<2; i++){
        	if(pthread_create(&th,&ta,(void*)thread_main,0)<0)
			errexit("pthread_create: %s\n", strerror(errno));
        	printf("New thread %u is created!\n", th);
        }
	iput = iget = 0;
	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr*)&fsin, &alen);
		if (ssock < 0) {
			if (errno == EINTR)
				continue;
			errexit("accept: %s\n", strerror(errno));
		}

		pthread_mutex_lock(&cli_mutex);
		client_array[iput] = ssock;
		if( ++iput == 10)
			iput=0;	
		if(iput == iget)
			errexit("error: iput == iget %d \n",iput);

		pthread_cond_signal(&cli_cond);
		pthread_mutex_unlock(&cli_mutex);

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

