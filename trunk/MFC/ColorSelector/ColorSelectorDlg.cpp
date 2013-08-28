// ColorSelectorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColorSelector.h"
#include "ColorSelectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CColorSelectorDlg �Ի���




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


// CColorSelectorDlg ��Ϣ�������

BOOL CColorSelectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CColorSelectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CColorSelectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CColorSelectorDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg; 
	//����ɫ�Ի���,��ȡѡ������ɫ 
	COLORREF m_curCol;
	if(dlg.DoModal()==IDOK) 
		m_curCol=dlg.GetColor();  
	TRACE("%08x\r\n",m_curCol);
	//m_curID = ID_COLOR_CUSTOM; //���õ�ǰѡ�����ɫID    

	CRect rect;
	CDC *dc; 
	CBrush *pOldBrush;
	CBrush cBrush;

	cBrush.CreateSolidBrush(m_curCol);  

	//m_e��һ��Edit Controll
	m_b.GetClientRect(&rect);
	dc = m_b.GetDC();
	pOldBrush = dc->SelectObject(&cBrush);
	dc->FillRect(rect, &cBrush);   
	dc->SelectObject(pOldBrush);

	UpdateData(false);


}

void CColorSelectorDlg::OnEnSetfocusEdit3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TRACE("focus\r\n");
}
void CColorSelectorDlg::OnClickStatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TRACE("focus\r\n");

	CColorDialog dlg; 
	//����ɫ�Ի���,��ȡѡ������ɫ 
	COLORREF m_curCol;
	if(dlg.DoModal()==IDOK) 
	{
		m_curCol=dlg.GetColor();  
		CRect rect;
		CDC *dc; 
		CBrush *pOldBrush;
		CBrush cBrush;

		cBrush.CreateSolidBrush(m_curCol);  

		//m_e��һ��Edit Controll
		m_s.GetClientRect(&rect);
		dc = m_s.GetDC();
		pOldBrush = dc->SelectObject(&cBrush);
		dc->FillRect(rect, &cBrush);   
		dc->SelectObject(pOldBrush);

		UpdateData(false);
	}
}
