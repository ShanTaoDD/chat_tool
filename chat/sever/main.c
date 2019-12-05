#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "addr_list.h"
#include "protocol.h"
#include "sever.h"


int main()
{
    addr_t * addr_list;
    NET_PACKET  net_packet;
    struct sockaddr_in server_addr;
	/*创建在线用户列表*/
    addr_list = addr_list_creat();
    printf("用户在线列表已创建！\n"); 
    /*初始化服务器*/
    int severfd = sever_init();
    printf("服务器初始化成功！\n");
    printf("等待接收数据。。。。\n");  
    while(1)
    {
        /*接收数据*/
        recv_data(severfd,&net_packet);
        /*解析数据*/
        switch(net_packet.data_type)
        {
            case DATA_CHAT:
                send_data(severfd,&net_packet); /*转发消息*/
                break;
            case DATA_ONLINE:
                send_onlines(severfd,addr_list,&net_packet);/*发送在线用户*/
                break;
            case DATA_LOGIN:
                add_usr(severfd,addr_list,&net_packet);/*添加在线用户*/
                break;
            case DATA_LOGOUT:
                remove_usr(severfd,addr_list,&net_packet);/*移除用户*/
                break;
            default:
                break;
                    
        }
    }
    addr_list_destory(addr_list);/*销毁在线用户链表*/
    close(severfd); /*关闭服务器套接字*/

}
