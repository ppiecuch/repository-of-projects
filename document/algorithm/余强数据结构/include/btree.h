/************************************************************************************

  文件： C语言二叉树 源码及注解 接口文件
  作者： 余   强
  日期： 2009-11-3
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef struct node{
	int data;
	struct node* left;
	struct node* right;
}BNode;


/***********************************************************/
/* 操作函数 */


BNode*  InitBTree(void);    /* 二叉树初始化函数 */

void    DeleteBTree(BNode* root);      /* 删除二叉树 */

BNode*  CopyBTree(BNode* srcroot);     /* 拷贝二叉树 */

BNode*  GetBNode(int data, BNode* left, BNode* right);  /* 创建二叉树节点 */


/* 树扫描算法的应用 */
int CountLeaf(BNode* root); /* 计算二叉树子叶节点个数 */
int Depth(BNode* root);                    /* 计算二叉树深度 */


#endif
/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

