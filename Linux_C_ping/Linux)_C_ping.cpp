/***********************************************************
 * 作者:梁俊輝                                             *
 * 時間:2001年10月                                         *
 * 名稱：myping.c                                          *
 * 說明:本程式用於演示ping命令的實現原理                   *
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
/*校驗和演算法*/
unsigned short cal_chksum(unsigned short *addr,int len)
{       int nleft=len;
        int sum=0;
        unsigned short *w=addr;
        unsigned short answer=0;
		
/*把ICMP報頭二進位資料以2位元組為單位累加起來*/
        while(nleft>1)
        {       sum+=*w++;
                nleft-=2;
        }
		/*若ICMP報頭為奇數個位元組，會剩下最後一位元組。把最後一個位元組視為一個2位元組資料的高位元組，這個2位元組資料的低位元組為0，繼續累加*/
        if( nleft==1)
        {       *(unsigned char *)(&answer)=*(unsigned char *)w;
                sum+=answer;
        }
        sum=(sum>>16)+(sum&0xffff);
        sum+=(sum>>16);
        answer=~sum;
        return answer;
}
/*設置ICMP報頭*/
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
        gettimeofday(tval,NULL);    /*記錄發送時間*/
        icmp->icmp_cksum=cal_chksum( (unsigned short *)icmp,packsize); /*校驗演算法*/
        return packsize;
}
/*發送三個ICMP報文*/
void send_packet()
{       int packetsize;
        while( nsend<MAX_NO_PACKETS)
        {       nsend++;
                packetsize=pack(nsend); /*設置ICMP報頭*/
                if( sendto(sockfd,sendpacket,packetsize,0,
                          (struct sockaddr *)&dest_addr,sizeof(dest_addr) )<0  )
                {       perror("sendto error");
                        continue;
                }
                sleep(1); /*每隔一秒發送一個ICMP報文*/
        }
}
/*接收所有ICMP報文*/
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
                gettimeofday(&tvrecv,NULL);  /*記錄接收時間*/
                if(unpack(recvpacket,n)==-1)continue;
                nreceived++;
        }
}
/*剝去ICMP報頭*/
int unpack(char *buf,int len)
{       int i,iphdrlen;
        struct ip *ip;
        struct icmp *icmp;
        struct timeval *tvsend;
        double rtt;
        ip=(struct ip *)buf;
        iphdrlen=ip->ip_hl<<2;    /*求ip報頭長度,即ip報頭的長度標誌乘4*/
        icmp=(struct icmp *)(buf+iphdrlen);  /*越過ip報頭,指向ICMP報頭*/
        len-=iphdrlen;            /*ICMP報頭及ICMP數據報的總長度*/
        if( len<8)                /*小於ICMP報頭長度則不合理*/
        {       printf("ICMP packets\'s length is less than 8\n");
                return -1;
        }
        /*確保所接收的是我所發的的ICMP的回應*/
        if( (icmp->icmp_type==ICMP_ECHOREPLY) && (icmp->icmp_id==pid) )
        {       tvsend=(struct timeval *)icmp->icmp_data;
                tv_sub(&tvrecv,tvsend);  /*接收和發送的時間差*/
                rtt=tvrecv.tv_sec*1000+tvrecv.tv_usec/1000;  /*以毫秒為單位計算rtt*/
                /*顯示相關資訊*/
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
        /*生成使用ICMP的原始套接字,這種套接字只有root才能生成*/
        if( (sockfd=socket(AF_INET,SOCK_RAW,protocol->p_proto) )<0)
        {       perror("socket error");
                exit(1);
        }
        /* 回收root許可權,設置當前用戶許可權*/
        setuid(getuid());
        /*擴大套接字接收緩衝區到50K這樣做主要為了減小接收緩衝區溢出的
          的可能性,若無意中ping一個廣播位址或多播位址,將會引來大量應答*/
        setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size) );
        bzero(&dest_addr,sizeof(dest_addr));
        dest_addr.sin_family=AF_INET;
        /*判斷是主機名還是ip位址*/
        if( inaddr=inet_addr(argv[1])==INADDR_NONE)
        {       if((host=gethostbyname(argv[1]) )==NULL) /*是主機名*/
                {       perror("gethostbyname error");
                        exit(1);
                }
                memcpy( (char *)&dest_addr.sin_addr,host->h_addr,host->h_length);
        }
        else    /*是ip地址*/
                memcpy( (char *)&dest_addr,(char *)&inaddr,host->h_length);
        /*獲取main的進程id,用於設置ICMP的標誌符*/
        pid=getpid();
        printf("PING %s(%s): %d bytes data in ICMP packets.\n",argv[1],
                        inet_ntoa(dest_addr.sin_addr),datalen);
        send_packet();  /*發送所有ICMP報文*/
        recv_packet();  /*接收所有ICMP報文*/
        statistics(SIGALRM); /*進行統計*/
        return 0;
}
/*兩個timeval結構相減*/
void tv_sub(struct timeval *out,struct timeval *in)
{       if( (out->tv_usec-=in->tv_usec)<0)
        {       --out->tv_sec;
                out->tv_usec+=1000000;
        }
        out->tv_sec-=in->tv_sec;
}
/*------------- The End -----------*/
