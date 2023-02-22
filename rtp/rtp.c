#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "rtp.h"
#include "mudp.h"
#include "msocket.h"
#include "register.h"

#include "parseh264.h"

int serverport = 6970;

#define  MAX_SIZE  100 * 1024
#define  FILE_PATH "test.h264"

void *rtp_transport_fun(void *arg)
{
    pthread_detach(pthread_self());

    int  comm_fd = *(int *)arg;

    char temp;
    int rtp_fd, rtcp_fd;
    int i, j, k, flag, ret;
    net_comm_st st;

    get_register_rtpsock(comm_fd, &rtp_fd, &rtcp_fd);
    get_register_netinfo(comm_fd, RTSP_NETINFO_CLI, &st);

    rtp_header hrtp;
    init_rtp_header(&hrtp);
    char *rtp_buf = malloc(1500);
    if(rtp_buf == NULL){
        perror("malloc:");
    }

    FILE *fp = NULL;
    fp = fopen(FILE_PATH, "r");
    if(fp == NULL){
        perror("fopen:");
    }

    NALU_t nalu_st;
    nalu_st.max_size = MAX_SIZE;
    nalu_st.buf = (char *)malloc(MAX_SIZE);
    if(nalu_st.buf == NULL){
        perror("malloc:");
        fclose(fp);
    }

    int timeinterval = 90 * 1000 / 25;
    while(!feof(fp)) 
	{
        memset(nalu_st.buf, 0, MAX_SIZE);
        ret = GetAnnexbNALU(fp, &nalu_st);
        if(ret < 0){
            break;
        }    

        if(nalu_st.len > 1436)
        {
            j = nalu_st.len / 1436;
            k = nalu_st.len % 1436;
            i = 0;
            flag = 1;
            while (i < j)
            {
                memset(rtp_buf, 0, 1500);

                hrtp.marker = 0;
                memcpy(rtp_buf, &hrtp, sizeof(rtp_header));

                temp = 0;
                temp = nalu_st.forbidden_bit | nalu_st.nal_reference_idc | 0x1c;

                strncpy(rtp_buf + sizeof(rtp_header), &temp, 1);

                temp = 0;
                if(flag == 1)
                {
                    flag = 0;
                    temp = 1 << 7 | nalu_st.nal_unit_type;
                }
                else
                    temp = nalu_st.nal_unit_type;

                memcpy(rtp_buf + sizeof(rtp_header) + 1, &temp, 1);
                memcpy(rtp_buf + sizeof(rtp_header) + 2, nalu_st.buf + i * 1436, 1436);

                mudp_sendto(rtp_fd, st.ip, st.rtpport, rtp_buf, 1450);                
            
                i++;
                hrtp.seq = ntohs(hrtp.seq);
                hrtp.seq++;
                hrtp.seq = htons(hrtp.seq);        
            }

            memset(rtp_buf, 0, 1500);
            hrtp.marker = 1;
            memcpy(rtp_buf, (char *)&hrtp, sizeof(rtp_header));

            temp = 0;
            temp = nalu_st.forbidden_bit | nalu_st.nal_reference_idc | 0x1c;
            memcpy(rtp_buf + sizeof(rtp_header), &temp, 1);

            temp = 0;
            temp = 1 << 6 | nalu_st.nal_unit_type;
            memcpy(rtp_buf + sizeof(rtp_header) + 1, &temp, 1);
            memcpy(rtp_buf + sizeof(rtp_header) + 2, nalu_st.buf + i * 1436, k);

            mudp_sendto(rtp_fd, st.ip, st.rtpport, rtp_buf, k + 14);        

            hrtp.seq = ntohs(hrtp.seq);
            hrtp.seq++;
            hrtp.seq = htons(hrtp.seq);   
        }
        else
        {
            memset(rtp_buf, 0, 1500);
            if(nalu_st.nal_unit_type == NALU_TYPE_PPS)
                hrtp.marker = 0;
            else
                hrtp.marker = 1;
            memcpy(rtp_buf, (char *)&hrtp, sizeof(rtp_header));

            temp = 0;
            temp = nalu_st.forbidden_bit | nalu_st.nal_reference_idc | nalu_st.nal_unit_type;
            memcpy(rtp_buf + sizeof(rtp_header), &temp, 1);

            memcpy(rtp_buf + sizeof(rtp_header) + 1, nalu_st.buf, nalu_st.len);
            mudp_sendto(rtp_fd, st.ip, st.rtpport, rtp_buf, nalu_st.len + 13);

            hrtp.seq = ntohs(hrtp.seq);
            hrtp.seq++;
            hrtp.seq = htons(hrtp.seq);       
        }

        if(nalu_st.nal_unit_type == NALU_TYPE_SLICE || nalu_st.nal_unit_type == NALU_TYPE_IDR)
        {   
            hrtp.timestamp =  ntohl(hrtp.timestamp);
            hrtp.timestamp += timeinterval;
            hrtp.timestamp =  htonl(hrtp.timestamp);
            usleep(38 * 1000);
        }
    }

    free(rtp_buf);
    free(nalu_st.buf);
    fclose(fp);

    pthread_exit(NULL);
}

void create_rtpsock(int *rtp_fd, int *rtcp_fd, void *pst)
{
    net_comm_st *p = pst;

    char ip[16] = {0};
    inet_ntop(AF_INET, &p->ip, ip, 16);

    *rtp_fd  = msocket_create(CREATE_SOCK_UDP);
    *rtcp_fd = msocket_create(CREATE_SOCK_UDP);

    p->rtpport  = serverport++;
    p->rtcpport = serverport++;

    msocket_bind(*rtp_fd, p->rtpport, ip);
    msocket_bind(*rtcp_fd, p->rtcpport, ip);
}

void strart_rtp_thread(int comm_fd)
{
    int ret;
    pthread_t tid;

    ret = pthread_create(&tid, NULL, rtp_transport_fun, &comm_fd);
    if(ret != 0)
        exit(-1); 
}

void init_rtp_header(rtp_header *phrtp)
{
    phrtp->version     = 2;
    phrtp->padding     = 0;
    phrtp->extension   = 0;
    phrtp->csrclen     = 0;
    phrtp->marker      = 0;
    phrtp->payloadtype = 96;
    phrtp->seq         = htons(8888);
    phrtp->timestamp   = htonl(time(NULL));

    phrtp->ssrc        = htonl(66666);
}
