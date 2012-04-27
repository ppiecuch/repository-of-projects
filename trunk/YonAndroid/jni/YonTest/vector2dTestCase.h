
#ifndef _VECTOR2D_TEST_CASE_
#define _VECTOR2D_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class vector2dTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( vector2dTestCase );
	CPPUNIT_TEST( rotateBy );
	CPPUNIT_TEST( getAngle );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void rotateBy();
	void getAngle();
};

#endif