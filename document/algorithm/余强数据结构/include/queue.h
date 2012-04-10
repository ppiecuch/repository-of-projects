/************************************************************************************

  文件： C语言 链式队列的接口
  作者： 余   强
  日期： 2009-11-4
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_

/* 队列需要的数据结构 */
typedef struct queue_node{
	int data;
	struct queue_node* next;
}QNode;


typedef struct queue_head{
	QNode* front;
	QNode* rear;
}QHead;

/* 
 使用说明：
 当使用QHead* your_queue定义自己的队列的时候
 在使用QHead之前 现调用init_queue(&your_queue)函数

 队列是从头部(front)删除元素，相当于栈顶出栈一样
 从尾部(rear)添加元素
 */


/* 创建一个新的QNode节点，并返回新节点的指针 */
QNode* get_qnode(int data);

/* 初始化一个队列 */
void init_queue(QHead** qhead_p);

/* 向队列尾部(rear)中添加元素 */
int append(QHead* qhead, QNode* qnode);

/* 从队列头部(front)取出元素 */
int serve(QHead* qhead);

/* 提取队列头部元素 */
int retrieve(QHead* qhead,int* pItem);

/* 遍历队列 */
int Traversing(QHead* qhead, void process(QNode*));


/* 检查栈是否为空C语言没有bool类型 */
int empty(const QHead* qhead);

/* 删除队列 */
int cleanup(QHead** qhead_p);

/* 队列扩展功能 */
//int full(QNode** queue_p) const;
//int size(QNode** queue_p) const;
//void clearup(QNode** queue_p);

#endif

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

