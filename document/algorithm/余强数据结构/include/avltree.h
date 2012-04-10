/************************************************************************************

  文件： C语言AVL高度平衡树 源码及注解
  作者： 余   强
  日期： 2009-11-20~2009-11-26
  协议： Copyleft (C) GPL
 E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#ifndef AVL_TREE_H
#define AVL_TREE_H

/***********************************************************/
/* 相关结构体定义 */

/* 平衡因子类型 */
enum balance_factor{LEFT_H=-1, EQUAL_H=0, RIGHT_H=1};

/* 左右类型 */
typedef enum{left, right}type_p;/* 删除节点的时候会用到 */

typedef struct node{
	int                  data;    /* 数据     */
	struct node*         left;    /* 左孩子   */
	struct node*         right;   /* 右孩子   */
	enum balance_factor  factor;  /* 平衡因子 */
}ANode, ATree;

/* BOOL类型 */
typedef enum{true, false}BOOL;

/* 额外结构体，用于Succssor */
typedef struct{
	ANode*  parent;     /* 父节点指针         */
	type_p  type;       /* 父节点的左或右孩子 */	
}INFOR;
/***********************************************************/
/* 二叉查找树基本操作函数 */


int     InitTree(ATree** root_p);              /* 二叉树初始化函数 */

ANode*  GetANode(int data, ANode* left, ANode* right);  /* 创建二叉树节点 */

ANode*  Search(ANode* root,const int Key);     /* 在查找二叉树中搜索Key */

//覆盖int   Insert(ATree** root_p, int data);   /* 向查找二叉树中插入一个新的节点 */
//覆盖int   Delete(ANode** node_p);             /* 从查找二叉树中删除一个节点 */
int     Remove(ATree** root);                  /* 删除二叉树 */

void LNR(ANode* root, void process(ANode*));   /* 中序遍历二叉树（左、节点、右） */
void LRN(ANode* root, void process(ANode*));   /* 后序遍历二叉树（左、右、节点） */
void NLR(ANode* root, void process(ANode*));   /* 前序遍历二叉树（节点、左、右） */

/***********************************************************/
/* 其他操作 */
ANode*  CopyATree(ANode* root);        /* 拷贝二叉树 */
int     CountLeaf(ANode* root);        /* 计算二叉树子叶节点个数 */
int     Depth(ANode* root);            /* 计算二叉树深度 */

/***********************************************************/
/* AVL树操作 */

/* AVL树的插入、删除 */
int Insert(ATree** root_p, int data);
int Delete(ATree** root_p, int data);

#endif

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

