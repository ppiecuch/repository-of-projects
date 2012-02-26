#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "EVideoType.h"
#include "dimension2d.h"

namespace yon{
	struct SYonEngineParameters{

		SYonEngineParameters():
			videoType(video::ENUM_VIDEO_TYPE_OGLES1),
			windowSize(core::dimension2du(800,600))
			{}

		//视频模式
		video::ENUM_VIDEO_TYPE videoType;
		//窗口尺寸
		core::dimension2du windowSize;
	};
}
#endif