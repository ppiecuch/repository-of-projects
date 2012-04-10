/************************************************************************************

  文件： C语言二叉树 源码及注解
  作者： 余   强
  日期： 2009-11-3
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc */
#include "btree.h"


/**********************************************************************************/

/* 此初始化函数将初始化如下结构的一个满二叉树。以测试各个函数的功能
         1
       /   \
      2     3
     / \   / \
   4    5 6    7
 *
*/

/* 初始化二叉树 */
/* ---------------------------------------------------------------------
 函数名称： InitBTree
 参    数： 无
 返回参数： 树根节点
 功能描述： 初始化一颗二叉树
 -----------------------------------------------------------------------
*/
BNode* InitBTree(void)
{
	BNode* newnode = NULL;
	printf("test ! \n");
	BNode* newl_p = NULL;
	BNode* newr_p = NULL;
	BNode* newl1_p = NULL;
	BNode* newr1_p = NULL;

	BNode* new2_p = NULL;
	new2_p = GetBNode(8, NULL, NULL);
	
	
	newl_p = GetBNode(4, NULL, NULL);
	newr_p = GetBNode(5, NULL, new2_p);
	newl1_p = GetBNode(2, newl_p, newr_p);
	

	newl_p = GetBNode(6, NULL, NULL);
	newr_p = GetBNode(7, NULL, NULL);
	newr1_p = GetBNode(3, newl_p, newr_p);

	printf("test ! \n");

	newnode = GetBNode(1, newl1_p, newr1_p);

	printf("test ! \n");

	return newnode;
}

/* ---------------------------------------------------------------------
 函数名称： GetBNode
 参    数： data  --创建节点的数据
	   left --新节点的左子树
	   right --新节点的右子树
 返回参数： nCount -- 叶子的数目
 功能描述： 创建一个新节点的函数
 -----------------------------------------------------------------------
*/
BNode*  GetBNode(int data, BNode* left, BNode* right)
{
	BNode* newnode = (BNode*)malloc(sizeof(struct node));
	if(newnode == NULL){
		printf("error:no memory!\n");
		return NULL;
	}
	
	newnode->data = data;
	newnode->left = left;
	newnode->right = right;

	return newnode;
}

/* ---------------------------------------------------------------------
 函数名称： CopyATree
 参    数： srcroot     -- 树的根节点
 返回参数： newroot  -- 新术的根节点指针
          NULL     -- 空树
 功能描述： 利用后序遍历的方法拷贝一个二叉树
 -----------------------------------------------------------------------
*/
BNode*  CopyBTree(BNode* srcroot)
{
	if(srcroot != NULL){
		BNode* newlptr = CopyBTree(srcroot->left);
		BNode* newrptr = CopyBTree(srcroot->right);
		BNode* newroot = GetBNode(srcroot->data, newlptr, newrptr);
		return newroot;
	}

	return NULL;
}

/* ---------------------------------------------------------------------
 函数名称： CountLeaf
 参    数： root   -- 树的跟节点
 返回参数： nCount -- 叶子的数目
 功能描述： 计算二叉树的叶子
 -----------------------------------------------------------------------
*/
int CountLeaf(BNode* root)
{
	int nCount = 0;
	if(root != NULL){
		nCount += CountLeaf(root->left);
		nCount += CountLeaf(root->right);

		if(root->left == NULL && root->right == NULL)
			nCount++;
	}

	return nCount;
}

/* ---------------------------------------------------------------------
 函数名称： Depth
 参    数： root   -- 树的跟节点
 返回参数： 无
 功能描述： 计算二叉树的深度
 -----------------------------------------------------------------------
*/
int Depth(BNode* root)
{
	int depth = -1, ldepth=0, rdepth=0;
	if(root != NULL){

		ldepth = Depth(root->left);
		rdepth = Depth(root->right);
		depth  = 1 + (ldepth > rdepth ? ldepth : rdepth);	
	}

	return depth;
}

/* 前缀词中的“前”“中”“后”分别表示对节点的访问发生在何时 */
/* 中序遍历二叉树（左、节点、右） */
void LNR(BNode* root, void process(int))
{
	if(root != NULL){
		LNR(root->left, process);
		process(root->data);
		LNR(root->right, process);
	}
}


/* 后序遍历二叉树（左、右、节点）*/
void LRN(BNode* root, void process(int))
{
	if(root != NULL){
		LRN(root->left, process); /* 遍历左子树 */
		LRN(root->right, process); /* 遍历右子树 */
		process(root->data);      /* 访问节点 */
	}
}

/* 前序遍历二叉树（节点、左、右） */
void NLR(BNode* root, void process(int))
{
	if(root != NULL){
		process(root->data);
		NLR(root->left, process);
		NLR(root->right, process);
	}
}


/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

