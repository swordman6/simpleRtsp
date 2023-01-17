#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>

#include "ntpcfg.h"

static struct ntp_server ntpserver;

int ntp_srvinfo_init(char *ipaddr)
{
	int sockfd;
    unsigned int ipnum;

	memset(&ntpserver, 0, sizeof(struct ntp_server));
	ipnum = inet_addr(ipaddr);
	if(ipnum == 0xffffffff)
	{
		struct hostent* lphost;
		lphost = gethostbyname(ipaddr);
		if (lphost != NULL)
		{
			ipnum = ((struct in_addr*)lphost->h_addr)->s_addr;
		}
		else
		{
			return -1;
		}
	}
	if(ipnum == 0)
	{
		return -1;
	}
		
	/* create a datagram (UDP) socket */
	sockfd= socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		return -1;
	}

	ntpserver.sockfd    = sockfd;
    ntpserver.serverip  = ipnum;
	ntpserver.ntp_state = NTP_CREATED;
		
	return 0;
}

void ntp_srv_close()
{
	if(ntpserver.sockfd > 0)
		close(ntpserver.sockfd);
	ntpserver.sockfd = -1;
}

int ntp_get_serverip()
{
    return ntpserver.serverip;
}

int ntp_get_sock()
{
    return ntpserver.sockfd;
}

int ntp_get_state()
{
    return ntpserver.ntp_state;
}

int ntp_send_info(void *buf, int buflen)
{
    int ret;
    struct sockaddr_in srv_sock;
    memset(&srv_sock, 0, sizeof(struct sockaddr_in));

    srv_sock.sin_family = AF_INET;
    srv_sock.sin_port = htons(DEFAULT_NTP_PORT);
	srv_sock.sin_addr.s_addr = ntpserver.serverip;

    ret = sendto(ntpserver.sockfd, (void *)buf, buflen, 0, (struct sockaddr *)&srv_sock, sizeof(struct sockaddr_in));
    if(ret == buflen)
    {	
        ntpserver.ntp_state = NTP_SENDED;
        return 0;	
    }
    else
    {
        ntpserver.ntp_state = NTP_CREATED;
        return -1;
    }
	
}

int ntp_recv_info(void *buf)
{
    int recv_len;
    struct sockaddr_in srv_sock;

    int addr_len = sizeof(struct sockaddr_in);
    
    recv_len = recvfrom(ntpserver.sockfd, buf, 1024, 0, (struct sockaddr *)&srv_sock, (socklen_t*)&addr_len);
    if(-1 == recv_len)		
    {	
        ntp_srv_close();
        ntpserver.ntp_state = NTP_IDLE;					
        return -1;
    }
    else if(recv_len > 0)
    {

    }

    return recv_len;
}
