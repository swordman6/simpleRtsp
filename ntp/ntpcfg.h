#ifndef __NTPCFG_H__
#define __NTPCFG_H__

/*NTPCLI状态机*/
typedef enum hiNTP_STATE_E
{
    NTP_IDLE      = 0,  /*NTP 服务未创建或销毁后*/
    NTP_CREATED   = 1,	/*NTP 服务创建成功*/
    NTP_SENDED    = 2,  /*NTP发送了对时请求*/
    NTP_RECEVED   = 3   /*NTP接收到对时响应*/
}NTP_STATE_E;

struct ntp_server {
	int sockfd;
	int ntp_state;       //NTP服务的当前状态
    unsigned int serverip;
};

#define  DEFAULT_NTP_PORT  123 

int ntp_srvinfo_init(char *ipaddr);
int ntp_send_info(void *buf, int buflen);
int ntp_recv_info(void *buf);

#endif