// Drawing.cpp: implementation of the CDrawing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Drawing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Globals
extern int IMG_WIDTH;
extern int IMG_HEIGHT;
extern int FACE_WIDTH;
extern int FACE_HEIGHT;
extern int IMG_WMARGIN;
extern int IMG_HMARGIN;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawing::CDrawing()
{
	m_dNearPlane   = 10; 
	m_dFarPlane    = 10000.0; 
	
	m_xpos = 0.0f;
	m_ypos = 0.0f;
	m_zoom = 500.0f;
	m_xrot = 0.0f;
	m_yrot = 0.0f;
	m_zrot = 0.0f;

	multitexturing=true;

}

CDrawing::~CDrawing()
{

}

/***********************************************************

  Set view project model
  
***********************************************************/
void CDrawing::SetView(CView *pView, int index)
{
	if (index == 0) {
		CRect Rect;
		pView->GetClientRect(&Rect);
		GLfloat fAspect;
		if (Rect.bottom)
			fAspect = (GLfloat)Rect.right / Rect.bottom;
		else
			fAspect = 1.0f;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, fAspect, m_dNearPlane, m_dFarPlane);
		glMatrixMode(GL_MODELVIEW);
	}else if (index == 1) {
		
		CRect rect;
		pView->GetClientRect(&rect);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, rect.right, 0.0, rect.bottom, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);		
	}
}

void CDrawing::SetRC()
{
	/*------------------------------------------------------------------------------*/
	// 1 Global ambient light
	
	GLfloat gAmbientLight[] = {0.5f,0.5f,0.5f,1.0f};		// Yellow ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gAmbientLight);	// Set light model to use ambient light specified by ambientLight[]
	/*------------------------------------------------------------------------------*/
	// 2 Set up and enable light0
	GLfloat ambientLight[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat diffuseLight[] = {1.0f,0.0f,0.7f,1.0f};
	GLfloat specular[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat lightpos[] = {0, 10, 10, 0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	
	glEnable(GL_LIGHT0);
}

void CDrawing::SpecialEffect()
{
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_POINT_SMOOTH);	
	
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);	

	/** 初始化 */
	if(!GLEnvironment::initMultiTexture())
	{
		MessageBox(NULL,_T("不支持多重纹理!"),_T("错误"),MB_OK);
		exit(-1);
	}

	/** 载入纹理 */
	if(!GLEnvironment::loadTexture())
	{
		MessageBox(NULL,_T("载入纹理错误!"),_T("错误"),MB_OK);
		exit(-1);
	}
	

//	glEnable(GL_LIGHTING);
	
//	glEnable(GL_COLOR_MATERIAL);
//	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void CDrawing::PrepareSetting(GLfloat xpos, GLfloat ypos, GLfloat xrot, GLfloat yrot, GLfloat zoom)
{
	m_zoom = zoom;
	m_xpos = xpos;
	m_ypos = ypos;
	m_xrot = xrot;
	m_yrot = yrot;
}

void CDrawing::DrawScene1()
{
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_LINE_STIPPLE);// needed
	glLineStipple(1,0x00ff);
	
	// Position the camera
	glPushMatrix();
	glTranslatef( m_xpos, -m_ypos, -(m_zoom + m_dNearPlane) );
	
	// Rotate the camera
	glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );
	
	//////////////////////////////////////////////////////////////////////////////////////
	glTranslatef(-2, 0, 0);
	glRotatef(90,1,0,0);
	
	glColor3f(0,1,0);
	//glutWireCone(10,3,8,4);
	

	glPopMatrix();
	
	glFinish();
	SwapBuffers(wglGetCurrentDC());	
}

