#ifndef _YON_VIDEO_CFPSCOUNTER_H_
#define _YON_VIDEO_CFPSCOUNTER_H_

#include "yonTypes.h"

namespace yon{
	namespace video{
		
		class CFPSCounter{
		protected:
			u32 m_fps;
			u32 m_lastTime;
			u32 m_framesCounted;

			u32 m_primitiveCounted;
			u32 m_primitiveAverage;
		public:
			CFPSCounter();
			u32 getFPS() const;
			u32 getPrimitiveAverage() const{return m_primitiveAverage;}
			void registerFrame(u32 now, u32 primitive);
		};
	}
}
#endif