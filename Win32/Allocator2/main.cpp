#include <stdio.h>
#include <stdlib.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}


#include "lcAllocator.h"
#include "lcThread.h"
#include "lcMutex.h"
#include "lcUtil.h"
#include "lcLogger.h"
#include "lcMap.h"
#include "lcTimer.h"
#include "lcLimiter.h"
#include "lcRandomizer.h"
using namespace lc;

void outOfMemory(){
	printf("outOfMemory\r\n");
}

struct SMyRunnable1 : public lc::IRunnable{
	virtual void run(){
		LC_INFO("start SMyRunnable1()\r\n");
		for(u32 i=0;i<100;++i)
		{
			LC_NEW int;
			lc::sleep(50);
			LC_DEBG("SMyRunnable1(%d)\r\n",i);
		}
		lc::sleep(50);
		LC_INFO("end SMyRunnable1()\r\n");
	}
};
struct SMyRunnable2 : public lc::IRunnable{
	virtual void run(){
		LC_INFO("start SMyRunnable2()\r\n");
		for(u32 i=0;i<100;++i)
		{
			LC_NEW short;
			lc::sleep(50);
			LC_DEBG("SMyRunnable2(%d)\r\n",i);
		}
		lc::sleep(50);
		LC_INFO("end SMyRunnable2()\r\n");
	}
};
struct SMyRunnable3 : public lc::IRunnable{
	virtual void run(){
		LC_INFO("start SMyRunnable3()\r\n");
		for(u32 i=0;i<100;++i)
		{
			LC_NEW char;
			lc::sleep(50);
			LC_DEBG("SMyRunnable3(%d)\r\n",i);
		}
		lc::sleep(50);
		LC_INFO("end SMyRunnable3()\r\n");
	}
};
struct SMyRunnable4 : public lc::IRunnable{
	virtual void run(){
		LC_INFO("start SMyRunnable4()\r\n");
		for(u32 i=0;i<100;++i)
		{
			LC_NEW double;
			lc::sleep(50);
			LC_DEBG("SMyRunnable4(%d)\r\n",i);
		}
		lc::sleep(50);
		LC_INFO("end SMyRunnable4()\r\n");
	}
};

f32 FPS=30.f;
f32 sleepTime=30;
void limitFPSCallback(f32 value)
{
	LC_DEBG("limitFPSCallback:%.2f\r\n",value);
	sleepTime+=value;
	if(sleepTime<0)sleepTime=0;
}

lc::FPSLimiter fpsLimiter(false);

int main()
{
	//EnableMemLeakCheck();

#if 1

	MemoryTracer::create();

	timer::create();
	randomizer::create();
	{
#if 1
		fpsLimiter.setFPS((u32)FPS);
		for(int i=0;i<200;++i)
		{
			//u32 start=timer::getInstance().getTime();
			if(i==100)
				fpsLimiter.setFPS(20);
			//simulate update/draw
			lc::sleep(randomizer::getInstance().rand(20,30));
			fpsLimiter.tick();
			//u32 end=timer::getInstance().getTime();
			//u32 diff=end-start;
			//f32 fps=diff?1000.f/diff:1000;
			//LC_DEBG("FPS:%.2f,diff:%u\r\n",fps,diff);
			LC_DEBG("FPS:%.2f\r\n",fpsLimiter.getRealFPS());
		}
#elif 1
		limiter<f32,1000> l;
		l.setExceedCallback(&limitFPSCallback);
		l.setLimit(FPS);
		for(int i=0;i<200;++i)
		{
			u32 start=timer::getInstance().getTime();
			lc::sleep((s32)sleepTime);
			u32 end=timer::getInstance().getTime();
			u32 diff=end-start;
			f32 fps=diff?1000.f/diff:1000;
			l.add(fps);
			LC_DEBG("FPS:%.2f,diff:%u,sleepTime:%.2f\r\n",fps,diff,sleepTime);
		}
#else
		limiter<s32,10000> l;
		l.add(1);

		LC_DEBG("%d\r\n",randomizer::getInstance().rand(0,10));
		LC_DEBG("%d\r\n",randomizer::getInstance().rand(0,10));
		LC_DEBG("%d\r\n",randomizer::getInstance().rand(0,10));
		LC_DEBG("%d\r\n",randomizer::getInstance().rand(0,10));
#endif
	}

	LC_DEBG("%u\r\n",timer::getInstance().getTime());
	randomizer::getInstance().destroy();
	timer::getInstance().destroy();

#elif 1

	MemoryTracer::create();

	SMyRunnable1* r1=LC_NEW SMyRunnable1;
	lc::thread* td1=lc::thread::createInstance(r1);
	td1->start();

	SMyRunnable2* r2=LC_NEW SMyRunnable2;
	lc::thread* td2=lc::thread::createInstance(r2);
	td2->start();

	SMyRunnable3* r3=LC_NEW SMyRunnable3;
	lc::thread* td3=lc::thread::createInstance(r3);
	td3->start();

	SMyRunnable4* r4=LC_NEW SMyRunnable4;
	lc::thread* td4=lc::thread::createInstance(r4);
	td4->start();

	LC_DEBG("%s\r\n","!@#$%^&*()_+{}:\"|<>?[];'\\/.,-=");

#elif 1
	MemoryTracer::create();
	
	{
		core::map<s32,s32> m;
		m.set(0,0);
		m.set(1,1);
	}
#elif 1
	MemoryTracer::create();
	int* p=LC_NEW int;
	delete p;
	double* d=LC_NEW double;
	float* f=LC_NEW float;
	short* s=LC_NEW short;
	delete f;
	long* l= new  long;
	char* c=new char;
	delete c;
	MemoryTracer::getInstance().testDeprecated1();
	MemoryTracer::testDeprecated2(NULL);
#else
	Allocator<PRIMITIVE> allocator;
	allocator.setOutOfMemHandler(&outOfMemory);
	//allocator.setMaxSize(1);
	
	lcTry{
		s16* p1=LC_ALLOCATE(allocator,s16,sizeof(s16));
		printf("%d\r\n",*p1);
		allocator.construct<s16>(p1,10);
		printf("%d\r\n",*p1);
		allocator.destruct<s16>(p1);
		printf("%d\r\n",*p1);
		LC_DEALLOCATE(allocator,p1);
		s32* p2=LC_ALLOCATE(allocator,s32,sizeof(s32));
		//s32* p3=LUCID_NEW s32();
	}lcCatch(...){
		printf("Nothing!\r\n");
	}

	allocator.destroy();
#endif

	//int* p=new int;


	system("pause");
	MemoryTracer::getInstance().destroy();
	return 0;
}