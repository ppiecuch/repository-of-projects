#include<windows.h>
#include <iostream>
#include <map>
#include <tchar.h>
#define MAX_STR 100

// ȫ�ֱ���:
HINSTANCE hInst;												// ��ǰʵ��
TCHAR szTitle[MAX_STR]       = TEXT("���ڲ���");				// �������ı�
TCHAR szWindowClass[MAX_STR] = TEXT("Console_Win Demo");		// ����������
HMENU hMenu;													// ���洰���в˵����ľ�������˵����˵���������CreateMenu����������
struct SOption{
	wchar_t*	Name;
	int			Width;
	int			Height;
};
unsigned int wndID=0;

SOption options[]={
	{TEXT("800*480"),800,480},
	{TEXT("960*540"),960,540}
};
int optionCount=2;
unsigned int optionIDMin,optionIDMax;
std::map<unsigned int,SOption> optionMap;

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

//����һ����Win32������4���������˵�
//http://blog.csdn.net/tcjiaan/article/details/8510260
void CreateMyMenu(SOption options[],int size)  
{  
	hMenu = CreateMenu();  
	if(!hMenu)  
		return;  


#if 1
	HMENU net = CreatePopupMenu();
	AppendMenu(hMenu,  MF_POPUP,  (UINT_PTR)net,  L"�����ٶ�");
	wchar_t* speeds[]={L"����",L"һ��",L"Ƿ��"};
	for(int i=0;i<3;++i)
	{
		MENUITEMINFO mif;  
		mif.cbSize = sizeof(MENUITEMINFO);  
		mif.cch = 100;  
		mif.dwItemData  = NULL;  
		mif.dwTypeData = speeds[i];  
		mif.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;  
		mif.fState = MFS_ENABLED;  
		mif.fType = MIIM_STRING;  
		mif.wID = wndID++;  
		InsertMenuItem(net,mif.wID,FALSE,&mif);  
	}
#endif

#if 1
	HMENU pop;
	if(size>0)
	{
		pop = CreatePopupMenu();  
		AppendMenu(hMenu,  MF_POPUP,  (UINT_PTR)pop,  L"�ֱ���");  
	}
	optionIDMin=65535;
	optionIDMax=0;
	for(int i=0;i<size;++i)
	{
		
		MENUITEMINFO mif;  
		mif.cbSize = sizeof(MENUITEMINFO);  
		mif.cch = 100;  
		mif.dwItemData  = NULL;  
		mif.dwTypeData = options[i].Name;  
		mif.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;  
		mif.fState = MFS_ENABLED;  
		mif.fType = MIIM_STRING;  
		mif.wID = wndID++;  
		InsertMenuItem(pop,mif.wID,FALSE,&mif);  

		if(mif.wID<optionIDMin)
			optionIDMin=mif.wID;
		if(mif.wID>optionIDMax)
			optionIDMax=mif.wID;

		optionMap[mif.wID]=options[i];
	}
#else
#define IDM_OPT1     301  
#define IDM_OPT2     302 
	HMENU pop1 = CreatePopupMenu();  
	AppendMenu(hMenu,  
		MF_POPUP,  
		(UINT_PTR)pop1,  
		L"����");  
	// һ�ַ�����ʹ��AppendMenu����  
	AppendMenu(pop1,  
		MF_STRING,  
		IDM_OPT1,  
		L"�ɻ�");  

	// ��һ�ַ�����ʹ��InsertMenuItem����  
	MENUITEMINFO mif;  
	mif.cbSize = sizeof(MENUITEMINFO);  
	mif.cch = 100;  
	mif.dwItemData  = NULL;  
	mif.dwTypeData = L"����ǹ";  
	mif.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;  
	mif.fState = MFS_ENABLED;  
	mif.fType = MIIM_STRING;  
	mif.wID = IDM_OPT2;  

	InsertMenuItem(pop1,IDM_OPT2,FALSE,&mif);  
#endif



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

	CreateMyMenu(options,optionCount);

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
		hMenu,					//��һ�ּ��ز˵�
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

//����һ����Win32������5�������е�ѡ��ǵĲ˵�
//http://blog.csdn.net/tcjiaan/article/details/8513297

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
		{
			unsigned short id=LOWORD(wParam);
			std::map<unsigned int,SOption>::const_iterator it=optionMap.find(id);
			if(it!=optionMap.end())
			{
				//��ȡ����ߴ�
				RECT rect;
				GetWindowRect(hWnd,&rect);  
				rect.right=rect.left+it->second.Width;
				rect.bottom=rect.top+it->second.Height;
				//���ô���λ��
				SetWindowPos(hWnd,HWND_TOP,rect.left,rect.top,rect.right,rect.bottom,SWP_SHOWWINDOW);

				// ��ȡ�����ϵ������˵����ľ��  
				HMENU hmm = GetMenu(hWnd);  
				// ��ȡ�ڶ��������˵� 
				HMENU hfmn = GetSubMenu(hmm, 1);
				CheckMenuRadioItem(hfmn, optionIDMin, optionIDMax, id, MF_BYCOMMAND);
			}
		}
#if 0
		switch(LOWORD(wParam))  
		{  
		case IDM_OPT1:  
			MessageBox(hWnd,L"IDM_OPT1��",L"��ʾ",MB_OK);  
			break;  
		case IDM_OPT2:  
			MessageBox(hWnd,L"IDM_OPT2��",L"��ʾ",MB_OK);  
			break;  
		}
#endif
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

