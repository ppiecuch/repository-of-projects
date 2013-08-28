// CxImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CxImage.h"
#include "CxImageDlg.h"

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


// CCxImageDlg �Ի���




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


// CCxImageDlg ��Ϣ�������

BOOL CCxImageDlg::OnInitDialog()
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
	setlocale(LC_ALL, "chs");
	//SetWindowPos(NULL,��,��,��,��,0)
	picture.SetWindowPos(NULL,10,10,PICTURE_WIDTH,PICTURE_HEIGHT,0);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCxImageDlg::OnPaint()
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

	CString fileExt; //ͼƬ����չ�� 
	int len = fileName.GetLength(); 
	for(int i=len-1; i>=0; i--) //�õ�ͼƬ����չ�� 
	{ 
		if(fileName[i] == '.') 
		{ 
			fileExt=fileName.Mid(i+1); 
			break; 
		} 
	} 
	fileExt.MakeLower(); //����չ��תΪСд 
	TRACE(TEXT("ext:%s\r\n"),fileExt);

	if(fileExt != _T("")) 
	{ 
		//����CxImage�������о�̬����CxImage::GetTypeIdFromName����������չ�����ͼƬ��ʽ��ID���� 
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
