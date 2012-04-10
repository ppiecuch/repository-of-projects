/************************************************************************************

  文件： C语言二叉树 测试文件
  作者： 余   强
  日期： 2009-11-3
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#include <stdio.h>
#include "btree.h"

void display(int var)
{
	printf("%d ", var);
}

int main(int args, char* arr[])
{
	BNode* root;
	/* 创建一颗整数二叉树 */
	root = InitBTree();

	LNR(root, display);
	printf("\n");
	NLR(root, display);
	printf("\n");
	LRN(root, display);
	printf("\n");


	printf("InitBTree OK ! \n");
	/* 计算叶子节点的个数 */
	int pCount = 0;
	pCount = CountLeaf(root);
	printf("CountLeaf OK ! \n");
	printf("Number of leaf nodes is %d \n", pCount);

	/* 调用函数Depth计算二叉树的深度 */
	int dep = Depth(root);
	printf("the depth of the tree is %d \n", dep);

	return 0;
}


