#include "vector3dTestCase.h"
#include <Windows.h>

#include <sys/timeb.h>
#include <time.h>

// /CPPUNIT_TEST_SUITE_REGISTRATION( vector3dTestCase );

bool _isBetweenPoints(const vector3df& point,const vector3df& begin, const vector3df& end)
{
	const f32 ix=core::min_(begin.x,end.x);
	const f32 iy=core::min_(begin.y,end.y);
	const f32 iz=core::min_(begin.z,end.z);
	const f32 ax=core::max_(begin.x,end.x);
	const f32 ay=core::max_(begin.y,end.y);
	const f32 az=core::max_(begin.z,end.z);

	return point.x>=ix&&point.x<=ax&&point.y>=iy&&point.y<=ay&&point.z>=iz&&point.z<=az;
}

void vector3dTestCase::setUp()
{
}
void vector3dTestCase::isBetweenPoints(){

#define COUNT 10000000
	core::vector3df s(0,0,0);
	core::vector3df e(10,10,10);
	core::vector3df t1(1,1,1);
	core::vector3df t2(-1,-1,-1);
	core::vector3df t3(20,20,20);

	__time64_t deltaS;
	unsigned short deltaM;

	struct _timeb start1;
	_ftime64_s( &start1 ); 

	for(s32 i=0;i<COUNT;++i)
	{

		t1.isBetweenPoints(s,e);
		t1.isBetweenPoints(s,e);
		t1.isBetweenPoints(s,e);
	}
	
	struct _timeb end1;
	_ftime64_s( &end1 );
	deltaS=(end1.millitm<start1.millitm)?end1.time-start1.time-1:end1.time-start1.time;
	deltaM=(end1.millitm<start1.millitm)?end1.millitm-start1.millitm+1000:end1.millitm-start1.millitm;
	printf("\n%d.",deltaS);
	printf("%u\n",deltaM);


	struct _timeb start2;
	_ftime64_s( &start2 ); 


	for(s32 i=0;i<COUNT;++i)
	{
		_isBetweenPoints(t1,s,e);
		_isBetweenPoints(t2,s,e);
		_isBetweenPoints(t3,s,e);
	}
	
	struct _timeb end2;
	_ftime64_s( &end2 ); 
	deltaS=(end2.millitm<start2.millitm)?end2.time-start2.time-1:end2.time-start2.time;
	deltaM=(end2.millitm<start2.millitm)?end2.millitm-start2.millitm+1000:end2.millitm-start2.millitm;
	printf("\n%d.",deltaS);
	printf("%u\n",deltaM);

	CPPUNIT_ASSERT(t1.isBetweenPoints(s,e)==_isBetweenPoints(t1,s,e));
	CPPUNIT_ASSERT(t2.isBetweenPoints(s,e)==_isBetweenPoints(t2,s,e));
	CPPUNIT_ASSERT(t3.isBetweenPoints(s,e)==_isBetweenPoints(t3,s,e));
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