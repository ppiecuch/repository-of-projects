#ifndef _YON_PLATFORM_CTIMERWIN32_H_
#define _YON_PLATFORM_CTIMERWIN32_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_WIN32
#include "ITimer.h"
#include <windows.h>

namespace yon{
namespace platform{

	class CTimerWin32 : public virtual ITimer{
	private:
		f32 m_fVirtualTimerSpeed;
		u32 m_uVirtualTime;
		u32 m_uStartStaticTime;
		u32 m_uLastStaticTime;
		u32 m_uStaticTime;
		bool m_bSetup;

		BOOL m_bHighAccurateSupport;
		bool m_bMultiCore;
		LARGE_INTEGER m_performanceFreq;
		u32 getStaticTime();
	public:
		CTimerWin32();
		virtual ~CTimerWin32();
		virtual void start();
		virtual void pause();
		virtual void resume();
		virtual bool isPaused(){ return m_bSetup==false;}
		virtual void setSpeed(f32 speed);
		virtual u32 getTime() const;
		virtual u32 getRealTime() const;
		virtual void tick();
	};
}
}
#endif//YON_COMPILE_WITH_WIN32
#endif