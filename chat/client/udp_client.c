#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "udp_client.h"
#include "protocol.h"
/***********************************************************************
 * 客户端UDP初始化函数
 * 参数 无
 * 返回  udp_fd  UDP套接字标识符
 * **********************************************************************/
int  udp_client_init()
{
    struct sockaddr_in udp_addr;

    /*创建UDP套接字*/
    int udp_fd;
    if((udp_fd = socket(AF_INET,SOCK_DGRAM,0 )) <0 )
    {
        perror("udp_client_init : socket");
        exit(-1);  
    }
    /*配置UDP套接字地址属性*/
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_port = htonl (SEVER_PORT);
    inet_pton(AF_INET,SEVER_ADDR,&udp_addr.sin_addr.s_addr);

    /*允许重用地址*/
    int opt =1;
    if(setsockopt(udp_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) <0 )
    {
        perror("udp_client_init : setsockopt REUSEADDR");
        close(udp_fd);
        exit(-1);
    }
    /*允许广播*/
    if(setsockopt(udp_fd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)) <0 )
    {
        perror("udp_client_init : setsockopt BROADCAST");
        close(udp_fd);
        exit(-1);
    }
    return udp_fd;
}
/*******************************************************************
 * 发送数据函数
 * 参数 fd,UDP套接字  packet,要发送的数据包
 * 返回  无
 * ***************************************************************/
void  send_data(int fd,NET_PACKET*packet)
{
    struct sockaddr_in dst_addr;
    inet_pton(AF_INET,SEVER_ADDR,&dst_addr.sin_addr.s_addr);
    dst_addr.sin_port = htons(SEVER_PORT);/*坑！ 不能用htonl port是16位*/
    dst_addr.sin_family = AF_INET;

    sendto(fd,packet,sizeof(NET_PACKET),0,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
}
/********************************************************************
 * 接收数据函数
 * 参数  fd,UDP套接字  packet，存储接收到数据包
 * 返回  无
 * ****************************************************************/
 void recv_data(int fd,NET_PACKET *packet)
 {
    struct sockaddr_in src_addr;/*接收发包方地址信息*/
    socklen_t src_addr_size = sizeof(src_addr);
    recvfrom(fd,packet,sizeof(NET_PACKET),0,
            (struct sockaddr *)&src_addr,&src_addr_size);   
 }

 
