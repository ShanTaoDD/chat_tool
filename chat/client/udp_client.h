#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H
#include "protocol.h"
#include <pthread.h>
int  udp_client_init();
void  send_data(int fd,NET_PACKET*packet);
void recv_data(int fd,NET_PACKET*packet);



#endif