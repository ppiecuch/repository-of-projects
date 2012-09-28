#include "timerTestCase.h"
#include <Windows.h>
//#include "CTimerWin32.cpp"

//CPPUNIT_TEST_SUITE_REGISTRATION( timerTestCase );

void timerTestCase::setUp()
{
}
void timerTestCase::getTime(){
	yon::ITimer* timer=createTimer();
	u32 time,start,end;
	time=timer->getTime();
	printf("%d\n",time);
	CPPUNIT_ASSERT( time==0 );
	timer->start();
	timer->tick();
	start=timer->getTime();
	Sleep(20);
	timer->tick();
	end=timer->getTime();
	printf("\n%d,%d\n",start,end);
	CPPUNIT_ASSERT( end>start );
	timer->pause();
	start=timer->getTime();
	Sleep(20);
	timer->tick();
	end=timer->getTime();
	printf("\n%d,%d\n",start,end);
	CPPUNIT_ASSERT( end==start );
	timer->resume();
	timer->tick();
	start=timer->getTime();
	Sleep(20);
	timer->tick();
	end=timer->getTime();
	printf("\n%d,%d\n",start,end);
	CPPUNIT_ASSERT( end>start );
	timer->drop();
}
void timerTestCase::getRealTime(){
	yon::ITimer* timer=createTimer();
	u32 time,start,end;
	time=timer->getRealTime();
	printf("%d\n",time);
	CPPUNIT_ASSERT( time==0 );
	timer->start();
	//timer->tick();
	start=timer->getRealTime();
	Sleep(20);
	//timer->tick();
	end=timer->getRealTime();
	printf("\n%d,%d\n",start,end);
	CPPUNIT_ASSERT( end>start );
	timer->pause();
	start=timer->getRealTime();
	Sleep(20);
	//timer->tick();
	end=timer->getRealTime();
	printf("\n%d,%d\n",start,end);
	CPPUNIT_ASSERT( end>start );
	timer->resume();
	//timer->tick();
	start=timer->getRealTime();
	Sleep(20);
	//timer->tick();
	end=timer->getRealTime();
	printf("\n%d,%d\n",start,end);
	CPPUNIT_ASSERT( end>start );
	timer->drop();
}