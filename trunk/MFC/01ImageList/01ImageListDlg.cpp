// 01ImageListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "01ImageList.h"
#include "01ImageListDlg.h"

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


// CMy01ImageListDlg �Ի���




CMy01ImageListDlg::CMy01ImageListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy01ImageListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy01ImageListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CMy01ImageListDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMy01ImageListDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy01ImageListDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMy01ImageListDlg ��Ϣ�������

void CMy01ImageListDlg::enumFiles(const wchar_t* szSourceDir)
{
	if (!szSourceDir) return;

	WIN32_FIND_DATA FindFileData;   
	WIN32_FILE_ATTRIBUTE_DATA FindFileAttr;
	HANDLE   hFind = INVALID_HANDLE_VALUE;
	TCHAR    currDir[MAX_PATH];
	TCHAR    Result[MAX_PATH];

	CString strFile; 
	int  pos;
	CString  strFileExt;

	_tcsncpy(currDir, szSourceDir, _tcslen(szSourceDir) + 1);   
	_tcsncat(currDir, _T("\\*"), 3);

	hFind = FindFirstFile(currDir, &FindFileData);   

	if (hFind == INVALID_HANDLE_VALUE) 
		return;
	do
	{
		if (*FindFileData.cFileName == '.') continue;
		::_stprintf(Result, _T("%s\\%s"), szSourceDir, FindFileData.cFileName);
		if (GetFileAttributesEx(Result, GetFileExInfoStandard, &FindFileAttr))
		{
			//cout << Result << endl;
			//TRACE(TEXT("%s\r\n"),FindFileData.cFileName);
			strFile = FindFileData.cFileName;
			pos = strFile.ReverseFind('.');
			if (pos>0)
				strFileExt = strFile.Mid(pos+1,strFile.GetLength()-pos-1);
			wchar_t* wc=(LPTSTR)(LPCTSTR)strFileExt;
			//TRACE(TEXT("%s\r\n"),wc);
			wchar_t* file=TEXT("bmp");
			if(strFileExt.CompareNoCase(file)==0)
				//m_listBox.AddString(FindFileData.cFileName);
				m_images.push_back(FindFileData.cFileName);
		}
	} while (FindNextFile(hFind, &FindFileData));
}

BOOL CMy01ImageListDlg::OnInitDialog()
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

	m_ImageList.Create(64, 64, ILC_COLORDDB|ILC_MASK, 20, 1);
	//m_ListCtrl.SetImageList(&m_ImageList,LVSIL_SMALL);
	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);


#if 0
	CBitmap* bitmap;
	bitmap=new CBitmap[2];
	
	for(int i=0;i<2;++i)
	{
		HBITMAP hbitmap;
		if(i==0)
			hbitmap = (HBITMAP)LoadImage(NULL,L"D:\\Development\\Workspace\\VS2008\\MFC\\01ImageList\\test.bmp",IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
		else
			hbitmap = (HBITMAP)LoadImage(NULL,L"D:\\Development\\Workspace\\VS2008\\MFC\\01ImageList\\smoke.bmp",IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);

		bitmap[i].Attach(hbitmap);
		m_ImageList.Add(&bitmap[i], RGB(0, 128, 128));

		//������
		m_ListCtrl.InsertItem(i,L"item",i);
		m_ListCtrl.SetItemText(i,1,L"itemtext");
	}

	delete[] bitmap;
#endif



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMy01ImageListDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy01ImageListDlg::OnPaint()
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
HCURSOR CMy01ImageListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMy01ImageListDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
	{
		CString c;
		c.Format(L"%d",pNMLV->iItem);
		AfxMessageBox(c);
	}
	*pResult = 0;
}

void CMy01ImageListDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TRACE(L"m_ImageList.GetImageCount():%d\n",m_ImageList.GetImageCount());
	//��Ϊɾ��ǰ���Ԫ�غ󣬺����Ԫ�ػ��Զ���ǰ�ƣ���������Ӻ���ǰɾ��
	int count=m_ImageList.GetImageCount();
	for(int i=count-1;i>=0;--i)
	{
		TRACE(L"remove image:%d\n",m_ImageList.Remove(i));
	}
	count=m_ListCtrl.GetItemCount();
	m_ListCtrl.DeleteAllItems();
	for (int i=0;i<count;i++)
	{
		m_ListCtrl.DeleteColumn(i);
	}



	m_images.clear();
	enumFiles(L"D:\\Development\\Workspace\\VS2008\\MFC\\01ImageList\\");
	std::vector<CString>::iterator current=m_images.begin(),last=m_images.end();
	int index=0;
	while(current!=last)
	{
		CBitmap bitmap;
		HBITMAP hbitmap;
		CString str=L"D:\\Development\\Workspace\\VS2008\\MFC\\01ImageList\\";
		str.Append(*current);

		TRACE(L"%d,%s\n",index,str);

		hbitmap = (HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
		bitmap.Attach(hbitmap);
		m_ImageList.Add(&bitmap, RGB(0, 128, 128));

		//������
		m_ListCtrl.InsertItem(index,*current,index);
		m_ListCtrl.SetItemText(index,1,*current);

		++current;
		++index;
	}

	m_ListCtrl.SetRedraw();


}
