#ifndef __REPLYRTSP_H__
#define __REPLYRTSP_H__

#include "rtspstruct.h"

void response_options(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);
void response_describe(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);
void response_setup(int comm_fd, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);
void response_play(int comm_fd, rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);
void response_teardown(rtsp_body_st *pbrtsp, char *rspbuf, int *rsplen);

#endif