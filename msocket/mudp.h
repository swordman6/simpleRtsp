#ifndef __MUDP_H__
#define __MUDP_H__

int mudp_sendto(int sock_fd, unsigned int cli_ip, unsigned short cli_port, char *buf, int len);

#endif