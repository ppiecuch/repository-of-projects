#include "yonMapTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( yonMapTestCase );

void yonMapTestCase::setUp()
{
}
void yonMapTestCase::stringMap()
{
	core::map<u16,core::stringc> temp;
	CPPUNIT_ASSERT(temp.size()==0);
	temp[1]="1";
	CPPUNIT_ASSERT(((core::stringc)temp[1])=="1");
#define TEST_MACRO 0x3024
	temp[TEST_MACRO]="0x3024";
	CPPUNIT_ASSERT(((core::stringc)temp[TEST_MACRO])=="0x3024");
#undef TEST_MACRO
}
void yonMapTestCase::access()
{
	m[1]='1';
	m[2]='2';
	m[3]='3';

	printf("\n%d\n",m.find(2)->getValue());
	printf("\n%d\n",(u8)m[2]);

	CPPUNIT_ASSERT(m[2]=='2');
}
void yonMapTestCase::size(){
	CPPUNIT_ASSERT(m.size()==0);

	m.set(1,'b');
	CPPUNIT_ASSERT(m.size()==1);

	m.set(1,'a');
	CPPUNIT_ASSERT(m.size()==1);

	m.insert(2,'b');
	CPPUNIT_ASSERT(m.size()==2);
}

void yonMapTestCase::find(){
	CPPUNIT_ASSERT(m.find(1)==NULL);

	m.set(1,'a');
	m.insert(2,'b');
	CPPUNIT_ASSERT(m.find(1)->getValue()=='a');

	CPPUNIT_ASSERT(m.find(3)==NULL);
}
void yonMapTestCase::insert(){
	
	m.insert(4,'4');
	m.insert(6,'6');
	m.insert(9,'9');
	m.insert(8,'8');
	m.insert(11,'11');
	m.insert(12,'12');
	m.insert(14,'14');
	m.insert(1,'1');
	m.insert(3,'3');
	m.insert(7,'7');
	m.insert(5,'5');
	m.insert(2,'2');
	m.insert(10,'10');
	m.insert(13,'13');

	core::map<s32,c8>::Node* n2=m.find(2);
	core::map<s32,c8>::Node* n5=m.find(5);
	core::map<s32,c8>::Node* n7=m.find(7);
	core::map<s32,c8>::Node* n10=m.find(10);
	core::map<s32,c8>::Node* n12=m.find(12);
	core::map<s32,c8>::Node* n13=m.find(13);

	m.preorderTraversal(m.getRoot());


	CPPUNIT_ASSERT(n2->isRed()&&n5->isRed()&&n7->isRed()&&n10->isRed()&&n12->isRed()&&n13->isRed());
}
void yonMapTestCase::remove(){

	m.insert(4,'4');
	m.insert(6,'6');
	m.insert(9,'9');
	m.insert(8,'8');
	m.insert(11,'B');
	m.insert(12,'C');
	m.insert(14,'E');
	m.insert(1,'1');
	m.insert(3,'3');
	m.insert(7,'7');
	m.insert(5,'5');
	m.insert(2,'2');
	m.insert(10,'A');
	m.insert(13,'D');

	c8 c;

	m.preorderTraversal(m.getRoot());
	m.remove(6,&c);
	printf("%c\n",c);
	m.preorderTraversal(m.getRoot());
	m.remove(5);
	m.preorderTraversal(m.getRoot());
	m.remove(4);
	m.preorderTraversal(m.getRoot());
	m.remove(7);
	m.preorderTraversal(m.getRoot());
	m.remove(8);
	m.preorderTraversal(m.getRoot());
	m.remove(9);
	m.preorderTraversal(m.getRoot());
	m.remove(3,&c);
	printf("%c\n",c);
	m.preorderTraversal(m.getRoot());
	m.remove(10);
	m.preorderTraversal(m.getRoot());
	m.remove(11);
	m.preorderTraversal(m.getRoot());
	m.remove(12);
	m.preorderTraversal(m.getRoot());
	m.remove(2);
	m.preorderTraversal(m.getRoot());
	m.remove(14,&c);
	printf("%c\n",c);
	m.preorderTraversal(m.getRoot());
	m.remove(13,&c);
	printf("%c\n",c);
	m.preorderTraversal(m.getRoot());
	m.remove(1);


	CPPUNIT_ASSERT(m.size()==0);
}

void yonMapTestCase::insertAndRemove(){
	m.insert(4,'4');
	m.insert(6,'6');
	m.insert(9,'9');
	m.insert(8,'8');
	m.insert(11,'11');
	m.insert(12,'12');
	m.insert(14,'14');
	m.insert(1,'1');
	m.insert(3,'3');
	m.insert(7,'7');
	m.insert(5,'5');
	m.insert(2,'2');
	m.insert(10,'10');
	m.insert(13,'13');

	m.remove(6);
	m.insert(6,'6');
	m.remove(5);
	m.insert(5,'5');
	m.remove(4);
	m.insert(4,'4');
	m.remove(7);
	m.insert(7,'7');
	m.remove(8);
	m.remove(9);
	m.remove(3);
	m.remove(10);
	m.remove(11);
	m.remove(12);
	m.insert(9,'9');
	m.insert(8,'8');
	m.insert(11,'11');
	m.insert(12,'12');
	m.insert(10,'10');
	m.insert(3,'3');

	m.preorderTraversal(m.getRoot());
}