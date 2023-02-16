#ifndef __RTP_H__
#define __RTP_H__

typedef struct
{
    unsigned char  csrclen : 4;
    unsigned char  extension : 1;
    unsigned char  padding : 1;
    unsigned char  version : 2;
    unsigned char  payloadtype : 7;
    unsigned char  marker : 1;
    unsigned short seq;
    unsigned int   timestamp;
    unsigned int   ssrc;
}rtp_header;

void create_rtpsock(int *rtp_fd, int *rtcp_fd, void *pst);
void strart_rtp_thread(int comm_fd);
void init_rtp_header(rtp_header *phrtp);

#endif