#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msocket.h"
#include "rtspstruct.h"
#include "parsertsp.h"

#define  RTSP_BUFFSIZE  1024

int do_rtspcomm(int comm_fd)
{
    int buflen;
    int rsplen;
    char *buf = NULL;
    buf = (char *)malloc(RTSP_BUFFSIZE);
    if(NULL == buf)
        perror("malloc error");

    char *rspbuf = NULL;
    rspbuf = (char *)malloc(RTSP_BUFFSIZE);
    if(NULL == rspbuf)
        perror("malloc error");

    while(1)
    {
        memset(buf, 0, sizeof(RTSP_BUFFSIZE));
        memset(rspbuf, 0, sizeof(RTSP_BUFFSIZE));

        buflen = msocket_recv(comm_fd, buf, RTSP_BUFFSIZE);
        if(buflen < 0)
            printf("msocket_recv error!!!\n");
        else if(buflen == 0)
            return -1;

        parse_rtsp_msg(buf, buflen, rspbuf, &rsplen);

        msocket_send(comm_fd, rspbuf, rsplen);

    }
}

int main(int argc, char *argv[])
{
    int ret;
    int fd, cli_fd;
    fd = msocket_create();
    if(fd == -1)
        printf("msocket_create errno");

    msocket_bind(fd, SERVER_PORT, SERVER_IP);

    msocket_listen(fd, 5);

    while (1)
    {
        cli_fd = msocket_accept(fd);
        if(cli_fd < 0)
            printf("accept error!\n");
        else if(cli_fd == 0)
            continue;

        ret = do_rtspcomm(cli_fd);
        if(ret < 0)
        {
            printf("cli_fd socket connection close\n");
            msocket_close(cli_fd);
        }
    }

    return 0;
}
