#include "bstreeTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( bstreeTestCase );

void bstreeTestCase::setUp()
{
}
void bstreeTestCase::insert()
{
	bstree<c8> tree;

	tree.insert('u');
	tree.insert('p');
	tree.insert('z');
	tree.insert('k');

	printf("\n");
	tree.find('p')->getSibling()->print();
	printf("\n");

	tree.inorderTraversal();

	//CPPUNIT_ASSERT(arr[0]==13&&arr[1]==27&&arr[2]==38&&arr[3]==49&&arr[4]==49&&arr[5]==65&&arr[6]==76&&arr[7]==97);
}
