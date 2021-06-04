/***********************************************************
 * �@��:��T��                                             *
 * �ɶ�:2001�~10��                                         *
 * �W�١Gmyping.c                                          *
 * ����:���{���Ω�t��ping�R�O����{��z                   *
 ***********************************************************/
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#define PACKET_SIZE     4096
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS  3
char sendpacket[PACKET_SIZE];
char recvpacket[PACKET_SIZE];
int sockfd,datalen=56;
int nsend=0,nreceived=0;
struct sockaddr_in dest_addr;
pid_t pid;
struct sockaddr_in from;
struct timeval tvrecv;
void statistics(int signo);
unsigned short cal_chksum(unsigned short *addr,int len);
int pack(int pack_no);
void send_packet(void);
void recv_packet(void);
int unpack(char *buf,int len);
void tv_sub(struct timeval *out,struct timeval *in);
void statistics(int signo)
{       printf("\n--------------------PING statistics-------------------\n");
        printf("%d packets transmitted, %d received , %%%d lost\n",nsend,nreceived,
                        (nsend-nreceived)/nsend*100);
        close(sockfd);
        exit(1);
}
/*����M�t��k*/
unsigned short cal_chksum(unsigned short *addr,int len)
{       int nleft=len;
        int sum=0;
        unsigned short *w=addr;
        unsigned short answer=0;
		
/*��ICMP���Y�G�i���ƥH2�줸�լ����֥[�_��*/
        while(nleft>1)
        {       sum+=*w++;
                nleft-=2;
        }
		/*�YICMP���Y���_�ƭӦ줸�աA�|�ѤU�̫�@�줸�աC��̫�@�Ӧ줸�յ����@��2�줸�ո�ƪ����줸�աA�o��2�줸�ո�ƪ��C�줸�լ�0�A�~��֥[*/
        if( nleft==1)
        {       *(unsigned char *)(&answer)=*(unsigned char *)w;
                sum+=answer;
        }
        sum=(sum>>16)+(sum&0xffff);
        sum+=(sum>>16);
        answer=~sum;
        return answer;
}
/*�]�mICMP���Y*/
int pack(int pack_no)
{       int i,packsize;
        struct icmp *icmp;
        struct timeval *tval;
        icmp=(struct icmp*)sendpacket;
        icmp->icmp_type=ICMP_ECHO;
        icmp->icmp_code=0;
        icmp->icmp_cksum=0;
        icmp->icmp_seq=pack_no;
        icmp->icmp_id=pid;
        packsize=8+datalen;
        tval= (struct timeval *)icmp->icmp_data;
        gettimeofday(tval,NULL);    /*�O���o�e�ɶ�*/
        icmp->icmp_cksum=cal_chksum( (unsigned short *)icmp,packsize); /*����t��k*/
        return packsize;
}
/*�o�e�T��ICMP����*/
void send_packet()
{       int packetsize;
        while( nsend<MAX_NO_PACKETS)
        {       nsend++;
                packetsize=pack(nsend); /*�]�mICMP���Y*/
                if( sendto(sockfd,sendpacket,packetsize,0,
                          (struct sockaddr *)&dest_addr,sizeof(dest_addr) )<0  )
                {       perror("sendto error");
                        continue;
                }
                sleep(1); /*�C�j�@��o�e�@��ICMP����*/
        }
}
/*�����Ҧ�ICMP����*/
void recv_packet()
{       int n,fromlen;
        extern int errno;
        signal(SIGALRM,statistics);
        fromlen=sizeof(from);
        while( nreceived<nsend)
        {       alarm(MAX_WAIT_TIME);
                if( (n=recvfrom(sockfd,recvpacket,sizeof(recvpacket),0,
                                (struct sockaddr *)&from,&fromlen)) <0)
                {       if(errno==EINTR)continue;
                        perror("recvfrom error");
                        continue;
                }
                gettimeofday(&tvrecv,NULL);  /*�O�������ɶ�*/
                if(unpack(recvpacket,n)==-1)continue;
                nreceived++;
        }
}
/*��hICMP���Y*/
int unpack(char *buf,int len)
{       int i,iphdrlen;
        struct ip *ip;
        struct icmp *icmp;
        struct timeval *tvsend;
        double rtt;
        ip=(struct ip *)buf;
        iphdrlen=ip->ip_hl<<2;    /*�Dip���Y����,�Yip���Y�����׼лx��4*/
        icmp=(struct icmp *)(buf+iphdrlen);  /*�V�Lip���Y,���VICMP���Y*/
        len-=iphdrlen;            /*ICMP���Y��ICMP�ƾڳ����`����*/
        if( len<8)                /*�p��ICMP���Y���׫h���X�z*/
        {       printf("ICMP packets\'s length is less than 8\n");
                return -1;
        }
        /*�T�O�ұ������O�کҵo����ICMP���^��*/
        if( (icmp->icmp_type==ICMP_ECHOREPLY) && (icmp->icmp_id==pid) )
        {       tvsend=(struct timeval *)icmp->icmp_data;
                tv_sub(&tvrecv,tvsend);  /*�����M�o�e���ɶ��t*/
                rtt=tvrecv.tv_sec*1000+tvrecv.tv_usec/1000;  /*�H�@�����p��rtt*/
                /*��ܬ�����T*/
                printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%.3f ms\n",
                        len,
                        inet_ntoa(from.sin_addr),
                        icmp->icmp_seq,
                        ip->ip_ttl,
                        rtt);
        }
        else    return -1;
}
main(int argc,char *argv[])
{       struct hostent *host;
        struct protoent *protocol;
        unsigned long inaddr=0l;
        int waittime=MAX_WAIT_TIME;
        int size=50*1024;
        if(argc<2)
        {       printf("usage:%s hostname/IP address\n",argv[0]);
                exit(1);
        }
        if( (protocol=getprotobyname("icmp") )==NULL)
        {       perror("getprotobyname");
                exit(1);
        }
        /*�ͦ��ϥ�ICMP����l�M���r,�o�خM���r�u��root�~��ͦ�*/
        if( (sockfd=socket(AF_INET,SOCK_RAW,protocol->p_proto) )<0)
        {       perror("socket error");
                exit(1);
        }
        /* �^��root�\�i�v,�]�m��e�Τ�\�i�v*/
        setuid(getuid());
        /*�X�j�M���r�����w�İϨ�50K�o�˰��D�n���F��p�����w�İϷ��X��
          ���i���,�Y�L�N��ping�@�Ӽs����}�Φh����},�N�|�ިӤj�q����*/
        setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size) );
        bzero(&dest_addr,sizeof(dest_addr));
        dest_addr.sin_family=AF_INET;
        /*�P�_�O�D���W�٬Oip��}*/
        if( inaddr=inet_addr(argv[1])==INADDR_NONE)
        {       if((host=gethostbyname(argv[1]) )==NULL) /*�O�D���W*/
                {       perror("gethostbyname error");
                        exit(1);
                }
                memcpy( (char *)&dest_addr.sin_addr,host->h_addr,host->h_length);
        }
        else    /*�Oip�a�}*/
                memcpy( (char *)&dest_addr,(char *)&inaddr,host->h_length);
        /*���main���i�{id,�Ω�]�mICMP���лx��*/
        pid=getpid();
        printf("PING %s(%s): %d bytes data in ICMP packets.\n",argv[1],
                        inet_ntoa(dest_addr.sin_addr),datalen);
        send_packet();  /*�o�e�Ҧ�ICMP����*/
        recv_packet();  /*�����Ҧ�ICMP����*/
        statistics(SIGALRM); /*�i��έp*/
        return 0;
}
/*���timeval���c�۴�*/
void tv_sub(struct timeval *out,struct timeval *in)
{       if( (out->tv_usec-=in->tv_usec)<0)
        {       --out->tv_sec;
                out->tv_usec+=1000000;
        }
        out->tv_sec-=in->tv_sec;
}
/*------------- The End -----------*/
