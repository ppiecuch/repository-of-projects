
#ifndef _LOOP_TEST_CASE_
#define _LOOP_TEST_CASE_

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