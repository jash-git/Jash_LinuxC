linux c arp協議分析三 - 捕捉並分析tcp包
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

//乙太網的最大傳輸單元為1500
#define MTU 1500

struct ethernet_header
{
	unsigned char mac_target[ETH_ALEN];
	unsigned char mac_source[ETH_ALEN];
	unsigned short ether_type;
};

struct ip_header
{
	//ip頭部長度，及ip協議版本
	//說明首部有多少32位字長，如果是5則表示有5個32位字長，也就是20個位元組
	unsigned char header_len:4,
	version:4;
	//服務類型
	unsigned char st_routine:3,
	st_nd:1,
	st_nt:1,
	st_nr:1,
	st_ectbit:1,
	st_ce:1;
	unsigned short total_len;
	unsigned short iden;
	unsigned short flags;
	unsigned char time_to_live;
	unsigned char protocal;
	unsigned short hc;
	unsigned char source_ip[4];
	unsigned char target_ip[4];
};

struct tcp_header
{
	unsigned short int source_port;
	unsigned short int target_port;
	unsigned int seqnum;
	unsigned int acknum;
	unsigned char reserve:4,
	//tcp頭部的偏移，以4位元組為單位，為8時表示偏移量是8*4=32個位元組
	dataoffset:4;
	unsigned char flags;
	unsigned short int win;
	unsigned short int checksum;
	unsigned short int up;
};

void die(const char*pre);
void print_ethdr(struct ethernet_header **ppethdr);
void print_iphdr(struct ip_header **ppiphdr);
void print_tcphdr(struct tcp_header **pptcphdr);

int main(void)
{
	int sfd, len;
	struct sockaddr_ll sl;
	struct ethernet_header *pethdr;
	struct ip_header *piphdr = 0;
	struct tcp_header *ptcphdr = 0;
	char buf[MTU];
	
	sfd = socket(AF_PACKET, SOCK_RAW, htons(ETHERTYPE_IP));
	if(-1 == sfd)
	{
		die("socket");
	}

	memset(&sl, 0, sizeof(sl));
	sl.sll_family = AF_PACKET;
	//sl.sll_protocol = htons(ETH_P_ALL);
	//sl.sll_ifindex = IFF_BROADCAST;

	if(-1 == bind(sfd, (sockaddr*)&sl, sizeof(sl)))
	{
		die("bind");
	}

	
	while(1)
	{
		len = recv(sfd, buf, MTU, 0);
		if(!len)
		{
			perror("recv");
			continue;
		}
		
		//獲得ethernet_header的指針
		ethernet_header *pethdr = (ethernet_header *)&buf;
		print_ethdr(&pethdr);

		//獲得ip_header的指針
		piphdr = (ip_header*) ( (char*)pethdr + sizeof(ethernet_header) );
		print_iphdr(&piphdr);

		//檢測是否為IP協議
		if(0x6 != piphdr->protocal)
			continue;

		//獲得tcp_header的指針
		int optlen = piphdr->header_len > 5 ? 4 : 0;
		ptcphdr = (tcp_header*) ( (char*)piphdr + sizeof(ip_header) + optlen);
		print_tcphdr(&ptcphdr);

		//獲取資料區指標
		//數據區長度
		int datalen = ntohs(piphdr->total_len) - piphdr->header_len * 4 - ptcphdr->dataoffset * 4;
		char *pbuf = new char[datalen];
		//資料區首位元組指標
		char *pdata = (char*) ( (char*)ptcphdr + ptcphdr->dataoffset * 4);
		memcpy(pbuf, pdata, datalen);

		printf("---------------------------data begin----------------------------\n");
		printf("data = %s\n", pbuf);
		printf("---------------------------data end------------------------------\n");
	}
	
	return 0;
}

void die(const char*pre)
{
	perror(pre);
	exit(1);
}

void print_ethdr(struct ethernet_header **ppethdr)
{
	int i;

	struct ethernet_header *pethdr = *ppethdr;

	printf("---------------------------ethernet header begin------------------------------\n");
	
	for(i = 0; i < ETH_ALEN; i++)
	{
		printf(i == 0 ? "mac_target =  %.2x" : ":%.2x", pethdr->mac_target[i]);
	}

	for(i = 0; i < ETH_ALEN; i++)
	{
		printf(i == 0 ? "\nmac_source = %.2x" : ":%.2x", pethdr->mac_source[i]);
	}

	printf("\nether_type = %x\n", ntohs(pethdr->ether_type));
}

void print_iphdr(struct ip_header **ppiphdr)
{
	struct ip_header *piphdr = *ppiphdr;

	printf("---------------------------ip header begin------------------------------\n");
	printf("version    = %d\n", piphdr->version);
	printf("header_len = %d\n", piphdr->header_len);
	printf("source_ip  = %s\n", inet_ntoa(*(struct in_addr*)(piphdr->source_ip)));
	printf("target_ip  = %s\n", inet_ntoa(*(struct in_addr*)(piphdr->target_ip)));
	printf("total_len  = %d\n", ntohs(piphdr->total_len));
	printf("protocal   = %d\n", piphdr->protocal);
}

void print_tcphdr(struct tcp_header **pptcphdr)
{
	tcp_header *ptcphdr = *pptcphdr;
	printf("---------------------------tcp header begin------------------------------\n");
	printf("source_port     = %d\n", ntohs(ptcphdr->source_port));
	printf("target_port     = %d\n", ntohs(ptcphdr->target_port));
	printf("seqnum          = %d\n", ntohs(ptcphdr->seqnum));
	printf("acknum          = %d\n", ntohs(ptcphdr->acknum));
	printf("dataoffset      = %d\n", ptcphdr->dataoffset);
}
