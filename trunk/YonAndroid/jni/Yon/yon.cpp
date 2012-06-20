#include "yon.h"
#include "CLogger.h"
#include "CYonEngineWin32.h"
#include "CYonEngineAndroid.h"
#include "CMaterial.h"

namespace yon{

	

	namespace core
	{
		const matrix4f IDENTITY_MATRIX(true);

		const vector2df ORIGIN_VECTOR2DF(0,0);
		const vector2df IDENTITY_VECTOR2DF(1,1);
		const vector2di ORIGIN_VECTOR2DI(0,0);
		const vector2di IDENTITY_VECTOR2DI(1,1);

		const position2df ORIGIN_POSITION2DF(0,0);
		const position2di ORIGIN_POSITION2DI(0,0);

		const position3df ORIGIN_POSITION3DF(0,0,0);
		const position3di ORIGIN_POSITION3DI(0,0,0);

		const vector3df ORIGIN_VECTOR3DF(0,0,0);
		const vector3df UP_VECTOR3DF(0,1,0);
		const vector3df IDENTITY_VECTOR3DF(1,1,1);
	}

	namespace debug{
		ILogger* Logger=NULL;
	}

	namespace io{
		const path EMPTY_PATH;
	}

	namespace video{
		const SColor COLOR_BLACK(0xFF000000);
		const SColor COLOR_WHITE(0xFFFFFFFF);
		const SColor COLOR_RED(0xFF0000FF);
		const SColor COLOR_GREEN(0xFF00FF00);
		const SColor COLOR_BLUE(0xFFFF0000);

		IMaterial* DEFAULT_MATERIAL=NULL;

		IMaterial* MYGUI_MATERIAL=NULL;
	}

	using namespace debug;

	YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param){

		if(Logger==NULL){
			Logger=new debug::CLogger();
		}else{
			Logger->grab();
		}

		if(video::DEFAULT_MATERIAL==NULL){
			video::DEFAULT_MATERIAL=new video::CMaterial();
		}else{
			video::DEFAULT_MATERIAL->grab();
		}

		if(video::MYGUI_MATERIAL==NULL){
			video::MYGUI_MATERIAL=new video::CMaterial();
			video::MYGUI_MATERIAL->setMaterialType(video::ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
			video::MYGUI_MATERIAL->setFrontFace(video::ENUM_FRONT_FACE_CW);
			//TODO由于MyGUI引擎的freetype字体顶点序列是CCW的，而其UI界面顶点序列则是CW的，临时解决办法是关闭CullFace
			video::MYGUI_MATERIAL->setState(video::ENUM_MATERIAL_STATE_CULL_FACE,false);
		}else{
			video::MYGUI_MATERIAL->grab();
		}

		IYonEngine* engine;

#ifdef YON_COMPILE_WITH_WIN32
		engine=new platform::CYonEngineWin32(param);
#elif defined(YON_COMPILE_WITH_ANDROID)
		engine=new platform::CYonEngineAndroid(param);
#endif

		YON_DEBUG_BREAK_IF(engine==NULL)

		Logger->info(YON_LOG_SUCCEED_FORMAT,core::stringc("CreateEngine(%d,%d),version:%d.%d.%d",param.windowSize.w,param.windowSize.h,YON_VERSION_MAJOR,YON_VERSION_MINOR,YON_VERSION_PATCH).c_str());

		return engine;
	}
}