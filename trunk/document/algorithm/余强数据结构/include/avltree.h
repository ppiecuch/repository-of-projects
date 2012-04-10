/************************************************************************************

  �ļ��� C����AVL�߶�ƽ���� Դ�뼰ע��
  ���ߣ� ��   ǿ
  ���ڣ� 2009-11-20~2009-11-26
  Э�飺 Copyleft (C) GPL
 E-mail: yuembed@126.com
  �ռ䲩�� ��20065562's Blog�� ��ˮ�ĵط������� http://hi.baidu.com/20065562

************************************************************************************/
#ifndef AVL_TREE_H
#define AVL_TREE_H

/***********************************************************/
/* ��ؽṹ�嶨�� */

/* ƽ���������� */
enum balance_factor{LEFT_H=-1, EQUAL_H=0, RIGHT_H=1};

/* �������� */
typedef enum{left, right}type_p;/* ɾ���ڵ��ʱ����õ� */

typedef struct node{
	int                  data;    /* ����     */
	struct node*         left;    /* ����   */
	struct node*         right;   /* �Һ���   */
	enum balance_factor  factor;  /* ƽ������ */
}ANode, ATree;

/* BOOL���� */
typedef enum{true, false}BOOL;

/* ����ṹ�壬����Succssor */
typedef struct{
	ANode*  parent;     /* ���ڵ�ָ��         */
	type_p  type;       /* ���ڵ������Һ��� */	
}INFOR;
/***********************************************************/
/* ��������������������� */


int     InitTree(ATree** root_p);              /* ��������ʼ������ */

ANode*  GetANode(int data, ANode* left, ANode* right);  /* �����������ڵ� */

ANode*  Search(ANode* root,const int Key);     /* �ڲ��Ҷ�����������Key */

//����int   Insert(ATree** root_p, int data);   /* ����Ҷ������в���һ���µĽڵ� */
//����int   Delete(ANode** node_p);             /* �Ӳ��Ҷ�������ɾ��һ���ڵ� */
int     Remove(ATree** root);                  /* ɾ�������� */

void LNR(ANode* root, void process(ANode*));   /* ����������������󡢽ڵ㡢�ң� */
void LRN(ANode* root, void process(ANode*));   /* ������������������ҡ��ڵ㣩 */
void NLR(ANode* root, void process(ANode*));   /* ǰ��������������ڵ㡢���ң� */

/***********************************************************/
/* �������� */
ANode*  CopyATree(ANode* root);        /* ���������� */
int     CountLeaf(ANode* root);        /* �����������Ҷ�ڵ���� */
int     Depth(ANode* root);            /* ������������ */

/***********************************************************/
/* AVL������ */

/* AVL���Ĳ��롢ɾ�� */
int Insert(ATree** root_p, int data);
int Delete(ATree** root_p, int data);

#endif

/* �ļ���Դ����20065562's Blog����ˮ�ĵط������� http://hi.baidu.com/20065562 */

