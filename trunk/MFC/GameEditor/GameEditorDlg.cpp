// GameEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameEditor.h"
#include "GameEditorDlg.h"
#include "SetMap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameEditorDlg dialog

CGameEditorDlg::CGameEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ineditor=false;
}

void CGameEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameEditorDlg)
	DDX_Control(pDX, IDC_TAB1, m_ToolsTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGameEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CGameEditorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_EDITO, OnEdito)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameEditorDlg message handlers

BOOL CGameEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    // CImageList m_imglist;
	// m_imglist.Create(32,16,ILC_COLOR32,100,100);
    // m_imglist.Add());

	
	///////////////////////////////////////以下是增加的代码
	CRect rs;
	m_ToolsTab.InsertItem(0,"图片编辑");
	m_ToolsTab.InsertItem(1,"地图");
	m_ToolsTab.InsertItem(2,"功能3");
	m_ToolsTab.InsertItem(3,"功能4");
	m_ToolsTab.SetCurSel(0);


	m_ToolsTab.GetClientRect(&rs);
	rs.top+=20;
	rs.bottom-=5;
	rs.left+=1;
	rs.right-=2;
    m_editor.Create(IDD_EDITOR,GetDlgItem(IDC_TAB1));
    m_editor.MoveWindow(&rs);
	m_editor.ShowWindow(SW_SHOW);
    tag=-1;
	souce="";
	width=height=0;
    
////////////////////////////////////////////////////////结束

	








	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGameEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGameEditorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
	//	CDialog::OnPaint();
        
		CPaintDC dc(this);
		if (ineditor)
		{
			if (tag!=-1)
			{
                CRect select_rect;
				select_rect.left=pt.x-32;
				select_rect.top=pt.y-32;	      
				select_rect.right=pt.x+128;
				select_rect.bottom=pt.y+128;
				dc.SetTextColor(RGB(255,0,0));

			  if (tag==1)
			  {

		     dc.TextOut(pt.x,pt.y,souce);
			  }
				
			  if (tag==2)
			  {
				  select_rect.right-=64;
				 select_rect.bottom+=-64; 
				  
				  dc.TextOut(pt.x,pt.y,souce);


			  }
				
				//MessageBox("abc");
			    dc.MoveTo(select_rect.left,select_rect.top);
			    dc.LineTo(select_rect.right,select_rect.top);
			    dc.LineTo(select_rect.right,select_rect.bottom);
			    dc.LineTo(select_rect.left,select_rect.bottom);
		        dc.LineTo(select_rect.left,select_rect.top);
			
			}
		}




	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGameEditorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGameEditorDlg::OnButton2()   //新建按钮处理函数
{
	//MessageBox("新建");
	//      在这里进行初始化操作，如清空操作栈 
	//      删除保持数据，新建数据等
    CSetMap m_setmap;
	if(IDOK==m_setmap.DoModal())    //如果用户选择ok按钮
	{
       width=m_setmap.m_width;
	   height=m_setmap.m_height;
       CString temps;
	   temps.Format("%d",width);
	   MessageBox(temps);
	                             
	}
 

	
	
}

void CGameEditorDlg::OnButton1() //打开文档按钮处理函数 
{
//	MessageBox("打开");

CFileDialog opendlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"myfile (*.mplist)|(*.mplist)||");
opendlg.DoModal();

}

void CGameEditorDlg::OnButton3()  //功能3 处理函数 
{
	
	MessageBox("功能3 ");

}


void CGameEditorDlg::OnButton4()  //功能4 处理函数
{
	
	MessageBox("功能4");

}



void CGameEditorDlg::OnButton5()    //重做处理函数
{

	MessageBox("重做");

}



void CGameEditorDlg::OnButton7()   //撤销处理函数 
{

	MessageBox("撤销");

}


void CGameEditorDlg::OnButton6()   //导出处理函数
{
	//MessageBox("导出");
	CFileDialog savedlg(false,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"myfile (*.mplist)|(*.mplist)||");
    savedlg.DoModal();

}








void CGameEditorDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CRect editor_rect;
	GetDlgItem(IDC_EDITO)->GetWindowRect(&editor_rect);
	ScreenToClient(&editor_rect);

	if (editor_rect.left<=point.x&&point.x<=editor_rect.right&&editor_rect.top<=point.y&&point.y<=editor_rect.bottom)
		
	{

       ineditor=true;
	   pt=point;
	}
	else
		ineditor=false;


    InvalidateRect(&editor_rect);
	CDialog::OnLButtonDown(nFlags, point);
}


/////////////////////////
void CGameEditorDlg::OnEdito() 
{
	
}
////////////
