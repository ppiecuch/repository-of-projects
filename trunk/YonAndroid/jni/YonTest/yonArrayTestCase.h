
#ifndef _YON_ARRAY_TEST_CASE_
#define _YON_ARRAY_TEST_CASE_

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

class yonArrayTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( yonArrayTestCase );
	//CPPUNIT_TEST( reallocate );
	//CPPUNIT_TEST( push );
	//CPPUNIT_TEST( insert1 );
	//CPPUNIT_TEST( insert2 );
	//CPPUNIT_TEST( insert3 );
	//CPPUNIT_TEST( insert4 );
	CPPUNIT_TEST( pushString );
	//CPPUNIT_TEST( assign );
	CPPUNIT_TEST_SUITE_END();
protected:
	array<vector3df*> arr;
	array<s32> buffer;
	core::array<core::stringc> strArray;

public:
	void setUp();

protected:
	void reallocate();
	void push();
	void insert1();
	void insert2();
	void insert3();
	void insert4();
	void pushString();
	void assign();
};

#endif