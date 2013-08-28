// ColorSelectorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorSelector.h"
#include "ColorSelectorDlg.h"

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


// CColorSelectorDlg 对话框




CColorSelectorDlg::CColorSelectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorSelectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorSelectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_e);
	DDX_Control(pDX, IDC_BUTTON3, m_b);
	DDX_Control(pDX, IDC_STATIC1, m_s);
}

BEGIN_MESSAGE_MAP(CColorSelectorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CColorSelectorDlg::OnBnClickedButton1)
	ON_EN_SETFOCUS(IDC_EDIT3, &CColorSelectorDlg::OnEnSetfocusEdit3)
	ON_BN_CLICKED(IDC_STATIC1, &CColorSelectorDlg::OnClickStatic)
END_MESSAGE_MAP()


// CColorSelectorDlg 消息处理程序

BOOL CColorSelectorDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CColorSelectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorSelectorDlg::OnPaint()
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
HCURSOR CColorSelectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CColorSelectorDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg; 
	//打开颜色对话框,获取选定的颜色 
	COLORREF m_curCol;
	if(dlg.DoModal()==IDOK) 
		m_curCol=dlg.GetColor();  
	TRACE("%08x\r\n",m_curCol);
	//m_curID = ID_COLOR_CUSTOM; //设置当前选择的颜色ID    

	CRect rect;
	CDC *dc; 
	CBrush *pOldBrush;
	CBrush cBrush;

	cBrush.CreateSolidBrush(m_curCol);  

	//m_e是一个Edit Controll
	m_b.GetClientRect(&rect);
	dc = m_b.GetDC();
	pOldBrush = dc->SelectObject(&cBrush);
	dc->FillRect(rect, &cBrush);   
	dc->SelectObject(pOldBrush);

	UpdateData(false);


}

void CColorSelectorDlg::OnEnSetfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	TRACE("focus\r\n");
}
void CColorSelectorDlg::OnClickStatic()
{
	// TODO: 在此添加控件通知处理程序代码
	TRACE("focus\r\n");

	CColorDialog dlg; 
	//打开颜色对话框,获取选定的颜色 
	COLORREF m_curCol;
	if(dlg.DoModal()==IDOK) 
	{
		m_curCol=dlg.GetColor();  
		CRect rect;
		CDC *dc; 
		CBrush *pOldBrush;
		CBrush cBrush;

		cBrush.CreateSolidBrush(m_curCol);  

		//m_e是一个Edit Controll
		m_s.GetClientRect(&rect);
		dc = m_s.GetDC();
		pOldBrush = dc->SelectObject(&cBrush);
		dc->FillRect(rect, &cBrush);   
		dc->SelectObject(pOldBrush);

		UpdateData(false);
	}
}
