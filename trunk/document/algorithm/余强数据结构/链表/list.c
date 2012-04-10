/************************************************************************************

  文件： C语言 链表的实现
  作者： 余   强
  日期： 2009-11-6
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdlib.h>
#include "list.h"

/* 本地函数声明 */
static LNode* set_position(LHead* list_head, int position)
{
	if(position < 0 || position >= size(list_head))
		return NULL;
	
	LNode* pNode = list_head;	
	while(position-- > 0){
		pNode = pNode->next;
	}

	return pNode;
}

/* 创建一个新的链表节点 */
LNode* GetLNode(dtype data, LNode* next)
{
	LNode* newnode = (LNode*)malloc(sizeof(LNode));

	if(newnode == NULL)
		return NULL;

	newnode->data = data;
	newnode->next = next;

	return newnode;
}

/* 链表初始化函数 */
BOOL InitList(LHead** list_head)
{
	(*list_head) = NULL;

	return true;
}

/* 检查栈是否为空 */
BOOL empty(LHead* list_head)
{
	if(list_head == NULL)
		return true;
	else
		return false;
}

/* 查看链表的长度 */
int size(LHead* list_head)
{
	int nCount = 0;
	LNode* pNode = list_head;
	while(pNode != NULL){
		nCount += 1;
		pNode = pNode->next;
	}

	return nCount;
}

/* 向链表中插入一个数据 */
int insert(LHead** list_head, int position, dtype data)
{
	LNode* parent = *list_head;
	LNode* item = GetLNode(data, NULL); /* 创建一个新的节点 */
	
	if(position < 0 || position > size(*list_head)) /* 可以等于num */
		return -1;
	
	while(--position > 0){
		parent = parent->next;
	}

	if(parent == *list_head){
		item->next = parent;
		*list_head = item;
	}else{
		item->next = parent->next;
		parent->next = item;
	}

	return 0;
}

/* 从链表中删除一个数据 */
int Remove(LHead** list_head, int position)
{
	if(position < 0 || position >= size(*list_head))
		return -1;

	LNode* parent = *list_head;
	LNode* toDelete = NULL;
	while(--position > 0)
		parent = parent->next;

	if(parent == *list_head){
		toDelete = parent;
		*list_head = toDelete->next;
	}else{
		toDelete = parent->next;
		parent->next = toDelete->next;
	}

	free(toDelete);
	
	return 0;
}

/* 情况链表 */
void clear(LNode** list_head)
{
	LNode* pNode = NULL;
	LNode* toDelete = *list_head;
	while(toDelete != NULL){
		pNode = toDelete->next;   /* 保存即将删除的那个节点的下一个节点地址 */
		free(toDelete);
		toDelete = pNode;
	}

	*list_head = NULL;
}

/* 替换链表中指定位置的数据 */
int replace(LHead* list_head, int position, dtype data)
{
	if(position < 0 || position >= size(list_head))	/* position不能等于size(list_head) */
		return -1;

	LNode* pNode = list_head;
	while(position-- > 0)
		pNode = pNode->next;

	pNode->data = data;
	
	return 0;
}

/* 检索链表中指定位置的数据 */
int retrieve(LHead* list_head, int position, dtype* data_p)
{
	if(position < 0 || position >= size(list_head))	/* position不能等于size(list_head) */
		return -1;

	LNode* pNode = list_head;
	while(position-- > 0)
		pNode = pNode->next;

	*data_p = pNode->data;

	return 0;
}

/* 遍历链表 */
void traverse(LHead* list_head,void process(LNode*))
{
	LNode* pNode = list_head;
	while(pNode != NULL){
		process(pNode);
		pNode = pNode->next;
	}
}


/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

