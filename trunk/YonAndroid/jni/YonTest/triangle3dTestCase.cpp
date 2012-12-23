#include "triangle3dTestCase.h"
#include "triangle3d.h"

CPPUNIT_TEST_SUITE_REGISTRATION( triangle3dTestCase );

void triangle3dTestCase::setUp()
{
}

void triangle3dTestCase::isPointInside()
{
	//z=kx+b
	//z=-2x+50
	//2x+z-50=0;
	//2*30+30-50=40>0
	bool r1=triangle3di(vector3di(25, 0, 0), vector3di(0, 0, 50), vector3di(50, 0, 50)).isPointInside(vector3di(30,0,30));
	printf("\r\nr1:%s\r\n",r1?"true":"false");
	CPPUNIT_ASSERT(r1==true);

	bool r2=triangle3di(vector3di(250, 0, 0), vector3di(0, 0, 500), vector3di(500, 0, 500)).isPointInside(vector3di(300,0,300));
	printf("r2:%s\r\n",r2?"true":"false");
	CPPUNIT_ASSERT(r2==true);
}

void triangle3dTestCase::isPointInsideFast()
{
	bool r1=triangle3di(vector3di(25, 0, 0), vector3di(0, 0, 50), vector3di(50, 0, 50)).isPointInsideFast(vector3di(30,0,30));
	printf("\r\nr1:%s\r\n",r1?"true":"false");
	CPPUNIT_ASSERT(r1==true);

	bool r2=triangle3di(vector3di(250, 0, 0), vector3di(0, 0, 500), vector3di(500, 0, 500)).isPointInsideFast(vector3di(300,0,300));
	printf("r2:%s\r\n",r2?"true":"false");
	CPPUNIT_ASSERT(r2==true);
}