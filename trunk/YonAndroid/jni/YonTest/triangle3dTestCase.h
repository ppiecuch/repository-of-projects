#ifndef _TRIANGLE3D_TEST_CASE_
#define _TRIANGLE3D_TEST_CASE_

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