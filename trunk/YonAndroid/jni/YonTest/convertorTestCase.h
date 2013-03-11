
#ifndef _CONVERTOR_TEST_CASE_
#define _CONVERTOR_TEST_CASE_

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

class convertorTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( convertorTestCase );
	CPPUNIT_TEST( same );
	CPPUNIT_TEST( basic );
	CPPUNIT_TEST( string2other );
	CPPUNIT_TEST( other2string );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void same();
	void basic();
	void string2other();
	void other2string();
};

#endif