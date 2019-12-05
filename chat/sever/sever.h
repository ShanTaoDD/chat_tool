#ifndef _SEVER_H
#define _SEVER_H
#include "protocol.h"
#include "addr_list.h"
int server_init(struct sockaddr_in* server_addr);
int sever_init();
void send_data(int severfd,const NET_PACKET * netpacket);
void recv_data(int severfd,NET_PACKET * netpacket);
void add_usr(int serverfd,addr_t *list,NET_PACKET * netpacket);
void remove_usr(int fd,addr_t *list,NET_PACKET * netpacket);
void send_onlines(int severfd ,addr_t *addr_list,NET_PACKET * netpacket);
#endif