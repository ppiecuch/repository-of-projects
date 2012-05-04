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



TCHAR szWindowClass[100] = TEXT("GLES2");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;



//业务相关
/* 3D data. Vertex range -0.5..0.5 in all axes.
* Z -0.5 is near, 0.5 is far. */
GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f,  // Position 0
0.0f,  1.0f,        // TexCoord 0 
-0.5f, -0.5f, 0.0f,  // Position 1
0.0f,  0.0f,        // TexCoord 1
0.5f, -0.5f, 0.0f,  // Position 2
1.0f,  0.0f,        // TexCoord 2
0.5f,  0.5f, 0.0f,  // Position 3
1.0f,  1.0f         // TexCoord 3
};
GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

GLint iLocPosition = 0;
GLint iLocColour, iLocTexCoord;
GLint iLocRockSampler,iLocDirtSampler,iLocGrassSampler,iLocBlendSampler;
GLint iLocMVP = -1;
GLuint rockTexId,dirtTexId,grassTexId,blendTexId;
ESMatrix  mvpMatrix;
CTexture* pTexture0=NULL;
CTexture* pTexture1=NULL;
CTexture* pTexture2=NULL;
CTexture* pTexture3=NULL;

const unsigned int uiWidth  = 640;
const unsigned int uiHeight = 480;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg,end;
#define FRAME 10
unsigned int frame=0;



const char* pszVertShader= 
"uniform mat4 u_mvpMatrix;					\n"
"attribute vec4 a_position;					\n"
"attribute vec2 a_texCoord;					\n"
"varying vec2 v_texCoord;					\n"
"void main()								\n"
"{											\n"
"   gl_Position = u_mvpMatrix*a_position;	\n"
"   v_texCoord = a_texCoord;				\n"
"}											\n";
const char* pszFragShader=
"precision mediump float;                            \n"
"varying vec2 v_texCoord;                            \n"
"uniform sampler2D s_rock;							 \n"
"uniform sampler2D s_dirt;	                         \n"
"uniform sampler2D s_grass;	                         \n"
"uniform sampler2D s_blend;	                         \n"
"void main()                                         \n"
"{                                                   \n"
"  vec4 rockColor;                                   \n"
"  vec4 dirtColor;                                   \n"
"  vec4 grassColor;                                  \n"
"  vec4 blendColor;                                  \n"
"                                                    \n"
"  rockColor = texture2D( s_rock, v_texCoord );      \n"
"  dirtColor = texture2D( s_dirt, v_texCoord );      \n"
"  grassColor = texture2D( s_grass, v_texCoord );    \n"
"  blendColor = texture2D( s_blend, v_texCoord );    \n"
"  gl_FragColor = (rockColor*blendColor.r+dirtColor*blendColor.g+grassColor*blendColor.b)*blendColor.a;   \n"
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
		szWindowClass, TEXT("OpenGL ES1 Framework"), 
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
	GLint maxTextureImageUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxTextureImageUnits);
	printf("maxTextureImageUnits:%d\n", maxTextureImageUnits);

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
	iLocPosition = glGetAttribLocation (uiProgramObject, "a_position" );
	iLocTexCoord = glGetAttribLocation (uiProgramObject, "a_texCoord" );

	// Get the sampler location
	iLocRockSampler = glGetUniformLocation (uiProgramObject, "s_rock" );
	iLocDirtSampler = glGetUniformLocation (uiProgramObject, "s_dirt" );
	iLocGrassSampler = glGetUniformLocation (uiProgramObject, "s_grass" );
	iLocBlendSampler = glGetUniformLocation (uiProgramObject, "s_blend" );

	// Load the textures
	//rockTexId = LoadTexture ("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\rock.tga");
	//dirtTexId = LoadTexture ("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\dirt.tga");
	//grassTexId = LoadTexture ("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\grass.tga");
	//blendTexId = LoadTexture ("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\blend.tga");
	// Load the texture
	pTexture0=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\rock.png");
	pTexture1=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\dirt.png");
	pTexture2=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\brick.png");
	pTexture3=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\14ES2MultiTexture\\blend.png");


	glVertexAttribPointer (iLocPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices );
	glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );
	glEnableVertexAttribArray (iLocPosition);
	glEnableVertexAttribArray (iLocTexCoord);

	//void glUniform1i( GLint location,GLint v0);
	//location
	//Specifies the location of the uniform variable to be modified.
	//v0
	//Specifies the new values to be used for the specified uniform variable.
	//glUniform1i and glUniform1iv are the only two functions that may be used to load uniform variables defined as sampler types. 
	//Loading samplers with any other function will result in a GL_INVALID_OPERATION error.
	// glUniform1i(uniforms[UNIFORM_SAMPLER], 0);这个采样器的值是与前面所指定的纹理单元号对应的。
	//由于我们前面使用GL_TEXTURE0，因此这里用0。如果这里写为0以外的其它值，那么图像将无法被显示出来。
	//当然，如果我们用了纹理单元1，即GL_TEXTURE1，那么我们这里要使用glUniform1i(uniforms[UNIFORM_SAMPLER], 1)。
	glUniform1i (iLocRockSampler, 0 );
	glUniform1i (iLocDirtSampler, 1 );
	glUniform1i (iLocGrassSampler, 2 );
	glUniform1i (iLocBlendSampler, 3 );

	GL_CHECK

