/************************************************************************/
/* Simple_Texture2D
// This is a simple example that draws a quad with a 2D
// texture image. The purpose of this example is to demonstrate 
// the basics of 2D texturing 
/************************************************************************/
//加载Window程序需要的头文件

#include "config.h"

#include<windows.h>
#include <stdio.h>

#include "Texture.h"



TCHAR szWindowClass[100] = TEXT("GLES2");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;



//业务相关
/* 3D data. Vertex range -0.5..0.5 in all axes.
* Z -0.5 is near, 0.5 is far. */
GLfloat vVertices[] = { -1.5f,  1.5f, 0.0f,  // Position 0
0.0f,  1.0f,        // TexCoord 0 
-1.5f, -1.5f, 0.0f,  // Position 1
0.0f,  0.0f,        // TexCoord 1
1.5f, -1.5f, 0.0f,  // Position 2
1.0f,  0.0f,        // TexCoord 2
1.5f,  1.5f, 0.0f,  // Position 3
1.0f,  1.0f         // TexCoord 3
};

//GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

GLfloat *vertices=NULL;
GLushort *indices=NULL;
GLfloat *texcoords=NULL;
int     numIndices;
GLubyte cubeIndices[] =
{
	0, 2, 1,
	0, 3, 2, 
	4, 5, 6,
	4, 6, 7,
	8, 9, 10,
	8, 10, 11, 
	12, 15, 14,
	12, 14, 13, 
	16, 17, 18,
	16, 18, 19, 
	20, 23, 22,
	20, 22, 21
};

int rotY=0;

GLint iLocPosition = 0;
GLint iLocColour, iLocTexCoord;
GLint iLocSampler0;
GLint iLocSampler1;
GLint iLocMVP = -1;
ESMatrix  mvpMatrix;
CTexture* pTexture0=NULL;
CTexture* pTexture1=NULL;

GLint iLocMV=-1;
GLint iLocTG=-1;
GLint iLocIV=-1;
ESMatrix mvMatrix;
ESMatrix tgMatrix;
ESMatrix ivMatrix;

const unsigned int uiWidth  = 640;
const unsigned int uiHeight = 480;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg,end;
#define FRAME 10
unsigned int frame=0;



const char* pszVertShader= 
"uniform mat4 u_tgMatrix;						\n"
"uniform mat4 u_mvpMatrix;					\n"
"attribute vec4 a_position;					\n"
"attribute vec2 a_texCoord;					\n"
"varying vec2 v_texCoord0;					\n"
"varying vec4 v_texCoord1;					\n"
"void main()								\n"
"{											\n"
"   gl_Position = u_mvpMatrix * a_position;	\n"
"   v_texCoord0 = a_texCoord;					\n"
"  v_texCoord1 = u_tgMatrix * a_position;       \n"
"}											\n";
const char* pszFragShader=
"precision mediump float;                            \n"
"varying vec2 v_texCoord0;					\n"
"varying vec4 v_texCoord1;					\n"
"uniform sampler2D s_texture0;                        \n"
"uniform sampler2D s_texture1;                        \n"
"void main()                                         \n"
"{                                                   \n"
"     vec4 color0 = texture2D(s_texture0,v_texCoord0);\n"
"     vec4 color1 = v_texCoord1.w<0.0 ? vec4(1.0) : texture2DProj( s_texture1, v_texCoord1 );   \n"//(只对透视投影的投影机有效)
"     //vec4 color1 = texture2DProj( s_texture1, v_texCoord1 );   \n"
"     gl_FragColor = color0*color1; \n"
"}                                                   \n";


GLuint uiVertShader;
GLuint uiFragShader;
GLuint uiProgramObject;


LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	switch(uiMsg) {
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SIZE:
			return 0;
	}
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

BOOL InitWindow(HWND *result,int width, int height)
{
	WNDCLASS wc;
	RECT wRect;
	HWND hwnd;
	HINSTANCE hInstance;

	wRect.left = 0L;
	wRect.right = (long)width;
	wRect.top = 0L;
	wRect.bottom = (long)height;

	hInstance = GetModuleHandle(NULL);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWindowClass;

	RegisterClass(&wc);

	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		szWindowClass, TEXT("OpenGL ES2"), 
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
		0, 0, 
		wRect.right-wRect.left, wRect.bottom-wRect.top, 
		NULL, NULL, 
		hInstance, 
		NULL
		);

	if (!hwnd)
	{
		MessageBox(NULL,TEXT("Error in CreateWindow()!"),TEXT("Error"),MB_OK);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	*result = hwnd;

	return true;
}

