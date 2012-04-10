/************************************************************************************

  文    件: C语言红黑树 源码及注解
  作    者: 余   强
  日    期: 2009-12-04~2009-12~08
  协    议: Copyleft (C) GPL
  E-mail: yuembed@126.com
  博    客:『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>
#include "rbtree.h"

void display(RNode* node)
{
	printf("%d ", node->data);
}

void display2(RNode* node)
{
	printf("num: %d", node->data);
	if(node->color == BLACK)
		printf(" B\n");
	else
		printf(" R\n");
}

int main(int args, char* argv[])
{
	RTree* root;
	int i=0;

	/* 创建一颗整数AVL二叉树 */
	InitTree(&root);
	printf("\nInitRTree OK! \n");

	int array[] ={4, 7, 12, 15, 3, 5, 14};

	for(i=0; i<7; i++){
		Insert(&root, array[i]);
	}



	/* 接口测试 */	
	/* FIXME:done */

	/* 边缘测试 */
	/* FIXME:done */

	/* 功能测试 */

	printf("\n");

	LNR(root, display2);

	printf("\nNLR: ");
	NLR(root, display);

	printf("\nLRN: ");
	LRN(root, display);

	Delete(&root, 3);
	Delete(&root, 5);
	Delete(&root, 12);
	Delete(&root, 15);
	Delete(&root, 7);
	Delete(&root, 14);
	Delete(&root, 4);


	Insert(&root, 1);
	Insert(&root, 2);
	Insert(&root, 3);
	Insert(&root, 4);
	Insert(&root, 5);
	Insert(&root, 6);
	Insert(&root, 7);
	Insert(&root, 8);
	Insert(&root, 9);
	Insert(&root, 10);
	Insert(&root, 11);
	Insert(&root, 12);


	printf("\n---------------------\n");


	LNR(root, display2);

	printf("\nLNR: ");
	LNR(root, display);

	printf("\nNLR: ");
	NLR(root, display);

	printf("\n");
	if(Remove(&root) == 0){
		NLR(root, display);
		printf("Remove tree OK!\n");
	}

	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