void CDrawing::InitSetting()
{
	m_xpos = 0.0f;
	m_ypos = 0.0f;
	m_zoom = 500.0f;
	m_xrot = 0.0f;
	m_yrot = 0.0f;
}
static float wrap = 0;
void CDrawing::DrawRenderScene()
{
	/*glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_LINE_STIPPLE);// needed
	glLineStipple(1,0x00ff);
	
	// Position the camera
	glPushMatrix();
	glTranslatef( m_xpos, -m_ypos, -(m_zoom + m_dNearPlane) );
	
	// Rotate the camera
	glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );
	
	//////////////////////////////////////////////////////////////////////////////////////
	// Cube
	
	glColor3f(1,1,1);
	glutSolidTorus(2,10,5,3);
	
	glPopMatrix();

	
	glFinish();
	SwapBuffers(wglGetCurrentDC());*/

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		

	glPushMatrix();
	glTranslatef( m_xpos, -m_ypos, -(m_zoom + m_dNearPlane) );

	// Rotate the camera
	glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );


	glColor3f(1,1,1);
	glutSolidTorus(2,10,5,3);

	glPopMatrix();

	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-10.0f);


	static GLfloat constColor1[4]={1.0f,1.0f,1.0f,1.0f};
	static GLfloat constColor2[4]={0.2f,0.2f,0.2f,0.2f};
	static GLfloat constColor5[4]={0.5f,0.5f,0.5f,0.5f};
	static GLfloat constColor9[4]={0.9f,0.9f,0.9f,0.9f};
	

	
	/*detail terrain
	//激活纹理0,并绑定纹理
	GLEnvironment::glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_texture[0].ID);


	//激活纹理1,并绑定纹理
	GLEnvironment::glActiveTextureARB(GL_TEXTURE1_ARB);
	//如果多重纹理启用,则启用该纹理
	if (multitexturing) 
		glEnable(GL_TEXTURE_2D); 
	else 
		glDisable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);

	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_texture[2].ID);

	//变换纹理矩阵
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef((float)5, (float)5, 1);
	glMatrixMode(GL_MODELVIEW);
	*/
// 

// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
// 	glEnable(GL_TEXTURE_2D); 
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[4]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
// 
// 
// 
// 
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
// 	glEnable(GL_TEXTURE_2D); 
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[0]);
// 
// 
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);    //Get RGB of this texture (tex1)
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);  //Get ALPHA of previous TUI (tex0)
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[1]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);   //Interpolate RGB with RGB
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PREVIOUS);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_INTERPOLATE);   //Interpolate ALPHA with ALPHA
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_PREVIOUS);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);


// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
// 	glEnable(GL_TEXTURE_2D); 
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[0]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
// 
// 
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
// 	glEnable(GL_TEXTURE_2D); 
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[1]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE0);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE3);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
// 
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, GLEnvironment::m_textureId[3]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE3);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
// 
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE3);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, GLEnvironment::m_textureId[4]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_NONE);

//
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, GLEnvironment::m_textureId[5]);	
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_NONE); 

#if 0
	GLfloat color[4];
	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	color[0]=1;
	color[1]=1;
	color[2]=1;
	color[3]=1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, 0.f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,1);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	color[0]=1;
	color[1]=1;
	color[2]=1;
	color[3]=1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, 0.f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,1);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	color[0]=1;
	color[1]=1;
	color[2]=1;
	color[3]=1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	color[0]=0;
	color[1]=0;
	color[2]=0;
	color[3]=1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, 0.f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,1);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_FOG);
	glDisable(GL_NORMALIZE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glPointSize(1);
	glLineWidth(1);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
	glDisable(GL_MULTISAMPLE_ARB);



	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#endif

	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[0]);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[1]);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[3]);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_textureId[4]);


	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	//glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,constColor5);
	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR);


	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

	GLEnvironment::glActiveTextureARB(GL_TEXTURE3);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE3);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, GLEnvironment::m_textureId[4]);
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_NONE);

	










	/** 绘制一个四方形墙面 */
	//glPushMatrix();
	

	/** 左上点 */

	glTranslatef(-2.5, 0, 0);
	glScalef(2.0f,2.0f,2.0f);
	glBegin(GL_QUADS);
	

	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0, 0.0f, 1.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1, 0.0f, 1.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE2, 0.0f, 1.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE3, 0.0f, 1.0f);
	glColor4f(1,1,1,0.0f);
	//glColor4f(1.0f,1.0f,1.0f,1.0f);
	glVertex3f(-1, 1, 0);

	/** 左下点 */
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0, 0.0f, 0.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1, 0.0f, 0.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE2, 0.0f, 0.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE3, 0.0f, 0.0f);
	glColor4f(0.0f,0.0f,0.0f,1);
	//glColor4f(1.0f,1.0f,1.0f,1.0f);
	glVertex3f(-1, -1, 0);

	/** 右下点 */
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0, 1.0f, 0.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1, 1.0f, 0.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE2, 1.0f, 0.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE3, 1.0f, 0.0f);
	glColor4f(1,1,1,0.0f);
	//glColor4f(0.1f,0.1f,0.1f,0.1f);
	glVertex3f(1, -1, 0);

	/** 右上点 */
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0, 1.0f, 1.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1, 1.0f, 1.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE2, 1.0f, 1.0f);
	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE3, 1.0f, 1.0f);
	glColor4f(0,0,0,0);
	//glColor4f(0.1f,0.1f,0.1f,0.1f);
	glVertex3f(1, 1, 0);

	glEnd();    /**< 绘制结束 */		

	GLEnvironment::glActiveTextureARB(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D); 
	GLEnvironment::glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D); 
	GLEnvironment::glActiveTextureARB(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D); 
	GLEnvironment::glActiveTextureARB(GL_TEXTURE3);
	glDisable(GL_TEXTURE_2D); 
	//glPopMatrix();


