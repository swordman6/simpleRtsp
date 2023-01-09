#include <stdio.h>
#include <stdlib.h>

#include "register.h"

struct list_head *head = NULL;

int  init_register_list()
{
    head = malloc(sizeof(struct list_head));
    if(NULL == head)
        return -1;

    INIT_LIST_HEAD(head);

    return 0;
}

void destory_register_list()
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        list_del(&pos->list);
        free(pos);
        pos = NULL;
    }

    free(head);
    head = NULL;
}

int  add_register_usr(int comm_fd)
{
    rtsp_comm_st *p = malloc(sizeof(rtsp_comm_st));
    if(NULL == p)
        return -1;

    p->comm_fd = comm_fd;
    p->status  = RSTP_COMM_REQUEST;

    list_add(&p->list, head);

    return 0;
}

int  del_register_usr(int comm_fd)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            list_del(&pos->list);
            free(pos);
            pos = NULL;
            return 0;
        }
    }

    return -1;
}

int  modify_register_status(int comm_fd, int session, int status)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            pos->status = status;
            if(status == RSTP_COMM_SSESION)
                pos->session = session;
            pos = NULL;
            return 0;
        }
    }

    return -1;
}

int  modify_register_netinfo(int comm_fd, int flag, net_comm_st *pst)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            if(flag == RTSP_NETINFO_CLI)    
            {
                pos->cli.ip       = pst->ip;
                pos->cli.rtpport  = pst->rtpport;
                pos->cli.rtcpport = pst->rtcpport;
            }
            else if(flag == RTSP_NETINFO_SRV)
            {
                pos->srv.ip       = pst->ip;
                pos->srv.rtpport  = pst->rtpport;
                pos->srv.rtcpport = pst->rtcpport;
            }

            pos = NULL;
            return 0;
        }
    }

    return -1;
}
