#ifndef _PROTOCOL_H
#define _PROTOCOL_H
/*服务器地址和端口*/
#define SEVER_ADDR "192.168.1.111"
#define SEVER_PORT  8888

#define MAX_DATA_SIZE 1024  /*最大消息正文长度*/

/*定义消息类型*/
#define DATA_NONE	0x00
#define DATA_ONLINE 0x01 
#define DATA_CHAT	0x02
#define DATA_LOGIN	0x04
#define DATA_LOGOUT	0x08
#define DATA_FILE	0x10

/*******网络协议包************/
typedef struct
{
    int src_ip;/*接收方IP地址*/
    int src_port;/*接收方端口号*/

    int dst_ip;/*发送方IP地址*/
    int dst_port;/*发送方端口号*/

    int data_type;/*消息类型*/

    char data[MAX_DATA_SIZE]; /*消息正文*/
}NET_PACKET;



#endif 