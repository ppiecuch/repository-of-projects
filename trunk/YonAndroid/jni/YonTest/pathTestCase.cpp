#include "pathTestCase.h"
#include <sys/timeb.h>
#include <time.h>

//CPPUNIT_TEST_SUITE_REGISTRATION( pathTestCase );


void pathTestCase::setUp()
{
}

void pathTestCase::regularize()
{
	io::path p1("C:/fdafdas/fdsafdsa/");
	core::regularize(p1);
	CPPUNIT_ASSERT(p1=="C:/fdafdas/fdsafdsa/");

	io::path p2("C:/fdafdas/fdsafdsa");
	core::regularize(p2);
	CPPUNIT_ASSERT(p2=="C:/fdafdas/fdsafdsa/");

	io::path p3("C:\\fdafdas\\fdsafdsa");
	core::regularize(p3);
	CPPUNIT_ASSERT(p3=="C:/fdafdas/fdsafdsa/");
}