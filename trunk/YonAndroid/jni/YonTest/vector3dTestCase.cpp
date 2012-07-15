#include "vector3dTestCase.h"
#include <Windows.h>

//CPPUNIT_TEST_SUITE_REGISTRATION( vector3dTestCase );

void vector3dTestCase::setUp()
{
}
void vector3dTestCase::getHorizontalAngle(){
	core::vector3df v(0,0,1);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,0.0f,0.001f));

	v.set(0,1,0);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,90.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,0.0f,0.001f));

	v.set(1,0,0);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,90.0f,0.001f));

	v.set(-10,0,0);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,270.0f,0.001f));

	v.set(0,-1,0);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,-90.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,0.0f,0.001f));

}