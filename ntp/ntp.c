#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include "ntp.h"
#include "ntpcfg.h"

/*
 * Byte order conversions
 */
#define	HTONS_FP(x)	(htonl(x))
#define	HTONL_FP(h, n)	do { (n)->l_ui = htonl((h)->l_ui); \
			     (n)->l_uf = htonl((h)->l_uf); } while (0)
#define	NTOHS_FP(x)	(ntohl(x))
#define	NTOHL_FP(n, h)	do { (h)->l_ui = ntohl((n)->l_ui); \
			     (h)->l_uf = ntohl((n)->l_uf); } while (0)

#define FRAC		4294967296. 		/* 2^32 as a double */
#define JAN_1970	2208988800UL	    /* 1970 - 1900 in seconds */

struct NtpTime 
{
	unsigned int coarse;
	unsigned int fine;
};

#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))
#define Data(i) ntohl(((unsigned int *)data)[i])

void getNewTime(unsigned int *data)
{
	struct NtpTime trantime;
	trantime.coarse = Data(10);
	trantime.fine   = Data(11);

    struct timeval timeval;
	
	timeval.tv_sec 	= trantime.coarse - JAN_1970;
	timeval.tv_usec = USEC(trantime.fine);

    printf("sec = %ld usec = %ld\n", timeval.tv_sec, timeval.tv_usec);//打印输出NTP服务器返回的时间

    struct tm *ptm = localtime(&timeval.tv_sec);

    printf("time : %d:%d:%d\n", ptm->tm_hour,  ptm->tm_min,  ptm->tm_sec);
	printf("date: %d:%d:%d\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday);
	printf("year: wday:%d yday:%d isdst:%d\n", ptm->tm_wday, ptm->tm_yday, ptm->tm_isdst);	
}

static inline void get_systime(l_fp *now)
{
	double dtemp;
	struct timeval tv;

	gettimeofday(&tv, (struct timezone *)0);
	now->l_ui = tv.tv_sec+JAN_1970;
	dtemp = tv.tv_usec * FRAC / 1e6;
 	 if (dtemp >= FRAC)
		now->l_ui++;

	now->l_uf = (unsigned int)dtemp;
}

static inline void ntp_init_packet(struct ntp_pkt *xpkt)
{
    xpkt->li_vn_mode     = PKT_LI_VN_MODE(LEAP_NOTINSYNC, NTP_VERSION, MODE_CLIENT);
	xpkt->stratum        = STRATUM_TO_PKT(STRATUM_UNSPEC);
	xpkt->ppoll          = NTP_MINPOLL;
	xpkt->precision      = NTPDATE_PRECISION;
	xpkt->rootdelay      = htonl(NTPDATE_DISTANCE);
	xpkt->rootdispersion = htonl(NTPDATE_DISP);
	xpkt->refid          = htonl(NTPDATE_REFID);
	L_CLR(&(xpkt->reftime));
	L_CLR(&(xpkt->org));
	L_CLR(&(xpkt->rec));

    l_fp now;
    get_systime(&now);
    HTONL_FP(&now, &(xpkt->xmt));   
}

void ntp_server_test()
{
    int ret;

    unsigned char rcvbuf[1024] = {0};

    struct ntp_pkt *pkt = NULL;
    pkt = malloc(sizeof(struct ntp_pkt));
    
    ntp_init_packet(pkt);

    ntp_srvinfo_init("192.168.1.82");

    ret = ntp_send_info((void *)pkt, sizeof(struct ntp_pkt));
    if(ret < 0)
        printf("send error\n");

    ret = ntp_recv_info((void *)rcvbuf);
    printf("ret = %d\n", ret);

    getNewTime((unsigned int *)rcvbuf);

    free(pkt);
}
