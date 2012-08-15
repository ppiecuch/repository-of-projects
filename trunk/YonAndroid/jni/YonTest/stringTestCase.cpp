#include <cppunit/config/SourcePrefix.h>
#include "stringTestCase.h"
#include "yonUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( stringTestCase );

void stringTestCase::setUp()
{
}
void stringTestCase::append()
{
	core::stringc str1("OpenGL");
	str1.append("1234");
	CPPUNIT_ASSERT( str1=="OpenGL1234" );
	str1.append("56789");
	CPPUNIT_ASSERT( str1=="OpenGL123456789" );

	core::stringc str2;
	str2.append('c');
	str2.append('h');
	CPPUNIT_ASSERT( str2=="ch" );

	core::stringc str3;
	str3.append("china");
	CPPUNIT_ASSERT( str3=="china" );
	str3.append("me");
	CPPUNIT_ASSERT( str3=="chiname" );
	printf("\n%s\n",str3.c_str());
}
void stringTestCase::intelatof()
{
	core::stringc str1("OpenGL ES-CM 1.1");
	printf("\n%.2f\n",str1.superatof());
	CPPUNIT_ASSERT(core::equals(str1.superatof(),1.1f));

	core::stringc str2(".1");
	CPPUNIT_ASSERT(core::equals(str2.superatof(),0.1f));

	core::stringc str3("hello");
	CPPUNIT_ASSERT(core::equals(str3.superatof(),0.0f));
}
void stringTestCase::les()
{
	core::stringc str1("hello");
	core::stringc str2("hello");
	CPPUNIT_ASSERT(str1<str2==false);

	core::stringc str3("hello");
	core::stringc str4("Hello");
	CPPUNIT_ASSERT(str3<str4==false);

	core::stringc str5("hello");
	core::stringc str6("");
	CPPUNIT_ASSERT(str5<str6==false);
}
void stringTestCase::splits(){
	core::stringc str1("Make In China");
	core::array<core::stringc> arr;
	str1.split(arr," ");
	printf("\n");
	for(u32 i=0;i<arr.size();++i)
		printf("%s\n",arr[i].c_str());
	CPPUNIT_ASSERT(arr.size()==3);
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