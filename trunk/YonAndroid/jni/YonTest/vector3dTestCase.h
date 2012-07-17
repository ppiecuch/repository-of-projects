
#ifndef _VECTOR3D_TEST_CASE_
#define _VECTOR3D_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class vector3dTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( vector3dTestCase );
	CPPUNIT_TEST( getHorizontalAngle1 );
	CPPUNIT_TEST( getHorizontalAngle2 );
	CPPUNIT_TEST( getHorizontalAngle3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void getHorizontalAngle1();
	void getHorizontalAngle2();
	void getHorizontalAngle3();
};

#endif