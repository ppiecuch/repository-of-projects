#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_WIN32

#include "CYonEngineWin32.h"
#include "CGraphicsAdapter.h"

#include "ILogger.h"

namespace yon{
namespace platform{

	struct SEnginePair
	{
		HWND hWnd;
		CYonEngineWin32* engine;
	};
	core::list<SEnginePair> EngineMap;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

	CYonEngineWin32* getEngineByHWnd(HWND hWnd)
	{
		core::list<SEnginePair>::Iterator it = EngineMap.begin();
		for (; it!= EngineMap.end(); ++it)
			if ((*it).hWnd == hWnd)
				return (*it).engine;

		return NULL;
	}
	void eraseEngineByHWnd(HWND hWnd)
	{
		core::list<SEnginePair>::Iterator it = EngineMap.begin();
		for (; it!= EngineMap.end(); ++it)
		{
			if ((*it).hWnd == hWnd)
			{
				EngineMap.erase(it);
				break;
			}
		}
	}

	const c16* szWindowClass=TEXT("CYonPlatformWin32");
	
	CYonEngineWin32::CYonEngineWin32(const yon::SYonEngineParameters& params)
		:m_hWnd(NULL),m_bExternalWindow(false),
		m_pVideoDriver(NULL),m_pSceneManager(NULL),
		m_pGUIEnvirenment(NULL),m_pAudioDriver(NULL),
		m_pGraphicsAdapter(NULL),m_pFileSystem(NULL),
		m_pUserListener(params.pEventReceiver),m_pTimer(NULL),
		m_params(params),m_bClose(false),m_bResized(false)
	{
		if(params.windowId==NULL)
		{
			//��������
			initWindow(params);
		}
		else
		{
			//�󶨵��ⲿ����
			m_hWnd = static_cast<HWND>(params.windowId);
			RECT r;
			GetWindowRect(m_hWnd, &r);
			m_params.windowSize.w = r.right - r.left;
			m_params.windowSize.h = r.bottom - r.top;
			m_bExternalWindow = true;
		}

		//��ʼ����ʱ��
		m_pTimer=createTimer();

		//��ʼ���ļ�ϵͳ
		m_pFileSystem=io::createFileSystem();

		//��ʼ������������
		m_pSceneManager=scene::createSceneManager();

		//��ʼ����Ƶ������
		createDriver();

		//��ʼ��GUI����
		m_pGUIEnvirenment=gui::createGUIEnvirenment(m_pFileSystem,m_pVideoDriver,m_pTimer,m_pSceneManager->getGeometryFactory());

		//��ʼ��Graphics������
		m_pGraphicsAdapter=scene::createGraphicsAdapter(m_pVideoDriver,m_pSceneManager);

		//��ʼ������������
		m_pAudioDriver=audio::createAudioDriver(m_pFileSystem);

		SEnginePair ep;
		ep.hWnd=m_hWnd;
		ep.engine=this;
		EngineMap.push_back(ep);
		Logger->debug("EngineMap.push_back(ep)\n");
		//ϵͳWM_SIZE��Ϣ�����ڽ�������push��EngineMap֮ǰ�Ѿ�������,Ϊ��ȷ��������,�����ٷ���һ��WM_SIZE
		PostMessage(m_hWnd,WM_SIZE,0,0);

		SetActiveWindow(m_hWnd);
		SetForegroundWindow(m_hWnd);

		//������ʱ��
		m_pTimer->start();
	}
	CYonEngineWin32::~CYonEngineWin32(){
		eraseEngineByHWnd(m_hWnd);
		m_pAudioDriver->drop();
		m_pGraphicsAdapter->drop();
		m_pGUIEnvirenment->drop();
		m_pVideoDriver->drop();
		m_pSceneManager->drop();
		m_pFileSystem->drop();
		m_pTimer->drop();
		if(m_bExternalWindow==false){
			DestroyWindow(m_hWnd);
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy Window");
		}
		if(UnregisterClass(szWindowClass, GetModuleHandle(NULL)))
			Logger->info(YON_LOG_SUCCEED_FORMAT,"UnregisterClass");
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy CYonEngineWin32");
		if(video::DEFAULT_MATERIAL->drop()){
			video::DEFAULT_MATERIAL=NULL;
		}
		//if(video::DEFAULT_3D_MATERIAL->drop()){
		//	video::DEFAULT_3D_MATERIAL=NULL;
		//}
		//if(video::DEFAULT_2D_MATERIAL->drop()){
		//	video::DEFAULT_2D_MATERIAL=NULL;
		//}
		if(video::MYGUI_MATERIAL->drop()){
			video::MYGUI_MATERIAL=NULL;
		}
		if(Logger->drop()){
			Logger=NULL;
		}
			
	}

