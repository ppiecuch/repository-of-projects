
#ifndef _HEAPSORT_TEST_CASE_
#define _HEAPSORT_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class heapsortTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( heapsortTestCase );
	CPPUNIT_TEST( sort1 );
	CPPUNIT_TEST( sort2 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void sort1();
	void sort2();
};

#endif