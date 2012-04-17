
#ifndef _BSTREE_TEST_CASE_
#define _BSTREE_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class bstreeTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( bstreeTestCase );
	CPPUNIT_TEST( insert );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void insert();
};

#endif