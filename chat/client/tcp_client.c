#include "tcp_client.h"
extern int LOCAL_PORT;
extern int  LOCAL_IP;
/*****************************************************************************
 * tcp客户端初始化函数
 * 参数　　无
 * 返回　tcp_fd　ｔｃｐ套接字
 * **********************************************************************/
int  tcp_client_init()
{
    struct sockaddr_in tcp_addr;
    int tcp_fd ;
    /*创建TCP套接字*/
    if((tcp_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
    {
        perror("tcp_clinet: socket");
        exit(-1);
    }
    /*配置套接字*/
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = LOCAL_PORT;
    tcp_addr.sin_addr.s_addr = LOCAL_IP;
    
    /*允许地址重用*/
    int opt = 1;
    if(setsockopt(tcp_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0 )
    {
        perror("tcp_client : setsockopt: REUSADDR");
        close(tcp_fd);
        exit(-1);
    }
    /*绑定套接字*/
    if(bind(tcp_fd,(struct sockaddr  *)&tcp_addr,sizeof(tcp_addr)) < 0)
    {
        perror("tcp_client : bind");
        close(tcp_fd);
        exit(-1);
    }

    /*监听 最多允许10*/
    if(listen(tcp_fd, 10) < 0 )
    {
        perror("tcp_client : listen ");
        close(tcp_fd);
        exit(-1);
    }
    return tcp_fd;
}