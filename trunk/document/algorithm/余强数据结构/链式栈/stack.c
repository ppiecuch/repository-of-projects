/************************************************************************************

  文件： C语言 链式栈的实现
  作者： 余   强
  日期： 2009-11-4
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* 检查栈是否为空 */
int empty(SNode* top_node)
{
	if(top_node == NULL)
		return 0;
	else
		return -1;
}


//
int push(SNode** top_node, const int item)
{
	SNode* newnode = (SNode*)malloc(sizeof(struct stack_node));
	if(newnode == NULL)
		return -1;

	newnode->item = item;
	newnode->next = *top_node;
	
	*top_node = newnode;

	return 0;
}

//
int pop(SNode** top_node)
{
	SNode* oldtop = (*top_node);

	if((*top_node) == NULL)
		return -1;

	(*top_node) = oldtop->next;
	free(oldtop); /* 释放空间内存 */

	return 0;
}


int top(SNode* top_node,int *item)
{
	*item = top_node->item;
	
	return 0;
}


void Traversing(SNode* top_node,void process(SNode*))
{
	SNode* pNode = top_node;
	while(pNode != NULL){
		process(pNode);
		pNode = pNode->next;
	}
}

void invert(SNode** top_node)
{
	SNode* newStack = NULL; /* 当新建一个栈的时候，必须初始化为空NULL */

	while((*top_node) != NULL){
		push(&newStack,(*top_node)->item);
		pop(top_node);
	}

	(*top_node) = newStack;

}

void clearup(SNode** top_node)
{
	while(*top_node != NULL)
		pop(top_node);
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

