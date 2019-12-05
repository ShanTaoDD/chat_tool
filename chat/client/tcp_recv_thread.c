#include "tcp_client.h"

static void *file_recv_thread(void * src_fd);
static void recv_filename(int src_fd,char *filename);
extern int LOCAL_IP;
extern int LOCAL_PORT;
/********************************************************
 * TCP接收线程，堵塞线程　负责给新连接创建子进程
 * 参数　tcp套接字
 * 返回　　无　
 * *****************************************************/
void *tcp_recv_thread(void *tcp_fd)
{
    /*设置线程分离属性*/
    pthread_detach(pthread_self()); 

    int fd = *(int *)tcp_fd;
    struct sockaddr_in src_addr;
	src_addr.sin_family = AF_INET;			
	src_addr.sin_port = LOCAL_PORT;		
	src_addr.sin_addr.s_addr = LOCAL_IP;
	
    socklen_t socklen = sizeof(src_addr);
    while(1)
    {   
       
        /*等待连接　　没有连接会一直堵塞*/
        int src_fd;
        if ((src_fd = accept(fd,(struct sockaddr *)&src_addr,&socklen)) < 0 )
        {
            perror("tcp_recv_thread: accpet");
            close(src_fd);
            return NULL;
        }
      
        /*接收文件子线程*/
        pthread_t file_recv_thid;
        if(pthread_create(&file_recv_thid,NULL,file_recv_thread,&src_fd) < 0)
        {
            perror("tcp_recv_thread : pthread_creare");
            exit(-1);
        }
    }
}

/******************************************************
 * 文件接收子线程
 * 参数　文件接收线程传回来已经连接上的　ＴＣＰｆｄ
 * 返回　无
 * ****************************************************/
void *file_recv_thread(void * src_fd)
{
    /*线程分离*/
    pthread_detach(pthread_self());
    int fd = *(int *)src_fd;
    char filename [MAX_DATA_SIZE];
    recv_filename(fd,filename);
    /*创建文件*/
    int file_fd ;
    if ((file_fd = open(filename,O_WRONLY | O_CREAT,0666)) < 0)
    {
        perror("file_recv_thread :open");
        exit(-1);
    }
    char buf[1024];
    int real_write;
    /*接收文件并转存*/
    while((real_write = recv(fd,&buf,sizeof(buf),0)) > 0)
    {
        write(file_fd,&buf,real_write);
    }
    close(fd);
    close(file_fd);
    return NULL;
}

/*******************************************************
 * 文件头部信息接收函数　（文件名，发送方，路径设置）
 * 参数　src_fd tcp套接字  filename 文件名存放地址
 * 返回  无
 * ****************************************************/

void recv_filename(int src_fd,char *filename)
{
    NET_PACKET packet;
    /*接收文件头部信息*/
    recv(src_fd,&packet,sizeof(packet),0);

    /*获取当前时间*/
    time_t t;
    time(&t);
    struct tm *pt;
    pt = gmtime(&t);
    char ip_str[20];
    inet_ntop(AF_INET,&packet.src_ip,ip_str,sizeof(ip_str));
    /*打印发送者信息*/
    printf("[收到文件! %d:%d:%d]\n"
            "[文件名：%s]\n"
            "[来自用户:IP%s,端口%d]\n",
            pt->tm_hour+8,pt->tm_min,pt->tm_sec,
            packet.data,
            ip_str,
            ntohs(packet.src_port)
            );
    /*设置接收文件路径*/
    strcpy(filename,"./file/");
    /*将文件名连接到保存路径后边　生成新的路径*/
    strcat(filename,packet.data);
}