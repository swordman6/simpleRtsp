#ifndef __MSOCKET_H__
#define __MSOCKET_H__

typedef enum{
    GET_PEER_IP,
    GET_SOCK_IP
}get_ip;

typedef enum{
    CREATE_SOCK_TCP,
    CREATE_SOCK_UDP
}comm_type;

int  msocket_create(int flag);
void msocket_close(int sock_fd);
int  msocket_bind(int sock_fd, short port, const char *addr);
void msocket_listen(int sock_fd, int num);
int  msocket_accept(int sock_fd);
int  msocket_send(int sock_fd, char *buf, int bufsize);
int  msocket_recv(int sock_fd, char *buf, int bufsize);

void msocket_fdset_init();
void msocket_fdset_set(int sock_fd);
void msocket_fdset_clr(int sock_fd);
int  msocket_select(int sock_fd, int max, int *flag);

int  msocket_getip(int sock_fd, int flag);

#endif
