#include <cppunit/config/SourcePrefix.h>
#include "stringTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( stringTestCase );

void stringTestCase::setUp()
{
}
void stringTestCase::makeLower(){
	core::stringc str("Make");
	str.makeLower();
	CPPUNIT_ASSERT( str=="make" );
}
void stringTestCase::makeUpper(){
	core::stringc str("Make");
	str.makeUpper();
	CPPUNIT_ASSERT( str=="MAKE" );
}