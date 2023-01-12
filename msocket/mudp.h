#ifndef __MUDP_H__
#define __MUDP_H__

int mudp_sendto(int sock_fd, int cli_ip, short cli_port, char *buf, int len);

#endif