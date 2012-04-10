/************************************************************************************

  文件： C语言 链表的接口
  作者： 余   强
  日期： 2009-11-6
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

typedef enum{true, false}BOOL;

typedef int dtype;      /* 在这里可以更改dtype的类型为你想要的类型 */

typedef struct list_node{
	dtype data;
	struct list_node* next;
}LNode,LHead;


/* 检查栈是否为空C语言没有bool类型 */
LNode* GetLNode(dtype data, LNode* next);         /* 创建一个链表节点 */

BOOL InitList(LHead** list_head);

BOOL empty(LHead* list_head);          /* 判断此表是否为空 */

int size(LHead* list_head);            /* 查看表的长度 */

int insert(LHead** list_head, int position, dtype data);   /* 向表中插入 */

int Remove(LHead** list_head, int position);      /* 从表中删除 */

int retrieve(LHead* list_head, int position, dtype* data_p);    /* 检索表中指定位置的元素 */

int replace(LHead* list_head, int position, dtype data);        /* 替换表中指定位置的元素 */

void traverse(LHead* list_head,void process(LNode*)); /* 遍历表 */

void clear(LNode** list_head);            /* 将表清空 */

#endif

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

