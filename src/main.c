#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msocket.h"
#include "register.h"
#include "rtspstruct.h"
#include "parsertsp.h"

#define  RTSP_BUFFSIZE  1024

int do_rtspcomm(int comm_fd)
{
    int  buflen;
    int  rsplen;
    char buf[RTSP_BUFFSIZE]    = {0};
    char rspbuf[RTSP_BUFFSIZE] = {0};

    buflen = msocket_recv(comm_fd, buf, RTSP_BUFFSIZE);
    if(buflen < 0)
        printf("msocket_recv error!!!\n");
    else if(buflen == 0)
        return -1;

    parse_rtsp_msg(comm_fd, buf, buflen, rspbuf, &rsplen);

    msocket_send(comm_fd, rspbuf, rsplen);

    return 0;
}

int main(int argc, char *argv[])
{
    int ret, flag = 0;
    int srv_fd, cli_fd, max;

    init_register_list();

    srv_fd = msocket_create();
    if(srv_fd == -1)
        printf("msocket_create errno");

    msocket_bind(srv_fd, SERVER_PORT, SERVER_IP);

    msocket_listen(srv_fd, 5);

    msocket_fdset_init();
    msocket_fdset_set(srv_fd);
    max = srv_fd; 
    while (1)
    {
        cli_fd = msocket_select(srv_fd, max, &flag);
        printf("cli_fd = %d\n", cli_fd);
        if(cli_fd < 0)
            return -1;
        else if(cli_fd == 0)
            continue;

        if(flag == 1) //new usr
        {
            msocket_fdset_set(cli_fd);
            max = cli_fd > max ? cli_fd : max;
            add_register_usr(cli_fd);
        }
        else
        {
            ret = do_rtspcomm(cli_fd);
            if(ret < 0)
            {
                printf("cli_fd socket connection close\n");
                msocket_fdset_clr(cli_fd);
                msocket_close(cli_fd);
            }
        }

        cli_fd = 0;
    }

    msocket_close(srv_fd);
    destory_register_list();
    return 0;
}
