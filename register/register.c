#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "register.h"

struct list_head *head = NULL;

void init_register_list()
{
    head = malloc(sizeof(struct list_head));
    if(NULL == head)
    {
        exit(-1);
    }

    INIT_LIST_HEAD(head);

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

int add_register_usr(int comm_fd)
{
    rtsp_comm_st *p = malloc(sizeof(rtsp_comm_st));
    if(NULL == p)
        return -1;

    bzero(p, sizeof(rtsp_comm_st));

    p->comm_fd = comm_fd;

    list_add(&p->list, head);

    return 0;
}

int del_register_usr(int comm_fd)
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

int modify_register_status(int comm_fd, int status)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            pos->status = status;
            pos = NULL;
            return 0;
        }
    }

    return -1;
}

int get_register_status(int comm_fd)
{
    int status;

    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            status = pos->status;
            pos = NULL;
            return status;
        }
    }

    return -1;
}

int modify_register_session(int comm_fd, int session, int timeout)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            pos->session = session;
            pos->timeout = timeout;
            pos = NULL;
            return 0;
        }
    }

    return -1;
}

int get_register_session(int comm_fd, int *psession, int *ptimeout)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            if(psession != NULL)
                *psession = pos->session;
            if(ptimeout != NULL)
                *ptimeout = pos->timeout;
            pos = NULL;
            return 0;
        }
    }

    return -1;
}

int modify_register_netinfo(int comm_fd, int flag, net_comm_st *pst)
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

int get_register_netinfo(int comm_fd, int flag, net_comm_st *pst)
{
    rtsp_comm_st *pos = NULL;
    list_for_each_entry(pos, head, list)
    {
        if(pos->comm_fd == comm_fd)
        {
            if(flag == RTSP_NETINFO_CLI)    
            {
                pst->ip       = pos->cli.ip;
                pst->rtpport  = pos->cli.rtpport;
                pst->rtcpport = pos->cli.rtcpport;
            }
            else if(flag == RTSP_NETINFO_SRV)
            {
                pst->ip       = pos->srv.ip;
                pst->rtpport  = pos->srv.rtpport;
                pst->rtcpport = pos->srv.rtcpport;
            }

            pos = NULL;
            return 0;
        }
    }

    return -1;
}
