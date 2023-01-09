#ifndef __PARSERTSP_H__
#define __PARSERTSP_H__

int parse_rtsp_msg(int comm_fd, char *buf, int buflen, char *rspbuf, int *rsplen);

#endif