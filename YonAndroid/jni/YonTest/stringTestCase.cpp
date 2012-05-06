#include <cppunit/config/SourcePrefix.h>
#include "stringTestCase.h"
#include "yonUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( stringTestCase );

void stringTestCase::setUp()
{
}
void stringTestCase::find(){
	core::stringc str1("Make In China");
	CPPUNIT_ASSERT(str1.find("Make")==0);

	core::stringc str2("Make In China");
	CPPUNIT_ASSERT(str2.find("In")==5);

	core::stringc str3("Make In China");
	CPPUNIT_ASSERT(str3.find("China ")==-1);
}

void stringTestCase::subString(){
	core::stringc str1("Make");
	CPPUNIT_ASSERT( str1.subString(0)=="Make" );
	core::stringc str2("Make");
	CPPUNIT_ASSERT( str2.subString(5)=="" );
	core::stringc str3("Make");
	CPPUNIT_ASSERT( str3.subString(1)=="ake" );
	core::stringc str4("Make");
	CPPUNIT_ASSERT( str4.subString(1,2)=="ak" );

	io::path str5("../Make.txt");
	printf("\ngetFileName:%s\n", core::getFileName(str5).c_str());
	//printf("\ngetFileName:%s\n", str5.subString(2).c_str());
	//CPPUNIT_ASSERT(  core::getFileName(str5)=="Make.txt" );
	
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