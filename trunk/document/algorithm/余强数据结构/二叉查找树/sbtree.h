/************************************************************************************

  文件： C语言二叉树 源码及注解
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
}BNode, BTree;

/***********************************************************/
/* 操作函数 */


int     InitBTree(BTree** root_p);     /* 二叉树初始化函数 */

void    DeleteBTree(BNode* root);      /* 删除二叉树 */

BNode*  CopyBTree(BNode* srcroot);     /* 拷贝二叉树 */

BNode*  GetBNode(int data, BNode* left, BNode* right);  /* 创建二叉树节点 */

int     CountLeaf(BNode* root);        /* 计算二叉树子叶节点个数 */

int     Depth(BNode* root);            /* 计算二叉树深度 */


/***********************************************************/
/* 在二叉树的基础上添加查找二叉树的特性 */

BNode*  Search(BNode* root,const int Key);  /* 在查找二叉树中搜索Key */

int     Insert(BTree** root_p, int data);   /* 向查找二叉树中插入一个新的节点 */

int     Delete(BNode** node_p);             /* 从查找二叉树中删除一个节点 */

#endif
/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

