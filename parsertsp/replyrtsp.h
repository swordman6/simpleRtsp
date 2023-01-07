#ifndef __REPLYRTSP_H__
#define __REPLYRTSP_H__

#include "rtspstruct.h"

void response_setup(rtsp_header_st *phrtsp, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);
void response_options(rtsp_header_st *phrtsp, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);
void response_describe(rtsp_header_st *phrtsp, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);

#endif