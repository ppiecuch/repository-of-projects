
#ifndef _PATH_TEST_CASE_
#define _PATH_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class pathTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( pathTestCase );
	CPPUNIT_TEST( regularize );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void regularize();
};

#endif