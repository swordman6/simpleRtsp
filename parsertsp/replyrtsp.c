#include <stdio.h>
#include <time.h>
#include <string.h>

#include "replyrtsp.h"

rtsp_support_str options[] = {
  {OPTIONS,       "OPTIONS"},
  {DESCRIBE,      "DESCRIBE"},
  {SETUP,         "SETUP"},
  {PLAY,          "PLAY"},
  {TEARDOWN,      "TEARDOWN"},
  {PAUSE,         "PAUSE"},
  {GET_PARAMETER, "GET_PARAMETER"},
  {SET_PARAMETER, "SET_PARAMETER"},
  {MAX_SUPPORTS,  NULL},
};

static void rtsp_gen_date(char *timestr)
{
    time_t t;
    struct tm *st;
    
    time(&t);
    st = localtime(&t);
    strftime(timestr, 64, "%a, %b %d %Y %H:%M:%S GMT", st);
}

static void rtsp_gen_sdp(char *sdpstr, int *psdplen)
{
    time_t t;
    time(&t);

    *psdplen = sprintf(sdpstr, "v=0\r\no=- %d 1 IN IP4 192.17.1.202\r\ns=simple\r\nt=0 0\r\nm=video 5504 RTP/AVP 98\r\na=rtpmap:98 H264/90000\r\na=control:track1\r\n", (int)t);
}

void response_options(rtsp_header_st *phrtsp, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{
    char optstr[128] = {0};
    char timestr[64] = {0};

    int i = 0;
    int ret = 0;
    char *p = optstr;

    rtsp_gen_date(timestr);

    while(options[i].num != MAX_SUPPORTS)
    {        
        ret = sprintf(p, "%s, ", options[i].name);
        p += ret;
        i++;
    }
    *(p-1) = '\0';
    *(p-2) = '\0';

    *rsplen = sprintf(rspbuf, "%s %d OK\r\nCSeq: %s\r\nDate: %s\r\nPublic: %s\r\n\r\n", \
                               phrtsp->rtsp_vesion, 200, \
                               pbrtsp->cseq,\
                               timestr, \
                               optstr);
}

void response_describe(rtsp_header_st *phrtsp, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{   
    char timestr[64] = {0};
    char sdpstr[512] = {0};

    int sdplen = 0;

    rtsp_gen_date(timestr);

    if(!strncmp(pbrtsp->accept, "application/sdp" , strlen("application/sdp")))
        rtsp_gen_sdp(sdpstr, &sdplen);

    *rsplen = sprintf(rspbuf, "%s %d OK\r\nCSeq: %s\r\nDate: %s\r\nContent-Base: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n%s", \
                            phrtsp->rtsp_vesion, 200, \
                            pbrtsp->cseq, \
                            timestr, \
                            RTSP_DOMAIN, \
                            pbrtsp->accept,  \
                            sdplen,  \
                            sdpstr);
}

void response_setup(rtsp_header_st *phrtsp, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{
    char timestr[64] = {0};

    rtsp_gen_date(timestr);

    *rsplen = sprintf(rspbuf,"%s %d OK\r\nCSeq: %s\r\nDate: %s\r\n", \
                              phrtsp->rtsp_vesion, 200, \
                              pbrtsp->cseq, \
                              timestr \
                              );
}
