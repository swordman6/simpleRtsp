#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "public.h"
#include "msocket.h"
#include "register.h"
#include "replyrtsp.h"
#include "parsertsp.h"

int serverip = 6970;

static void parse_rtsp_transport(int comm_fd, rtsp_body_st *pbrtsp)
{
    char str[32] = {0};
    net_comm_st cli_net;

    pickstr(str, pbrtsp->transport, '=', '\0', strlen(pbrtsp->transport));

    sscanf(str, "%hd-%hd", &cli_net.rtpport, &cli_net.rtcpport);

    cli_net.ip = msocket_getip(comm_fd, GET_PEER_IP); 

    modify_register_netinfo(comm_fd, RTSP_NETINFO_CLI, &cli_net); 
}

void parse_rtsp_body(int comm_fd, rtsp_body_st *pbrtsp, char *buf)
{
    char *pbuf = buf;
    int ret;

    char type[16];
    char content[128];
    char line_buf[128];

    while((ret = pickstr(line_buf, pbuf, '\n', '\r', 512)) && line_buf[0] != '\0')
    {
        pbuf += ret;

        memset(type, 0, sizeof(type));
        memset(content, 0, sizeof(content));

        sscanf(line_buf, "%[^:]: %s\r\n", type, content);
        printf("ret = %d,type = %s\ncontent = %s\n", ret, type, content);
   
        if(!strncmp(type, "CSeq", strlen("CSeq")))
            strncpy(pbrtsp->cseq, content, sizeof(pbrtsp->cseq));
        else if(!strncmp(type, "Session", strlen("Session")))
            strncpy(pbrtsp->session, content, sizeof(pbrtsp->session));
        else if(!strncmp(type, "User-Agent", strlen("User-Agent")))
            strncpy(pbrtsp->useragent, content, sizeof(pbrtsp->useragent));
        else if(!strncmp(type, "Accept", strlen("Accept")))
            strncpy(pbrtsp->accept, content, sizeof(pbrtsp->accept));
        else if(!strncmp(type, "Transport", strlen("Transport")))
        {
            strncpy(pbrtsp->transport, content, sizeof(pbrtsp->transport));
            parse_rtsp_transport(comm_fd, pbrtsp);
        }
        else if(!strncmp(type, "Range", strlen("Range")))
            strncpy(pbrtsp->range, content, sizeof(pbrtsp->range));

        memset(line_buf, 0, sizeof(line_buf));
    }
}

int parse_rtsp_msg(int comm_fd, char *buf, int buflen, char *rspbuf, int *rsplen)
{
    int status, session;
    rtsp_header_st hrtsp = {0};
    rtsp_body_st   brtsp = {0};

    sscanf(buf, "%s %s %s\r\n", hrtsp.rtsp_order, hrtsp.rtsp_domain, hrtsp.rtsp_vesion);

    parse_rtsp_body(comm_fd, &brtsp, buf);

    if(!strncmp(hrtsp.rtsp_order, "OPTIONS", strlen("OPTIONS")))
    {
        response_options(&brtsp, rspbuf, rsplen);
        modify_register_status(comm_fd, RSTP_COMM_REQUEST);
    }
    else if (!strncmp(hrtsp.rtsp_order, "DESCRIBE", strlen("DESCRIBE")))
    {
        response_describe(&brtsp, rspbuf, rsplen);       
    }
    else if (!strncmp(hrtsp.rtsp_order, "SETUP", strlen("SETUP")))
    {
        status = get_register_status(comm_fd);
        if(status < RSTP_COMM_SSESION)
        {
            session = time(NULL);
            modify_register_session(comm_fd, session, 65);

            net_comm_st srv_net;
            srv_net.ip       = msocket_getip(comm_fd, GET_SOCK_IP);
            srv_net.rtpport  = serverip++;
            srv_net.rtcpport = serverip++;
            modify_register_netinfo(comm_fd, RTSP_NETINFO_SRV, &srv_net);
        }
        
        response_setup(comm_fd, &brtsp, rspbuf, rsplen);
        modify_register_status(comm_fd, RSTP_COMM_SSESION);
    }
    else if (!strncmp(hrtsp.rtsp_order, "PLAY", strlen("PLAY")))
    {
        status = get_register_status(comm_fd);
        if(status != RSTP_COMM_SSESION && status != RSTP_COMM_PAUSE)
            return -1;

        get_register_session(comm_fd, &session, NULL);
        if(session != transfrom_16_int(brtsp.session))
        {
            response_errsession(&brtsp, rspbuf, rsplen);
        }
        else
        {
            response_play(comm_fd, &brtsp, rspbuf, rsplen);
            modify_register_status(comm_fd, RSTP_COMM_PLAY);
        }
    }
    else if (!strncmp(hrtsp.rtsp_order, "TEARDOWN", strlen("TEARDOWN")))
    {
        status = get_register_status(comm_fd);

        get_register_session(comm_fd, &session, NULL);
        if(session != transfrom_16_int(brtsp.session))
        {
            response_errsession(&brtsp, rspbuf, rsplen);
        }
        else
        {
            response_teardown(&brtsp, rspbuf, rsplen);
            return -1;
        }
    }
    else if (!strncmp(hrtsp.rtsp_order, "PAUSE", strlen("PAUSE")))
        ;
    else if (!strncmp(hrtsp.rtsp_order, "GET_PARAMETER", strlen("GET_PARAMETER")))
        ;
    else if (!strncmp(hrtsp.rtsp_order, "SET_PARAMETER", strlen("SET_PARAMETER")))
        ;
    else
        printf("not support!!!\n");

    return 0;
}