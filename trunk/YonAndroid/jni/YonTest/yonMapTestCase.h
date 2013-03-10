
#ifndef _YONMAP_TEST_CASE_
#define _YONMAP_TEST_CASE_

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

class yonMapTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( yonMapTestCase );
	//CPPUNIT_TEST( size );
	//CPPUNIT_TEST( find );
	//CPPUNIT_TEST( insert );
	CPPUNIT_TEST( remove );
	//CPPUNIT_TEST( insertAndRemove );
	//CPPUNIT_TEST( access );
	//CPPUNIT_TEST( stringMap );
	CPPUNIT_TEST_SUITE_END();
protected:
	map<s32,c8> m;

public:
	void setUp();

protected:
	void stringMap();
	void access();
	void size();
	void find();
	void insert();
	void remove();
	void insertAndRemove();
};

#endif