// MatrixToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MatrixTool.h"
#include "MatrixToolDlg.h"

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


// CMatrixToolDlg �Ի���




CMatrixToolDlg::CMatrixToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMatrixToolDlg::IDD, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMatrixToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CMatrixToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_INVERSE, &CMatrixToolDlg::OnBnClickedButtonInverse)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, &CMatrixToolDlg::OnBnClickedButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_COPY_RESULT, &CMatrixToolDlg::OnBnClickedButtonCopyResult)
	ON_COMMAND_RANGE(IDC_RADIO_COMMA,IDC_RADIO_SPACE,OnRadioSeparator)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMatrixToolDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CMatrixToolDlg::OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFORM, &CMatrixToolDlg::OnBnClickedButtonTransform)
	ON_BN_CLICKED(IDC_BUTTON_FORMAT, &CMatrixToolDlg::OnBnClickedButtonFormat)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CMatrixToolDlg::OnBnClickedButtonSort)
	ON_BN_CLICKED(IDC_BUTTON_GROUP, &CMatrixToolDlg::OnBnClickedButtonGroup)
END_MESSAGE_MAP()


// CMatrixToolDlg ��Ϣ�������

BOOL CMatrixToolDlg::OnInitDialog()
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
	CButton *comma=(CButton*)GetDlgItem(IDC_RADIO_COMMA);
	comma->SetCheck(1);
	m_separator=',';

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMatrixToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMatrixToolDlg::OnPaint()
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
HCURSOR CMatrixToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMatrixToolDlg::OnRadioSeparator(UINT  nID)
{
	CButton *comma=(CButton*)GetDlgItem(IDC_RADIO_COMMA);

	if(comma->GetCheck())
		m_separator=',';
	else
		m_separator=' ';
}
void CMatrixToolDlg::OnBnClickedButtonInverse()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	if(array.GetSize()!=16)
	{
		TRACE("array.size:%d\r\n",array.GetSize());
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double src[16],r[16];
	for(int i=0;i<16;i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		src[i]=atof(str);
	}
	printMatrix(src);
	inverseMatrix4(src,r);
	printMatrix(r);

	m_strOutput.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonInvert()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	if(array.GetSize()!=16)
	{
		TRACE("array.size:%d\r\n",array.GetSize());
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double src[16],r[16];
	for(int i=0;i<16;i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		src[i]=atof(str);
	}
	printMatrix(src);
	invertMatrix(src,r);
	printMatrix(r);

	m_strOutput.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonCopyResult()
{
	UpdateData(true);
	copyToClipBoard(m_strOutput);
}

void CMatrixToolDlg::OnBnClickedButtonClear()
{
	m_strInput="";
	m_strOutput="";
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonConvert()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	if(array.GetSize()!=16)
	{
		TRACE("array.size:%d\r\n",array.GetSize());
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double r[16];
	for(int i=0;i<16;i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		r[i]=atof(str);
	}
	printMatrix(r);

	m_strOutput.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonTransform()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	if(array.GetSize()!=16)
	{
		TRACE("array.size:%d\r\n",array.GetSize());
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double src[16],r[16];
	for(int i=0;i<16;i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		src[i]=atof(str);
	}
	printMatrix(src);

	double invert[16];
	for(int i=0;i<16;i++)
		invert[i]=0;
	invert[0]=invert[6]=invert[9]=invert[15]=1;

	multipleMatrix(invert,src,r);

	printMatrix(r);

	m_strOutput.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonFormat()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	if(array.GetSize()!=16)
	{
		TRACE("array.size:%d\r\n",array.GetSize());
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double r[16];
	for(int i=0;i<16;i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		r[i]=atof(str);
		if(fabs(r[i])<0.0001)
			r[i]=0;
	}
	printMatrix(r);

	m_strOutput.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonSort()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	if(array.GetSize()!=16)
	{
		TRACE("array.size:%d\r\n",array.GetSize());
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double r[16];
	for(int i=0;i<16;i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		r[i]=atof(str);
	}
	printMatrix(r);

	m_strOutput.Format("%g,%g,%g,%g\r\n%g,%g,%g,%g\r\n%g,%g,%g,%g\r\n%g,%g,%g,%g",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
	
	UpdateData(false);
}

void CMatrixToolDlg::OnBnClickedButtonGroup()
{
	UpdateData(true);
	CStringArray array;
	splitString(m_strInput,m_separator,array);
	TRACE("array.size:%d\r\n",array.GetSize());
	if(array.GetSize()%16!=0)
	{
		AfxMessageBox(TEXT("�������4*4����!"));
		return;
	}

	double* r=new double[array.GetSize()];
	for(int i=0;i<array.GetSize();i++)
	{
		char* str=(LPTSTR)(LPCTSTR)(array.GetAt(i));
		r[i]=atof(str);
	}

	m_strOutput="";
	for(int i=0;i<array.GetSize()/16;i++)
	{
		CString temp;
		temp.Format("%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\r\n",r[0+i*16],r[1+i*16],r[2+i*16],r[3+i*16],r[4+i*16],r[5+i*16],r[6+i*16],r[7+i*16],r[8+i*16],r[9+i*16],r[10+i*16],r[11+i*16],r[12+i*16],r[13+i*16],r[14+i*16],r[15+i*16]);
		m_strOutput+=temp;
	}

	delete[] r;
	UpdateData(false);
}
