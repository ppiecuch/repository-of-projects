/************************************************************************************

  文件： C语言查找二叉树 源码及注解
  作者： 余   强
  日期： 2009-11-5~2009-11-6
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* malloc() */
#include "sbtree.h"

/********************************
  下面是二叉树基本操作
*/ 

/* ---------------------------------------------------------------------
 函数名称： InitBTree
 参    数： root_p   -- 指向树根节点的指针的指针
 返回参数： 0 -- 初始化成功
 功能描述： 初始化一颗二叉树
 -----------------------------------------------------------------------
*/
int  InitBTree(BTree** root_p)
{
	*root_p = NULL;
	return 0;
}

/* ---------------------------------------------------------------------
 函数名称： GetBNode
 参    数： root   -- 树的根节点
 返回参数： nCount -- 叶子的数目
 功能描述： 创建一个新的节点函数
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
 函数名称： CountLeaf
 参    数： root   -- 树的根节点
 返回参数： nCount -- 叶子的数目
 功能描述： 利用后序遍历的方法拷贝一个二叉树
 -----------------------------------------------------------------------
*/
BNode*  CopyBTree(BTree* root)
{
	if(root != NULL){
		BNode* newlptr = CopyBTree(root->left);
		BNode* newrptr = CopyBTree(root->right);
		BNode* newroot = GetBNode(root->data, newlptr, newrptr);
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

/* ---------------------------------------------------------------------
 函数名称： LNR LRN NLR
 参    数： root   -- 树的跟节点     process  -- 函数指针
 返回参数： 无
 功能描述： 遍历
 -----------------------------------------------------------------------
*/
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
		LRN(root->left, process);  /* 遍历左子树 */
		LRN(root->right, process); /* 遍历右子树 */
		process(root->data);       /* 访问节点 */
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

/********************************
  下面是查找二叉树所具有的特性
*/ 

/* ---------------------------------------------------------------------
 函数名称： Search
 参    数： root   -- 树的跟节点     Key    -- 查找的数据
 返回参数： root   -- 指向找到的树节点
          NULL   -- 未找到数据
 功能描述： 在查找二叉树中搜索Key
 -----------------------------------------------------------------------
*/
BNode* Search(BNode* root,const int Key)
{
	if(root == NULL || root->data == Key)
		return root;
	
	if(Key < root->data)
		return Search(root->left, Key);
	else
		return Search(root->right, Key);
}

/* ---------------------------------------------------------------------
 函数名称： Insert
 参    数： root_p   -- 指向节点之指针的指针
         data     -- 插入数的数据
 返回参数： 0 --成功    -1 --失败
 功能描述： 向查找二叉树中插入一个新的节点
 -----------------------------------------------------------------------
*/
int Insert(BTree** root_p, int data)
{
	if((*root_p) == NULL){
		BNode* newnode = GetBNode(data, NULL, NULL);
		(*root_p) = newnode;
		return 0;
	}
	
	if((*root_p)->data == data)
		return -1;

	if(data < (*root_p)->data)
		return Insert(&((*root_p)->left), data);
	else
		return Insert(&((*root_p)->right), data);
}

/* ---------------------------------------------------------------------
 函数名称： Delete
 参    数： node_p  指向节点之指针的指针
 返回参数： 0 --成功    -1 --失败
 功能描述： 从查找二叉树中删除一个节点

 注    意： 调用的实际参数（*node_p）必须是树中的一个链
        而不是书中的链的一个副本： 例如
	BNode* del = Search(root, 34);
	Delete(&del)         （错）

	Delete(&(root->left)) (对)
 -----------------------------------------------------------------------
*/
int Delete(BNode** node_p)
{
	if((*node_p) == NULL)
		return -1;
	
	/* 需要删除的节点 */
	BNode* toDelete = (*node_p);

	if((*node_p)->right == NULL)      /* 右子树为空 */
		(*node_p) = (*node_p)->left;
	else if((*node_p)->left == NULL)  /* 左子树为空 */
		(*node_p) = (*node_p)->right;
	else{ /* 左右子树都不为空 */
		BNode* parent = (*node_p);
		toDelete = (*node_p)->left;
		while(toDelete->right != NULL){
			parent = toDelete;
			toDelete = toDelete->right;	
		}

		(*node_p)->data = toDelete->data; /* 交换数据 */
		
		if(parent == (*node_p))
			parent->left = toDelete->left;    /* 注意，这里是赋给paren的left哦。这是一个特列 */
		else
			parent->right = toDelete->left;   /* 通常情况，在（*node_p）的左子树的右倾中，应该付给parent的右子树 */
	}

	free(toDelete);
	return 0;
}

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

