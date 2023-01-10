#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msocket.h"
#include "register.h"
#include "rtspstruct.h"
#include "parsertsp.h"

#define  RTSP_BUFFSIZE  1024

int main(int argc, char *argv[])
{
    int ret, flag = 0;
    int srv_fd, cli_fd, max;

    int  rcvlen;
    int  rsplen;
    char rcvbuf[RTSP_BUFFSIZE] = {0};
    char rspbuf[RTSP_BUFFSIZE] = {0};

    init_register_list();

    srv_fd = msocket_create(CREATE_SOCK_TCP);

    msocket_bind(srv_fd, SERVER_PORT, SERVER_IP);

    msocket_listen(srv_fd, 5);

    msocket_fdset_init();
    msocket_fdset_set(srv_fd);
    max = srv_fd;
    while (1)
    {
        cli_fd = msocket_select(srv_fd, max, &flag);
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
            rcvlen = msocket_recv(cli_fd, rcvbuf, RTSP_BUFFSIZE);
            if(rcvlen == 0)
            {
                printf("cli_fd socket connection close\n");
                msocket_fdset_clr(cli_fd);
                del_register_usr(cli_fd);
                msocket_close(cli_fd);

                continue;
            }

            ret = parse_rtsp_msg(cli_fd, rcvbuf, rcvlen, rspbuf, &rsplen);
                      
            msocket_send(cli_fd, rspbuf, rsplen);

            if(ret == -1)
            {  
                printf("cli_fd socket connection close\n");
                msocket_fdset_clr(cli_fd);
                del_register_usr(cli_fd);
                msocket_close(cli_fd);
            }
        }

        cli_fd = 0;
    }

    msocket_close(srv_fd);
    destory_register_list();
    return 0;
}
