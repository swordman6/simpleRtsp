#ifndef __RTSPSTRUCT_H__
#define __RTSPSTRUCT_H__

typedef struct{
    char rtsp_order[16];
    char rtsp_domain[64];
    char rtsp_vesion[16];
}rtsp_header_st;

typedef struct{
  char  cseq[16];
  char  session[32];
  char  range[16];
  char  useragent[64];
  char  accept[64];
  char  transport[64];
}rtsp_body_st;

typedef enum rtsp_support_options{
  OPTIONS = 0,
  DESCRIBE,
  SETUP,
  PLAY,
  TEARDOWN,
  PAUSE,
  GET_PARAMETER,
  SET_PARAMETER,
  MAX_SUPPORTS
}rtsp_options;

typedef struct{
  rtsp_options num;
  char *name;
}rtsp_support_str;

#define  SERVER_IP      "192.168.1.83"
#define  SERVER_PORT    8554
#define  RTSP_DOMAIN    "rtsp://192.168.1.83:8554/live/"

#endif