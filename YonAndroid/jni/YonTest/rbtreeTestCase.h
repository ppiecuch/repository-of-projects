
#ifndef _RBTREE_TEST_CASE_
#define _RBTREE_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#ifdef YON_COMPILE_WITH_WIN32
#ifdef _DEBUG
#pragma comment(lib, "Yon_debug.lib")
#else
#pragma comment(lib, "Yon.lib")
#endif
#endif

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class rbtreeTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( rbtreeTestCase );
	//CPPUNIT_TEST( insert1 );
	//CPPUNIT_TEST( insert2 );
	//CPPUNIT_TEST( insert3 );
	//CPPUNIT_TEST( insert4 );
	CPPUNIT_TEST( remove1 );
	CPPUNIT_TEST( remove2 );
	CPPUNIT_TEST( remove3 );
	CPPUNIT_TEST( remove4 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void insert1();
	void insert2();
	void insert3();
	void insert4();
	void remove1();
	void remove2();
	void remove3();
	void remove4();
};

#endif