// COpenGLView.cpp : implementation of the COpenGLView class
//
#include <stdio.h>		// 标准输入/输出库的头文件 (新增)
#include "stdafx.h"
#include "COpenGLView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



const char* const COpenGLView::_ErrorStrings[]= {
				{"No Error"},					// 0
				{"Unable to get a DC"},			// 1
				{"ChoosePixelFormat failed"},	// 2
				{"SelectPixelFormat failed"},	// 3
				{"wglCreateContext failed"},	// 4
				{"wglMakeCurrent failed"},		// 5
				{"wglDeleteContext failed"},	// 6
				{"SwapBuffers failed"},			// 7

		};
/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView construction/destruction

COpenGLView::COpenGLView() :
	 m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0])

{
	// TODO: add construction code here
    m_bFullView=FALSE;
	xrot=yrot=zrot=wiggle_count=0;
	mywave.center_wave_x=mywave.center_wave_y=90;
	mywave.distance_wave=0;
	mywave.move=FALSE;
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	// An OpenGL window must be created with the following flags and must not
    // include CS_PARENTDC for the class style. 
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  	
	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// COpenGLView drawing


void COpenGLView::OnDraw(CDC* pDC)
{
	CMyOpenGLFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here


/*
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	PreRenderScene();

	::glPushMatrix();
	//RenderStockScene();
	::glPopMatrix();
*/

	//::glPushMatrix();
	RenderScene();
	//::glPopMatrix();

	::glFinish();

	if ( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc() ) )
		{
		SetError(7);
		}

}


/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyOpenGLFrameDoc* COpenGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyOpenGLFrameDoc)));
	return (CMyOpenGLFrameDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	
	InitializeOpenGL();

	SetTimer(1,20,0);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// GL helper functions

void COpenGLView::SetError( int e )
{
	// if there was no previous error,
	// then save this one
	if ( _ErrorStrings[0] == m_ErrorString ) 
		{
		m_ErrorString = _ErrorStrings[e];
		}
}


BOOL COpenGLView::InitializeOpenGL()
{
	// Can we put this in the constructor?
    m_pDC = new CClientDC(this);

    if ( NULL == m_pDC ) // failure to get DC
		{
		SetError(1);
		return FALSE;
		}

	if (!SetupPixelFormat())
		{
        return FALSE;
		}

    //n = ::GetPixelFormat(m_pDC->GetSafeHdc());
    //::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

  //  CreateRGBPalette();

    if ( 0 == (m_hRC = ::wglCreateContext( m_pDC->GetSafeHdc() ) ) )
		{
		SetError(4);
		return FALSE;
		}

    if ( FALSE == ::wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ) )
		{
		SetError(5);
		return FALSE;
		}	

	// specify black as clear color
    ::glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	// specify the back of the buffer as clear depth
    ::glClearDepth( 1.0f );
	// enable depth testing
    ::glEnable( GL_DEPTH_TEST );

	if (!LoadGLTextures())								// 调用纹理载入子例程(新增)
	{
		return FALSE;								// 如果未能载入，返回FALSE(新增)
	}
	glEnable(GL_TEXTURE_2D);							// 启用纹理映射(新增)


//	glPolygonMode( GL_BACK, GL_FILL );			// Back Face Is Filled In
//	glPolygonMode( GL_FRONT, GL_LINE );			// Front Face Is Drawn With Lines
	
	// Loop Through The X Plane
	
	for(int i=0;i<=16;i++)	
		wave[i]=sin(i/8.0*PAI)*0.2;
	for(int x=0; x<180; x++)
	{
		// Loop Through The Y Plane
		for(int y=0; y<180; y++)
		{
			// Apply The Wave To Our Mesh center_wave_x=center_wave_y
			points[x][y][0]=float((x/12.0f)-7.5f);
			points[x][y][1]=float((y/20.0f)-4.5f);
			points[x][y][2]=0;//wave[d];//float((float)(rand())/RAND_MAX*1.3f);//*3.141592654
		}
	}
	

	

    //设置光照
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// Ambient Light Values ( NEW )
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
	GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };				 // Light Position ( NEW )
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
	glEnable(GL_LIGHT1);
	
	glEnable(GL_LIGHT0);

	
	return TRUE;
}


