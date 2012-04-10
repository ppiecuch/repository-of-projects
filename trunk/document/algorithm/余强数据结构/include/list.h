/************************************************************************************

  �ļ��� C���� ����Ľӿ�
  ���ߣ� ��   ǿ
  ���ڣ� 2009-11-6
  Э�飺 Copyleft (C) GPL
 E-mail: yuembed@126.com
  �ռ䲩�� ��20065562's Blog�� ��ˮ�ĵط������� http://hi.baidu.com/20065562

************************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

typedef enum{true, false}BOOL;

typedef int dtype;      /* ��������Ը���dtype������Ϊ����Ҫ������ */

typedef struct list_node{
	dtype data;
	struct list_node* next;
}LNode,LHead;


/* ���ջ�Ƿ�Ϊ��C����û��bool���� */
LNode* GetLNode(dtype data, LNode* next);         /* ����һ������ڵ� */

BOOL InitList(LHead** list_head);

BOOL empty(LHead* list_head);          /* �жϴ˱��Ƿ�Ϊ�� */

int size(LHead* list_head);            /* �鿴��ĳ��� */

int insert(LHead** list_head, int position, dtype data);   /* ����в��� */

int Remove(LHead** list_head, int position);      /* �ӱ���ɾ�� */

int retrieve(LHead* list_head, int position, dtype* data_p);    /* ��������ָ��λ�õ�Ԫ�� */

int replace(LHead* list_head, int position, dtype data);        /* �滻����ָ��λ�õ�Ԫ�� */

void traverse(LHead* list_head,void process(LNode*)); /* ������ */

void clear(LNode** list_head);            /* ������� */

#endif

/* �ļ���Դ����20065562's Blog����ˮ�ĵط������� http://hi.baidu.com/20065562 */

