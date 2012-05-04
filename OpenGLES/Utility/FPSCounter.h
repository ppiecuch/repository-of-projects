//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.h
//	Frames per sceond counter class
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FPS_COUNTER_H
#define _FPS_COUNTER_H
#include "macro.h"

#pragma comment(lib,"winmm.lib")

class UTILITY_API FPSCounter
{
public:
	FPSCounter()	:	fps(0.0f), lastTime(0.0f), frames(0), time(0.0f)
	{}
	~FPSCounter()	{}

	void update(void);
	float getFPS(void)	{return fps;}

protected:
	float fps;

	float lastTime;
	int frames;
	float time;
};

#endif	//FPS_COUNTER_H