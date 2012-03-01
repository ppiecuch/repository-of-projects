#include "CYonEngineWin32.h"
#include <stdio.h>

namespace yon{
namespace platform{

	CYonEngineWin32::CYonEngineWin32(const yon::SYonEngineParameters& params)
		:m_hWnd(NULL),m_bExternalWindow(false),m_videoDriver(NULL),m_params(params)
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

		//��ʼ����Ƶ������
		createDriver();
	}
	CYonEngineWin32::~CYonEngineWin32(){
		if(m_videoDriver!=NULL)
			m_videoDriver->drop();
		DestroyWindow(m_hWnd);
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
		switch(uiMsg) {
		case WM_CREATE:
			printf("���ڴ����ɹ�\n");
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

