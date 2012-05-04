#include<windows.h>
#include <iostream>
#include <tchar.h>
#define MAX_STR 100

// ȫ�ֱ���:
HINSTANCE hInst;												// ��ǰʵ��
TCHAR szTitle[MAX_STR]       = TEXT("���ڲ���");				// �������ı�
TCHAR szWindowClass[MAX_STR] = TEXT("Console_Win Demo");		// ����������

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int _tmain(int argc, _TCHAR* argv[])
{
	int nCmdShow = SW_SHOW;//�ñ���ȡֵ�μ�MSDN
	HINSTANCE hInstance = NULL;
	//��GetModuleHandle()������õ�ǰ����ʵ�����
	hInstance = GetModuleHandle(NULL);
	std::cout << "hInstance: " << hInstance << std::endl;
	std::cout << "hInstance->unused: " << hInstance->unused << std::endl << std::endl;
	std::cout << "================================" << std::endl << std::endl;
	MSG msg;
	//ע�ᴰ����
	MyRegisterClass(hInstance);
	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		std::cout << "Error in InitInstance()!" << std::endl;
		MessageBox(NULL,TEXT("Error in InitInstance()!"),TEXT("Error"),MB_OK);
		return FALSE;
	}

	//��Ϣѭ��,WM_QIUT��ֹͣѭ��,������Ѿ��Ƴ�   
	/* 

	BOOL GetMessage( 
	LPMSG lpMsg,       //ָ��һ����Ϣ�ṹ��MSG�ṹ�����, 
	//���ڱ������Ϣ�����л�ȡ����Ϣ  
	HWND hWnd,         //ָ��ĳ�����ڵľ��,NULL��ָ��ȡ�����������Ϣ 
	UINT wMsgFilterMin, //��Ϣ��������ϢID����Сֵ  
	UINT wMsgFilterMax  //��Ϣ��������ϢID�����ֵ 
	//�����������������ָ����ϢID�ķ�Χ,�����Ϊ0,��ʾ��ȡ��Ϣ����������Ϣ 
	); 

	//��������ֵ��ΪTRUE,�����WM_QIUT����0,����ʱ����-1 
	//�˵��е�"�ļ�"�е�"�˳�",���ڵ�"X"�رհ�ť,������ϵͳ�˵���"�ر�"���� 
	//�����3��������ᷢ��һ��WM_QUIT��Ϣ 

	*/ 

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);						//��WindowsΪ�������ص���Ϣ��һЩת��
		DispatchMessage(&msg);						//������Ϣ�����ڹ��̺����ж���Ϣ����   
	}
	return (int) msg.wParam;
}
//
//   ����: MyRegisterClass()
//
//   Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style           = CS_HREDRAW | CS_VREDRAW;		//ʹ��|�Ѷ��ִ�����ʽ������һ�� 
														//���ڵ���ʽ   
														//CS_HREDRAW  ��ʾ��ˮƽ�����ȷ����仯ʱ�ػ���������   
														//CS_VREDRAW  ��ʾ����ֱ����߶ȷ����仯ʱ�ػ���������   
														//CS_NOCLOSE  ����ϵͳ�˵��е�Close����,����û�йرհ�ť   
														//CS_DBLCLKS  ���û�˫�����ʱ�򴰿ڹ��̺����������˫����Ϣ
	wcex.lpfnWndProc     = (WNDPROC)WndProc;			//���ں���(��Ϣ������),lpfnWndProc�Ǹ�����ָ�� 
	wcex.cbClsExtra     = 0;							//���������������û�����Windows�ڲ��ṩ����Ŀռ��Ա���������봰��   
	wcex.cbWndExtra     = 0;							//ʵ��������ϵ,ͨ��������ռ�   
	wcex.hInstance       = hInstance;
	wcex.hIcon    = LoadCursor(NULL, IDI_WINLOGO);		//���ڵ�ͼ��,���ΪNULL,ϵͳ���ṩһ��Ĭ�ϵ�ͼ��   
														//LoadIcon()����ͼ����Դ,����ͼ����,   
														//�������ϵͳ��׼��ͼ���һ����������ΪNULL,�ڶ�������ֵΪ   
														//IDI_ERROR        ����ͼ��   
														//IDI_APPLICATION  Ĭ��Ӧ�ó���ͼ��   
														//IDI_QUESTION     �ʺ�ͼ��    
														//IDI_EXCLAMATION  ��̾��ͼ��    
														//IDI_ASTERISK     �Ǻ�ͼ��   
														//IDI_WARNING      ����ͼ��   
														//IDI_WINLOGO      Windowsͼ��   
														//IDI_HAND         ��IDI_ERROR��ͬ   
														//IDI_INFORMATION  ��Ϣͼ��   
														//���кܶ�.....   
	wcex.hCursor   = LoadCursor(NULL, IDC_ARROW);		//���,LoadCursor()��ʹ����LoadIcon()��ͬ  
	wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;							//�˵�,ָ���˵���Դ������,NULL�Ǳ�ʾû�в˵�,  
	wcex.lpszClassName   = szWindowClass;				//��������
	wcex.hIconSm   = LoadCursor(NULL, IDI_WINLOGO);		//�ʹ����������Сͼ�ꡣ�����ֵΪNULL�����hIcon�е�ͼ��ת���ɴ�С���ʵ�Сͼ��

	//ע�ᴰ����,����Windowsϵͳ��������ƺ���
	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HANDLE, int)
//
//   Ŀ��: ����ʵ����������������ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(
		szWindowClass,			//����������
		szTitle,				//���ڱ��� 
		WS_OVERLAPPEDWINDOW,	//������ʽ,������ʽ
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
		CW_USEDEFAULT,			//x����,Ĭ��
								//CW_USEDEFAULT��������WS_OVERLAPPED��ʽ����  
		CW_USEDEFAULT,			//y����,Ĭ�� 
		CW_USEDEFAULT,			//��
		CW_USEDEFAULT,			//��
		NULL,					//������
		NULL,					//��һ�ּ��ز˵�
								//LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1)), //�ڶ��ּ��ز˵�   
								//hMenu, //��̬�˵�   
		hInstance,				//����ʵ�����
		NULL					//���ڴ���ʱ���������ָ��,   
								//���ĵ�ʱ����ָ��CLIENTCREATESTRUCT
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
//   ����: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//   hWnd: handle to window ���ھ��
//   message: message identifier  ��Ϣ��ʶ
//   wParam: first message parameter
//   lParam: second message parameter
//   Ŀ��: ���������ڵ���Ϣ��
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_CREATE:
		std::cout << "���ڴ����ɹ�" << std::endl;
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);	//��ȡ�豸�������
		EndPaint(hWnd, &ps);			//�ͷ���Դ
		break;
	case WM_DESTROY:
		PostQuitMessage(0);				//���̽���,��ȫ�˳�����
		std::cout << "Goodbye!" << std::endl << std::endl;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);		//����δ�������Ϣ 
	}
	return 0;
}

