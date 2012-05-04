// 06GLBmpTextureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "06GLBmpTexture.h"
#include "06GLBmpTextureDlg.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Z_MAX 1
GLuint m_texture[Z_MAX];

GLvoid* bmpbits;

BITMAPINFOHEADER LoadGLBitmap(char *filename)
{
	FILE* fp;
	BITMAPFILEHEADER fileheader;
	BITMAPINFO info;
	BITMAPINFOHEADER header;
	int infosize,bitsize;

	if((fp=fopen(filename,"rb"))==NULL)
		return header;

	fread(&fileheader,sizeof(BITMAPFILEHEADER),1,fp);

	if(fileheader.bfType!='MB')
	{
		fclose(fp);
		return header;
	}

	infosize=fileheader.bfOffBits-sizeof(BITMAPFILEHEADER);
	fread(&info,1,infosize,fp);
	header=info.bmiHeader;
	if((bitsize=info.bmiHeader.biSizeImage)==0)
		bitsize=(info.bmiHeader.biWidth*info.bmiHeader.biBitCount+7)/8*
		abs(info.bmiHeader.biHeight);

	bmpbits=malloc(bitsize);
	fread(bmpbits,1,bitsize,fp);
	fclose(fp);
	return header;
}

void CreateTexture()
{
	glGenTextures(Z_MAX,m_texture);
	//Z_MAX=10,m_texture�Ķ��壺GLuint m_texture[Z_MAX]
	int i;
	char filename[50];
	BITMAPINFOHEADER bmpheader;
	for(i=0;i<=Z_MAX-1;i++)
	{
		sprintf(filename,"texture%d.bmp",i);
		//sprintf(filename,"test.bmp");
		//ͼƬ������Data/gl�У���������b1.bmp....b10.bmp
		bmpheader=LoadGLBitmap(filename);
		glBindTexture(GL_TEXTURE_2D,m_texture[i]);
		glTexImage2D(GL_TEXTURE_2D,0,3,bmpheader.biWidth,bmpheader.biHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmpbits);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		delete[] bmpbits;
	}
}


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


// CMy06GLBmpTextureDlg �Ի���




CMy06GLBmpTextureDlg::CMy06GLBmpTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy06GLBmpTextureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy06GLBmpTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy06GLBmpTextureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMy06GLBmpTextureDlg ��Ϣ�������

BOOL CMy06GLBmpTextureDlg::OnInitDialog()
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

void CMy06GLBmpTextureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy06GLBmpTextureDlg::OnPaint()
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

		DrawGLScene(); // Render the OpenGL scene
		SwapBuffers(hDC);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy06GLBmpTextureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy06GLBmpTextureDlg::OnDestroy()
{
	CDialog::OnDestroy();

	glDeleteTextures(Z_MAX,m_texture);
	// TODO: �ڴ˴������Ϣ����������
	if(wglGetCurrentContext()!=NULL)
	{
		wglMakeCurrent(NULL,NULL);
	}
	if(this->m_hGLContext!=NULL)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}
}

int CMy06GLBmpTextureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	HWND hWnd = this->GetSafeHwnd();    
	hDC = ::GetDC(hWnd);
	if(this->SetWindowPixelFormat(hDC)==FALSE)
	{
		return 0;
	}
	if(this->CreateViewGLContext(hDC)==FALSE)
	{
		return 0;
	}
	//��ȡOpenGL�Ļ�����Ϣ
	//GetGLInfo();
	InitGL();   // Initialize OpenGL
	SetTimer(1,50,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣

	return 0;
}

void CMy06GLBmpTextureDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	GLsizei width,height;
	GLdouble aspect;
	width = cx;
	height = cy;
	if(cy==0)
	{
		aspect = (GLdouble)width;
	}
	else
	{
		aspect = (GLdouble)width/(GLdouble)height;
	}
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0.0,500.0*aspect,0.0,500.0);
	gluPerspective(50.0f,aspect,0.0f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	//�����޳�����
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//glLoadIdentity();

	//�ں�̨�����л�ͼ
	glDrawBuffer (GL_BACK);
}

void CMy06GLBmpTextureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nIDEvent==1)
	{
		InvalidateRect(NULL,FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CMy06GLBmpTextureDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}

bool CMy06GLBmpTextureDlg::SetWindowPixelFormat(HDC hDC)
{
	//���崰�ڵ����ظ�ʽ
	PIXELFORMATDESCRIPTOR pixelDesc=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
		PFD_DOUBLEBUFFER|PFD_STEREO_DONTCARE,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};



	this->m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(this->m_GLPixelIndex==0)
	{
		this->m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,this->m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
		{
			return FALSE;
		}
	}

	if(SetPixelFormat(hDC,this->m_GLPixelIndex,&pixelDesc)==FALSE)
	{
		return FALSE;
	}
	return TRUE;


}

bool CMy06GLBmpTextureDlg::CreateViewGLContext(HDC hDC)
{
	//�������ƻ���(RC)��ʹ֮��Ϊ��ǰ���ƻ���
	this->m_hGLContext = wglCreateContext(hDC);
	if(this->m_hGLContext==NULL)
	{//����ʧ��
		return FALSE;
	}

	if(wglMakeCurrent(hDC,this->m_hGLContext)==FALSE)
	{//ѡΪ��ǰRCʧ��
		return FALSE;
	}

	return TRUE;




}

void CMy06GLBmpTextureDlg::GetGLInfo(void)
{
	CString oglinfo;

	oglinfo = "OpenGL Information\n\nWho: ";
	oglinfo +=  (char *)glGetString( GL_VENDOR );
	oglinfo += "\nWhich: ";
	oglinfo +=  (char *)glGetString( GL_RENDERER );
	oglinfo += "\nVersion: ";
	oglinfo +=  (char *)glGetString( GL_VERSION );
	oglinfo += "\nExtensions: ";
	oglinfo +=  (char *)glGetString( GL_EXTENSIONS );


	AfxMessageBox(oglinfo,MB_ICONINFORMATION); // Display OpenGL Information

}

void CMy06GLBmpTextureDlg::DrawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_texture[0]);
	glBegin(GL_POLYGON);
		glVertex3f(0.25,0.25,0.0);
		glTexCoord2d(0,0);
		glVertex3f(1.75,0.25,0.0);
		glTexCoord2d(1,0);
		glVertex3f(1.75,1.75,0.0);
		glTexCoord2d(1,1);
		glVertex3f(0.25,1.75,0.0);
		glTexCoord2d(0,1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glFlush();
}

bool CMy06GLBmpTextureDlg::InitGL(void)
{

	CreateTexture();
	// from NeHe's Tutorial 3
	glShadeModel(GL_SMOOTH);        // Enable Smooth Shading
	glClearColor(0.1, 0.2, 0.3, 1);      // Black Background
	glClearDepth(1.0);          // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);         // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
	return TRUE;           // Initialization Went OK

}