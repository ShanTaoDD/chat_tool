#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
#include "protocol.h"
#include "headr.h"  

int  tcp_client_init();
void *tcp_recv_thread(void * tcp_fd);
//void *file_send_thread(void *udp_fd);
void* tcp_send_thread(void *tcp_fd);
#endif 