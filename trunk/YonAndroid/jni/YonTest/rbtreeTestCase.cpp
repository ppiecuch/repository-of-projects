#include "rbtreeTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( rbtreeTestCase );

void rbtreeTestCase::setUp()
{
}
rbtreenode<c8>* print(rbtreenode<c8>* tree){
	while(tree->getParent()){
		tree=(rbtreenode<c8>*)tree->getParent();
	}
	printf("\nroot:%c",tree->getValue());
	tree->preorderTraversal();
	tree->inorderTraversal();
	return tree;
}
rbtreenode<s32>* print(rbtreenode<s32>* tree){
	while(tree->getParent()){
		tree=(rbtreenode<s32>*)tree->getParent();
	}
	printf("\nroot:%d",tree->getValue());
	tree->preorderTraversal();
	tree->inorderTraversal();
	return tree;
}
void rbtreeTestCase::remove1()
{
	rbtree<c8>* tree=new rbtree<c8>();
	rbtreenode<c8>* node=(rbtreenode<c8>*)tree->insert('u');

	tree=print(tree);

	tree->remove('u');

	CPPUNIT_ASSERT(tree->isEmpty());
}
void rbtreeTestCase::remove2()
{
	rbtree<c8>* tree=new rbtree<c8>();
	tree->insert('p');
	tree->insert('z');
	tree->insert('u');

	tree=print(tree);

	tree->remove('u');

	tree=print(tree);

	tree->remove('z');

	tree=print(tree);

	CPPUNIT_ASSERT(tree->getValue()=='p');
}

void rbtreeTestCase::remove3()
{
	rbtree<c8>* tree=new rbtree<c8>();
	tree->insert('z');
	tree->insert('u');
	tree=print(tree);
	tree->insert('k');
	tree=print(tree);
	tree->insert('a');
	tree=print(tree);
	tree->insert('g');
	tree=print(tree);
	tree->insert('n');
	tree=print(tree);

	tree->remove('k');
	tree=print(tree);
	tree->remove('a');
	tree=print(tree);
	tree->remove('u');
	tree=print(tree);
	tree->remove('z');
	tree=print(tree);
	tree->remove('g');
	tree=print(tree);

	CPPUNIT_ASSERT(tree->getValue()=='n');
}

void rbtreeTestCase::remove4()
{
	rbtree<s32>* tree=new rbtree<s32>();
	tree->insert(4);
	tree->insert(6);
	tree=print(tree);
	tree->insert(9);
	tree=print(tree);
	tree->insert(8);
	tree=print(tree);
	tree->insert(11);
	tree=print(tree);
	tree->insert(12);
	tree=print(tree);
	tree->insert(14);
	tree=print(tree);
	tree->insert(1);
	tree=print(tree);
	tree->insert(3);
	tree=print(tree);
	tree->insert(7);
	tree=print(tree);
	tree->insert(5);
	tree=print(tree);
	tree->insert(2);
	tree=print(tree);
	tree->insert(10);
	tree=print(tree);
	tree->insert(13);
	tree=print(tree);

	tree->remove(6);
	tree=print(tree);
	tree->remove(5);
	tree=print(tree);
	tree->remove(4);
	tree=print(tree);
	tree->remove(7);
	tree=print(tree);
	tree->remove(8);
	tree=print(tree);
	tree->remove(9);
	tree=print(tree);
	tree->remove(3);
	tree=print(tree);
	tree->remove(10);
	tree=print(tree);
	tree->remove(11);
	tree=print(tree);
	tree->remove(12);
	tree=print(tree);
	tree->remove(2);
	tree=print(tree);
	tree->remove(14);
	tree=print(tree);
	tree->remove(13);
	tree=print(tree);
	tree->remove(1);
	tree=print(tree);

	CPPUNIT_ASSERT(tree->isEmpty());
}
void rbtreeTestCase::insert1()
{
	rbtree<c8> tree;
	rbtreenode<c8>* node=(rbtreenode<c8>*)tree.insert('u');

	tree.inorderTraversal();

	CPPUNIT_ASSERT(node->isBlack());
}
void rbtreeTestCase::insert2()
{
	rbtree<c8>* tree=new rbtree<c8>();
	tree->insert('p');
	tree->insert('z');

	rbtreenode<c8>* node=(rbtreenode<c8>*)tree->insert('u');

	tree=(rbtreenode<c8>*)tree->getParent();
	tree->inorderTraversal();

	rbtreenode<c8>* p=(rbtreenode<c8>*)tree->find('p');
	rbtreenode<c8>* u=(rbtreenode<c8>*)tree->find('u');
	rbtreenode<c8>* z=(rbtreenode<c8>*)tree->find('z');

	CPPUNIT_ASSERT(u->isBlack()&&p->isRed()&&z->isRed());
}
void rbtreeTestCase::insert3()
{
	rbtree<c8>* tree=new rbtree<c8>();
	tree->insert('z');
	tree->insert('u');
	tree=print(tree);
	tree->insert('k');
	tree=print(tree);
	tree->insert('a');
	tree=print(tree);
	tree->insert('g');
	tree=print(tree);

	rbtreenode<c8>* node=(rbtreenode<c8>*)tree->insert('n');

	tree=print(tree);

	rbtreenode<c8>* g=(rbtreenode<c8>*)tree->find('g');
	rbtreenode<c8>* n=(rbtreenode<c8>*)tree->find('n');

	CPPUNIT_ASSERT(g->isRed()&&n->isRed());
}
void rbtreeTestCase::insert4()
{
	rbtree<s32>* tree=new rbtree<s32>();
	tree->insert(4);
	tree->insert(6);
	tree=print(tree);
	tree->insert(9);
	tree=print(tree);
	tree->insert(8);
	tree=print(tree);
	tree->insert(11);
	tree=print(tree);
	tree->insert(12);
	tree=print(tree);
	tree->insert(14);
	tree=print(tree);
	tree->insert(1);
	tree=print(tree);
	tree->insert(3);
	tree=print(tree);
	tree->insert(7);
	tree=print(tree);
	tree->insert(5);
	tree=print(tree);
	tree->insert(2);
	tree=print(tree);
	tree->insert(10);
	tree=print(tree);
	tree->insert(13);

	tree=print(tree);

	rbtreenode<s32>* n2=(rbtreenode<s32>*)tree->find(2);
	rbtreenode<s32>* n5=(rbtreenode<s32>*)tree->find(5);
	rbtreenode<s32>* n7=(rbtreenode<s32>*)tree->find(7);
	rbtreenode<s32>* n10=(rbtreenode<s32>*)tree->find(10);
	rbtreenode<s32>* n12=(rbtreenode<s32>*)tree->find(12);
	rbtreenode<s32>* n13=(rbtreenode<s32>*)tree->find(13);

	CPPUNIT_ASSERT(n2->isRed()&&n5->isRed()&&n7->isRed()&&n10->isRed()&&n12->isRed()&&n13->isRed());
}