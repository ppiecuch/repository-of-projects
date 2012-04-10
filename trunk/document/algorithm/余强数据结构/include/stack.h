/************************************************************************************

  �ļ��� C���� ��ʽջ�Ľӿ�
  ���ߣ� ��   ǿ
  ���ڣ� 2009-11-4
  Э�飺 Copyleft (C) GPL
 E-mail: yuembed@126.com
  �ռ䲩�� ��20065562's Blog�� ��ˮ�ĵط������� http://hi.baidu.com/20065562

************************************************************************************/
#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_node{
	int item;
	struct stack_node* next;
}SNode;

enum Error_code{success=0, overflow=-1, underflow=-2};

/* ���ջ�Ƿ�Ϊ��C����û��bool���� */
int empty(SNode* top_node);

int push(SNode** top_node, const int item);

int pop(SNode** top_node);

int top(SNode* top_node,int* pitem);

void Traversing(SNode* top_node,void process(SNode*));

void invert(SNode** top_node);

#endif

/* �ļ���Դ����20065562's Blog����ˮ�ĵط������� http://hi.baidu.com/20065562 */

