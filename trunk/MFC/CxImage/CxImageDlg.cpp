// CxImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CxImage.h"
#include "CxImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCxImageDlg 对话框




CCxImageDlg::CCxImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCxImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCxImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, picture);
}

BEGIN_MESSAGE_MAP(CCxImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCxImageDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCxImageDlg 消息处理程序

BOOL CCxImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	setlocale(LC_ALL, "chs");
	//SetWindowPos(NULL,左,右,宽,高,0)
	picture.SetWindowPos(NULL,10,10,PICTURE_WIDTH,PICTURE_HEIGHT,0);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCxImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCxImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCxImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCxImageDlg::OnBnClickedButton1()
{
	CFileDialog dlg(true);
	if(IDOK != dlg.DoModal()) 
		return;

	CString& fileName=dlg.GetPathName();
	TRACE(TEXT("path:%s\r\n"),fileName);

	CString fileExt; //图片的扩展名 
	int len = fileName.GetLength(); 
	for(int i=len-1; i>=0; i--) //得到图片的扩展名 
	{ 
		if(fileName[i] == '.') 
		{ 
			fileExt=fileName.Mid(i+1); 
			break; 
		} 
	} 
	fileExt.MakeLower(); //将扩展名转为小写 
	TRACE(TEXT("ext:%s\r\n"),fileExt);

	if(fileExt != _T("")) 
	{ 
		//创建CxImage对象，其中静态方法CxImage::GetTypeIdFromName用来根据扩展名获得图片格式的ID代表 
		CxImage image; 
		uint32_t t=CxImage::GetTypeIdFromName(fileExt);
		TRACE("type:%d\r\n",t);
		image.Load(fileName,t);

		if(image.IsValid()) 
		{ 
			CRect rect;
			//picture.GetWindowRect(&rect);

			float rectRatio=(float)PICTURE_WIDTH/PICTURE_HEIGHT;
			float imageRatio=(float)image.GetWidth()/image.GetHeight();
			int width,height;
			if(imageRatio>rectRatio)
			{
				width=PICTURE_WIDTH;
				height=width/imageRatio;
			}
			else
			{
				height=PICTURE_HEIGHT;
				width=height*imageRatio;
			}
			image.Resample(width,height);

			TRACE("valid\r\n");
			CDC* hDC=picture.GetDC();
			HBITMAP bitmap=image.MakeBitmap(hDC->m_hDC);
			HBITMAP hOldBmp=picture.SetBitmap(bitmap);
			if(hOldBmp)DeleteObject(hOldBmp);
			if(hDC->m_hDC)ReleaseDC(hDC);
			//image.Destroy(); 
		} 
	} 


}
