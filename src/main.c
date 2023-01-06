#include <stdio.h>
#include <stdlib.h>

#include "msocket.h"

#define  SERVER_IP      "127.0.0.1"
#define  SERVER_PORT    554
#define  RTSP_BUFFSIZE  1024

void do_rtspcomm(int comm_fd)
{
    int buflen;
    char *buf = NULL;
    buf = (char *)malloc(RTSP_BUFFSIZE);
    if(NULL == buf)
        perror("malloc error");

    buflen = msocket_recv(comm_fd, buf, RTSP_BUFFSIZE);
    if(buflen < 0)
        printf("msocket_recv error!!!\n");

}

int main(int argc, char *argv[])
{
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

        do_rtspcomm(cli_fd);
    }

    return 0;
}
