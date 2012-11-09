#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::event;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;
using namespace yon::scene::animator;
using namespace yon::audio;

#ifdef YON_COMPILE_WITH_WIN32
#pragma comment(lib, "Yon.lib")
#endif

//����ϵͳ��IMaterial����ΪSMaterial�����Զ�������飬������ԭ��Чģ��Ľ���
const ENUM_BLEND_FACTOR MATERIAL_BLEND_FACTORS[]={ENUM_BLEND_FACTOR_ZERO,ENUM_BLEND_FACTOR_ONE,ENUM_BLEND_FACTOR_DST_COLOR,
ENUM_BLEND_FACTOR_ONE_MINUS_DST_COLOR,ENUM_BLEND_FACTOR_SRC_COLOR,ENUM_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,ENUM_BLEND_FACTOR_SRC_ALPHA,
ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,ENUM_BLEND_FACTOR_DST_ALPHA,ENUM_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,ENUM_BLEND_FACTOR_SRC_ALPHA_SATURATE};


bool init(void *pJNIEnv,u32 width,u32 height);
void resize(u32 width,u32 height);
void drawFrame();
void destroy();
IYonEngine* getEngine();

#endif