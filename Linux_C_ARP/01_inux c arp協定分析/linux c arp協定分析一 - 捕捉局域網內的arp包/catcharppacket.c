#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>

struct arp_packet
{
	//DLC Header
	//接收方mac
	unsigned char mac_target[ETH_ALEN];
	//發送方mac
	unsigned char mac_source[ETH_ALEN];
	//Ethertype - 0x0806是ARP幀的類型值
	unsigned short ethertype;

	//ARP Frame
	//硬體類型 - 乙太網類型值0x1
	unsigned short hw_type;
	//上層協定類型 - IP協定(0x0800)
	unsigned short proto_type;
	//MAC地址長度
	unsigned char mac_addr_len;
	//IP地址長度
	unsigned char ip_addr_len;
	//操作碼 - 0x1表示ARP請求包,0x2表示應答包
	unsigned short operation_code;
	//發送方mac
	unsigned char mac_sender[ETH_ALEN];
	//發送方ip
	unsigned char ip_sender[4];
	//接收方mac
	unsigned char mac_receiver[ETH_ALEN];
	//接收方ip
	unsigned char ip_receiver[4];
	//填充數據
	unsigned char padding[18];
};

void die(const char*pre);
void print_arp_packet(struct arp_packet ap);

int main()
{
	int sfd;
	struct sockaddr_ll my_etheraddr;
	struct arp_packet rcvBuffer;

	sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if(-1 == sfd)
	{
		die("socket");
	}

	memset(&my_etheraddr, 0, sizeof(sockaddr_ll));
	my_etheraddr.sll_family = AF_PACKET;
	my_etheraddr.sll_protocol = htons(ETH_P_ARP);
	my_etheraddr.sll_ifindex = IFF_BROADCAST;

	if(-1 == bind(sfd, (struct sockaddr *)&my_etheraddr, sizeof(my_etheraddr))){
		die("bind");
	}
	
	while(1)
	{
		if(-1 == recv(sfd, &rcvBuffer, sizeof(rcvBuffer), 0))
			continue;

		print_arp_packet(rcvBuffer);
	}

	return 0;
}

void die(const char*pre)
{
	perror(pre);
	exit(1);
}

void print_arp_packet(struct arp_packet ap)
{
	printf("\n\n-----------------arp package begin--------------------------\n");

	printf("mac_target = ");
	for(int i = 0; i < ETH_ALEN; i++)
	{
		printf(i > 0 ? ":0x%.2x" : "0x%.2x", ap.mac_target[i]);
	}
	
	printf("\nmac_source = ");
	for(int i = 0; i < ETH_ALEN; i++)
	{
		printf(i > 0 ? ":0x%.2x" : "0x%.2x", ap.mac_source[i]);
	}

	printf("\nethertype = 0x%x", ntohs(ap.ethertype));
	printf("\nhw_type = 0x%x", ntohs(ap.hw_type));
	printf("\nproto_type = 0x%x", ntohs(ap.proto_type));
	printf("\nmac_addr_len = 0x%x", ap.mac_addr_len);
	printf("\nip_addr_len = 0x%x", ap.ip_addr_len);
	printf("\noperation_code = 0x%x", ntohs(ap.operation_code));

	printf("\nmac_sender = ");
	for(int i = 0; i < ETH_ALEN; i++)
	{
		printf(i > 0 ? ":0x%.2x" : "0x%.2x", ap.mac_sender[i]);
	}

	printf("\nip_sender = %s", inet_ntoa(*(struct in_addr*)(ap.ip_sender)));

	printf("\nmac_receiver = ");
	for(int i = 0; i < ETH_ALEN; i++)
	{
		printf(i > 0 ? ":0x%.2x" : "0x%.2x", ap.mac_receiver[i]);
	}

	printf("\nip_receiver = %s", inet_ntoa(*(struct in_addr*)(ap.ip_receiver)));

	printf("\n-----------------arp package end----------------------------\n");
}
