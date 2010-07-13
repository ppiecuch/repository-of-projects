// MySkyView.cpp : implementation of the CMySkyView class
//

#include "stdafx.h"
#include "MySky.h"

#include "MySkyDoc.h"
#include "MySkyView.h"
#include <math.h>
#include <mmsystem.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySkyView

IMPLEMENT_DYNCREATE(CMySkyView, CView)

BEGIN_MESSAGE_MAP(CMySkyView, CView)
	//{{AFX_MSG_MAP(CMySkyView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySkyView construction/destruction

CMySkyView::CMySkyView()
{
	// TODO: add construction code here
	Vertices=new VERTEX;
}

CMySkyView::~CMySkyView()
{
}

BOOL CMySkyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
////////////////////////////////////////////////////////////////
//���ô�������
	cs.style |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
////////////////////////////////////////////////////////////////
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMySkyView drawing

void CMySkyView::OnDraw(CDC* pDC)
{
	CMySkyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
//////////////////////////////////////////////////////////////////
	RenderScene();	//��Ⱦ����
//////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////
// CMySkyView printing

BOOL CMySkyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMySkyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMySkyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMySkyView diagnostics

#ifdef _DEBUG
void CMySkyView::AssertValid() const
{
	CView::AssertValid();
}

void CMySkyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMySkyDoc* CMySkyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMySkyDoc)));
	return (CMySkyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMySkyView message handlers

int CMySkyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//////////////////////////////////////////////////////////////////
//��ʼ��OpenGL�����ö�ʱ��
	m_pDC = new CClientDC(this);
	SetTimer(1, 20, NULL);
	InitializeOpenGL(m_pDC);
//////////////////////////////////////////////////////////////////
	InitGL();
	return 0;
}

void CMySkyView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
/////////////////////////////////////////////////////////////////
//ɾ����ɫ�����Ⱦ�����ġ���ʱ��
	::wglMakeCurrent(0,0);
	::wglDeleteContext( m_hRC);
	if (m_hPalette)
	    DeleteObject(m_hPalette);
	if ( m_pDC )
	{
		delete m_pDC;
	}
	KillTimer(1);		
/////////////////////////////////////////////////////////////////
	ReleaseDome();	
}

void CMySkyView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
/////////////////////////////////////////////////////////////////
//��Ӵ�������ʱ��ͼ�α任����
	glViewport(0,0,cx,cy);
/////////////////////////////////////////////////////////////////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (cy == 0) cy = 1;
	gluPerspective(45.0f, (GLfloat)(cx/cy), 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

void CMySkyView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
/////////////////////////////////////////////////////////////////
//��Ӷ�ʱ����Ӧ�����ͳ������º���
	Invalidate(FALSE);	
/////////////////////////////////////////////////////////////////
	
	CView::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////
//	                  �����߼���ɫ��
//////////////////////////////////////////////////////////////////////
void CMySkyView::SetLogicalPalette(void)
{
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } logicalPalette = { 0x300, 256 };

	BYTE reds[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE greens[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE blues[] = {0, 85, 170, 255};

    for (int colorNum=0; colorNum<256; ++colorNum)
    {
        logicalPalette.aEntries[colorNum].peRed =
            reds[colorNum & 0x07];
        logicalPalette.aEntries[colorNum].peGreen =
            greens[(colorNum >> 0x03) & 0x07];
        logicalPalette.aEntries[colorNum].peBlue =
            blues[(colorNum >> 0x06) & 0x03];
        logicalPalette.aEntries[colorNum].peFlags = 0;
    }

    m_hPalette = CreatePalette ((LOGPALETTE*)&logicalPalette);
}


//////////////////////////////////////////////////////////
//						��ʼ��openGL����
//////////////////////////////////////////////////////////
BOOL CMySkyView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();
	//���ɻ���������
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//�õ�ǰ����������
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	return TRUE;
}

//////////////////////////////////////////////////////////
//						�������ظ�ʽ
//////////////////////////////////////////////////////////
BOOL CMySkyView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
	    1,                                // �汾�� 
	    PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
	    PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
	    PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
	    PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
	    24,                               // 24 λ��ɫ��� 
	    0, 0, 0, 0, 0, 0,                 // ������ɫλ 
	    0,                                // û�з�͸���Ȼ��� 
	    0,                                // ������λλ 
	    0,                                // ���ۼӻ��� 
	    0, 0, 0, 0,                       // �����ۼ�λ 
	    32,                               // 32 λ��Ȼ���     
	    0,                                // ��ģ�建�� 
	    0,                                // �޸������� 
	    PFD_MAIN_PLANE,                   // ���� 
	    0,                                // ���� 
	    0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	}; 	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//ѡ�����ظ�ʽ
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	//�������ظ�ʽ
	if(pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();	//�����߼���ɫ��
	return TRUE;
}

//////////////////////////////////////////////////////////
//						������������Ⱦ
//////////////////////////////////////////////////////////
BOOL CMySkyView::RenderScene() 
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	// Render the selected dome
	RenderSkyDome();

	::SwapBuffers(m_pDC->GetSafeHdc());		//����������
	return TRUE;
}

