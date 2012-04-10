/************************************************************************************

  文  件： C语言红黑树 源码及注解
  作  者： 余   强
  日  期： 2009-12-04~2009-12-08
  协  议： Copyleft (C) GPL
  E-mail: yuembed@126.com
  博  客：『20065562's Blog』 有水的地方就有余 http://hi.baidu.com/20065562

************************************************************************************/

#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* malloc() */
#include "rbtree.h"

/********************************
  本地红黑树辅助函数声明
*/ 
/* 右高----左旋转，右平衡 */
static void right_balance(RNode** sub_root_p);
static void rotate_left(RNode** sub_root_p);

/* 左高----右旋转，左平衡 */
static void left_balance(RNode** sub_root_p);
static void rotate_right(RNode** sub_root_p);

static void dright_balance(RTree** root_p, RNode* node, type_t type);

/* 递归辅助函数 */
static int RBInsert(RTree** root_p, RNode* parent, RTree** sub_root_p, int data);
static int RBDelete(RTree** root_p, RNode* parent, RNode* child);

/* 直接后继函数 */
static RNode*  Successor(RNode* node);


/* 定义一个全局的黑空节点 */
RNode BEN;  /* black empty null */

/*
  我们定义一个黑空节点指针BNULL,相当于NULL的作用，
  但它却能标记空节点，且节点具有黑色的属性,所以请
  在下面的程序中始终将BNULL想象成NULL
*/
#define  BNULL  &BEN

/********************************
  下面是二叉树基本操作
*/ 
/* ---------------------------------------------------------------------
 函数名称： InitRTree
 参    数： root_p   -- 指向树根节点的指针的指针
 返回参数： 0 -- 初始化成功
 功能描述： 初始化一颗二叉树
 -----------------------------------------------------------------------
*/
int  InitTree(RTree** root_p)
{
	*root_p = BNULL;

	/* 初始化一个黑空节点 */
	BEN.data   = -100;
	BEN.color  = BLACK;
	BEN.parent = NULL;
	BEN.left   = NULL;
	BEN.right  = NULL;
	return 0;
}

/* ---------------------------------------------------------------------
 函数名称： GetRNode
 参    数： data  --创建节点的数据
	   parent --新节点的父亲
	   left --新节点的左子树
	   right --新节点的右子树
 返回参数： nCount -- 叶子的数目
 功能描述： 创建一个新节点的函数
 -----------------------------------------------------------------------
*/
RNode*  GetRNode(int data, RNode* parent, RNode* left, RNode* right)
{
	RNode* newnode = (RNode*)malloc(sizeof(RNode));
	if(newnode == NULL){
		printf("error:no memory!\n");
		return NULL;
	}
	
	newnode->data   = data;
	newnode->color  = RED;   /* 初始化为红色 */
	newnode->parent = parent;
	newnode->left   = left;
	newnode->right  = right;

	return newnode;
}

/* ---------------------------------------------------------------------
 函数名称： DeleteRTree
 参    数： root_p   -- 指向树根地址的指针
 返回参数： 0        -- 成功
 功能描述：清除一颗二叉树
 -----------------------------------------------------------------------
*/
int  Remove(RNode** root_p)
{
	if(*root_p == BNULL){
		return 0;
	}else{
		Remove(&((*root_p)->left));
		Remove(&((*root_p)->right));
		free(*root_p);
		*root_p = BNULL;
	}

	return 0;
}

