#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sever.h"


/*****************************************************************
 * 服务器初始化函数
 * 参数  无
 * 返回  severfd 创建并绑定好的服务器套接字
 * *************************************************************/
#if 0
int server_init(struct sockaddr_in* server_addr)
{
	/* 创建UDP套接字 */
	int server_fd = socket(AF_INET, SOCK_DGRAM, 0);	
	if (server_fd < 0)
	{
#ifdef DEBUG
		perror("server_init: 创建UDP套接字失败");
#endif
		exit(1);
	}

	/* 设置UDP套接字地址 */
	server_addr->sin_family = AF_INET;			
	server_addr->sin_port = htons(SEVER_PORT);		
	(server_addr->sin_addr).s_addr = htonl(INADDR_ANY);
	
	/* 允许重用地址 */ 
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
			&opt, sizeof(opt)) != 0)
	{

		perror("server_init: 允许重用地址失败");
		close(server_fd);
		exit(1);
	}

	/* 允许发送广播数据  */
	if (setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST,
			&opt, sizeof(opt)) != 0)
	{
		perror("server_init: 允许发送广播数据失败");
		close(server_fd);
		exit(1);
	}
	
	/* 绑定套接字地址 */
	if (bind(server_fd, (struct sockaddr*)server_addr, 
		sizeof(*server_addr)) != 0)
	{
		perror("server_init: 绑定套接字地址失败");
		close(server_fd);
		exit(1);
	}	
	return server_fd;
}

#endif 
int sever_init()
{
    struct sockaddr_in sever_addr;
    int severfd;
    /*创建UDP套接字*/
    if((severfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("server_init:socket");
        exit(-1);
    }
    /*配置UDP套接字，填充 sokaddr_in 结构体*/
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_port = htons(SEVER_PORT); /*端口号注意字节序变换*/
    sever_addr.sin_addr.s_addr = htonl(INADDR_ANY);/*任意IP任意机器任意网卡*/

    /*允许地址重用*/
    int opt = 1;
    if (setsockopt(severfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
    {
        perror("sever_init:setsockopt:REUSEADDR");
        close(severfd);
        exit(-1);
    }

    /*允许广播*/
    if (setsockopt(severfd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)) < 0)
    {
        perror("sever_init : sersocket : BROADCAST");
        close(severfd);
        exit(-1);
    }
    /*绑定套接字*/
    if (bind(severfd,(struct sockaddr *)&sever_addr,sizeof(sever_addr)) < 0 )
    {
        perror("sever_init : bind");
        close (severfd);
        exit(-1);
    }

    return severfd;
}

/****************************************************************
 * 发送数据函数
 * 参数 severfd 服务器套接字  netpacket 要发送的数据包
 * 返回 无
 * *************************************************************/
void send_data(int severfd,const NET_PACKET * netpacket)
{
    struct sockaddr_in dst_addr;
    /*设置目标地址信息*/
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = netpacket->dst_ip;
    dst_addr.sin_port = netpacket->dst_port;
    /*发送消息数据包*/
    sendto(severfd,netpacket,sizeof(NET_PACKET),0,
                                (struct sockaddr *)&dst_addr,sizeof(dst_addr));
}

/*******************************************************************************
 * 接收数据函数
 * 参数 severfd 服务器套接字 netpacket 接收到的数据包存放地址
 * 返回  无
 * ****************************************************************************/

void recv_data(int severfd,NET_PACKET * netpacket)
{
    struct sockaddr_in src_addr;
    socklen_t addr_size = sizeof(src_addr);
    /*接收数据包*/
    if(recvfrom(severfd,netpacket,sizeof(NET_PACKET),0,
                                        (struct sockaddr *)&src_addr,&addr_size) <0)
    {
        perror("recvfrom");
    }
    /*获取接发送方的IP信息*/
    netpacket->src_ip = src_addr.sin_addr.s_addr ;
    netpacket->src_port = src_addr.sin_port;
}

/***************************************************************************
 * 增加在线用户函数
 * 参数 serverfd 服务器套接字（保留） list 在线用户链表头结点  netpacket 数据包
 * 返回  无
 * ************************************************************************/
void add_usr(int serverfd,addr_t *list,NET_PACKET * netpacket)
{
    netpacket->dst_ip = netpacket->src_ip;
    netpacket->dst_port =netpacket->src_port;
    send_data(serverfd,netpacket);
    addr_list_insert(list,netpacket->src_ip,netpacket->src_port);
   
    
}
/**********************************************************************
 * 移除在线用户函数
 * 参数 fd 服务器套接字（保留） list 在线用户链表 netpacket 数据包
 * 返回 无
 * ********************************************************************/
void remove_usr(int fd,addr_t *list,NET_PACKET * netpacket)
{
    addr_list_remove(list,netpacket->src_ip,netpacket->src_port);
}
/*********************************************************************
 * 发送其它用户函数
 * 参数 severfd 服务器套接字 addr_list 在线用户链表 netpacket 数据包
 * 返回 无
 * ******************************************************************/
void send_onlines(int severfd ,addr_t *addr_list,NET_PACKET * netpacket)
{
    addr_t *p = addr_list->next; /*头部不用发送*/

    /*设置目标地址*/
    netpacket->dst_ip = netpacket->src_ip;
    netpacket->dst_port = netpacket->src_port;
    /*如果没有其它用户*/
    if((p != NULL)&& (p->next ==NULL))
    {
        netpacket->src_ip = -1;
        netpacket->src_port = -1;

        send_data(severfd,netpacket);
        return ;
    }

    /*发送其它用户*/
    while(p !=NULL)
    {
        if((p->ip != netpacket->dst_ip) || (p->port != netpacket->dst_port)) 
        {
            /*设置发送内容*/
            netpacket->src_ip = p->ip;
            netpacket->src_port = p->port;

            send_data(severfd,netpacket);
        }   
        p = p->next;
    }
}