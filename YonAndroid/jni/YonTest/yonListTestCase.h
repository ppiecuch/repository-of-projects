
#ifndef _YON_LIST_TEST_CASE_
#define _YON_LIST_TEST_CASE_

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

class yonListTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( yonListTestCase );
	CPPUNIT_TEST( push_back );
	CPPUNIT_TEST( push_front );
	CPPUNIT_TEST( empty );
	CPPUNIT_TEST( clear );
	CPPUNIT_TEST( begin );
	CPPUNIT_TEST( erase );
	CPPUNIT_TEST( insert_before );
	CPPUNIT_TEST( insert_after );
	CPPUNIT_TEST ( eraseSpecific );
	CPPUNIT_TEST_SUITE_END();
protected:
	list<u32> l;

public:
	void setUp();

protected:
	void push_back();
	void push_front();
	void empty();
	void clear();
	void begin();
	void erase();
	void insert_before();
	void insert_after();
	void eraseSpecific();
};

#endif