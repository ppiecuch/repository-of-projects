#include <cppunit/config/SourcePrefix.h>
#include "yonListTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( yonListTestCase );

void yonListTestCase::setUp()
{
}
void yonListTestCase::insert_before()
{
	l.push_back(1);
	l.push_back(3);
	l.insert_after(l.begin(),2);
	list<u32>::Iterator it=l.begin();
	++it;
	printf("\n%d\n",(*it));
	CPPUNIT_ASSERT((*it)==2);
}
void yonListTestCase::insert_after()
{
	l.push_back(1);
	l.push_back(3);
	l.insert_before(l.getLast(),2);
	list<u32>::Iterator it=l.begin();
	++it;
	printf("\n%d\n",(*it));
	CPPUNIT_ASSERT((*it)==2);
}
void yonListTestCase::push_back()
{
	l.push_back(1);
	l.push_back(2);
	CPPUNIT_ASSERT( l.size() == 2 );
}

void yonListTestCase::push_front()
{
	l.push_front(1);
	l.push_front(2);
	CPPUNIT_ASSERT( l.size() == 2 );
}

void yonListTestCase::empty()
{
	l.push_back(1);
	l.push_front(2);
	CPPUNIT_ASSERT( l.empty() == false );
}

void yonListTestCase::clear()
{
	l.push_back(1);
	l.push_front(2);
	l.clear();
	CPPUNIT_ASSERT( l.size() == 0 );
}

void yonListTestCase::begin()
{
	l.push_back(1);
	l.push_front(2);
	list<u32>::ConstIterator iterator=l.begin();
	printf("begin:%d\n",*iterator);
	CPPUNIT_ASSERT( *iterator==2 );
}

void yonListTestCase::erase()
{
	l.push_back(1);
	l.push_front(2);
	list<u32>::Iterator iterator=l.begin();
	l.erase(iterator);
	CPPUNIT_ASSERT( l.size()==1 );
	iterator=l.getLast();
	printf("begin:%d\n",*iterator);
	l.erase(iterator);
	CPPUNIT_ASSERT( l.size()==0 );
}