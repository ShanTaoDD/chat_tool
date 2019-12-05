#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "udp_send_thread.h"

int LOCAL_IP;
int LOCAL_PORT;
extern pthread_cond_t login_cond;
extern pthread_cond_t file_send_cond;
/*等待文件发送完成的互斥量和条件变量*/
pthread_mutex_t chat_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t chat_cond = PTHREAD_COND_INITIALIZER;
static void get_msg(char * msg);
void login(int,NET_PACKET *);
/*********************************************************
 * udp消息发送线程
 * 参数　ｕｄｐ套接字
 * 返回　　无
 * ******************************************************/
void *udp_send_thread(void *udp_fd)
{
    int fd = *(int *)udp_fd;
    NET_PACKET packet;
    login(fd,&packet);/*登录*/
    while(1)
    {
        packet.data_type = get_cmd();/*获取指令*/
        /*解析指令*/
        switch (packet.data_type)
        {
            case DATA_ONLINE :
                online(fd,&packet);
                break;
            case DATA_CHAT :
                chat(fd,&packet);
                break;
            case DATA_LOGOUT:
                logout(fd,&packet);
                return NULL;
            case DATA_FILE :
                pthread_cond_signal(&file_send_cond);/*唤醒文件发送线程*/
                /*等待文件发送完成*/
                pthread_mutex_lock(&chat_mutex);
                pthread_cond_wait(&chat_cond,&chat_mutex);
                pthread_mutex_unlock(&chat_mutex);
                break;

            default:
                show_help();
        }

    }
}

/*********************************************************
 * 登录函数
 * 参数 fd udp套接字  packet 数据包
 * 返回 无
 * *****************************************************/
void login(int fd,NET_PACKET * packet)
{
    
    packet->data_type = DATA_NONE;
    send_data(fd,packet);

    /*请求登录*/
    packet->data_type = DATA_LOGIN;
    send_data(fd,packet);
    /*接收回执*/
    recv_data(fd,packet);
    LOCAL_PORT = packet->src_port;
    LOCAL_IP = packet->src_ip;
    char ip_str[20];
    inet_ntop(AF_INET,&(packet->src_ip),ip_str,sizeof(ip_str));
    /*登录成功*/
    pthread_cond_signal(&login_cond);/*唤醒主进程*/
    printf("登录成功!\n");
    printf("【本地用户：IP%s,端口号：%d】\n",ip_str,ntohs(LOCAL_PORT));   
    show_help();
}
/*****************************************************************
 * 登出函数
 * 参数  fd 套接字  packet 数据包
 * 返回 无
 * **************************************************************/
void logout(int fd,NET_PACKET * packet)
{
    packet->data_type = DATA_LOGOUT;
    send_data(fd,packet);
}
/****************************************************************
 * 获取在线用户函数
 * 参数 fd 套接字  packet 数据包
 * 返回  无
 * ***************************************************************/
void online(int fd ,NET_PACKET * packet)
{
    packet->data_type = DATA_ONLINE;
    send_data(fd,packet);
}
/*************************************************************
 * 聊天命令函数
 * 参数 fd  套接字  packet 数据包
 * 返回  无
 * **********************************************************/
void chat(int fd ,NET_PACKET * packet)
{
    char ip[20];
    int port;
    packet->data_type = DATA_CHAT;
    printf("请输入对方IP地址：\n");
    scanf("%s",ip);
    getchar();/*scanf 会在缓冲区留下一个 \n */
    printf("请输入对方端口号：\n");
    scanf("%d",&port);
    getchar();
    packet->dst_port = htons(port);
    inet_pton(AF_INET,ip,&packet->dst_ip);

    printf("[开始聊天]\n");
    printf("[请输入要发送的消息，按回车键发送，输入quit退出聊天]\n");
    while(1)
    {    
        get_msg(packet->data);
        if (strncmp(packet->data,"quit",4) == 0)
        {
            printf("[聊天结束]\n");
            break;
        }
        send_data(fd,packet);
    }
}
/***********************************************************************
 * 获取指令函数
 * 参数  无
 * 返回 对应的消息类型
 * *****************************************************************/
int  get_cmd()
{
    char cmd[50];
    scanf("%s",cmd);
    if (strcmp(cmd,"online") == 0)
    {
        return DATA_ONLINE;
    }
    if (strcmp(cmd,"logout") == 0)
    {
        return DATA_LOGOUT;
    }
    if (strcmp(cmd,"chat") == 0)
    {
        return DATA_CHAT;
    }
    if (strcmp(cmd,"file") == 0)
    {
        return DATA_FILE;
    }
}

/********************************************************
 * 获取聊天内容函数
 * 参数 
 * 返回  无
 * *****************************************************/
void get_msg(char * msg)
{
    if(fgets(msg,MAX_DATA_SIZE,stdin) == NULL)
    {
        perror("get_msg: fgets");
        exit(1);
    }
    int len = strlen(msg);
    if(msg[len -1 ] == '\n')
    {
        msg[len - 1 ] = '\0';
    }
}
void show_help()
{
    printf("【输入 online 获取当前在线用户】\n"
           "【输入 chat   开始聊天】\n" 
           "【输入 logout 退出登录】\n"
           "【输入 file   发送文件】\n"
           "【输入 help   获取帮助】\n");
}