GLint CMySkyView::InitGL(GLvoid)
{

	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������Ч��
	GLfloat fogColor[4]= {0.9f, 0.9f, 0.9f, 0.6f};
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.002f);
	glFogf(GL_FOG_START, 100.0f);
	glFogf(GL_FOG_END, 800.0f);
	glHint(GL_FOG_HINT, GL_FASTEST);

	LoadTextures("clouds2.bmp", 0);

	GenerateDome(600.0f, 5.0f, 5.0f, 1.0f, 1.0f);

	return true;
}

void CMySkyView::GenerateDome(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	int theta, phi;

	// ����������д��ڣ�����ն�������
	if (Vertices) 
	{
		delete Vertices;
		Vertices = NULL;
		NumVertices = 0;
	}

	// ��ʼ����������
	NumVertices = (int)((360/dtheta)*(90/dphi)*4);
	Vertices = new VERTEX[NumVertices];
	ZeroMemory(Vertices, sizeof(VERTEX)*NumVertices);

	// �������� UV ����
	float vx, vy, vz, mag;

	// ���ɹ������dome
	int n = 0;
	for (phi=0; phi <= 90 - dphi; phi += (int)dphi)
	{
		for (theta=0; theta <= 360 - dtheta; theta += (int)dtheta)
		{
			// ���㶥��� phi, theta
			Vertices[n].x = radius * sinf(phi*DTOR) * cosf(DTOR*theta);
			Vertices[n].y = radius * sinf(phi*DTOR) * sinf(DTOR*theta);
			Vertices[n].z = radius * cosf(phi*DTOR);

			// �����ԭ�㵽�ö����ʸ��
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			// ʸ��������
			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			// ����������������
			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// ����λ����phi+dphi, theta���Ķ���
			Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(theta*DTOR);
			Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(theta*DTOR);
			Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
			
			// ������������
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// ����λ��phi, theta+dtheta���Ķ�������
			Vertices[n].x = radius * sinf(DTOR*phi) * cosf(DTOR*(theta+dtheta));
			Vertices[n].y = radius * sinf(DTOR*phi) * sinf(DTOR*(theta+dtheta));
			Vertices[n].z = radius * cosf(DTOR*phi);
			
			// ������������
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			if (phi > -90 && phi < 90)
			{
				// ����λ�� phi+dphi, theta+dtheta���Ķ�������
				Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta));
				Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta));
				Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
				
				// ������������
				vx = Vertices[n].x;
				vy = Vertices[n].y;
				vz = Vertices[n].z;

				mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
				vx /= mag;
				vy /= mag;
				vz /= mag;

				Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
				Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
				n++;
			}
		}
	}
	// �޸���������
	for (int i=0; i < NumVertices-3; i++)
	{
		if (Vertices[i].u - Vertices[i+1].u > 0.9f)
			Vertices[i+1].u += 1.0f;

		if (Vertices[i+1].u - Vertices[i].u > 0.9f)
			Vertices[i].u += 1.0f;

		if (Vertices[i].u - Vertices[i+2].u > 0.9f)
			Vertices[i+2].u += 1.0f;

		if (Vertices[i+2].u - Vertices[i].u > 0.9f)
			Vertices[i].u += 1.0f;

		if (Vertices[i+1].u - Vertices[i+2].u > 0.9f)
			Vertices[i+2].u += 1.0f;

		if (Vertices[i+2].u - Vertices[i+1].u > 0.9f)
			Vertices[i+1].u += 1.0f;

		if (Vertices[i].v - Vertices[i+1].v > 0.8f)
			Vertices[i+1].v += 1.0f;

		if (Vertices[i+1].v - Vertices[i].v > 0.8f)
			Vertices[i].v += 1.0f;

		if (Vertices[i].v - Vertices[i+2].v > 0.8f)
			Vertices[i+2].v += 1.0f;

		if (Vertices[i+2].v - Vertices[i].v > 0.8f)
			Vertices[i].v += 1.0f;

		if (Vertices[i+1].v - Vertices[i+2].v > 0.8f)
			Vertices[i+2].v += 1.0f;

		if (Vertices[i+2].v - Vertices[i+1].v > 0.8f)
			Vertices[i+1].v += 1.0f;
	}
}

int CMySkyView::RenderSkyDome()
{
	glPushMatrix();
	glTranslatef(0.0f, -250.0f, 0.0f);
	glRotatef(timeGetTime()/2000.0f,0.0f, 1.0f, 0.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);

	for (int i=0; i < NumVertices; i++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(Vertices[i].u, Vertices[i].v);
		glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
	}

	glEnd();

	glPopMatrix();
	return 1;
}

void CMySkyView::ReleaseDome()
{
	if (Vertices)
	{
		delete Vertices;
		Vertices = NULL;
	}
}

int CMySkyView::LoadTextures(char *filename, int id)
{
	int status = false;

	AUX_RGBImageRec *TextureImage = NULL;

	ZeroMemory(&TextureImage, sizeof(void*)*1);

	if (TextureImage = LoadBMP(filename))
	{
		status = true;

		glGenTextures(1, &texture[id]);
		glBindTexture(GL_TEXTURE_2D, texture[id]);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage)
	{
		if (TextureImage->data)
			free(TextureImage->data);
		free(TextureImage);
	}

	return status;

}

AUX_RGBImageRec *CMySkyView::LoadBMP(char *filename)
{
	FILE *file = NULL;

	if (!filename)
		return NULL;

	file = fopen(filename, "r");
	if (file)
	{
		fclose(file);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}
