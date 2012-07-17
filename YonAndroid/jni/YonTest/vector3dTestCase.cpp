#include "vector3dTestCase.h"
#include <Windows.h>

CPPUNIT_TEST_SUITE_REGISTRATION( vector3dTestCase );

void vector3dTestCase::setUp()
{
}
void vector3dTestCase::getHorizontalAngle1(){
	core::vector3df v;
	core::matrix4f m;
	core::vector3df o;

	v.set(0,0,1);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,0.0f,0.001f));

	o.set(1,0,0);
	m.makeIdentity();
	m.setRotationDegrees(v.getHorizontalAngle());
	m.transformVect(o);
	printf("%.2f,%.3f,%.2f\n",o.x,o.y,o.z);
	CPPUNIT_ASSERT(core::equals(o.x,1.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(o.y,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(o.z,0.0f,0.001f));


}

void vector3dTestCase::getHorizontalAngle2(){
	core::vector3df v;
	core::matrix4f m;
	core::vector3df o;

	v.set(0,1,0);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,270.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,0.0f,0.001f));

	o.set(0,0,1);
	m.makeIdentity();
	m.setRotationDegrees(v.getHorizontalAngle());
	m.transformVect(o);
	printf("%.2f,%.2f,%.2f\n",o.x,o.y,o.z);
	CPPUNIT_ASSERT(core::equals(o.x,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(o.y,1.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(o.z,0.0f,0.001f));



}

void vector3dTestCase::getHorizontalAngle3(){
	core::vector3df v;
	core::matrix4f m;
	core::vector3df o;

	v.set(0,-1,0);
	printf("\n%.2f,%.2f\n",v.getHorizontalAngle().x,v.getHorizontalAngle().y);
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().x,90.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(v.getHorizontalAngle().y,0.0f,0.001f));

	o.set(0,0,1);
	m.makeIdentity();
	m.setRotationDegrees(v.getHorizontalAngle());
	m.transformVect(o);
	printf("%.2f,%.2f,%.2f\n",o.x,o.y,o.z);
	CPPUNIT_ASSERT(core::equals(o.x,0.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(o.y,-1.0f,0.001f));
	CPPUNIT_ASSERT(core::equals(o.z,0.0f,0.001f));

}