BOOL InitEGL(HWND& hwnd)
{
	EGLint attribs[] =
	{
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 5,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, 1,
		EGL_BUFFER_SIZE, 16,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_DEPTH_SIZE, 1,
		EGL_STENCIL_SIZE, false,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_SAMPLES, 0,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE, 0
	};
	EGLConfig config;
	EGLint num_configs;
	HDC hdc = GetDC(hwnd);
	m_eglDisplay = eglGetDisplay(hdc);
	EGLint majorVersion, minorVersion;
	if ( !eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
	{
		MessageBox(NULL,TEXT("Could not initialize OpenGL-ES2 display."),TEXT("Error"),MB_OK);
		return false;
	}
	else
	{
		printf("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
	}

	if (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs))
	{
		MessageBox(NULL,TEXT("Could not get config for OpenGL-ES2 display."),TEXT("Error"),MB_OK);
		return false;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, hwnd, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, NULL, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
	{
		MessageBox(NULL,TEXT("Could not create surface for OpenGL-ES2 display."),TEXT("Error"),MB_OK);
		return false;
	}

	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);

	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

	GLubyte* version =  (GLubyte*)glGetString(GL_VERSION);
	printf("gl version:%s\n", version);
	const char* eglClient=eglQueryString(m_eglDisplay, EGL_CLIENT_APIS);
	printf("egl client api:%s\n", eglClient);
	GLubyte* vendorName = (GLubyte*)glGetString( GL_VENDOR );
	printf("verdor name:%s\n", vendorName);

	return true;

}

void DestroyEGL()
{
	eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglDestroySurface(m_eglDisplay, m_eglSurface);
	eglTerminate(m_eglDisplay);
}

void InitGL()
{	
	uiVertShader = glCreateShader(GL_VERTEX_SHADER);
	uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(uiVertShader,1,(const char**)&pszVertShader,NULL);
	glShaderSource(uiFragShader,1,(const char**)&pszFragShader,NULL);

	glCompileShader(uiVertShader);
	glCompileShader(uiFragShader);

	GLint iShaderCompiled;
	glGetShaderiv(uiVertShader,GL_COMPILE_STATUS,&iShaderCompiled);
	if(!iShaderCompiled)
	{
		int i32LogLength,i32CharsWritten;
		glGetShaderiv(uiVertShader,GL_INFO_LOG_LENGTH,&i32LogLength);

		char* pszLog = new char[i32LogLength];
		glGetShaderInfoLog(uiVertShader,i32LogLength,&i32CharsWritten,pszLog);

		MessageBoxA(NULL,"Failed to compile vertex shader:%s",pszLog,MB_OK);

		delete[] pszLog;
		return;
	}

	glGetShaderiv(uiFragShader,GL_COMPILE_STATUS,&iShaderCompiled);
	if(!iShaderCompiled)
	{
		int i32LogLength,i32CharsWritten;
		glGetShaderiv(uiFragShader,GL_INFO_LOG_LENGTH,&i32LogLength);
		char* pszLog = new char[i32LogLength];
		glGetShaderInfoLog(uiFragShader,i32LogLength,&i32CharsWritten,pszLog);
		MessageBoxA(NULL,"Failed to compile fragment shader:%s",pszLog,MB_OK);
		delete[] pszLog;
		return;
	}

	uiProgramObject = glCreateProgram();

	glAttachShader(uiProgramObject,uiFragShader);
	glAttachShader(uiProgramObject,uiVertShader);

	glLinkProgram(uiProgramObject);

	GLint iLinked;
	glGetProgramiv(uiProgramObject,GL_LINK_STATUS,&iLinked);
	if(!iLinked)
	{
		int ui32LogLength,ui32CharsWritten;
		glGetProgramiv(uiProgramObject,GL_INFO_LOG_LENGTH,&ui32LogLength);
		char* pszLog = new char[ui32LogLength];
		glGetProgramInfoLog(uiProgramObject,ui32LogLength,&ui32CharsWritten,pszLog);
		MessageBoxA(NULL,"Failed to link program:%s",pszLog,MB_OK);
		delete[] pszLog;
		return;
	}

	glUseProgram(uiProgramObject);


	// Get the attribute locations
	iLocMVP=glGetUniformLocation(uiProgramObject, "u_mvpMatrix");
	//iLocMV=glGetUniformLocation(uiProgramObject, "u_mvMatrix");
	iLocTG=glGetUniformLocation(uiProgramObject, "u_tgMatrix");
	//iLocIV=glGetUniformLocation(uiProgramObject, "u_ivMatrix");
	iLocPosition = glGetAttribLocation (uiProgramObject, "a_position" );
	iLocTexCoord = glGetAttribLocation (uiProgramObject, "a_texCoord" );

	// Get the sampler location
	iLocSampler0 = glGetUniformLocation (uiProgramObject, "s_texture0" );
	iLocSampler1 = glGetUniformLocation (uiProgramObject, "s_texture1" );

	// Load the texture
	pTexture0=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\22ES2ProjectiveTexture\\wood.png");
	pTexture1=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\22ES2ProjectiveTexture\\test.png");


	//glVertexAttribPointer (iLocPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices );
	//glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );
	numIndices = genCube(1.5f, &vertices,NULL, &texcoords, &indices );
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertices);
	glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), texcoords);

	glEnableVertexAttribArray (iLocPosition);
	glEnableVertexAttribArray (iLocTexCoord);

	//注意：其实第1和3句调用其实并不是必须的，有时候，你可能会看见别人的代码里面可能并没有包含这几行代码。
	//这是因为我们假设GL_TEXTURE0就是当前激活的纹理单元了，我们也不需要设置uniform，因为它默认就是0.
	//我在这篇教程中添加这3行代码的意思是方便初学者更好地理解代码
	//void glUniform4i(GLint location,  GLint v0,  GLint v1,  GLint v2,  GLint v3); 
	//location
	//Specifies the location of the uniform variable to be modified.
	//v0, v1, v2, v3
	//Specifies the new values to be used for the specified uniform variable.
	glActiveTexture ( GL_TEXTURE0 );
	pTexture0->bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glUniform1i(iLocSampler0, 0);

	glActiveTexture ( GL_TEXTURE1 );
	pTexture1->bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glUniform1i(iLocSampler1, 1);

	glViewport(0,0,uiWidth,uiHeight);
	glClearColor(0.6f,0.8f,1.0f,1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	esMatrixLoadIdentity( &ivMatrix );
	esLookAt(&ivMatrix,0,0,3,0,0,0,0,1,0);
	esMatrixInverse(&ivMatrix,&ivMatrix);

}

