#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "public.h"
#include "register.h"
#include "replyrtsp.h"
#include "parsertsp.h"

static void parse_rtsp_transport(rtsp_body_st *pbrtsp)
{
    char str[32] = {0};

    pickstr(str, pbrtsp->transport, '=', '\0', strlen(pbrtsp->transport));

    //sscanf(str, "%hd-%hd", &pbrtsp->rtpport, &pbrtsp->rtcpport);
}

void parse_rtsp_body(rtsp_body_st *pbrtsp, char *buf)
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
            parse_rtsp_transport(pbrtsp);
        }
        else if(!strncmp(type, "Range", strlen("Range")))
            strncpy(pbrtsp->range, content, sizeof(pbrtsp->range));

        memset(line_buf, 0, sizeof(line_buf));
    }
}

int parse_rtsp_msg(char *buf, int buflen, char *rspbuf, int *rsplen)
{
    rtsp_header_st hrtsp = {0};
    rtsp_body_st   brtsp = {0};

    sscanf(buf, "%s %s %s\r\n", hrtsp.rtsp_order, hrtsp.rtsp_domain, hrtsp.rtsp_vesion);

    parse_rtsp_body(&brtsp, buf);

    if(!strncmp(hrtsp.rtsp_order, "OPTIONS", strlen("OPTIONS")))
        response_options(&hrtsp, &brtsp, rspbuf, rsplen);
    else if (!strncmp(hrtsp.rtsp_order, "DESCRIBE", strlen("DESCRIBE")))
        response_describe(&hrtsp, &brtsp, rspbuf, rsplen);
    else if (!strncmp(hrtsp.rtsp_order, "SETUP", strlen("SETUP")))
        response_setup(&hrtsp, &brtsp, rspbuf, rsplen);
    else if (!strncmp(hrtsp.rtsp_order, "PLAY", strlen("PLAY")))
        ;
    else if (!strncmp(hrtsp.rtsp_order, "TEARDOWN", strlen("TEARDOWN")))
        ;
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