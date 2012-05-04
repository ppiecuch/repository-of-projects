#include<windows.h>
#include <iostream>
#include <tchar.h>
#define MAX_STR 100

// 全局变量:
HINSTANCE hInst;												// 当前实例
TCHAR szTitle[MAX_STR]       = TEXT("窗口测试");				// 标题栏文本
TCHAR szWindowClass[MAX_STR] = TEXT("Console_Win Demo");		// 主窗口类名

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int _tmain(int argc, _TCHAR* argv[])
{
	int nCmdShow = SW_SHOW;//该变量取值参见MSDN
	HINSTANCE hInstance = NULL;
	//用GetModuleHandle()函数获得当前程序实例句柄
	hInstance = GetModuleHandle(NULL);
	std::cout << "hInstance: " << hInstance << std::endl;
	std::cout << "hInstance->unused: " << hInstance->unused << std::endl << std::endl;
	std::cout << "================================" << std::endl << std::endl;
	MSG msg;
	//注册窗口类
	MyRegisterClass(hInstance);
	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		std::cout << "Error in InitInstance()!" << std::endl;
		MessageBox(NULL,TEXT("Error in InitInstance()!"),TEXT("Error"),MB_OK);
		return FALSE;
	}

	//消息循环,WM_QIUT才停止循环,或程序已经推出   
	/* 

	BOOL GetMessage( 
	LPMSG lpMsg,       //指向一个消息结构体MSG结构体对象, 
	//用于保存从消息队列中获取的消息  
	HWND hWnd,         //指向某个窗口的句柄,NULL是指获取程序的所有消息 
	UINT wMsgFilterMin, //消息队列中消息ID的最小值  
	UINT wMsgFilterMax  //消息队列中消息ID的最大值 
	//最后两个参数适用于指定消息ID的范围,如果都为0,表示获取消息队列所有消息 
	); 

	//函数返回值总为TRUE,当获得WM_QIUT返回0,出错时返回-1 
	//菜单中的"文件"中的"退出",窗口的"X"关闭按钮,窗口上系统菜单的"关闭"命令 
	//上面的3中情况都会发送一个WM_QUIT消息 

	*/ 

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);						//让Windows为与键盘相关的消息做一些转换
		DispatchMessage(&msg);						//分派消息到窗口过程函数中对消息处理   
	}
	return (int) msg.wParam;
}
//
//   函数: MyRegisterClass()
//
//   目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style           = CS_HREDRAW | CS_VREDRAW;		//使用|把多种窗口样式连接在一起 
														//窗口的样式   
														//CS_HREDRAW  表示当水平方向宽度发生变化时重绘整个窗口   
														//CS_VREDRAW  表示当垂直方向高度发生变化时重绘整个窗口   
														//CS_NOCLOSE  禁用系统菜单中的Close命令,既是没有关闭按钮   
														//CS_DBLCLKS  当用户双击鼠标时向窗口过程函数发送鼠标双击消息
	wcex.lpfnWndProc     = (WNDPROC)WndProc;			//窗口函数(消息处理函数),lpfnWndProc是个函数指针 
	wcex.cbClsExtra     = 0;							//这两个变量允许用户请求Windows内部提供额外的空间以便额外数据与窗口   
	wcex.cbWndExtra     = 0;							//实例发生联系,通常不分配空间   
	wcex.hInstance       = hInstance;
	wcex.hIcon    = LoadCursor(NULL, IDI_WINLOGO);		//窗口的图标,如果为NULL,系统将提供一个默认的图标   
														//LoadIcon()加载图标资源,返回图标句柄,   
														//如果加载系统标准的图标第一个参数必须为NULL,第二个参数值为   
														//IDI_ERROR        错误图标   
														//IDI_APPLICATION  默认应用程序图标   
														//IDI_QUESTION     问号图标    
														//IDI_EXCLAMATION  感叹号图标    
														//IDI_ASTERISK     星号图标   
														//IDI_WARNING      警告图标   
														//IDI_WINLOGO      Windows图标   
														//IDI_HAND         与IDI_ERROR相同   
														//IDI_INFORMATION  消息图标   
														//还有很多.....   
	wcex.hCursor   = LoadCursor(NULL, IDC_ARROW);		//光标,LoadCursor()的使用与LoadIcon()相同  
	wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;							//菜单,指定菜单资源的名字,NULL是表示没有菜单,  
	wcex.lpszClassName   = szWindowClass;				//窗口类名
	wcex.hIconSm   = LoadCursor(NULL, IDI_WINLOGO);		//和窗口类关联的小图标。如果该值为NULL。则把hIcon中的图标转换成大小合适的小图标

	//注册窗口类,告诉Windows系统窗口类设计好了
	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HANDLE, int)
//
//   目的: 保存实例句柄并创建主窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(
		szWindowClass,			//窗口类名称
		szTitle,				//窗口标题 
		WS_OVERLAPPEDWINDOW,	//窗口样式,多种样式
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
		CW_USEDEFAULT,			//x坐标,默认
								//CW_USEDEFAULT仅适用于WS_OVERLAPPED样式窗口  
		CW_USEDEFAULT,			//y坐标,默认 
		CW_USEDEFAULT,			//宽
		CW_USEDEFAULT,			//高
		NULL,					//父窗口
		NULL,					//第一种加载菜单
								//LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1)), //第二种加载菜单   
								//hMenu, //动态菜单   
		hInstance,				//窗口实例标记
		NULL					//窗口创建时传入的数据指针,   
								//多文档时必须指向CLIENTCREATESTRUCT
		);

	if (!hWnd)
	{
		MessageBox(NULL,TEXT("Error in CreateWindow()!"),TEXT("Error"),MB_OK);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//   函数: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//   hWnd: handle to window 窗口句柄
//   message: message identifier  消息标识
//   wParam: first message parameter
//   lParam: second message parameter
//   目的: 处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_CREATE:
		std::cout << "窗口创建成功" << std::endl;
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);	//获取设备环境句柄
		EndPaint(hWnd, &ps);			//释放资源
		break;
	case WM_DESTROY:
		PostQuitMessage(0);				//进程结束,完全退出程序
		std::cout << "Goodbye!" << std::endl << std::endl;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);		//处理未处理的消息 
	}
	return 0;
}