// 	/** 激活纹理0,并绑定纹理 */
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE0_ARB);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_texture[2].ID);
// 
// 	/** 激活纹理1,并绑定纹理 */
// 	GLEnvironment::glActiveTextureARB(GL_TEXTURE1_ARB);
// 
// 	/** 如果多重纹理启用,则启用该纹理 */
// 	if (multitexturing) 
// 		glEnable(GL_TEXTURE_2D); 
// 	else 
// 		glDisable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D,  GLEnvironment::m_texture[3].ID);
// 
// 	static float wrap = 0;      /**< 用于雾的流动 */     
// 
// 	glTranslatef(2.5, 2.5, 0);
// 	glScalef(2.0f,2.0f,2.0f);
// 	glBegin(GL_QUADS);
// 
// 	/** 左上点 */
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 1.0f);
// 	glColor4f(0.9f,0.9f,0.9f,0.9f);
// 	glVertex3f(-1, 1, 0);
// 
// 	/** 左下点 */
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 0.0f);
// 	glColor4f(0.9f,0.9f,0.9f,0.9f);
// 	glVertex3f(-1, -1, 0);
// 
// 	/** 右下点 */
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 0.0f);
// 	glColor4f(0.1f,0.1f,0.1f,0.1f);
// 	glVertex3f(1, -1, 0);
// 
// 	/** 右上点 */
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
// 	GLEnvironment::glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 1.0f);
// 	glColor4f(0.1f,0.1f,0.1f,0.1f);
// 	glVertex3f(1, 1, 0);
// 	glEnd();											
// 
// 	wrap += 0.001f;                   /**< 递增 */

	glFinish();
	SwapBuffers(wglGetCurrentDC());
}

void CDrawing::DrawTestScene()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glEnable(GL_LINE_STIPPLE);// needed
	glLineStipple(1,0x00ff);

	// Position the camera
	glPushMatrix();
	glTranslatef( m_xpos, -m_ypos, -(m_zoom + m_dNearPlane) );

	// Rotate the camera
	glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );

	//////////////////////////////////////////////////////////////////////////////////////
	// Cube
	//glColor3f(0,1,1);
	glutSolidTeapot(10);


	glPopMatrix();
	
	
	glFinish();
	SwapBuffers(wglGetCurrentDC());	
}

void CDrawing::DrawScene4()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_LINE_STIPPLE);// needed
	glLineStipple(1,0x00ff);
	
	// Position the camera
	glPushMatrix();
	glTranslatef( m_xpos, -m_ypos, -(m_zoom + m_dNearPlane) );
	
	// Rotate the camera
	glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );
	
	//////////////////////////////////////////////////////////////////////////////////////
	// Cube
	glColor3f(0,1,1);
	//glutWireTeapot(10);
	

	glPopMatrix();
	
	glFinish();
	SwapBuffers(wglGetCurrentDC());
}
