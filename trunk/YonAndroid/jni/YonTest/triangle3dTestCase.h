#ifndef _TRIANGLE3D_TEST_CASE_
#define _TRIANGLE3D_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class triangle3dTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( triangle3dTestCase );
	CPPUNIT_TEST( isPointInside );
	CPPUNIT_TEST( isPointInsideFast );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void isPointInside();
	void isPointInsideFast();
};

#endif