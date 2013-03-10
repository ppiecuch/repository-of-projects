
#ifndef _HEAPSORT_TEST_CASE_
#define _HEAPSORT_TEST_CASE_

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

class heapsortTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( heapsortTestCase );
	CPPUNIT_TEST( sort1 );
	CPPUNIT_TEST( sort2 );
	CPPUNIT_TEST( sort3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void sort1();
	void sort2();
	void sort3();
};

#endif