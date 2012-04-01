
#ifndef _TIMER_TEST_CASE_
#define _TIMER_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class timerTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( timerTestCase );
	CPPUNIT_TEST( getTime );
	CPPUNIT_TEST( getRealTime );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void getTime();
	void getRealTime();
};

#endif