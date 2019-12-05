#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "protocol.h"
#include "udp_client.h"
#include "udp_send_thread.h"
#include "udp_recv_thread.h"
#include "tcp_client.h"

pthread_mutex_t login_mutex = PTHREAD_MUTEX_INITIALIZER; /*登录成功互斥变量*/
pthread_cond_t  login_cond = PTHREAD_COND_INITIALIZER; /*登录成功条件变量*/

int main()  
{
    /*初始化客户端*/
    int udp_fd = udp_client_init();
   
    printf("初始化成功\n");
    pthread_t udp_send_tid,udp_recv_tid,tcp_send_tid,tcp_recv_tid;
   
    if(pthread_create(&udp_send_tid,NULL,udp_send_thread,&udp_fd) < 0 )
    {
        perror("main : pthread_create: udp_send_thread");
        exit(-1);
    }
    /*等待登录成功*/
    pthread_mutex_lock(&login_mutex);
    pthread_cond_wait(&login_cond,&login_mutex);
    pthread_mutex_unlock(&login_mutex);

    
     int tcp_fd = tcp_client_init();
    if(pthread_create(&udp_recv_tid,NULL,udp_recv_thread,&udp_fd) < 0 )
    {
        perror("main : pthread_create: udp_recv_thread");
        exit(-1);
    }
    if(pthread_create(&tcp_recv_tid,NULL,tcp_recv_thread,&tcp_fd) < 0 )
    {
        perror("main :pthread_cread:file_recv_thread");
        exit(-1);
    }
    if(pthread_create(&tcp_send_tid,NULL,tcp_send_thread,&tcp_fd) < 0 )
    {
        perror("main :pthread_cread:file_send_thread");
        exit(-1);
    }
    pthread_join(udp_send_tid,NULL);
    exit(1);
}
