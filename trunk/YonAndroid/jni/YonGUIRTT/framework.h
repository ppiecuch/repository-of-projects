#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include <MyGUI.h>
#include "MyGUIAdapter.h"
#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::event;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::platform;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;
using namespace yon::scene::animator;
using namespace yon::audio;

#ifdef YON_COMPILE_WITH_WIN32
#ifdef _DEBUG
#pragma comment(lib, "Yon_debug.lib")
#pragma comment(lib, "MyGUIAdapter.lib")
#else
#pragma comment(lib, "Yon_release.lib")
#pragma comment(lib, "MyGUIAdapter.lib")
#endif
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "MyGUIEngine.lib")
#endif


bool init(void *pJNIEnv,const c8* appPath,const c8* resPath,u32 width,u32 height);
void resize(u32 width,u32 height);
void drawFrame();
void destroy();
IYonEngine* getEngine();

#endif