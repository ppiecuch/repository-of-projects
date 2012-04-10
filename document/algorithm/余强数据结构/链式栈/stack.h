/************************************************************************************

  文件： C语言 链式栈的接口
  作者： 余   强
  日期： 2009-11-4
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_node{
	int item;
	struct stack_node* next;
}SNode;

enum Error_code{success=0, overflow=-1, underflow=-2};

/* 检查栈是否为空C语言没有bool类型 */
int empty(SNode* top_node);

int push(SNode** top_node, const int item);

int pop(SNode** top_node);

int top(SNode* top_node,int* pitem);

void Traversing(SNode* top_node,void process(SNode*));

void invert(SNode** top_node);

#endif

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

