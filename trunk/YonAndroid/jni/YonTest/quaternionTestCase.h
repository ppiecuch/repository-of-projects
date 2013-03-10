
#ifndef _QUATERNION_TEST_CASE_
#define _QUATERNION_TEST_CASE_

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

class quaternionTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( quaternionTestCase );
	CPPUNIT_TEST( Matrix );
	CPPUNIT_TEST( AngleAxis );
	CPPUNIT_TEST( Euler );
	CPPUNIT_TEST( slerp );
	CPPUNIT_TEST( transformVec );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void Matrix();
	void AngleAxis();
	void Euler();
	void slerp();
	void transformVec();
};

#endif