/* ---------------------------------------------------------------------
 函数名称：  Search
 参    数：  root   -- 树的跟节点     Key    -- 查找的数据
 返回参数：  root   -- 指向找到的树节点
         BNULL   -- 未找到数据
 功能描述： 在查找二叉树中搜索Key
 -----------------------------------------------------------------------
*/
RNode* Search(RNode* root, const int data)
{
	if(root == BNULL || root->data == data){
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
void LNR(RNode* root, void process(RNode*))
{
	if(root != BNULL){
		LNR(root->left, process);
		process(root);
		LNR(root->right, process);
	}
}

/* 后序遍历二叉树（左、右、节点）*/
void LRN(RNode* root, void process(RNode*))
{
	if(root != BNULL){
		LRN(root->left, process);  /* 遍历左子树 */
		LRN(root->right, process); /* 遍历右子树 */
		process(root);       /* 访问节点 */
	}
}
/* 前序遍历二叉树（节点、左、右） */
void NLR(RNode* root, void process(RNode*))
{
	if(root != BNULL){
		process(root);
		NLR(root->left, process);
		NLR(root->right, process);
	}
}

/***********************************************************/
/* 其他操作 */

/* ---------------------------------------------------------------------
 函数名称： CopyRTree
 参    数： root     -- 树的根节点
 返回参数： newroot  -- 新术的根节点指针
          BNULL     -- 空树
 功能描述： 利用后序遍历的方法拷贝一个二叉树
 -----------------------------------------------------------------------
*/
RNode*  CopyRTree(RTree* root)
{
	if(root != BNULL){
		RNode* newlptr = CopyRTree(root->left);
		RNode* newrptr = CopyRTree(root->right);
		RNode* newroot = GetRNode(root->data, root->parent, newlptr, newrptr);
		newroot->color = root->color;

		return newroot;
	}

	return BNULL;
}

/* ---------------------------------------------------------------------
 函数名称： CountLeaf
 参    数： root   -- 树的跟节点
 返回参数： nCount -- 叶子的数目
 功能描述： 计算二叉树的叶子
 -----------------------------------------------------------------------
*/
int CountLeaf(RNode* root)
{
	int nCount = 0;
	if(root != BNULL){
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
int Depth(RNode* root)
{
	int depth = -1, ldepth=0, rdepth=0;
	if(root != BNULL){

		ldepth = Depth(root->left);
		rdepth = Depth(root->right);
		depth  = 1 + (ldepth > rdepth ? ldepth : rdepth);	
	}

	return depth;
}

/***********************************************************/
/* RB树操作 */


/* ---------------------------------------------------------------------
 函数名称： Insert
 参    数： root_p   -- 指向节点之指针的指针
         data     -- 插入数的数据
 返回参数： 0 --成功    -1 --失败
 功能描述： 向红黑树中插入一个新的节点
 -----------------------------------------------------------------------
*/
int Insert(RTree** root_p, int data)
{
	int error = 0;

	if(*root_p == BNULL){ /* 红黑树中没有节点 */
		RNode* new = GetRNode(data, BNULL, BNULL, BNULL);
		new->color = BLACK;
		*root_p    = new;
		return 0;
	}else if(data < (*root_p)->data)
		error = RBInsert(root_p, *root_p, &((*root_p)->left), data); /* 此时*root_p不为空 */
	else
		error = RBInsert(root_p, *root_p, &((*root_p)->right), data); /* 此时*root_p不为空 */


	/* 当插入之后当前节点变成了红色，且当前节点为根节点，那么将它变成黑色 */
	if((*root_p)->color == RED)
		(*root_p)->color = BLACK;
	
	return error;
}


/* ---------------------------------------------------------------------
 函数名称： RBInsert
 参    数： parent      --*sub_root_p的父节点指针
	   sub_root_p   -- 指向树根地址的指针
         data         -- 插入的数据
 返回参数： 0            -- 插入成功
              -1           -- 插入失败
 功能描述： 插入递归辅助函数
 -----------------------------------------------------------------------
*/
int RBInsert(RTree** root_p, RNode* parent, RTree** sub_root_p, int data)
{
	int error = 0;
	RNode* grand = BNULL;
	RNode* uncle = BNULL;

	if(*sub_root_p == BNULL){
		*sub_root_p = GetRNode(data, parent, BNULL, BNULL);
		return error;
	}else if(data < (*sub_root_p)->data){
		RBInsert(root_p, *sub_root_p, &((*sub_root_p)->left), data); /* 将新节点插入到当前节点的 左子树 */
		
		if(!((*sub_root_p)->color == RED && (*sub_root_p)->left->color == RED))/* 判断当前节点和父亲是否遇到红色警戒 */
			return error;

		switch((*sub_root_p)->color){     /* 当前节点为父亲 */
			case BLACK: /* 在黑色节点处插入 */
				break;
			case RED:   /* 在红色节点处插入，需考虑叔叔 */
				grand = (*sub_root_p)->parent;                /* 获得新节点的祖父 */
				if(grand->left == *sub_root_p)                /* 获得新节点的叔叔；参考1.2.1的情况 */
					uncle = grand->right;
				else
					uncle = grand->left;

				switch(uncle->color){
				case RED:  /* 参考图1.2.1-1 还有一种未画出的情况 这种情况部分叔与父的位置关系 */
					(*sub_root_p)->color = BLACK;      /* 父=>黑 */
					uncle->color = BLACK;              /* 叔=>黑 */
					grand->color = RED;                /* 祖=>红 这种情况会往上遍历 */
					break;
				case BLACK: /* 参考图1.2.2-1 */
					if(grand->right == uncle){/* 叔在右边 */
						(*sub_root_p)->color = BLACK;      /* 父=>黑 */
						grand->color = RED;                /* 祖=>红 */

						if(grand->parent == BNULL)         /* 祖父右旋转 */
							rotate_right(root_p);                
						else if(grand->parent->left == grand)
							rotate_right(&(grand->parent->left));
						else
							rotate_right(&(grand->parent->right));
					}else{/* ｜叔在左边，未画出 */
						(*sub_root_p)->left->color = BLACK;  /* |新=>黑 */
						grand->color = RED;                  /* |祖=>红 */
						
						rotate_right(sub_root_p);	         /* |父亲右旋转 */

						if(grand->parent == BNULL)           /* ｜祖父左旋转 */
							rotate_left(root_p);                
						else if(grand->parent->left == grand)
							rotate_left(&(grand->parent->left));
						else
							rotate_left(&(grand->parent->right));
					}
					break;
				}
			break;
		}
	}else{
		RBInsert(root_p, *sub_root_p, &((*sub_root_p)->right), data); /* 将新节点插入到当前节点的 右子树 */

		if(!((*sub_root_p)->color == RED && (*sub_root_p)->right->color == RED))/* 判断当前节点和父亲是否遇到红色警戒 */
			return error;
		
		switch((*sub_root_p)->color){
			case BLACK: /* 在黑色节点处插入 */
				break;
			case RED:   /* 在红色节点处插入，需考虑叔叔 */
				grand = (*sub_root_p)->parent;         /* 获得新节点的祖父，一定存在 */
	
				if(grand->left == *sub_root_p)         /* 获得新节点的叔叔；参考1.2.1的情况 */
					uncle = grand->right;
				else
					uncle = grand->left;
		
				switch(uncle->color){
				case RED:   /* 参考图1.2.1-2 还有一种未画出的情况 这种情况部分叔与父的位置关系 */
					(*sub_root_p)->color = BLACK;        /* 父=>黑 */
					uncle->color = BLACK;                /* 叔=>黑 */
					grand->color = RED;                  /* 祖=>红 这种情况会往上遍历 */
					break;
				case BLACK: /* 参考图1.2.2-2 */
					if(grand->right == uncle){/* 叔在右边 */
						(*sub_root_p)->right->color = BLACK;/* 新=>黑 */
						grand->color = RED;                 /* 祖=>红 */

						rotate_left(sub_root_p);		/* 父左旋转 */
		 							                        
						if(grand->parent == BNULL)          /* 祖父右旋转 */
							rotate_right(root_p);                
						else if(grand->parent->left == grand)
							rotate_right(&(grand->parent->left));
						else
							rotate_right(&(grand->parent->right));
					}else{/* ｜叔在左边，未画出 */
						(*sub_root_p)->color = BLACK;        /* |父=>黑 */
						grand->color = RED;                  /* |祖=>红 */
						                                                
						if(grand->parent == BNULL)           /* ｜祖父左旋转 */
							rotate_left(root_p);                
						else if(grand->parent->left == grand)
							rotate_left(&(grand->parent->left));
						else
							rotate_left(&(grand->parent->right));
					}
					break;
				}
				break;
		}/* end switch */
	}/* end if */

	return error;
}

/* ---------------------------------------------------------------------
 函数名称： Delete
 参    数： node_p  指向节点之指针的指针
 返回参数： 0 --成功    -1 --失败
 功能描述： 从红黑树中删除一个节点
 -----------------------------------------------------------------------
*/
int Delete(RTree** root_p, int data)
{
	int error = 0;
	/* 第一步：定位到需要删除的那个节点 */
	RNode* del = Search(*root_p, data);
	RNode* suc = BNULL;

	if(del == BNULL){
		return -1;
	}
	
	/* 第二步：找到需要删除的那个节点的后继节点 */
	if(del->right == BNULL){/* 要删除的节点del只有左孩子或无孩子,无需找后继节点 */
		if(del->parent == BNULL){/* 删除根节点 */
			*root_p = del->left;
			(*root_p)->color = BLACK;
		}else if(del->parent->left == del)
			del->parent->left = del->left;
		else
			del->parent->right = del->left;

		/* 更新孩子的父亲 */
		if(del->left != BNULL)
			del->left->parent = del->parent;

		/* suc用于清除内存空间 */
		suc = del;
	}else{/* del的右孩子不为空，有后继节点 */
		suc = Successor(del);
		del->data = suc->data;
		/* 第三步：递归调用更新红黑树 */
		if(suc->color == RED){/* 说明要删除的节点是红色叶节点，这种情况最简单 */
			if(suc->parent->right == suc)
				suc->parent->right = suc->right; /* suc->right = BNULL */
			else
				suc->parent->left = suc->right;
			
			/* 不需要往上更新，结束 */
		}else if(suc->right == BNULL){/* 删除黑色叶节点 参考图"删黑a"*/
			if(suc->parent->left == suc){
				suc->parent->left = BNULL;
				RBDelete(root_p, suc->parent, suc->parent->left);/* 往上更新 */
			}else{
				suc->parent->right = BNULL;
				RBDelete(root_p, suc->parent, suc->parent->right);
			}
		}else{/* 右子树不为空，那么右子树一定为一红节点 参考图"删黑b"*/
			suc->right->parent = suc->parent;  /* 更新suc右下节点的父亲属性 */
			if(suc->parent->left == suc){
				suc->parent->left = suc->right; /* suc->right == BNULL */
				RBDelete(root_p, suc->parent, suc->parent->left);/* 往上更新 */
			}else{
				suc->parent->right = suc->right;
				RBDelete(root_p, suc->parent, suc->parent->right);
			}
		}
	}

	/* 第四步：释放节点所占的空间 */
	free(suc);

	return error;
}

/* ---------------------------------------------------------------------
 函数名称： RBDelete
 参    数： root_p  -- 指向树根节点地址的指针
	    parent -- child的父节点指针
	    child  -- 需要判定更新的节点指针
 返回参数： 无
 功能描述： 其实RBDelete更像一个检测更新函数，而非删除函数，因为
	    child有可能为BNULL节点，所以它的parent＝NULL,所以
	    需要传递一个parent参数。
 -----------------------------------------------------------------------
*/
int RBDelete(RTree** root_p, RNode* parent, RNode* child)
{
	int    error    = 0;
	type_t type     = left;      /* 用来标记child为parent的左孩子还是右孩子 */
	RNode* brother  = BNULL;         /* 子的兄弟 */	
	RNode* grand    = BNULL;         /* 子的祖父 */
	RNode* nephew1  = BNULL;         /* 子的左侄子 */
	RNode* nephew2  = BNULL;         /* 子的右侄子 */
	
	/* 处理parent为BNULL的情况 */
	if(parent == BNULL){
		return error;
	}

	if(parent->left == child){
		brother = parent->right;
		type = left;
	}else{
		brother = parent->left;
		type = right;
	}
		
	switch(brother->color){
	case RED:
		parent->color  = RED;        /* 父=>红 */
		brother->color = BLACK;      /* 兄=>黑 */
		dright_balance(root_p, parent, type);  /* parent进行平衡 */
		break;
	case BLACK:
		nephew1 = brother->left;      /* 当child的兄弟节点为黑色的时候，我们引入侄子 */
		nephew2 = brother->right;
		if(nephew1->color == BLACK && nephew2->color == BLACK){ /* 双黑侄 */
			brother->color = RED;        /* 兄=>红 */
			if(parent->color == RED)
				parent->color = BLACK; /* 父=>黑 */
			else{
				
				RBDelete(root_p, parent->parent, parent); /* 或往上遍历 */
			}
		}else if(nephew1->color == RED){/* 左红侄 */
			if(type == left){/* child是parent的左孩子 */
				nephew1->color = parent->color;       /* 侄=>父色 */
				parent->color  = BLACK;               /* 父=>黑 */

				rotate_right(&(parent->right));       /* 兄右旋转 */
				if(parent->parent == BNULL)
					rotate_left(root_p);            /* 父左旋转 */
				else if(parent->parent->right == parent)
					rotate_left(&(parent->parent->right));
				else
					rotate_left(&(parent->parent->left));

				if(nephew1->color == BLACK)/* 通过旋转brother已经取代了parent的位置，所以判断brother的颜色 */
					RBDelete(root_p, nephew1->parent, nephew1);/* 往上遍历 */ /* nephew1一定不为空 */
			}else{/* child是parent的右孩子 */
				nephew1->color = BLACK;         /* 侄=>黑 */
				brother->color = parent->color; /* 兄=>父色 */

				if(parent->parent == BNULL)     /* 父右旋转 */
					rotate_right(root_p);
				else if(parent->parent->right == parent)
					rotate_right(&(parent->parent->right));
				else
					rotate_right(&(parent->parent->left));

				if(brother->color == BLACK)
					RBDelete(root_p, brother->parent, brother);/* 往上遍历 */
			}
		}else{/* 右红侄＋左右红侄默认当右红侄来处理 */
			if(type == left){/* child是parent的左孩子 */
				nephew2->color = BLACK;         /* 侄=>黑 */
				brother->color = parent->color; /* 兄=>父色 */

				if(parent->parent == BNULL)     /* 父左旋转 */
					rotate_left(root_p);
				else if(parent->parent->right == parent)
					rotate_left(&(parent->parent->right));
				else
					rotate_left(&(parent->parent->left));
				if(brother->color == BLACK)
					RBDelete(root_p, brother->parent, brother);/* 往上遍历 */
			}else{/* child是parent的右孩子 */ 
				nephew2->color = parent->color;    /* 侄=>父色 */
				parent->color  = BLACK;            /* 父=>黑 *//* 其实父亲一定为黑 */

				rotate_left(&(parent->right));     /* 兄左旋转 */
				if(parent->parent == BNULL)
					rotate_right(root_p);        /* 父右旋转 */
				else if(parent->parent->right == parent)
					rotate_right(&(parent->parent->right));
				else
					rotate_right(&(parent->parent->left));

				if(nephew2->color == RED)
					RBDelete(root_p, nephew2->parent, nephew2);/* 往上遍历 */
			}
		}/* end of if双黑侄 */
		break;
	}/* end of switch */
}

/* ---------------------------------------------------------------------
 函数名称： dright_balance
 参    数： root_p   -- 指向树根节点地址的指针
 	    node    -- 需要平衡的节点指针
	    type    -- 标记旋转的方向
 返回参数： 无
 功能描述： 左树节点减少、导致右高，需要右平衡RBDelete专用函数
 -----------------------------------------------------------------------
*/
void dright_balance(RTree** root_p, RNode* node, type_t type)
{
	if(node->parent == BNULL)
		rotate_left(root_p);
	else if(node->parent->left == node){
		if(type == left)
			rotate_left(&(node->parent->left));
		else
			rotate_right(&(node->parent->left));
	}else{
		if(type == left)
			rotate_left(&(node->parent->right));
		else
			rotate_right(&(node->parent->right));

	}
}

/* ---------------------------------------------------------------------
 函数名称： rotate_left
 参    数： sub_root_p   -- 指向节点地址的指针
 返回参数： 无
 功能描述： 左旋转
 -----------------------------------------------------------------------
*/
void rotate_left(RNode** sub_root_p)
{
	if(*sub_root_p == BNULL || (*sub_root_p)->right == BNULL)/* 不可能的情况 */
		printf("WARNING: program error detected in rotate_left!\n");
	else{
		RNode* right_tree    =  (*sub_root_p)->right;

		/* 更新旋转后的父节点指针，这点很重要 */
		right_tree->parent       = (*sub_root_p)->parent;
		(*sub_root_p)->parent    = right_tree;
		right_tree->left->parent = *sub_root_p;

		/* 左旋转树 */
		(*sub_root_p)->right =  right_tree->left;
		right_tree->left     =  *sub_root_p;
		*sub_root_p          =  right_tree;
	}
}

/* ---------------------------------------------------------------------
 函数名称： rotate_right
 参    数： sub_root_p   -- 指向节点地址的指针
 返回参数： 无
 功能描述： 右旋转
 -----------------------------------------------------------------------
*/
void rotate_right(RNode** sub_root_p)
{
	if(*sub_root_p == BNULL || (*sub_root_p)->left == BNULL)/* 不可能的情况 */
		printf("WARNING: program error detected in rotate_right!\n");
	else{
		RNode* left_tree     =  (*sub_root_p)->left;

		/* 更新旋转后的父节点指针，这点很重要 */
		left_tree->parent        = (*sub_root_p)->parent;
		(*sub_root_p)->parent    = left_tree;
		left_tree->right->parent = *sub_root_p;

		/* 右旋转树 */
		(*sub_root_p)->left  =  left_tree->right;
		left_tree->right     =  *sub_root_p;
		*sub_root_p          =  left_tree;
	}
}

/* ---------------------------------------------------------------------
 函数名称： Successor
 参    数： node   -- 要删除的节点指针
 返回参数： BNULL   -- 参数错误
          suc   -- 后继节点信息
 功能描述： 直接后继,但当此node无有节点时，我们返回的是node本身
 -----------------------------------------------------------------------
*/
RNode* Successor(RNode* node)
{
	if(node == BNULL)
		return BNULL;

	RNode* suc = BNULL;
	if(node->right == BNULL) /* 判断node是否具有右子树 */
		return node;
	else
		suc = node->right;

	while(suc->left != BNULL){
		suc = suc->left;
	};

	return suc;
}


/* 文件来源：『20065562's Blog』有水的地方就有余 http://hi.baidu.com/20065562 */

