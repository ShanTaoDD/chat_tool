#ifndef _ADDR_LIST_H
#define _ADDR_LIST_H
/***********************
 * 在线用户以链表的形式存储。
 * *********************/

/**定义在线用户存储数据结构*/
typedef struct addr
{
    int ip;
    int port;
    struct addr *next;
}addr_t;

/*创建链表*/
addr_t *addr_list_creat();
/*增加在线用户*/
void addr_list_insert(addr_t *head,int ip,int port);
/*移除在线用户*/
void addr_list_remove(addr_t *head,int ip,int port);
/*销毁链表*/
void addr_list_destory(addr_t *head);
/*打印链表*/
void display(addr_t *head);

#endif