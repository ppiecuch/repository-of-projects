/************************************************************************************

  �ļ��� C���Զ����� Դ�뼰ע�� �ӿ��ļ�
  ���ߣ� ��   ǿ
  ���ڣ� 2009-11-3
  Э�飺 Copyleft (C) GPL
 E-mail: yuembed@126.com
  �ռ䲩�� ��20065562's Blog�� ��ˮ�ĵط������� http://hi.baidu.com/20065562

************************************************************************************/
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef struct node{
	int data;
	struct node* left;
	struct node* right;
}BNode;


/***********************************************************/
/* �������� */


BNode*  InitBTree(void);    /* ��������ʼ������ */

void    DeleteBTree(BNode* root);      /* ɾ�������� */

BNode*  CopyBTree(BNode* srcroot);     /* ���������� */

BNode*  GetBNode(int data, BNode* left, BNode* right);  /* �����������ڵ� */


/* ��ɨ���㷨��Ӧ�� */
int CountLeaf(BNode* root); /* �����������Ҷ�ڵ���� */
int Depth(BNode* root);                    /* ������������ */


#endif
/* �ļ���Դ����20065562's Blog����ˮ�ĵط������� http://hi.baidu.com/20065562 */

