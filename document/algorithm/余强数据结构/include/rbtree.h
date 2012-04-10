/************************************************************************************

  文件： C语言红黑树 源码及注解
  作者： 余   强
  日期： 2009-12-04~2009-12-08
  协议： Copyleft (C) GPL
  E-mail: yuembed@126.com
  空间博客 『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/
#ifndef RB_TREE_H
#define RB_TREE_H

/***********************************************************/
/* 相关结构体定义 */

/* 平衡因子类型 */
typedef enum{BLACK, RED}COLOR;

/* 左右类型 */
typedef enum{left, right}type_t;/* 删除节点的时候会用到 */

/* 红黑树节点 */
typedef struct node{
	int           data;    /* 数据     */
	COLOR         color;   /* 颜色     */
	struct node*  parent;  /* 父节点   */
	struct node*  left;    /* 左孩子   */
	struct node*  right;   /* 右孩子   */

}RNode, RTree;

/* BOOL类型 */
typedef enum{true, false}BOOL;


/***********************************************************/
/* 二叉查找树基本操作函数 */


int     InitTree(RTree** root_p);              /* 二叉树初始化函数 */

RNode*  GetRNode(int data, RNode* parent, RNode* left, RNode* right);  /* 创建二叉树节点 */

RNode*  Search(RNode* root,const int Key);     /* 在查找二叉树中搜索Key */

//覆盖int   Insert(RTree** root_p, int data);   /* 向查找二叉树中插入一个新的节点 */
//覆盖int   Delete(RNode** node_p);             /* 从查找二叉树中删除一个节点 */
int     Remove(RTree** root);                  /* 删除二叉树 */

void LNR(RNode* root, void process(RNode*));   /* 中序遍历二叉树（左、节点、右） */
void LRN(RNode* root, void process(RNode*));   /* 后序遍历二叉树（左、右、节点） */
void NLR(RNode* root, void process(RNode*));   /* 前序遍历二叉树（节点、左、右） */

/***********************************************************/
/* 其他操作 */
RNode*  CopyRTree(RNode* root);        /* 拷贝二叉树 */
int     CountLeaf(RNode* root);        /* 计算二叉树子叶节点个数 */
int     Depth(RNode* root);            /* 计算二叉树深度 */

/***********************************************************/
/* 红黑树操作 */

/* 红黑树的插入、删除 */
int Insert(RTree** root_p, int data);
int Delete(RTree** root_p, int data);

#endif

/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