	void CYonEngineWin32::onResize(u32 w,u32 h){
		m_bResized=true;
	}

	bool CYonEngineWin32::run(){
		m_pTimer->tick();
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/*if(msg.message == WM_QUIT)
			{
				m_bClose=true;
			}
			else 
			{
				//TranslateMessage(&msg);
				DispatchMessage(&msg);
			}*/
				
			/*if(msg.message==WM_DESTROY){
				Logger->debug("PeekMessage WM_DESTROY\n");
			}else if(msg.message==WM_QUIT){
				Logger->debug("PeekMessage WM_QUIT\n");
			}*/
			if (m_bExternalWindow && msg.hwnd == m_hWnd)
				WndProc(m_hWnd, msg.message, msg.wParam, msg.lParam);
			else
				DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{
				m_bClose=true;
			}
		}
		if(!m_bClose)
			resizeIfNecessary();
		//Sleep(20);
		return !m_bClose;
	}

	void CYonEngineWin32::resizeIfNecessary()
	{
		if (!m_bResized)
			return;

		RECT r;
		GetClientRect(m_hWnd, &r);

		core::dimension2du newsize((u32)r.right, (u32)r.bottom);
		m_pVideoDriver->onResize(newsize);
		m_pSceneManager->onResize(newsize);
		m_pGUIEnvirenment->onResize(newsize);
		m_bResized = false;
	}

