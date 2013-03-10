
#ifndef _BSTREE_TEST_CASE_
#define _BSTREE_TEST_CASE_

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

class bstreeTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( bstreeTestCase );
	CPPUNIT_TEST( insert );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void insert();
};

#endif