#include "CYonEngineWin32.h"
#include <stdio.h>

namespace yon{
namespace platform{

	CYonEngineWin32::CYonEngineWin32(const yon::SYonEngineParameters& params)
		:m_hWnd(NULL),m_bExternalWindow(false),m_videoDriver(NULL),m_params(params)
	{
		if(params.windowId==NULL)
		{
			//创建窗口
			initWindow(params);
		}
		else
		{
			//绑定到外部窗口
			m_hWnd = static_cast<HWND>(params.windowId);
			RECT r;
			GetWindowRect(m_hWnd, &r);
			m_params.windowSize.w = r.right - r.left;
			m_params.windowSize.h = r.bottom - r.top;
			m_bExternalWindow = true;
		}

		//初始化视频驱动器
		createDriver();
	}
	CYonEngineWin32::~CYonEngineWin32(){
		if(m_videoDriver!=NULL)
			m_videoDriver->drop();
		DestroyWindow(m_hWnd);
	}

	//
	//函数: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	//hWnd: handle to window 窗口句柄
	//message: message identifier  消息标识
	//wParam: first message parameter
	//lParam: second message parameter
	//目的: 处理主窗口的消息。
	//
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
		switch(uiMsg) {
		case WM_CREATE:
			printf("窗口创建成功\n");
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SIZE:
			return 0;
		}
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	bool CYonEngineWin32::initWindow(const yon::SYonEngineParameters& params){

		WNDCLASS wc;
		RECT wRect;
		HINSTANCE hInstance;
		const c16* szWindowClass=TEXT("CYonPlatformWin32");

		wRect.left = 0L;
		wRect.right = (long)params.windowSize.w;
		wRect.top = 0L;
		wRect.bottom = (long)params.windowSize.h;

		//用GetModuleHandle()函数获得当前程序实例句柄
		hInstance = GetModuleHandle(NULL);

		//使用|把多种窗口样式连接在一起 
		//窗口的样式   
		//CS_HREDRAW  表示当水平方向宽度发生变化时重绘整个窗口   
		//CS_VREDRAW  表示当垂直方向高度发生变化时重绘整个窗口   
		//CS_NOCLOSE  禁用系统菜单中的Close命令,既是没有关闭按钮   
		//CS_DBLCLKS  当用户双击鼠标时向窗口过程函数发送鼠标双击消息
		//通常，在您呼叫GetDC或BeginPaint时，Windows用默认值建立一个新的设备内容，
		//您对属性所做的一切改变在设备内容用 ReleaseDC或EndPaint呼叫释放时，都会丢失。
		//虽然在通常情况下这种方法已经很令人满意了，但是您还可能想要在释放设备内容之后，
		//仍然保存程序中对设备内容属性所做的改变，以便在下一次呼叫GetDC 和BeginPaint时
		//它们仍然能够起作用。为此，可在登录窗口类别时，将CS_OWNDC旗标纳入窗口类别的一部分：
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;			//这两个变量允许用户请求Windows内部提供额外的空间以便额外数据与窗口
		wc.cbWndExtra = 0;			//实例发生联系,通常不分配空间
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);		//窗口的图标,如果为NULL,系统将提供一个默认的图标
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;		//菜单,指定菜单资源的名字,NULL是表示没有菜单,
		wc.lpszClassName = szWindowClass;

		//注册窗口类
		RegisterClass(&wc);

		//该函数依据所需客户矩形大小，计算需要的窗口矩形的大小。计算出的窗口矩形随后可以传送给CreateWindowEx函数，用于创建一个客户区所需大小的窗口。 
		AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			szWindowClass,						//窗口类名称
			params.windowCaption.c_str(),		//窗口标题 
			WS_OVERLAPPEDWINDOW |				//窗口样式,多种样式
												//这个样式要与WNDCLASS的样式区别开,这个是指定某个具体窗口的样式   
												//而WNDCLASS的样式是指基于该窗口类的所有窗口都具有的样式   
												//WS_OVERLAPPED  一个可层叠窗口   
												//WS_CAPTION     有标题栏   
												//WS_SYSMENU     在标题栏带有系统菜单,WS_CAPTION一起使用   
												//WS_THICKFRAME  具有可调边框窗口   
												//WS_MINIMIZEBOX 有最小按钮,必须设定WS_SYSMENU   
												//WS_MAXIMIZEBOX 有最大按钮,必须设定WS_SYSMENU
												//WS_CLIPCHILDREN，使得父窗体在绘制时留出其上的子窗体的位置不去画它，而那片区域留着子窗体去画。
												//WS_CLIPSIBLING，必须用于子窗体，使得该子窗体在收到WM_PAINT时同时令其共父的诸多子窗体（MSDN讲必须不在该区域，即不与之重叠）也会被PAINT。
			WS_CLIPSIBLINGS | 
			WS_CLIPCHILDREN, 
			0, 0,								//x坐标与y坐标
			wRect.right-wRect.left,				//宽
			wRect.bottom-wRect.top,				//高
			NULL,								//父窗口
			NULL,								//加载菜单
			hInstance,							//窗口实例标记
			NULL								//窗口创建时传入的数据指针
												//多文档时必须指向CLIENTCREATESTRUCT
		);

		YON_DEBUG_BREAK_IF(!m_hWnd)

		if (!m_hWnd)
		{
			MessageBox(NULL,TEXT("Error in CreateWindow()!"),TEXT("Error"),MB_OK);
			return false;
		}

		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		return true;
	}

	void CYonEngineWin32::createDriver(){

#ifdef YON_VIDEO_MODE_OGLES1
	yon::video::ogles1::SOGLES1Parameters params(m_hWnd);
	m_videoDriver=new yon::video::ogles1::COGLES1Driver(params);
#endif //YON_VIDEO_MODE_OGLES1
	}
	//yon::ITimer* yon::platform::CYonEngineWin32::getTimer(){return NULL;}
}//namespace platform
}//namespace yon

