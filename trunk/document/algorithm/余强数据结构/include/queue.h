/************************************************************************************

  �ļ��� C���� ��ʽ���еĽӿ�
  ���ߣ� ��   ǿ
  ���ڣ� 2009-11-4
  Э�飺 Copyleft (C) GPL
 E-mail: yuembed@126.com
  �ռ䲩�� ��20065562's Blog�� ��ˮ�ĵط������� http://hi.baidu.com/20065562

************************************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_

/* ������Ҫ�����ݽṹ */
typedef struct queue_node{
	int data;
	struct queue_node* next;
}QNode;


typedef struct queue_head{
	QNode* front;
	QNode* rear;
}QHead;

/* 
 ʹ��˵����
 ��ʹ��QHead* your_queue�����Լ��Ķ��е�ʱ��
 ��ʹ��QHead֮ǰ �ֵ���init_queue(&your_queue)����

 �����Ǵ�ͷ��(front)ɾ��Ԫ�أ��൱��ջ����ջһ��
 ��β��(rear)���Ԫ��
 */


/* ����һ���µ�QNode�ڵ㣬�������½ڵ��ָ�� */
QNode* get_qnode(int data);

/* ��ʼ��һ������ */
void init_queue(QHead** qhead_p);

/* �����β��(rear)�����Ԫ�� */
int append(QHead* qhead, QNode* qnode);

/* �Ӷ���ͷ��(front)ȡ��Ԫ�� */
int serve(QHead* qhead);

/* ��ȡ����ͷ��Ԫ�� */
int retrieve(QHead* qhead,int* pItem);

/* �������� */
int Traversing(QHead* qhead, void process(QNode*));


/* ���ջ�Ƿ�Ϊ��C����û��bool���� */
int empty(const QHead* qhead);

/* ɾ������ */
int cleanup(QHead** qhead_p);

/* ������չ���� */
//int full(QNode** queue_p) const;
//int size(QNode** queue_p) const;
//void clearup(QNode** queue_p);

#endif

/* �ļ���Դ����20065562's Blog����ˮ�ĵط������� http://hi.baidu.com/20065562 */

