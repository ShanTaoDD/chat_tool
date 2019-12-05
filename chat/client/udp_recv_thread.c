#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "udp_recv_thread.h"

static void print_onlines(int ip,int port);
static void print_msg(NET_PACKET *packet);

void *udp_recv_thread(void * udp_fd)
{
    pthread_detach(pthread_self()); /*设置分离属性*/
    int fd = *(int *)udp_fd;
    NET_PACKET packet;
    while(1)
    {
        /*接收数据*/
        recv_data(fd,&packet);
        /*解析数据*/
        switch(packet.data_type)
        {
            case DATA_ONLINE :
                print_onlines(packet.src_ip,packet.src_port);
                break;
            case DATA_CHAT :
                print_msg(&packet);
                break;
            default :
                break;
        }
    }
    return NULL;
}
/*****************************************************
 * 打印其它在线用户函数
 * 参数 ip 网络字节序ip port网络字节序端口号
 * 返回 无
 * ****************************************************/
void print_onlines(int ip,int port)
{
    char ip_str [20];
    if (ip < 0 || port < 0)
    {
        printf("[没有其它在线用户]\n");
    }
    inet_ntop(AF_INET,&ip,ip_str,sizeof(ip_str));
    printf("[在线用户:IP%s,端口%d]\n",ip_str,ntohs(port));

}
/**************************************************
 * 消息打印函数
 * 参数 packet 数据包
 * 返回 无
 * ***********************************************/
void print_msg(NET_PACKET *packet)
{
    char ip_str[20];
    time_t t;
    time(&t);
    struct tm *pt;
    pt = gmtime(&t);
    inet_ntop(AF_INET,&(packet->src_ip),ip_str,sizeof(ip_str));
    printf("[来自用户:IP[%s],端口[%d],%d:%d:%d]\n",
            ip_str,ntohs(packet->src_port),pt->tm_hour+8,pt->tm_min,pt->tm_sec);
    printf("\033[0;31m%s\n.\033[0m",packet->data);
}