#ifndef _UDP_SEND_THREAD_H
#define _UDP_SEND_THREAD_H
#include "udp_client.h"
#include <pthread.h>



void* udp_send_thread(void *fd);
void login(int fd,NET_PACKET * packet);
void logout(int fd,NET_PACKET * packet);
void online(int fd ,NET_PACKET * packet);
void chat(int fd ,NET_PACKET * packet);
int  get_cmd();
void show_help();

#endif