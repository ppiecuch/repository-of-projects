/************************************************************************************

  文件： C语言查找二叉树 测试程序 
  作者： 余   强
  日期： 2009-11-5
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>
#include "sbtree.h"

void display(int var)
{
	printf("%d ", var);
}

int main(int args, char* arr[])
{
	BTree* root;
	int i=0;

	/* 创建一颗整数二叉树 */
	InitBTree(&root);
	printf("\nInitBTree OK! \n");


	int array[] ={ 45, 56, 34, 13, 23, 75, 15, 20, 65 ,11};
	for(i=0; i<10; i++){
		Insert(&root, array[i]);
	}



	int num = 34;


	Delete(&(root->left));

	LNR(root, display);
	printf("\n");

	
	NLR(root, display);
	printf("\n");

	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

