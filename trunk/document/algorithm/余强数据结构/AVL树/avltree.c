/************************************************************************************

  文  件： C语言AVL高度平衡树 源码及注解
  作  者： 余   强
  日  期： 2009-11-20~2009-11-27
  协  议： Copyleft (C) GPL
  E-mail: yuembed@126.com
  博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* malloc() */
#include "avltree.h"

/********************************
  本地AVL辅助函数声明
*/ 
/* 右高----左旋转，右平衡 */
static void right_balance(ANode** sub_root_p);
static void rotate_left(ANode** sub_root_p);

/* 右高----左旋转，右平衡 */
static void left_balance(ANode** sub_root_p);
static void rotate_right(ANode** sub_root_p);

static void dright_balance(ANode** root_p, BOOL* shorter);
static void dleft_balance(ANode** root_p, BOOL* shorter);

/* 递归辅助函数 */
static int AVLInsert(ATree** sub_root_p, int data, BOOL* taller);
static int AVLDelete(ATree** root_p, ATree** sub_root_p, type_p type, BOOL* shorter);

/* 直接前趋和直接后继函数 */
static ANode*  Predecessor(ANode* node, ANode** parent_pre);
static ANode*  GetParent(ATree* root, ANode* node);
static ANode** GetQuote(ATree** root, ANode* node);


/********************************
  下面是二叉树基本操作
*/ 
/* ---------------------------------------------------------------------
 函数名称： InitTree
 参    数： root_p   -- 指向树根节点的指针的指针
 返回参数： 0 -- 初始化成功
 功能描述： 初始化一颗二叉树
 -----------------------------------------------------------------------
*/
int  InitTree(ATree** root_p)
{
	*root_p = NULL;
	return 0;
}

/* ---------------------------------------------------------------------
 函数名称： GetANode
 参    数： data  --创建节点的数据
	   left --新节点的左子树
	   right --新节点的右子树
 返回参数： nCount -- 叶子的数目
 功能描述： 创建一个新节点的函数
 -----------------------------------------------------------------------
*/
ANode*  GetANode(int data, ANode* left, ANode* right)
{
	ANode* newnode = (ANode*)malloc(sizeof(ANode));
	if(newnode == NULL){
		printf("error:no memory!\n");
		return NULL;
	}
	
	newnode->data   = data;
	newnode->left   = left;
	newnode->right  = right;
	newnode->factor = EQUAL_H;  /* 默认平衡因子值 */

	return newnode;
}

/* ---------------------------------------------------------------------
 函数名称： DeleteATree
 参    数： root_p   -- 指向树根地址的指针
 返回参数： 0        -- 成功
 功能描述：清除一颗二叉树
 -----------------------------------------------------------------------
*/
int  Remove(ANode** root_p)
{
	if(*root_p == NULL){
		return 0;
	}else{
		Remove(&((*root_p)->left));
		Remove(&((*root_p)->right));
		free(*root_p);
		*root_p = NULL;
	}

	return 0;
}

/* ---------------------------------------------------------------------
 函数名称：  Search
 参    数：  root   -- 树的跟节点     Key    -- 查找的数据
 返回参数： root   -- 指向找到的树节点
         NULL   -- 未找到数据
 功能描述： 在查找二叉树中搜索Key
 -----------------------------------------------------------------------
*/
ANode* Search(ANode* root,const int data)
{
	if(root == NULL || root->data == data){
		return root;		
	}

	if(data < root->data)
		return Search(root->left, data);
	else
		return Search(root->right, data);
}

/* ---------------------------------------------------------------------
 函数名称： LNR LRN NLR
 参    数： root   -- 树的跟节点     process  -- 函数指针
 返回参数： 无
 功能描述： 遍历
 -----------------------------------------------------------------------
*/
/* 中序遍历二叉树（左、节点、右） */
void LNR(ANode* root, void process(ANode*))
{
	if(root != NULL){
		LNR(root->left, process);
		process(root);
		LNR(root->right, process);
	}
}

