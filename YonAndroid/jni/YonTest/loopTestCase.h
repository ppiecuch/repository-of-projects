
#ifndef _LOOP_TEST_CASE_
#define _LOOP_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class loopTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( loopTestCase );
	CPPUNIT_TEST( size );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void size();
};

#endif