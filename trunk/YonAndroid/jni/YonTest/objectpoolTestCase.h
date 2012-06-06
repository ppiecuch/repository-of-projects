
#ifndef _MATRIX_TEST_CASE_
#define _MATRIX_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class objectpoolTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( objectpoolTestCase );
	CPPUNIT_TEST( recycle );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void recycle();
};

#endif