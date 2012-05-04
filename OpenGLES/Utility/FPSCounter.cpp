//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.cpp
//	Functions for Frames per second counter class
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "FPSCounter.h"

void FPSCounter::update(void)
{
	//keep track of time passed and frame count
	time=timeGetTime()*0.001f;
	++frames;

	//If a second has passed
	if(time-lastTime>1.0f)
	{
		fps=frames/(time-lastTime);	//update the number of frames per second
		lastTime=time;				//set time for the start of the next count
		frames=0;					//reset fps for this second
	}
}