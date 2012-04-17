
#ifndef _RBTREE_TEST_CASE_
#define _RBTREE_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class rbtreeTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( rbtreeTestCase );
	CPPUNIT_TEST( insert1 );
	CPPUNIT_TEST( insert2 );
	CPPUNIT_TEST( insert3 );
	CPPUNIT_TEST( insert4 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void insert1();
	void insert2();
	void insert3();
	void insert4();
};

#endif