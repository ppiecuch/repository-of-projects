/************************************************************************************

  文    件: C语言AVL高度平衡树 测试程序 
  作    者: 余   强
  日    期: 2009-11-20~2009-11-27
  协    议: Copyleft (C) GPL
 E-mail: yuembed@126.com
  博    客:『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>
#include "avltree.h"

void display(ANode* node)
{
	printf("%d ", node->data);
}

void display2(ANode* node)
{
	printf("num: %d %d \n", node->data, node->factor);
}

int main(int args, char* argv[])
{
	ATree* root;
	int i=0;

	/* 创建一颗整数AVL二叉树 */
	InitTree(&root);
	printf("\nInitATree OK! \n");

	int array[] ={45, 56, 34, 13, 23, 75, 15, 20, 65 ,11};
	//int array[] ={66, 33, 77, 11, 22, 99, 55, 44, 88, 0};
	for(i=0; i<10; i++){
		Insert(&root, array[i]);
	}

	/* 接口测试 */	
	/* FIXME:done */

	/* 边缘测试 */
	/* FIXME:done */

	/* 功能测试 */
	LNR(root, display2);
	printf("\n");

	NLR(root, display);
	printf("\n");

	LRN(root, display);
	printf("\n");

	
	printf("\ndel 56\n");
	printf("del 75\n");
	Delete(&root, 56);
	Delete(&root, 75);

	//Delete(&root, 56);
	//Delete(&root, 45);
	//Delete(&root, 34);
	//Delete(&root, 23);

	LNR(root, display2);
	printf("\n");

	LNR(root, display);
	printf("\n");

	NLR(root, display);
	printf("\n");

	LRN(root, display);
	printf("\n");

	if(Remove(&root) == 0){
		NLR(root, display);
		printf("Remove tree OK!\n");
	}

	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

