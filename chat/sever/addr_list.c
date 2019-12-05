#include <stdio.h>
#include <stdlib.h>

#include "addr_list.h"

static addr_t* addr_list_find(addr_t *head,int ip,int port);

/******************************
 * 创建链表函数
 * 参数 无
 * 返回 addr_t的头节点指针
 * ***************************/
addr_t *addr_list_creat()
{
    addr_t *head;
    head = (addr_t *)malloc(sizeof(addr_t));
    if (head == NULL)
    {
        perror("add_list_creat :malloc");
        exit(-1);
    }
    head->next = NULL; /*创建头结点完毕后指向空*/

}

/*******************************************************************
 * 寻找节点函数
 * 参数 head 链表头结点  ip要寻找用户的ip地址  port 要寻找用户的端口号
 * 返回 addr_t寻找节点的指针
 * ****************************************************************/
addr_t *addr_list_find(addr_t *head,int ip,int port)
{
    addr_t *p = head->next;/*来接收头结点指针，防止丢失*/
    while((p != NULL) &&((p->ip!= ip) || (p->port != port)))
    {
        p = p->next; /*p指向当前节点*/
    }
    return p;
}
/**********************************************************************
 * 增加节点函数
 * 参数 head 链表头节点指针 IP 增加用户IP地址 port 增加用户端口号
 * 返回值 无
 * ******************************************************************/
void addr_list_insert(addr_t *head,int ip, int port)
{
    if(addr_list_find(head,ip,port)  != NULL)
        return ;  /*如果该节点存在直接return*/
    addr_t *new = (addr_t * )malloc(sizeof(addr_t));/*为新节点申请空间*/
    if (new == NULL)
    {
        perror("addr_list_insert : malloc");
        exit(-1);
    }
    /*将新节点填充并添加到链表头节点后边*/
    new->ip = ip;
    new->port = port;

    new->next = head->next;
    head->next = new;
}
/*********************************************************
 * 删除节点函数
 * 参数 head 链表头结点指针， IP 要删除节点的ip  port 要删除节点的端口号
 * 返回  无
 * *****************************************************/
void addr_list_remove(addr_t *head,int ip,int port)
{
    addr_t *p = head;
    /*想要删除节点，必须找到他的前一个节点*/
    while((p->next !=NULL) && 
                            ((p->next->ip != ip) || (p->next->port != port)))
    {
        p = p->next;
    }
    if(p->next == NULL)
        return ;
    addr_t *tmp = p->next; /*要释放的是 tmp 而不是p*/
    /*将要删除的节点从链表断开，前后重连*/
    p->next = p->next->next;

    free (tmp);
}
/*****************************************************
 * 销毁链表函数
 * 参数 head 链表头结点函数
 * 返回值 无
 * **************************************************/
void addr_list_destory(addr_t *head)
{
    addr_t *p = head->next;
    while(p !=NULL)
    {
        head->next = p->next;
        free(p);
        p = head->next;
    }
    free(head);
    head =NULL;
   
}

/**************************************
 * 链表打印函数
 * 参数 head: 链表头结点指针
 * 返回  无
 * ***********************************/
void display(addr_t * head )
{
    addr_t *p = head->next;
    while(p !=NULL)
    {    
        printf("ip[%d],port[%d]\n",p->ip,p->port);
        p = p->next;
    }

    
}