void DrawFrame()
{
	static ESMatrix perspective;
	static ESMatrix modelview;
	static float aspect = (float)uiWidth/uiHeight;
	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 45.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,0,3,0,0,0,0,1,0);

	esMatrixLoadIdentity(&mvMatrix);
	esRotate(&mvMatrix,rotY,0,1,0);
	esMatrixMultiply(&modelview,&mvMatrix,&modelview);

	

	
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);

	
	esMatrixLoadIdentity(&tgMatrix);
	esTranslate(&tgMatrix,0.5f,0.5f,0.5f);
	esScale(&tgMatrix,0.5f,0.5f,0.5f);
	esPerspective(&tgMatrix,45.0f,1,1,2);
	//esOrtho(&tgMatrix,-0.235f,0.235f,-0.235f,0.235f,0.1f,0.2f);
	esLookAt(&tgMatrix,0,0,0,-1,0,-1,0,1,0);
	esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
	//等同于下面两句
	//esMatrixMultiply(&tgMatrix,&ivMatrix,&tgMatrix);
	//esMatrixMultiply(&tgMatrix,&modelview,&tgMatrix);
	glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,indices);

	rotY+=1;

	if (!eglSwapBuffers(m_eglDisplay, m_eglSurface)) {
		printf("Failed to swap buffers.\n");
	}
}

void DestroyGL()
{
	if(pTexture0)
		delete pTexture0;

	if(pTexture1)
		delete pTexture1;

	glUseProgram(NULL);
	glDeleteProgram(uiProgramObject);
	glDeleteShader(uiVertShader);
	glDeleteShader(uiFragShader);

	free(vertices);
	free(texcoords);
	free(indices);
}

int main(void) {

	EnableMemLeakCheck();

	HWND hwnd;
	MSG sMessage;
	bool bDone = false;
	char buffer[128];

	InitWindow(&hwnd,uiWidth,uiHeight);

	InitEGL(hwnd);
	InitGL();

	while(!bDone)
	{
		if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE))
		{
			if(sMessage.message == WM_QUIT)
			{
				bDone = true;
			}
			else 
			{
				TranslateMessage(&sMessage);
				DispatchMessage(&sMessage);
			}
		}

		if(frame==0)
			_ftime64_s(&beg); 
		DrawFrame();
		if(frame==FRAME)
		{
			_ftime64_s(&end); 
			total_time = (float)(end.time - beg.time) + (float)(end.millitm - beg.millitm) / 1000;
			sprintf_s(buffer,"OpenGL ES2 ProjectiveTexture(fps: %.2f)", (float)(FRAME+1)/total_time);
			SetWindowTextA(hwnd,buffer);
			frame=0;
		}
		else
		{
			++frame;
		}

		Sleep(20);
	}

	DestroyGL();
	DestroyEGL();
	DestroyWindow(hwnd);

	return 0;
}