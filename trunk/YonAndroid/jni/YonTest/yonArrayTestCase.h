
#ifndef _YON_ARRAY_TEST_CASE_
#define _YON_ARRAY_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class yonArrayTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( yonArrayTestCase );
	CPPUNIT_TEST( reallocate );
	CPPUNIT_TEST( push );
	CPPUNIT_TEST_SUITE_END();
protected:
	array<vector3df*> arr;

public:
	void setUp();

protected:
	void reallocate();
	void push();
};

#endif