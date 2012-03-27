#include <cppunit/config/SourcePrefix.h>
#include "stringTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( stringTestCase );

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
void stringTestCase::findLast(){
	core::stringc str("Make");
	CPPUNIT_ASSERT( str.findLast('M')==0 );
	CPPUNIT_ASSERT( str.findLast('e')==3 );
	CPPUNIT_ASSERT( str.findLast('1')==-1 );
}
void stringTestCase::equalsSubstringIgnoreCase(){
	core::stringc str("MadeInChina");
	CPPUNIT_ASSERT( str.equalsSubstringIgnoreCase("Madeinchina")==true );
	CPPUNIT_ASSERT( str.equalsSubstringIgnoreCase("madeinchina")==true );
	CPPUNIT_ASSERT( str.equalsSubstringIgnoreCase("china",5)==false );
	CPPUNIT_ASSERT( str.equalsSubstringIgnoreCase("china",6)==true );
	CPPUNIT_ASSERT( str.equalsSubstringIgnoreCase("china",7)==false );
}