/* 后序遍历二叉树（左、右、节点）*/
void LRN(ANode* root, void process(ANode*))
{
	if(root != NULL){
		LRN(root->left, process);  /* 遍历左子树 */
		LRN(root->right, process); /* 遍历右子树 */
		process(root);       /* 访问节点 */
	}
}
/* 前序遍历二叉树（节点、左、右） */
void NLR(ANode* root, void process(ANode*))
{
	if(root != NULL){
		process(root);
		NLR(root->left, process);
		NLR(root->right, process);
	}
}



/***********************************************************/
/* 其他操作 */

/* ---------------------------------------------------------------------
 函数名称： CopyATree
 参    数： root     -- 树的根节点
 返回参数： newroot  -- 新术的根节点指针
          NULL     -- 空树
 功能描述： 利用后序遍历的方法拷贝一个二叉树
 -----------------------------------------------------------------------
*/
ANode*  CopyATree(ATree* root)
{
	if(root != NULL){
		ANode* newlptr = CopyATree(root->left);
		ANode* newrptr = CopyATree(root->right);
		ANode* newroot = GetANode(root->data, newlptr, newrptr);
		
		newroot->factor = root->factor;
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
int CountLeaf(ANode* root)
{
	int nCount = 0;
	if(root != NULL){
		nCount += CountLeaf(root->left);
		nCount += CountLeaf(root->right);

		
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
int Depth(ANode* root)
{
	int depth = -1, ldepth=0, rdepth=0;
	if(root != NULL){

		ldepth = Depth(root->left);
		rdepth = Depth(root->right);
		depth  = 1 + (ldepth > rdepth ? ldepth : rdepth);	
	}

	return depth;
}


/***********************************************************/
/* AVL树操作 */

/* ---------------------------------------------------------------------
 函数名称： Insert
 参    数： root_p   -- 指向节点之指针的指针
         data     -- 插入数的数据
 返回参数： 0 --成功    -1 --失败
 功能描述： 向查找二叉树中插入一个新的节点
 -----------------------------------------------------------------------
*/
int Insert(ATree** root, int data)
{
	BOOL taller; /* 用于记录树是否增高 */
	return AVLInsert(root, data, &taller);
}

/* ---------------------------------------------------------------------
 函数名称： Delete
 参    数： node_p  指向节点之指针的指针
 返回参数： 0 --成功    -1 --失败
 功能描述： 从查找二叉树中删除一个节点

 注    意： 调用的实际参数（*node_p）必须是树中的一个链
        而不是书中的链的一个副本： 例如
 -----------------------------------------------------------------------
*/
int Delete(ATree** root_p, int data)
{
	BOOL    shorter = true; /* 是否减短 */
	type_p  type;  /* 用来标记要parent的左子树减短，还是右子树减短 */

	/* 第一步：定位到需要删除的那个节点 */
	ANode*  toDele  = Search(*root_p, data);
	ANode*  parent  = NULL;
	ANode*  pre     = toDele;
	ANode** quote   = NULL; /* 用来保存parent的父节点指针的指针 */

	if(toDele == NULL)
		return -1;


	/* 第二步：找到需要删除的那个节点的前趋节点 */
	if(pre->right == NULL){ /* 要删除的节点 只有左子树 */
		parent = GetParent(*root_p, toDele);
		if((parent != NULL) && (parent->left == pre)){ /* 判断toDele是parent的左子树还是右子树 */
			parent->left = pre->left;  /* 包含了toDele->left == NULL的情况 */
			type = left;
		}else if((parent != NULL) && (parent->right == pre)){
			parent->right = pre->left;
			type = right;
		}else{/* parent == NULL */
			*root_p = NULL;
		}
		/* 通过上面的条件，pre被独立出来了 */
	}
	else if(pre->left == NULL){/* 要删除的几点 只有右子树 */
		parent = GetParent(*root_p, toDele);
		if((parent != NULL) && (parent->right == pre)){
			parent->right = pre->right;
			type = right;
		}else if((parent != NULL) && (parent->left == pre)){
			parent->left = pre->right;
			type = left;
		}else{/* parent == NULL */
			*root_p = NULL;
		}
		/* 通过上面的条件，pre被独立出来了 */
	}
	else{/* 要删除的节点有 左右子树 */
		parent = pre;
		pre = pre->left;

		while(pre->right != NULL){
			parent = pre;
			pre = pre->right;
		}

		/* 只改变了数据，没有改变连接 */
		toDele->data = pre->data;  /* 用前趋节点值覆盖需要删除的节点的值 */
		if(parent == toDele){/* 通过这个可以判断pre是parent的左子树，还是parent的右子树 */
			parent->left  = pre->left; /* 将前趋节点的左树付给 它的父节点的右树 */
			type = left;
		}else{
			parent->right = pre->left;
			type = right;
		}

		/* 通过上面的条件，pre被独立出来了 */
	}

	/* 第三步：释放节点所占的空间 */
	free(pre);
	if(*root_p == NULL) /* 删除最后一个节点 */
		return 0;	

	/* 第四步：递归调用更新平衡树 */
	quote = GetQuote(root_p, parent);

	if(type == left)
		return AVLDelete(root_p, quote, left, &shorter);
	else
		return AVLDelete(root_p, quote, right, &shorter); /* 不能使用&parent代替quote */
}

/* 其实AVLDelete更像一个检测更新函数，而非删除函数 */
int AVLDelete(ATree** root_p, ATree** sub_root_p, type_p type, BOOL* shorter)
{
	ANode*  parent = NULL;
	ANode** quote  = NULL; /* 用来保存parent的父节点指针 */

	if(type == right){ /* sub_root_p的右子树减短 */
		dleft_balance(sub_root_p, shorter);   /* 平衡因子和shorter均在这个函数里面已经设置 */
	}
	else if(type == left){
		dright_balance(sub_root_p, shorter);
	}
	else{
		printf("WARNING:error detected in AVLDelete()! \n");
		return -1;
	}

	/* 如果上面的调整标记 调整后的树高度减少。那么对其双亲仅仅检测 */
	if(*shorter == true){
		parent = GetParent(*root_p, *sub_root_p);  /* 查找**sub_root_p的后继节点 */
		if(parent != NULL)
			quote = GetQuote(root_p, parent);    /* 现在quote为parent的父节点引用信息 */

		if(parent == NULL)
			return 0;
		else if(parent->right == *sub_root_p)
			return AVLDelete(root_p, quote, right, shorter);
		else
			return AVLDelete(root_p, quote, left, shorter);
	}
	return 0;
}

/* 右树节点减少，需要左平衡  shorter 为输出参数*/
void dleft_balance(ANode** root_p, BOOL* shorter)
{
	ANode* p = *root_p; /* 请参考《AVL图解》删除部分图来阅读源代码，从而知道局部变量的意义和值 */
	ANode* q = NULL;
	ANode* r = NULL;

	switch(p->factor){   /* 图X情况1：无旋转、高度未变 */
		case EQUAL_H:
			p->factor = LEFT_H;
			*shorter = false;
			break;
		case RIGHT_H:   /* 图X情况2：无旋转、高度减短 */
			p->factor = EQUAL_H;
			*shorter = true;
			break;
		case LEFT_H:
			q = p->left;
			switch(q->factor){
				case EQUAL_H: /* 图X情况3a:单个左旋转、高度未变 */
					q->factor = RIGHT_H;
					p->factor = LEFT_H;
					rotate_right(root_p); /* 这里不能用&p代替root_p */
					*shorter = false;
					break;
				case LEFT_H: /* 情况3b:单个左旋转、高度减短 */
					q->factor = EQUAL_H;
					p->factor = EQUAL_H;
					rotate_right(root_p);
					*shorter = true;
					break;
				case RIGHT_H: /* 情况3c:双旋转、高度减短 */
					r = q->right;
					switch(r->factor){
						case EQUAL_H:
							p->factor = EQUAL_H; /* 请参考《AVL图解》表三 */
							q->factor = EQUAL_H;
							break;
						case RIGHT_H:
							p->factor = EQUAL_H; /* 请参考《AVL图解》表三 */
							q->factor = LEFT_H;
							break;
						case LEFT_H:
							p->factor = RIGHT_H;  /* 请参考《AVL图解》表三 */
							q->factor = EQUAL_H;
							break;
					}
					r->factor = EQUAL_H;
					rotate_left(&(p->right)); /* 这里不能用&q来代替&(p->right) */
					rotate_right(root_p);       /* 这里不能用&p代替root_p */
					*shorter = true;
					break;
			}
			break;
	}	
}

/* 左树节点减少、导致右高，需要右平衡AVLDelete专用函数 */
void dright_balance(ANode** root_p, BOOL* shorter)
{
	ANode* p = *root_p; /* 请参考《AVL图解》删除部分图来阅读源代码，从而知道局部变量的意义和值 */
	ANode* q = NULL;
	ANode* r = NULL;

	switch(p->factor){   /* 情况1：无旋转、高度未变 */
		case EQUAL_H:
			p->factor = RIGHT_H;
			*shorter = false;
			break;
		case LEFT_H:   /* 情况2：无旋转、高度减短 */
			p->factor = EQUAL_H;
			*shorter = true;
			break;
		case RIGHT_H:
			q = p->right;
			switch(q->factor){
				case EQUAL_H: /* 情况3a:单个左旋转、高度未变 */
					q->factor = LEFT_H;
					p->factor = RIGHT_H;
					rotate_left(root_p); /* 这里不能用&p代替root_p */
					*shorter = false;
					break;
				case RIGHT_H: /* 情况3b:单个左旋转、高度减短 */
					q->factor = EQUAL_H;
					p->factor = EQUAL_H;
					rotate_left(root_p);
					*shorter = true;
					break;
				case LEFT_H: /* 情况3c:双旋转、高度减短 */
					r = q->left;
					switch(r->factor){
						case EQUAL_H:
							p->factor = EQUAL_H; /* 请参考《AVL图解》表三 */
							q->factor = EQUAL_H;
							break;
						case LEFT_H:
							p->factor = EQUAL_H; /* 请参考《AVL图解》表三 */
							q->factor = RIGHT_H;
							break;
						case RIGHT_H:
							p->factor = LEFT_H;  /* 请参考《AVL图解》表三 */
							q->factor = EQUAL_H;
							break;
					}
					r->factor = EQUAL_H;
					rotate_right(&(p->right)); /* 这里不能用&q来代替&(p->right) */
					rotate_left(root_p);       /* 这里不能用&p代替root_p */
					*shorter = true;
					break;
			}
			break;
	}
}
/* ---------------------------------------------------------------------
 函数名称： AVLInsert
 参    数： sub_root_p   -- 指向树根地址的指针
         data         -- 插入的数据
	   taller       -- 输入参数：判断插入后子树是否增高
 返回参数： 0            -- 插入成功
              -1           -- 插入失败
 功能描述： 插入递归辅助函数
 -----------------------------------------------------------------------
*/
int AVLInsert(ATree** sub_root_p, int data, BOOL* taller)
{

	int error = 0;
	if((*sub_root_p) == NULL){
		*sub_root_p = GetANode(data, NULL, NULL);
		*taller = true; /* 树的高度增加 */
	}
	else if(data == (*sub_root_p)->data){   /* 插入数据已存在 */
		error = -1;
		*taller = false;
	}
	else if(data < (*sub_root_p)->data){     /* 插入左树 */
		error = AVLInsert(&((*sub_root_p)->left), data, taller);
		if(*taller == true)
			switch((*sub_root_p)->factor){  /* 修改平衡因子 */
				case LEFT_H:/* 插入前:左高 */
					left_balance(sub_root_p);
					*taller = false; /* 这种情况平衡后，相对于此节点的双亲高度不增加 */
					break;
				case EQUAL_H:/* 插入前:等高 */
					(*sub_root_p)->factor = LEFT_H;
					break;
				case RIGHT_H:/* 插入前:右高 */
					(*sub_root_p)->factor = EQUAL_H;
					*taller = false;
					break;
			}
	}
	else{/* 插入右子树 */
		error = AVLInsert(&((*sub_root_p)->right), data, taller);

		if(*taller == true) /* 如果高度增加 */
			switch((*sub_root_p)->factor){ /* 插入后修改平衡因子 */
				case LEFT_H:
					(*sub_root_p)->factor = EQUAL_H;
					*taller = false;
					break;
				case EQUAL_H:
					(*sub_root_p)->factor = RIGHT_H;
					break;
				case RIGHT_H:
					right_balance(sub_root_p);
					*taller = false;
					break;
			}
	}
	
	return error;
}


/* ---------------------------------------------------------------------
 函数名称： right_balance
 参    数： sub_root_p   -- 指向树根地址的指针
 返回参数： 无
 功能描述： 右平衡 请参考《AVL图解》图二
 -----------------------------------------------------------------------
*/
void right_balance(ANode** sub_root_p)
{
	ANode* right_tree = (*sub_root_p)->right;
	ANode* sub_tree  = NULL;
	switch(right_tree->factor){
		case RIGHT_H:/* 简单：仅仅左旋转就可以了 */
			(*sub_root_p)->factor = EQUAL_H;
			right_tree->factor = EQUAL_H;
			rotate_left(sub_root_p);
			break;
		case EQUAL_H:/* 不可能的情况 */
			printf("WARNING:program error detected in right_balance!\n");
			break;
		case LEFT_H:/* 复杂：双旋转 */
			sub_tree = right_tree->left;
			switch(sub_tree->factor){  /* 这些值的设置，请参考《AVL图解》 中"表-1"分析所得 */
				case EQUAL_H:
					(*sub_root_p)->factor = EQUAL_H; /*请参考《AVL图解》表一*/
					right_tree->factor = EQUAL_H;
					break;
				case LEFT_H:
					(*sub_root_p)->factor = EQUAL_H; /*请参考《AVL图解》表一*/
					right_tree->factor = RIGHT_H;
					break;
				case RIGHT_H:
					(*sub_root_p)->factor = LEFT_H; /*请参考《AVL图解》表一*/
					right_tree->factor = EQUAL_H;
					break;
			}
			sub_tree->factor=EQUAL_H;
			rotate_right(&((*sub_root_p)->right)); /* 不能用right_tree代替 (*sub_roo_p)->right */
			rotate_left(sub_root_p);
			break;
	}
}

/* ---------------------------------------------------------------------
 函数名称：left_balance
 参    数： sub_root_p   -- 指向节点地址的指针
 返回参数： 无
 功能描述： 左高、左平衡 请参考《AVL图解》图四
 -----------------------------------------------------------------------
*/
void left_balance(ANode** sub_root_p)
{
	ANode* left_tree = (*sub_root_p)->left;
	ANode* sub_tree  = NULL;

	switch(left_tree->factor){
		case LEFT_H:/* 简单：仅仅右旋转就可以了 */
			(*sub_root_p)->factor = EQUAL_H;
			left_tree->factor = EQUAL_H;
			rotate_right(sub_root_p); /* 只负责旋转而不更改平衡因子，所以上面需要设置平衡因子 */
			break;
		case EQUAL_H:
			printf("WARNING:program error detected in left_balance!\n");
			break;
		case RIGHT_H:/* 复杂：双旋转 */
			sub_tree = left_tree->right;
			switch(sub_tree->factor){/* 更新平衡因子 */
				case EQUAL_H:
					left_tree->factor = EQUAL_H;
					(*sub_root_p)->factor = EQUAL_H; /*请参考《AVL图解》表二*/
					break;
				case LEFT_H:
					left_tree->factor = EQUAL_H;
					(*sub_root_p)->factor = RIGHT_H; /*请参考《AVL图解》表二*/
					break;
				case RIGHT_H:
					left_tree->factor = LEFT_H;
					(*sub_root_p)->factor = EQUAL_H; /*请参考《AVL图解》表二*/
					break;
			}
			sub_tree->factor = EQUAL_H;
			rotate_left(&((*sub_root_p)->left)); /* 注意:rotate_left(&left_tree)（错） */
			rotate_right(sub_root_p);
			break;
	}
}

/* ---------------------------------------------------------------------
 函数名称： rotate_left
 参    数： sub_root_p   -- 指向节点地址的指针
 返回参数： 无
 功能描述： 左旋转 请参考《AVL图解》图一
 -----------------------------------------------------------------------
*/
void rotate_left(ANode** sub_root_p)
{
	if(*sub_root_p == NULL || (*sub_root_p)->right == NULL)/* 不可能的情况 */
		printf("WARNING: program error detected in retate_left!\n");
	else{/* 参考图1 */
		ANode* right_tree    =  (*sub_root_p)->right;
		(*sub_root_p)->right =  right_tree->left;
		right_tree->left     =  *sub_root_p;
		*sub_root_p          =  right_tree;
	}
}

/* ---------------------------------------------------------------------
 函数名称： rotate_right
 参    数： sub_root_p   -- 指向树根地址的指针
 返回参数： 无
 功能描述： 右旋转 请参考《AVL图解》图三
 -----------------------------------------------------------------------
*/
void rotate_right(ANode** sub_root_p)
{
	if(*sub_root_p == NULL || (*sub_root_p)->left == NULL)/* 不可能的情况 */
		printf("WARNING: program error detected in retate_right!\n");
	else{
		ANode* left_tree    =  (*sub_root_p)->left;
		(*sub_root_p)->left =  left_tree->right;
		left_tree->right    =  *sub_root_p;
		*sub_root_p         =  left_tree;
	}
}

/* ---------------------------------------------------------------------
 函数名称： GetParent
 参    数： root   -- 树根地址
         node   -- 需要查找父节点的节点指针
 返回参数： NULL   -- 参数错误
         parent  -- 后继节点信息
 功能描述： 额外辅助函数
 -----------------------------------------------------------------------
*/
ANode* GetParent(ATree* root, ANode* node)
{
	if(root == node)
		return NULL;
	
	ANode* parent = root;

	while((parent->right != node && parent->right != NULL) && \
            (parent->left != node && parent->left != NULL)){
		if(node->data < parent->data)
			parent = parent->left;
		else
			parent = parent->right;
	};

	return parent;
}

/* ---------------------------------------------------------------------
 函数名称： GetQuote
 参    数： root   -- 树根地址
          node   -- 需要获得引用的节点指针
 返回参数： NULL   -- 无此节点
         &parent->child  -- node节点的引用
 功能描述： 由于C语言里面没有引用，所以这里提供这样一个函数来模拟
            引用，这样可以减小程序设计的复杂度。额外辅助函数
 -----------------------------------------------------------------------
*/
ANode** GetQuote(ATree** root, ANode* node)
{
	if(*root == node)
		return root;  /* 如果node已经是跟节点，则返回指向根节点的指针的指针。那么我们就可以修改指向根节点的指针了 */
	
	ANode* parent = *root;
	
	while((parent->right != node && parent->right != NULL) && \
            (parent->left != node && parent->left != NULL)){
		if(node->data < parent->data)
			parent = parent->left;
		else
			parent = parent->right;
	};

	if(parent->right == node)
		return &(parent->right);
	else if(parent->left == node)
		return &(parent->left);
	else/* 无此节点 */
		return NULL;
}

/* ---------------------------------------------------------------------
 函数名称： Predecessor
 参    数： node   -- 需要查找后继的节点指针
         parent_pre  -- 指向前趋节点的父节点指针
 返回参数： NULL   -- 参数错误
         pre -- 前趋
 功能描述：直接前趋
 -----------------------------------------------------------------------
*/
ANode* Predecessor(ANode* node, ANode** parent_pre)
{
	if(node == NULL)
		return NULL;

	*parent_pre = node;
	ANode* pre  = NULL;
	
	/* 判断node节点是否具有左子树 */
	if((*parent_pre)->left != NULL)
		pre = (*parent_pre)->left;
	else
		return NULL;

	/* 遍历左子树 */
	while(pre->right != NULL){
		*parent_pre = pre;
		pre = pre->right;
	};

	return pre;
}


/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

