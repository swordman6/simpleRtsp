#include <stdio.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>

#include "replyrtsp.h"
#include "register.h"

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

    *psdplen = sprintf(sdpstr, "v=0\r\no=- %d 1 IN IP4 192.168.1.88\r\ns=simple\r\nt=0 0\r\nm=video 0 RTP/AVP 96\r\na=rtpmap:96 H264/90000\r\na=control:track1\r\n", (int)t);
}

void response_options(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
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
                               "RTSP/1.0", 200, \
                               pbrtsp->cseq,\
                               timestr, \
                               optstr);
}

void response_describe(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{   
    char timestr[64] = {0};
    char sdpstr[512] = {0};

    int sdplen = 0;

    rtsp_gen_date(timestr);

    if(!strncmp(pbrtsp->accept, "application/sdp" , strlen("application/sdp")))
        rtsp_gen_sdp(sdpstr, &sdplen);

    *rsplen = sprintf(rspbuf, "%s %d OK\r\nCSeq: %s\r\nDate: %s\r\nContent-Base: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n%s", \
                            "RTSP/1.0", 200, \
                            pbrtsp->cseq, \
                            timestr, \
                            RTSP_DOMAIN, \
                            pbrtsp->accept,  \
                            sdplen,  \
                            sdpstr);
}

void response_setup(int comm_fd, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{
    int session, timeout;
    rtsp_comm_st st;

    char cli_ip[16]  = {0};
    char srv_ip[16]  = {0};
    char timestr[64] = {0};

    rtsp_gen_date(timestr);

    get_register_session(comm_fd, &session, &timeout);
    get_register_netinfo(comm_fd, RTSP_NETINFO_CLI, &st.cli);
    get_register_netinfo(comm_fd, RTSP_NETINFO_SRV, &st.srv);

    inet_ntop(AF_INET, &st.cli, cli_ip, 16);
    inet_ntop(AF_INET, &st.srv, srv_ip, 16);

    *rsplen = sprintf(rspbuf,"%s %d OK\r\nCSeq: %s\r\nDate: %s\r\nTransport: RTP/AVP;unicast;destination=%s;source=%s;client_port=%hu-%hu;server_port=%hu_%hu\r\nSession: %X;timeout=%d\r\n\r\n", \
                              "RTSP/1.0", 200, \
                              pbrtsp->cseq, \
                              timestr, \
                              cli_ip, srv_ip, st.cli.rtpport, st.cli.rtcpport, st.srv.rtpport, st.srv.rtcpport, \
                              session, timeout \
                              );
}

void response_play(int comm_fd, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{
    int session;
    char timestr[64] = {0};

    rtsp_gen_date(timestr);

    get_register_session(comm_fd, &session, NULL);

    *rsplen = sprintf(rspbuf, "%s %d OK\r\nCSeq: %s\r\nDate: %s\r\nRange: %s\r\nSession: %X\r\nRTP-Info: url=%s%s;seq=%d,rtptime=%d\r\n\r\n", \
                               "RTSP/1.0", 200, \
                               pbrtsp->cseq,\
                               timestr, \
                               pbrtsp->range, \
                               session,
                               RTSP_DOMAIN, "track1", 11111, 97310949 \
                               );
}

void response_teardown(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{
    char timestr[64] = {0};

    rtsp_gen_date(timestr);

    *rsplen = sprintf(rspbuf, "%s %d OK\r\nCSeq: %s\r\nDate: %s\r\n\r\n", \
                               "RTSP/1.0", 200, \
                               pbrtsp->cseq,\
                               timestr \
                               );
}

void response_errsession(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen)
{
    char timestr[64] = {0};

    rtsp_gen_date(timestr);

    *rsplen = sprintf(rspbuf, "%s %d SESSION ERROR\r\nCSeq: %s\r\nDate: %s\r\n\r\n", \
                               "RTSP/1.0", 404, \
                               pbrtsp->cseq,\
                               timestr \
                               );
}
