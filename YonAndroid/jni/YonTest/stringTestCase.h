
#ifndef _STRING_TEST_CASE_
#define _STRING_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class stringTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( stringTestCase );
	/*CPPUNIT_TEST( makeLower );
	CPPUNIT_TEST( makeUpper );
	CPPUNIT_TEST( findLast );
	CPPUNIT_TEST( equalsSubstringIgnoreCase );
	CPPUNIT_TEST( subString );*/
	//CPPUNIT_TEST( find );
	//CPPUNIT_TEST( splits);
	//CPPUNIT_TEST(les);
	//CPPUNIT_TEST(intelatof);
	CPPUNIT_TEST(append);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void les();
	void makeLower();
	void makeUpper();
	void findLast();
	void equalsSubstringIgnoreCase();
	void subString();
	void find();
	void intelatof();
	void append();
	//void splits();
};

#endif