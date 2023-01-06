#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include "msocket.h"

int msocket_create()
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
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

int msocket_send(int sock_fd, char *buf, int bufsize)
{
    if(buf == NULL || bufsize < 1)
        return 0;

    int ret;
    int cnt = 0;
    char *pbuf = buf;

    while (cnt < bufsize)
    {
        ret = send(sock_fd, pbuf, bufsize - cnt, 0);
        if(ret < 0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }

        cnt  += ret;
        pbuf += ret;
    }
    
    return cnt;
}

int msocket_recv(int sock_fd, char *buf, int bufsize)
{
     if(buf == NULL || bufsize < 1)
        return 0;

    int ret;
    int cnt = 0;
    char *pbuf = buf;

    while (cnt < bufsize)
    {
        ret = recv(sock_fd, pbuf, bufsize - cnt, 0);
        if(ret < 0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }

        cnt  += ret;
        pbuf += ret;
    }

    return cnt;
}
