#ifndef __REGISTER_H__
#define __REGISTER_H__

#include "list.h"

typedef enum{
    RTSP_NETINFO_CLI,
    RTSP_NETINFO_SRV,
}comm_netinfo_flag;

typedef enum{
    RSTP_COMM_REQUEST = 1,
    RSTP_COMM_SSESION,
    RSTP_COMM_PAUSE,
    RSTP_COMM_PLAY,
    RSTP_COMM_MAX
}comm_stage_flag;

typedef struct{
    int    ip;
    short  rtpport;
    short  rtcpport;
}net_comm_st;

typedef struct{
    int comm_fd;
    int session;
    int timeout;
    int seq;
    int rtptime;    
    net_comm_st srv;
    net_comm_st cli;
    comm_stage_flag status;
    struct list_head list;
}rtsp_comm_st;

void init_register_list();
void destory_register_list();
int  add_register_usr(int comm_fd);
int  del_register_usr(int comm_fd);
int  modify_register_status(int comm_fd, int status);
int  get_register_status(int comm_fd);
int  modify_register_session(int comm_fd, int session, int timeout);
int  get_register_session(int comm_fd, int *psession, int *ptimeout);
int  modify_register_netinfo(int comm_fd, int flag, net_comm_st *pst);
int  get_register_netinfo(int comm_fd, int flag, net_comm_st *pst);

#endif