BOOL COpenGLView::SetupPixelFormat()
{
  static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
          PFD_SUPPORT_OPENGL |          // support OpenGL
          PFD_DOUBLEBUFFER,             // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
//        32,                             // 32-bit z-buffer
		16,	// NOTE: better performance with 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int pixelformat;

    if ( 0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) )
	    {
		SetError(2);
        return FALSE;
		}

    if ( FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) )
	    {
       	SetError(3);
        return FALSE;
		}

    return TRUE;
}

void COpenGLView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here

    if ( FALSE == ::wglDeleteContext( m_hRC ) )
		{
		SetError(6);
 		}

    if ( m_pDC )
		{
        delete m_pDC;
		}
	KillTimer(1);
}


BOOL COpenGLView::OnEraseBkgnd( CDC* pDC ) 
{
	// TODO: Add your message handler code here and/or call default
	
	//	return CView::OnEraseBkgnd(pDC);
	return TRUE; // tell Windows not to erase the background
}

void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_ESCAPE)   //如果按的键为Esc键
	{                    //获取主框架窗口的指针
		CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
		//调用主窗口类的自定义函数 EndFullScreen ，便可退出全屏显示状态
		pFrame->EndFullScreen();
		m_bFullView=FALSE;
	}
	if(nChar==VK_F1)   //如果按的键为F1键
	{                    //获取主框架窗口的指针
		m_bFullView=!m_bFullView;
		CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
		//调用主窗口类的自定义函数 EndFullScreen ，便可退出全屏显示状态
		if (m_bFullView) 
		{
			pFrame->StartFullScreen();
		}
		else
		{
			pFrame->EndFullScreen();
		}
		
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenGLView::OnTimer(UINT nIDEvent)
{
  
	int x,y;

	//初始化波纹
				for( x=0; x<180; x++)
				{
					// Loop Through The Y Plane
					for( y=0; y<180; y++)
					{

							points[x][y][2]=0;
	
					}
				}
		WAVE node;

		for(int i=0;i<wavelist.GetCount();i++) 
		{
		node=wavelist.GetHead();
		wavelist.RemoveHead();
		//移动波纹
        if (node.move) 
		{
			node.move=FALSE;
			for( x=0; x<180; x++)
			{
				// Loop Through The Y Plane
				for( y=0; y<180; y++)
				{
					// Apply The Wave To Our Mesh center_wave_x=center_wave_y
					int d;
					d=sqrt((x-node.center_wave_x)*(x-node.center_wave_x)+(y-node.center_wave_y)*(y-node.center_wave_y));
					d=d-node.distance_wave;
					if (d<16&&d>0)
					{
						points[x][y][2]+=wave[d];
						node.move=TRUE;
					}

				}
			}
		}
		if (node.move) 
		{
			node.distance_wave=node.distance_wave+1;
			wavelist.AddTail(node);
		}
		}
		

	
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GLdouble aspect_ratio; // width/height ratio
	
	if ( 0 >= cx || 0 >= cy )
		{
		return;
		}


	SetupViewport( cx, cy );

	// select the projection matrix and clear it
    ::glMatrixMode( GL_PROJECTION );
    ::glLoadIdentity();

	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;

	// select the viewing volumn
	SetupViewingFrustum( aspect_ratio );
	
	// switch back to the modelview matrix
    ::glMatrixMode( GL_MODELVIEW );
    ::glLoadIdentity();

	// now perform any viewing transformations
	SetupViewingTransform();
}

 /////////////////////////////////////////////////////////////////////////////
// COpenGLView helper functions

BOOL COpenGLView::SetupViewport( int cx, int cy )
{
	// select the full client area
    ::glViewport(0, 0, cx, cy);

	return TRUE;
}

BOOL COpenGLView::SetupViewingFrustum( GLdouble aspect_ratio )
{
	// select a default viewing volumn
    ::gluPerspective( 40.0f, aspect_ratio, .1f, 20.0f );
	return TRUE;
}


BOOL COpenGLView::SetupViewingTransform()
{
	// select a default viewing transformation
	// of a 20 degree rotation about the X axis
	// then a -5 unit transformation along Z
	::glTranslatef( 0.0f, 0.0f, -5.0f );
	::glRotatef( 20.0f, 1.0f, 0.0f, 0.0f );
    return TRUE;
}


BOOL COpenGLView::RenderScene()
{

	int x, y;						// Loop Variables
	float float_x, float_y, float_xb, float_yb;		// Used To Break The Flag Into Tiny Quads
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And Depth Buffer	
	glLoadIdentity();					// Reset The Current Matrix
	
	glTranslatef(0.0f,0.0f,-12.0f);				// Translate 17 Units Into The Screen
	

/*
		glRotatef(xrot,1.0f,0.0f,0.0f);				// Rotate On The X Axis
		glRotatef(yrot,0.0f,1.0f,0.0f);				// Rotate On The Y Axis  
		glRotatef(zrot,0.0f,0.0f,1.0f);				// Rotate On The Z Axis* /
*/
/*
	for( x=0; x<450; x++)
	{
		// Loop Through The Y Plane
		for( y=0; y<450; y++)
		{
			// Apply The Wave To Our Mesh center_wave_x=center_wave_y
			points[x][y][0]=float((x/30.0f)-7.5f);
			points[x][y][1]=float((y/50.0f)-4.5f);
			int d=sqrt((x-center_wave_x)*(x-center_wave_x)+(y-center_wave_y)*(y-center_wave_y));
			d=d%16;
			points[x][y][2]=wave[d];//float((float)(rand())/RAND_MAX*1.3f);// *3.141592654
		}
	}*/

	


	
	glBindTexture(GL_TEXTURE_2D, texture[0]);		// Select Our Texture
	double t=1;
	glBegin(GL_QUADS);					// Start Drawing Our Quads
	for( x = 0; x < 179; x++ )				// Loop Through The X Plane 0-44 (45 Points)
	{
		for( y = 0; y < 179; y++ )			// Loop Through The Y Plane 0-44 (45 Points)
		{
			float_x = float(x)/179.0f;		// Create A Floating Point X Value
			float_y = float(y)/179.0f;		// Create A Floating Point Y Value
			float_xb = float(x+1)/179.0f;		// Create A Floating Point Y Value+0.0227f
			float_yb = float(y+1)/179.0f;		// Create A Floating Point Y Value+0.0227f
			glTexCoord2f(float_x, float_y);	// First Texture Coordinate (Bottom Left)
			glVertex3f( t*points[x][y][0], t*points[x][y][1], points[x][y][2] );
			
			glTexCoord2f(float_x, float_yb );	// Second Texture Coordinate (Top Left)
			glVertex3f( t*points[x][y+1][0], t*points[x][y+1][1], points[x][y+1][2] );
			
			glTexCoord2f(float_xb, float_yb );	// Third Texture Coordinate (Top Right)
			glVertex3f( t*points[x+1][y+1][0], t*points[x+1][y+1][1], points[x+1][y+1][2] );
			
			glTexCoord2f( float_xb, float_y );	// Fourth Texture Coordinate (Bottom Right)
			glVertex3f( t*points[x+1][y][0], t*points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();
	float hold;
/*
	if( wiggle_count== 2 )					// Used To Slow Down The Wave (Every 2nd Frame Only)
	{
		
		for( y = 0; y < 45; y++ )			// Loop Through The Y Plane
		{
			hold=points[0][y][2];			// Store Current Value One Left Side Of Wave
			for( x = 0; x < 44; x++)		// Loop Through The X Plane
			{
				// Current Wave Value Equals Value To The Right
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;			// Last Value Becomes The Far Left Stored Value
		}
		wiggle_count = 0;				// Set Counter Back To Zero
	}
	wiggle_count++;						// Increase The Counter*/


	xrot+=0.3f;						// Increase The X Rotation Variable
	yrot+=0.2f;						// Increase The Y Rotation Variable
	zrot+=0.4f;						// Increase The Z Rotation Variable


	

    return TRUE;
}	

AUX_RGBImageRec *COpenGLView::LoadBMP(char *Filename)	// 载入位图图象
{
	FILE *File=NULL;		// 文件句柄
	if (!Filename)			// 确保文件名已提供。
	{
		return NULL;		// 如果没提供，返回 NULL
	}
	
	File=fopen(Filename,"r");					// 尝试打开文件
	
	if (File)									// 文件存在么？
	{
		fclose(File);							// 关闭句柄
		return auxDIBImageLoad(Filename);		// 载入位图并返回指针
	}
	
	return NULL;				// 如果载入失败，返回 NULL
}

int COpenGLView::LoadGLTextures()					// 载入位图(调用上面的代码)并转换成纹理
{
	int Status=FALSE;					// Status 纹理载入状态指示器
	AUX_RGBImageRec *TextureImage[1];	// 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *) *1);	// 将指针设为 NULL
	// 载入位图，检查有无错误，如果位图没找到则退出。
	if (TextureImage[0]=LoadBMP("Data/texture3.bmp"))
	{
		Status=TRUE;						// 将 Status 设为 TRUE
		glGenTextures(1, &texture[0]);		// 创建纹理
		// 使用来自位图数据生成的典型纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3,
			TextureImage[0]->sizeX, TextureImage[0]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // 线形滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // 线形滤波
	}
	if (TextureImage[0])							// 纹理是否存在
	{
		if (TextureImage[0]->data)					// 纹理图像是否存在
		{
			free(TextureImage[0]->data);			// 释放纹理图像占用的内存
		}
		free(TextureImage[0]);						// 释放图像结构
	}
	return Status;	// 返回 Status
}


// Draw a square surface that looks like a
// black and white checkerboard
void COpenGLView::RenderStockScene()
{
	// define all vertices   X     Y     Z

	GLfloat v0[3], v1[3], v2[3], v3[3], delta;
	int color = 0;

	delta = 0.5f;

	// define the two colors
	GLfloat color1[3] = { 0.9f, 0.9f, 0.9f };
 	GLfloat color2[3] = { 0.05f, 0.05f, 0.05f };

	v0[1] = v1[1] = v2[1] = v3[1] = 0.0f;

	::glBegin( GL_QUADS );

	for ( int x = -5 ; x <= 5 ; x++ )
		{
		for ( int z = -5 ; z <= 5 ; z++ )
			{
			::glColor3fv( (color++)%2 ? color1 : color2 );
		
			v0[0] = 0.0f+delta*z;
			v0[2] = 0.0f+delta*x;

			v1[0] = v0[0]+delta;
			v1[2] = v0[2];

			v2[0] = v0[0]+delta;
			v2[2] = v0[2]+delta;

			v3[0] = v0[0];
			v3[2] = v0[2]+delta;

			::glVertex3fv( v0 );
			::glVertex3fv( v1 );
			::glVertex3fv( v2 );
			::glVertex3fv( v3 );
			}
		}
	::glEnd();	
	
	
}
void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int x=point.x;
	int y=point.y;
	CRect rect;
	GetClientRect(rect);
	int w=rect.right-rect.left;
	int h=rect.bottom-rect.top;


	mywave.center_wave_x=1.0*x/w*180;
	mywave.center_wave_y=180-1.0*y/h*180;
	mywave.move=TRUE;mywave.distance_wave=0;
    
	WAVE node;
	node.center_wave_x=mywave.center_wave_x;
	node.center_wave_y=mywave.center_wave_y;
	node.distance_wave=mywave.distance_wave;
	node.move=TRUE;

	wavelist.AddTail(node);
	CView::OnLButtonDown(nFlags, point);
	Invalidate(FALSE);
}












































