/************************************************************************************

  文件： C语言 链式队列 测试程序
  作者： 余   强
  日期： 2009-11-4
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>
#include "queue.h"


void display(QNode* node)
{
	printf("%d ", node->data);
}

int main()
{
	/* 接口测试 */
	QHead* myQueue;
	init_queue(&myQueue);

	/* 功能测试 */
	QNode* node;

	node = get_qnode(1);
	append(myQueue, node);
	node = get_qnode(2);
	append(myQueue, node);
	node = get_qnode(3);
	append(myQueue, node);
	node = get_qnode(4);
	append(myQueue, node);
	node = get_qnode(5);
	append(myQueue, node);
	node = get_qnode(6);
	append(myQueue, node);
	node = get_qnode(7);
	append(myQueue, node);

	Traversing(myQueue, display);
	printf("\n");

	serve(myQueue);
	serve(myQueue);
	
	Traversing(myQueue, display);
	printf("\n");

	node = get_qnode(1);
	append(myQueue, node);

	Traversing(myQueue, display);
	printf("\n");

	cleanup(&myQueue);
	Traversing(myQueue, display);
	printf("clean up OK! \n");
	printf("Nothing will be display blow OK! \n\n");
	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

