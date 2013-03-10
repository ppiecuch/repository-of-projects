
#ifndef _BTREE_TEST_CASE_
#define _BTREE_TEST_CASE_

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

class btreeTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( btreeTestCase );
	CPPUNIT_TEST( rotateLeft1 );
	CPPUNIT_TEST( rotateLeft2 );
	CPPUNIT_TEST( rotateRight1 );
	CPPUNIT_TEST( rotateRight2 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void rotateLeft1();
	void rotateLeft2();
	void rotateRight1();
	void rotateRight2();
};

#endif