#ifndef __REGISTER_H__
#define __REGISTER_H__

typedef struct
{
    int    ip;
    short  rtpport;
    short  rtcpport;
}net_comm_st;

typedef struct
{
    int comm_fd;
    int flag;
    int session;
    net_comm_st srv;
    net_comm_st cli;
}rtsp_comm_st;

#endif