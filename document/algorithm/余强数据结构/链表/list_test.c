/************************************************************************************

  文件： C语言 链表 测试程序
  作者： 余   强
  日期： 2009-11-6
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>
#include "list.h"


void display(LNode* node)
{
	printf("%d ", node->data);
}

int main()
{
	LHead* myList;

	/************************************************/
	/* 接口测试 */
	InitList(&myList);

	insert(&myList, 0, 1);

	insert(&myList, 0, 2);
	insert(&myList, 0, 3);
	insert(&myList, 0, 4);
	insert(&myList, 0, 5);
	insert(&myList, 0, 6);
	insert(&myList, 0, 7);
	insert(&myList, 0, 8);

	if(empty(myList) != true)
		printf("List is not empty !\n");
	printf("The size of list is %d !\n", size(myList));

	traverse(myList, display);
	printf("\n\n");


	/************************************************/
	/* 功能测试 */

	/* 测试在指定的位置插入数据 */
	insert(&myList, 3, 10);
	printf("Insert 10 in position 3 OK \n");
	traverse(myList, display);
	printf("\n\n");

	/* 测试删除指定位置的数据 */
	Remove(&myList, 1);
	printf("remove the number in position(1) OK \n");
	traverse(myList, display);
	printf("\n\n");


	/* 替换指定位置的数据 */
	replace(myList, 5, 100);
	printf("replace the number in positon(5) to 100 OK !\n");
	traverse(myList, display);
	printf("\n\n");

	/************************************************/
	/* 错误测试 */

	/* 在错误的位置插入数据 */
	int error = insert(&myList, 9, 200);
	if(error == -1)
	printf("failed insert: error position 10 !\n");
	traverse(myList, display);
	printf("\n\n");

	/************************************************/
	/* 临界点测试 */
	/* 测试在指定的位置插入数据 */
	insert(&myList, 8, 200);
	printf("Insert 200 in position 9 OK \n");
	traverse(myList, display);
	printf("\n\n");

	clear(&myList);
	printf("clear  OK \n");
	traverse(myList, display);
	printf("Nohing is here? \n\n");

	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

