#include "yoncore.h"
#include "CLogger.h"
#include "CHookWin32.h"

namespace yon{

	namespace core
	{
		const matrix4f IDENTITY_MATRIX(true);
		f32 tmp[16]={1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1};
		const matrix4f SWAP_MATRIX(tmp);

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

		const dimension2du ZERO_DIMENSION2DF(0,0);
		const dimension2du ZERO_DIMENSION2DU(0,0);
		const dimension2du ZERO_DIMENSION2DI(0,0);

		const recti ZERO_RECTI(0,0,0,0);

		template<class T, typename TAlloc>
		TAlloc string<T,TAlloc>::allocator;

		s32 randomizer::seed=0x0f0f0f0f;
	}

	namespace debug{
		ILogger* Logger=NULL;
	}

	using namespace debug;

#if defined(YON_COMPILE_WITH_WIN32)

	extern "C" int APIENTRY DllMain( HINSTANCE hModule,DWORD ul_reason_for_call,LPVOID lpReserved )
	{
		// Remove this if you use lpReserved
		UNREFERENCED_PARAMETER(lpReserved);

		//static AFX_EXTENSION_MODULE MousehookDLL = { NULL, NULL };

		if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		{
			//TRACE0("MOUSEHOOK.DLL Initializing!\n");

			// Extension DLL one-time initialization
			//if (!AfxInitExtensionModule(MousehookDLL, hInstance))
			//	return 0;

			// Insert this DLL into the resource chain
			// NOTE: If this Extension DLL is being implicitly linked to by
			//  an MFC Regular DLL (such as an ActiveX Control)
			//  instead of an MFC application, then you will want to
			//  remove this line from DllMain and put it in a separate
			//  function exported from this Extension DLL.  The Regular DLL
			//  that uses this Extension DLL should then explicitly call that
			//  function to initialize this Extension DLL.  Otherwise,
			//  the CDynLinkLibrary object will not be attached to the
			//  Regular DLL's resource chain, and serious problems will
			//  result.

			//new CDynLinkLibrary(MousehookDLL);
			platform::g_hInstance=hModule; //²åÈë±£´æDLLÊµÀý¾ä±ú
		}
		else if (ul_reason_for_call == DLL_PROCESS_DETACH)
		{
			//TRACE0("MOUSEHOOK.DLL Terminating!\n");
			// Terminate the library before destructors are called
			//AfxTermExtensionModule(MousehookDLL);
		}
		return TRUE;
	}

#endif

	void initYon(){
		if(Logger==NULL){
			io::path home;
#ifdef YON_COMPILE_WITH_WIN32
			home=".";
#elif defined(YON_COMPILE_WITH_ANDROID)
			home="/sdcard/";
#elif defined (YON_COMPILE_WITH_MACOSX)
#error no support maxosx
#endif
			Logger=new debug::CLogger(home);
		}else{
			Logger->grab();
		}
	}
	void destroyYon(){
		if(Logger!=NULL)
		{
			Logger->drop();
			Logger=NULL;
		}
	}

}