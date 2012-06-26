#include "CFPSCounter.h"
#include "yonMath.h"

#include "ILogger.h"

namespace yon{
	namespace video{

		CFPSCounter::CFPSCounter()
			:m_fps(60),m_framesCounted(0),m_lastTime(0),
			m_primitiveCounted(0),m_primitiveAverage(0){}

		void CFPSCounter::registerFrame(u32 now, u32 primitive){
			++m_framesCounted;
			m_primitiveCounted+=primitive;
			const u32 milliseconds = now - m_lastTime;
			if(milliseconds>=1500){
				const f32 invMilli = core::reciprocal((f32)milliseconds );
				m_fps = core::ceil32(( 1000 * m_framesCounted ) * invMilli);
				m_framesCounted=0;
				m_lastTime=now;

				m_primitiveAverage=core::ceil32 ( ( 1000 * m_primitiveCounted ) * invMilli );
				m_primitiveCounted=0;
				//Logger->debug("recal fps\n");
			}
		}

		u32 CFPSCounter::getFPS() const{
			return m_fps;
		}
	}
}