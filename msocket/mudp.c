#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "mudp.h"

int mudp_sendto(int sock_fd, int cli_ip, short cli_port, char *buf, int len)
{
    struct sockaddr_in dst_addr;

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = cli_ip;
    dst_addr.sin_port = htons(cli_port);

    int ret = sendto(sock_fd, buf, len, 0, (struct sockaddr *)&dst_addr, sizeof(struct sockaddr_in));

    return 1;
}