	bool CYonEngineWin32::postEventFromUser(const event::SEvent& event){
		bool absorbed = false;

		if (m_pUserListener)
			absorbed = m_pUserListener->onEvent(event);

		//TODO GUI
		//if (!absorbed && GUIEnvironment)
		//	absorbed = GUIEnvironment->postEventFromUser(event);

		if (!absorbed && m_pSceneManager)
			absorbed = m_pSceneManager->postEventFromUser(event);

		return absorbed;
	}
	//
	//����: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	//hWnd: handle to window ���ھ��
	//message: message identifier  ��Ϣ��ʶ
	//wParam: first message parameter
	//lParam: second message parameter
	//Ŀ��: ���������ڵ���Ϣ��
	//
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {

		event::SEvent evt;

		static s32 clickCount=0;
		if (GetCapture() != hWnd && clickCount > 0)
			clickCount = 0;

		struct MessageMap
		{
			s32 group;
			UINT winMessage;
			s32 xcMessage;
		};

		static MessageMap mouseMap[] =
		{
			{0, WM_LBUTTONDOWN, event::ENUM_MOUSE_INPUT_TYPE_LDOWN},
			{1, WM_LBUTTONUP,   event::ENUM_MOUSE_INPUT_TYPE_LUP},
			{0, WM_RBUTTONDOWN, event::ENUM_MOUSE_INPUT_TYPE_RDOWN},
			{1, WM_RBUTTONUP,   event::ENUM_MOUSE_INPUT_TYPE_RUP},
			{0, WM_MBUTTONDOWN, event::ENUM_MOUSE_INPUT_TYPE_MDOWN},
			{1, WM_MBUTTONUP,   event::ENUM_MOUSE_INPUT_TYPE_MUP},
			{2, WM_MOUSEMOVE,   event::ENUM_MOUSE_INPUT_TYPE_MOVED},
			{3, WM_MOUSEWHEEL,  event::ENUM_MOUSE_INPUT_TYPE_WHEEL},
			{-1, 0, 0}
		};

		MessageMap * m = mouseMap;
		while ( m->group >=0 && m->winMessage != uiMsg )
			m += 1;
		if ( m->group >= 0 )
		{
			if ( m->group == 0 )	// down
			{
				clickCount++;
				SetCapture(hWnd);
			}
			else
			{
				if ( m->group == 1 )	// up
				{
					clickCount--;
					if (clickCount<1)
					{
						clickCount=0;
						ReleaseCapture();
					}
				}
			}
			evt.type=event::ENUM_EVENT_TYPE_MOUSE;
			evt.mouseInput.type = (event::ENUM_MOUSE_INPUT_TYPE) m->xcMessage;
			evt.mouseInput.x = (short)LOWORD(lParam);
			evt.mouseInput.y = (short)HIWORD(lParam);
			evt.mouseInput.buttonMasks = wParam & ( MK_LBUTTON | MK_RBUTTON | MK_MBUTTON);
			evt.mouseInput.wheel = 0.f;

			// wheel
			if ( m->group == 3 )
			{
				POINT p;
				p.x = 0; p.y = 0;
				ClientToScreen(hWnd, &p);
				evt.mouseInput.x -= p.x;
				evt.mouseInput.y -= p.y;
				evt.mouseInput.wheel = ((f32)((short)HIWORD(wParam))) / (f32)WHEEL_DELTA;
			}

			IYonEngine* engine = getEngineByHWnd(hWnd);
			if (engine)
			{
				engine->postEventFromUser(evt);

				//TODO ˫���¼�/�����¼�
			}
			return 0;
		}


		CYonEngineWin32* engine=NULL;
		switch(uiMsg) {
		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_ERASEBKGND:
			return 0;
		case WM_CREATE:
			Logger->debug("WM_CREATE\n");
			break;
		case WM_DESTROY:
			Logger->debug("WM_DESTROY\n");
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
			return 0;
		case WM_KEYDOWN:
		case WM_KEYUP:
			return 0;
		case WM_SIZE:
			Logger->debug("WM_SIZE\n");
			engine=getEngineByHWnd(hWnd);
			if(engine){
				engine->onResize(0,0);
			}else{
				Logger->warn(YON_LOG_WARN_FORMAT,"getEngineByHWnd==NULL");
			}
			return 0;
		}
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	bool CYonEngineWin32::initWindow(const yon::SYonEngineParameters& params){

		WNDCLASS wc;
		RECT wRect;
		HINSTANCE hInstance;
		

		wRect.left = 0L;
		wRect.right = (long)params.windowSize.w;
		wRect.top = 0L;
		wRect.bottom = (long)params.windowSize.h;

		//��GetModuleHandle()������õ�ǰ����ʵ�����
		hInstance = GetModuleHandle(NULL);

		//ʹ��|�Ѷ��ִ�����ʽ������һ�� 
		//���ڵ���ʽ   
		//CS_HREDRAW  ��ʾ��ˮƽ�����ȷ����仯ʱ�ػ���������   
		//CS_VREDRAW  ��ʾ����ֱ����߶ȷ����仯ʱ�ػ���������   
		//CS_NOCLOSE  ����ϵͳ�˵��е�Close����,����û�йرհ�ť   
		//CS_DBLCLKS  ���û�˫�����ʱ�򴰿ڹ��̺����������˫����Ϣ
		//ͨ������������GetDC��BeginPaintʱ��Windows��Ĭ��ֵ����һ���µ��豸���ݣ�
		//��������������һ�иı����豸������ ReleaseDC��EndPaint�����ͷ�ʱ�����ᶪʧ��
		//��Ȼ��ͨ����������ַ����Ѿ������������ˣ���������������Ҫ���ͷ��豸����֮��
		//��Ȼ��������ж��豸�������������ĸı䣬�Ա�����һ�κ���GetDC ��BeginPaintʱ
		//������Ȼ�ܹ������á�Ϊ�ˣ����ڵ�¼�������ʱ����CS_OWNDC������봰������һ���֣�
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;			//���������������û�����Windows�ڲ��ṩ����Ŀռ��Ա���������봰��
		wc.cbWndExtra = 0;			//ʵ��������ϵ,ͨ��������ռ�
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);		//���ڵ�ͼ��,���ΪNULL,ϵͳ���ṩһ��Ĭ�ϵ�ͼ��
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;		//�˵�,ָ���˵���Դ������,NULL�Ǳ�ʾû�в˵�,
		wc.lpszClassName = szWindowClass;

		//ע�ᴰ����
		RegisterClass(&wc);

		//�ú�����������ͻ����δ�С��������Ҫ�Ĵ��ھ��εĴ�С��������Ĵ��ھ��������Դ��͸�CreateWindowEx���������ڴ���һ���ͻ��������С�Ĵ��ڡ� 
		AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			szWindowClass,						//����������
			params.windowCaption.c_str(),		//���ڱ��� 
			WS_OVERLAPPEDWINDOW |				//������ʽ,������ʽ
												//�����ʽҪ��WNDCLASS����ʽ����,�����ָ��ĳ�����崰�ڵ���ʽ   
												//��WNDCLASS����ʽ��ָ���ڸô���������д��ڶ����е���ʽ   
												//WS_OVERLAPPED  һ���ɲ������   
												//WS_CAPTION     �б�����   
												//WS_SYSMENU     �ڱ���������ϵͳ�˵�,WS_CAPTIONһ��ʹ��   
												//WS_THICKFRAME  ���пɵ��߿򴰿�   
												//WS_MINIMIZEBOX ����С��ť,�����趨WS_SYSMENU   
												//WS_MAXIMIZEBOX �����ť,�����趨WS_SYSMENU
												//WS_CLIPCHILDREN��ʹ�ø������ڻ���ʱ�������ϵ��Ӵ����λ�ò�ȥ����������Ƭ���������Ӵ���ȥ����
												//WS_CLIPSIBLING�����������Ӵ��壬ʹ�ø��Ӵ������յ�WM_PAINTʱͬʱ���乲��������Ӵ��壨MSDN�����벻�ڸ����򣬼�����֮�ص���Ҳ�ᱻPAINT��
			WS_CLIPSIBLINGS | 
			WS_CLIPCHILDREN, 
			0, 0,								//x������y����
			wRect.right-wRect.left,				//��
			wRect.bottom-wRect.top,				//��
			NULL,								//������
			NULL,								//���ز˵�
			hInstance,							//����ʵ�����
			NULL								//���ڴ���ʱ���������ָ��
												//���ĵ�ʱ����ָ��CLIENTCREATESTRUCT
		);

		YON_DEBUG_BREAK_IF(!m_hWnd)

		if (!m_hWnd)
		{
			Logger->info(YON_LOG_FAILED_FORMAT,"Create window");
			return false;
		}

		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		return true;
	}

	void CYonEngineWin32::createDriver(){

#ifdef YON_VIDEO_MODE_OGLES1
			video::ogles1::SOGLES1Parameters params(m_hWnd,m_params.windowSize,m_params.fpsLimit);
			m_pVideoDriver=new video::ogles1::COGLES1Driver(params,m_pFileSystem,m_pTimer,m_pSceneManager->getGeometryFactory());
#endif //YON_VIDEO_MODE_OGLES1
	}
	//yon::ITimer* yon::platform::CYonEngineWin32::getTimer(){return NULL;}
}//namespace platform
}//namespace yon
#endif
