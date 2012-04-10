/************************************************************************************

  文件： C语言 链式队列的接口
  作者： 余   强
  日期： 2009-11-4
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


/* 创建一个新的QNode节点，并返回新节点的指针 */
QNode* get_qnode(int data)
{
	QNode* newnode = (QNode*)malloc(sizeof(QNode));
	if(newnode == NULL)
		return NULL;
	newnode->data = data;
	newnode->next = NULL;

	return newnode;
}

/* 初始化一个队列 */
void init_queue(QHead** qhead_p)
{
	(*qhead_p) = (QHead*)malloc(sizeof(QHead));
	if((*qhead_p) == NULL)
		return;
	
	(*qhead_p)->front = NULL;
	(*qhead_p)->rear = NULL;
}

/* 向队列尾部(rear)中添加元素 */
int append(QHead* qhead, QNode* qnode)
{
	if(qhead == NULL || qnode==NULL)
		return -1;

	if(qhead->rear == NULL){
		qhead->front = qnode;
		qhead->rear  = qnode;
	}else{
		qhead->rear->next = qnode;
		qhead->rear = qnode;
	}
	
	return 0;
}

/* 从队列头部(front)取出元素 */
int serve(QHead* qhead)
{
	if(qhead->front == NULL)
		return -1;

	QNode* top_node = qhead->front;
	qhead->front = top_node->next;
	free(top_node);
	
	/* 判断是否删除最后一个元素 */
	if(qhead->front == NULL)
		qhead->rear = NULL;
	
	return 0;
}

/* 提取队列头部元素 */
int retrieve(QHead* qhead,int* pItem)
{
	if(qhead == NULL)
		return -1;

	if(qhead->front == NULL)
		return -1;

	*pItem = qhead->front->data;

	return 0;
}

/* 遍历队列 */
int Traversing(QHead* qhead, void process(QNode*))
{
	if(qhead == NULL || qhead->front == NULL)
		return -1;

	QNode *pnode = qhead->front;
	while(pnode != NULL){
		process(pnode);
		pnode = pnode->next;
	}
	
	return 0;
}

/* 判断队列是否为空 */
int empty(const QHead* qhead)
{
	if(qhead->front == NULL)
		return 0;
	else
		return -1;
}

/* 删除队列 */
int cleanup(QHead** qhead_p)
{
	if((*qhead_p) == NULL)
		return -1;

	QNode* qnode = (*qhead_p)->front;
	while(qnode != NULL){
		free(qnode);
		qnode = qnode->next;
	}

	(*qhead_p)->front = NULL;
	(*qhead_p)->rear = NULL;
	(*qhead_p) = NULL;

	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

