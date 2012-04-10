/************************************************************************************

  文件： C语言 链式栈 测试程序
  作者： 余   强
  日期： 2009-11-4
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>
#include "stack.h"


void display(SNode* node)
{
	printf("%d ", node->item);
}

int main()
{
	SNode* myStack = NULL;
	
	/* 接口测试 */
	push(&myStack, 1);
	push(&myStack, 2);
	push(&myStack, 3);
	push(&myStack, 4);
	pop(&myStack);
	pop(&myStack);
	push(&myStack, 5);
	push(&myStack, 6);
	push(&myStack, 9);
	push(&myStack, 8);
	pop(&myStack);

	/* 功能测试 */
	int num;
	top(myStack, &num);

	Traversing(myStack, display);
	printf("\n");

	printf("the top number of stack is: %d \n", num);

	invert(&myStack);
	printf("invert Ok \n");

	Traversing(myStack, display);
	printf("\n");

	clearup(&myStack);
	printf("clear up OK \n");
	Traversing(myStack, display);
	printf("Nohing below OK \n");

	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

