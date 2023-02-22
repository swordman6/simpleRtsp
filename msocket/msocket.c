#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include "msocket.h"

fd_set stFdr;

int msocket_create(int flag)
{
    int sock_fd;
    if(flag == CREATE_SOCK_TCP)
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    else if(flag == CREATE_SOCK_UDP)
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        perror("socket error:");
        return -1;
    }

    return sock_fd;
}

void msocket_close(int sock_fd)
{
    close(sock_fd);
}

int msocket_bind(int sock_fd, short port, const char *addr)
{
    int opt =1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in sock_st;

    memset(&sock_st, 0, sizeof(sock_st));
    sock_st.sin_family = AF_INET;
    sock_st.sin_addr.s_addr = inet_addr(addr);
    sock_st.sin_port = htons(port);

    bind(sock_fd, (struct sockaddr *)&sock_st, sizeof(struct sockaddr_in));

    return 0;
}

void msocket_listen(int sock_fd, int num)
{
    listen(sock_fd, num);
}

int msocket_accept(int sock_fd)
{
    int cli_fd;
    cli_fd = accept(sock_fd, NULL, NULL);
    if(cli_fd < 0)
    {
        if(errno == EINTR)
            return 0;
        return -1;
    }

    return cli_fd;
}

void msocket_fdset_init()
{
    FD_ZERO(&stFdr);
}

void msocket_fdset_set(int sock_fd)
{
    FD_SET(sock_fd, &stFdr);
}

void msocket_fdset_clr(int sock_fd)
{
    FD_CLR(sock_fd, &stFdr);
}

int msocket_select(int sock_fd, int max, int *flag)
{
    int ret, cli_fd;

    fd_set stFdrTmp = stFdr;
    ret = select(max + 1, &stFdrTmp, NULL, NULL, NULL);
    if(ret < 0)
        return -1;

    for(int i = 0; i < max + 1; i++)
    {
        if(FD_ISSET(i, &stFdrTmp))
        {
            if( i == sock_fd)
            {
                cli_fd = msocket_accept(sock_fd);
                *flag = 1;
                return cli_fd;
            }
            else
            {
                *flag = 0;
                return i;
            }
        }

    }

    return 0;  
}

int msocket_send(int sock_fd, char *buf, int bufsize)
{
    if(buf == NULL || bufsize < 1)
        return 0;

    int ret;
    int cnt = 0;

    while (cnt < bufsize)
    {
        ret = send(sock_fd, buf, bufsize - cnt, 0);
        if(ret < 0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }

        cnt  += ret;
        buf += ret;
    }
    
    return cnt;
}

int msocket_recv(int sock_fd, char *buf, int bufsize)
{
    if(buf == NULL || bufsize < 1)
        return 0;

    int cnt = 0;

    while (1)
    {
        cnt = recv(sock_fd, buf, bufsize, 0);
        if(cnt < 0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }

        return cnt;
    }

}

unsigned int msocket_getip(int sock_fd, int flag)
{
    int ret, len;
    struct sockaddr_in sock_st;
    len = sizeof(struct sockaddr_in);

    if(flag == GET_PEER_IP)
        ret = getpeername(sock_fd, (struct sockaddr *)&sock_st, (socklen_t *)&len);
    else if(flag == GET_SOCK_IP)
        ret = getsockname(sock_fd, (struct sockaddr *)&sock_st, (socklen_t *)&len);
    if(ret < 0)
        return -1;

    return sock_st.sin_addr.s_addr;
}
