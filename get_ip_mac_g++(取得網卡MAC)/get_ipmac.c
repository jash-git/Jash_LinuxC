/*
file name: get_ipmac.c
g++ get_ipmac.c -o get_ipmac
./a.out
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <unistd.h>//close
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXINTERFACES 16
int main(int argc, char **argv)
{
    register int fd, interface, retn = 0;
    struct ifreq buf[MAXINTERFACES];
    struct arpreq arp;
    struct ifconf ifc;
    char mac[32] = "";
 
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
	{
        ifc.ifc_len = sizeof buf;
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc))
		{
            interface = ifc.ifc_len / sizeof(struct ifreq);
            printf("interface num is interface=%d\n\n", interface);
            while (interface-- > 0)
			{
                printf("net device %s\n", buf[interface].ifr_name);
 
				/*Jugde whether the net card status is promisc */
                if (!(ioctl(fd, SIOCGIFFLAGS, (char *) &buf[interface])))
				{
                    if (buf[interface].ifr_flags & IFF_PROMISC)
					{
                        printf("the interface is PROMISC");
                        retn ;
                    }
                }
				else
				{
                    char str[256] = "";
 
                    sprintf(str, "cpm: ioctl device %s",
                            buf[interface].ifr_name);
                    perror(str);
                }
 
				/*Jugde whether the net card status is up */
                if (buf[interface].ifr_flags & IFF_UP)
				{
                    printf("the interface status is UP\n");
                }
				else
				{
                    printf("the interface status is DOWN\n");
                }
 
				/*Get IP of the net card */
                if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[interface])))
				{
                    printf("IP address is:");
                    printf("%s\n",inet_ntoa(((struct sockaddr_in*) (&buf[interface].ifr_addr))->sin_addr));
                }
				else
				{
                    char str[256] = "";
 
                    sprintf(str, "cpm: ioctl device %s",buf[interface].ifr_name);
                    perror(str);
                }
 
				/*Get HW ADDRESS of the net card */
                if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[interface])))
				{
                    printf("HW address is:");
 
                    sprintf(mac, "%02x%02x%02x%02x%02x%02x",(unsigned char) buf[interface].ifr_hwaddr.sa_data[0],(unsigned char) buf[interface].ifr_hwaddr.sa_data[1],(unsigned char) buf[interface].ifr_hwaddr.sa_data[2],(unsigned char) buf[interface].ifr_hwaddr.sa_data[3],(unsigned char) buf[interface].ifr_hwaddr.sa_data[4],(unsigned char) buf[interface].ifr_hwaddr.sa_data[5]); // 利用sprintf轉換成char *
                    
					printf("%s\n", mac);
 
                    printf("\n");
                }
                else
				{
                    char str[256];
 
                    sprintf(str, "cpm: ioctl device %s",buf[interface].ifr_name);
                    
					perror(str);
                }
            }//end of while
        }//end of if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc))
		else
		{
            perror("cpm: ioctl");
		}
    }//end of if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
	else
	{
        perror("cpm: socket");
	}
 
    close(fd);
    return retn;
}


