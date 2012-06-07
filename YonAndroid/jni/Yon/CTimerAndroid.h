#ifndef _YON_PLATFORM_CTIMERANDROID_H_
#define _YON_PLATFORM_CTIMERANDROID_H_

#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include "ITimer.h"

namespace yon{
	namespace platform{

		class CTimerAndroid : public virtual ITimer{
		private:
			f32 m_fVirtualTimerSpeed;
			u32 m_uVirtualTime;
			u32 m_uStartStaticTime;
			u32 m_uLastStaticTime;
			u32 m_uStaticTime;
			bool m_bSetup;

			u32 getStaticTime();
		public:
			CTimerAndroid();
			virtual ~CTimerAndroid();
			virtual void start();
			virtual void pause();
			virtual void resume();
			virtual bool isPaused(){ return m_bSetup==false;}
			virtual void setSpeed(f32 speed);
			virtual u32 getTime() const{
				return m_uVirtualTime;
			}
			virtual u32 getRealTime();
			virtual void tick();
		};
	}
}
#endif//YON_COMPILE_WITH_WIN32
#endif