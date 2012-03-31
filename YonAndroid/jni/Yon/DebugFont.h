#ifndef _YON_DEBUG_DEBUGFONT_H_
#define _YON_DEBUG_DEBUGFONT_H_

#include "yonString.h"
#include "position2d.h"
#include "rect.h"

namespace yon{
	namespace video{
		class IVideoDriver;
		class ITexture;
		class IImage;
		namespace ogles1{
			class COGLES1Driver;
		}
	}
	namespace debug{

		class DebugFont{
		private:
			const static u32 CHAR_COUNT_PER_ROW=16;
			const static u32 ROW_COUNT=8;
			const static u32 CHAR_SIZE=8;
			const static u32 DATA_SIZE=ROW_COUNT*CHAR_COUNT_PER_ROW*3;
			video::IVideoDriver* m_pDriver;
			video::ITexture* m_pTexture;
			static u8 data[];
			const static c8 chars[ROW_COUNT][CHAR_COUNT_PER_ROW];
			core::recti texcoords[ROW_COUNT][CHAR_COUNT_PER_ROW];
			DebugFont();
			DebugFont(const DebugFont&){}
			DebugFont& operator = (const DebugFont&){}

			video::IImage* createImage();

			friend class video::ogles1::COGLES1Driver;
		public:
			~DebugFont();
			static DebugFont& getInstance();
			void drawString(const core::stringc& str,const core::position2di& pos);
		};
	}
}
#endif