#include "btreeTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( btreeTestCase );

void btreeTestCase::setUp()
{
}
void btreeTestCase::rotateLeft1()
{
	btree<c8> tree('u');
	btreenode<c8> p('p');
	btreenode<c8> q('q');
	btreenode<c8> l('l');
	btreenode<c8> r('r');
	btreenode<c8> rl('t');
	tree.setLeftChild(&p);
	tree.setRightChild(&q);
	p.setLeftChild(&l);
	p.setRightChild(&r);
	r.setLeftChild(&rl);

	tree.inorderTraversal();

	p.rotateLeft();

	tree.inorderTraversal();
	 
	//CPPUNIT_ASSERT(arr[0]==13&&arr[1]==27&&arr[2]==38&&arr[3]==49&&arr[4]==49&&arr[5]==65&&arr[6]==76&&arr[7]==97);
}
void btreeTestCase::rotateLeft2()
{
	btree<c8> tree('u');
	btreenode<c8> p('p');
	btreenode<c8> q('q');
	btreenode<c8> l('l');
	btreenode<c8> r('r');
	btreenode<c8> rl('t');
	tree.setLeftChild(&q);
	tree.setRightChild(&p);
	p.setLeftChild(&l);
	p.setRightChild(&r);
	r.setLeftChild(&rl);

	tree.inorderTraversal();

	p.rotateLeft();

	tree.inorderTraversal();

	//CPPUNIT_ASSERT(arr[0]==13&&arr[1]==27&&arr[2]==38&&arr[3]==49&&arr[4]==49&&arr[5]==65&&arr[6]==76&&arr[7]==97);
}
void btreeTestCase::rotateRight1()
{
	btree<c8> tree('u');
	btreenode<c8> p('p');
	btreenode<c8> q('q');
	btreenode<c8> l('l');
	btreenode<c8> r('r');
	btreenode<c8> rl('t');
	tree.setLeftChild(&p);
	tree.setRightChild(&q);
	p.setLeftChild(&l);
	p.setRightChild(&r);
	l.setRightChild(&rl);

	tree.inorderTraversal();

	p.rotateRight();

	tree.inorderTraversal();
}
void btreeTestCase::rotateRight2()
{
	btree<c8> tree('u');
	btreenode<c8> p('p');
	btreenode<c8> q('q');
	btreenode<c8> l('l');
	btreenode<c8> r('r');
	btreenode<c8> rl('t');
	tree.setLeftChild(&q);
	tree.setRightChild(&p);
	p.setLeftChild(&l);
	p.setRightChild(&r);
	l.setRightChild(&rl);

	tree.inorderTraversal();

	p.rotateRight();

	tree.inorderTraversal();
}