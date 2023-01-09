#ifndef __MSOCKET_H__
#define __MSOCKET_H__

int  msocket_create();
void msocket_close(int sock_fd);
int  msocket_bind(int sock_fd, short port, const char *addr);
void msocket_listen(int sock_fd, int num);
int  msocket_accept(int sock_fd);
int  msocket_send(int sock_fd, char *buf, int bufsize);
int  msocket_recv(int sock_fd, char *buf, int bufsize);

void msocket_fdset_init();
void msocket_fdset_set(int fd);
void msocket_fdset_clr(int fd);
int  msocket_select(int sock_fd, int max, int *flag);

#endif