#define  LINEAR 1
#define WRAP GL_CLAMP_TO_EDGE

	// Bind the rock texture
	glActiveTexture (GL_TEXTURE0);
	//glBindTexture (GL_TEXTURE_2D, rockTexId);
	pTexture0->bind();
#if LINEAR
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,WRAP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,WRAP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#else
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif
	
	

	// Bind the dirt texture
	glActiveTexture ( GL_TEXTURE1 );
	//glBindTexture ( GL_TEXTURE_2D, dirtTexId);
	pTexture1->bind();
#if LINEAR
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,WRAP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,WRAP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#else
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif


	// Bind the grass texture
	glActiveTexture ( GL_TEXTURE2 );
	//glBindTexture ( GL_TEXTURE_2D, grassTexId);
	pTexture2->bind();
#if LINEAR
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,WRAP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,WRAP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#else
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif


	// Bind the blend texture
	glActiveTexture ( GL_TEXTURE3 );
	//glBindTexture ( GL_TEXTURE_2D, blendTexId);
	pTexture3->bind();
#if LINEAR
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,WRAP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,WRAP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#else
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif
	glViewport(0,0,uiWidth,uiHeight);
	glClearColor(0.6f,0.8f,1.0f,1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GL_CHECK
}

void DrawFrame()
{
	static ESMatrix perspective;
	static ESMatrix modelview;
	static float aspect = (float)uiWidth/uiHeight;
	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 60.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,0,5,0,0,-50,0,1,0);
	esScale(&modelview,4,4,4);
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );


	if (!eglSwapBuffers(m_eglDisplay, m_eglSurface)) {
		printf("Failed to swap buffers.\n");
	}
}

void DestroyGL()
{
	//glDeleteTextures(1,&rockTexId);
	//glDeleteTextures(1,&dirtTexId);
	//glDeleteTextures(1,&grassTexId);
	//glDeleteTextures(1,&blendTexId);
	if(pTexture0)
		delete pTexture0;
	if(pTexture1)
		delete pTexture1;
	if(pTexture2)
		delete pTexture2;
	if(pTexture3)
		delete pTexture3;

	glUseProgram(NULL);
	glDeleteProgram(uiProgramObject);
	glDeleteShader(uiVertShader);
	glDeleteShader(uiFragShader);
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
			sprintf_s(buffer,"OpenGL ES2 Framework(fps: %.2f)", (float)(FRAME+1)/total_time);
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