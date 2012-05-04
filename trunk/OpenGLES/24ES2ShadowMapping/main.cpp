/************************************************************************/
/* Simple_Texture2D
// This is a simple example that draws a quad with a 2D
// texture image. The purpose of this example is to demonstrate 
// the basics of 2D texturing 
/************************************************************************/
//加载Window程序需要的头文件

//加载OpenGL ES2需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include<windows.h>
#include <stdio.h>

#include "esMatrix.h"
#include "global.h"
#include "Texture.h"
#include "ShadowTexture.h"
#pragma comment(lib, "Math.lib")
#pragma comment(lib, "Utility.lib")
#pragma comment(lib, "Texture.lib")

TCHAR szWindowClass[100] = TEXT("GLES2");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;



//业务相关
/* 3D data. Vertex range -0.5..0.5 in all axes.
* Z -0.5 is near, 0.5 is far. */
GLfloat rectVertices[] = { -1.5f,  -0.6f, 1.5f,  // Position 0
0.0f,  1.0f,        // TexCoord 0 
-1.5f, -0.6f, -1.5f,  // Position 1
0.0f,  0.0f,        // TexCoord 1
1.5f, -0.6f, -1.5f,  // Position 2
1.0f,  0.0f,        // TexCoord 2
1.5f,  -0.6f, 1.5f,  // Position 3
1.0f,  1.0f         // TexCoord 3
};

GLushort rectIndices[] = { 0, 2, 1, 0, 3, 2 };

float *cubeVertices=NULL;
unsigned short *cubeIndices=NULL;
float *cubeTexcoords=NULL;
int     cubeNumIndices;

float *sphereVertices=NULL;
unsigned short *sphereIndices=NULL;
float *sphereTexcoords=NULL;
int     sphereNumIndices;

float *fishVertices=NULL;
unsigned short *fishIndices=NULL;
float *fishTexcoords=NULL;
int     fishNumIndices;


int rotY=0;
float lightPos[3]={-1.5f,1.5f,0};

GLint iLocPosition = 0;
GLint iLocColour, iLocTexCoord;
GLint iLocSampler0;
GLint iLocSampler1;
GLint iLocDepthMode;
GLint iLocMVP = -1;
ESMatrix  mvpMatrix;
ShadowTexture* pTexture0=NULL;
CTexture* pTexture1=NULL;
CTexture* pTexture2=NULL;
CTexture* pTexture3=NULL;
CTexture* pTexture4=NULL;
bool first=true;
bool depthMode=true;

GLint iLocMV=-1;
GLint iLocTG=-1;
GLint iLocIV=-1;
ESMatrix mvMatrix;
ESMatrix tgMatrix;
ESMatrix ivMatrix;

const unsigned int uiWidth  = 512;
const unsigned int uiHeight = 512;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg,end;
#define FRAME 10
unsigned int frame=0;


const char* pszShadowVertShader=
"attribute vec4 a_position; 	\n"
"attribute vec2 a_texCoord0;	\n"
"attribute vec3 a_normal;	\n"
"										\n"
"uniform vec3 lightPos;	\n"
"uniform vec3 camPos;	\n"
"uniform mat4 u_projectionViewMatrix;	\n"
"										\n"
"uniform mat4 shadowProjMatrix;			\n"
"										\n"
"varying vec2 v_texCoords;				\n"
"varying vec3 v_normal;					\n"
"varying vec3 v_lightDir;				\n"
"varying vec3 v_eye;					\n"
"										\n"
"varying vec4 shadowCoord;				\n"
"										\n"
"void main()							\n"
"{										\n"
"										\n"
"	vec4 shadowPos = shadowProjMatrix *  a_position;	\n"
"	//shadow map coodinates are mapped from [-1:1] to [0:1], fragment shader do the final division with w	\n"
"	shadowCoord = vec4((shadowPos.xyz + shadowPos.w) / 2.0,shadowPos.w);	\n"
"	\n"
"	v_texCoords = a_texCoord0;      	\n"
"	v_normal    = a_normal;	\n"
"	vec3 v_pos  = a_position.xyz;	\n"
"	v_lightDir  = lightPos - v_pos;	\n"
"	v_eye       = camPos   - v_pos;	\n"
"	\n"
"	gl_Position = u_projectionViewMatrix * a_position;	\n"
"}	\n";

const char* pszShadowFragShader=
"// Pixel shader to generate the Depth Map	\n"
"// Used for shadow mapping - generates depth map from the light's viewpoint precision highp float;	\n"
"varying vec4 position;	\n"
"// taken from Fabien Sangalard's DEngine - this is used to pack the depth into a 32-bit texture (GL_RGBA)	\n"
"vec4 pack (float depth)												\n"
"{																		\n"
"		const vec4 bitSh = vec4(256.0 * 256.0 * 256.0,	\n"
"	                       256.0 * 256.0,	\n"
"	                       256.0,	\n"
"	                      1.0);	\n"
"	    const vec4 bitMsk = vec4(0,	\n"
"	                         1.0 / 256.0,	\n"
"	                         1.0 / 256.0,	\n"
"	                             1.0 / 256.0);	\n"
"	    vec4 comp = fract(depth * bitSh);	\n"
"	    comp -= comp.xxyz * bitMsk;	\n"
"	    return comp;	\n"
"	}																	\n"
"	 																	\n"
"	void main() {	\n"
"	    // the depth	\n"
"	    float normalizedDistance  = position.z / position.w;	\n"
"	        // scale it from 0-1	\n"
"	    normalizedDistance = (normalizedDistance + 1.0) / 2.0;	\n"
"	     	\n"
"	    // bias (to remove artifacts)	\n"
"	    normalizedDistance += 0.0005;	\n"
"	 	\n"
"	    // pack value into 32-bit RGBA texture	\n"
"	    gl_FragColor = pack(normalizedDistance);	\n"
"	     	\n"
"	}	\n";


const char* pszGenShadowVertShader= 
"uniform mat4 u_mvpMatrix;					\n"
"uniform mat4 u_tgMatrix;					\n"
"uniform bool u_depthMode=true;					\n"
"attribute vec4 a_position;					\n"
"attribute vec2 a_texCoord;					\n"
"varying vec2 v_texCoord0;					\n"
"varying vec4 v_texCoord1;					\n"
"void main()								\n"
"{											\n"
"	if(u_depthMode){										\n"
"   gl_Position = u_tgMatrix * a_position;	\n"
"   v_texCoord0 = a_texCoord;					\n"
"   v_texCoord1 = gl_Position;       \n"
"	}else{											\n"
"   gl_Position = u_mvpMatrix * a_position;	\n"
"   v_texCoord0 = a_texCoord;					\n"
"   v_texCoord1 = u_tgMatrix * a_position;       \n"
"	}											\n"
"}											\n";
const char* pszGenShadowFragShader=
"precision highp float;                            \n"
"uniform bool u_depthMode=true;					\n"
"varying vec2 v_texCoord0;							\n"
"varying vec4 v_texCoord1;							\n"
"uniform sampler2D s_texture0;                        \n"
"uniform sampler2D s_texture1;                        \n"
"const float fromFixed = 256.0/255;              \n"
"//#define ABGR \n"
"#ifdef ABGR \n"
"const vec4 factorF = vec4( fromFixed / (256.0 * 256.0 * 256.0), fromFixed / (256.0 * 256.0), fromFixed / 256.0, fromFixed );\n"
"float unpack(in vec4 value)                                              \n"
"{                                                                        \n"
"   float res = dot(value, factorF);                                      \n"
"   return res;                                                           \n"
"}                                                                        \n"
"vec4 pack(float depth)                                                   \n"
"{                                                                        \n"
"   const vec4 bitSh = vec4(256.0*256.0*256.0, 256.0*256.0,   256.0, 1.0);\n"
"   const vec4 bitMsk = vec4(0, 1.0/256.0, 1.0/256.0, 1.0/256.0);         \n"
"   vec4 comp = vec4(fract(depth * bitSh));                               \n"
"   comp -= comp.xxyz * bitMsk;                                           \n"
"   return comp;                                                          \n"
"}                                                                        \n"
"#else //using RGBA \n"
"const vec4 factorF = vec4( fromFixed , fromFixed / 256.0, fromFixed / (256.0 * 256.0), fromFixed / (256.0 * 256.0 * 256.0));\n"
"float unpack(in vec4 value)                                              \n"
"{                                                                        \n"
"   float res = dot(value.wzyx, factorF);                                      \n"
"   return res;                                                           \n"
"}                                                                        \n"
"vec4 pack(float depth)                                                              \n"
"{                                                                                   \n"
"   const vec4 bitSh = vec4( 1.0, 256.0,  256.0*256.0, 256.0*256.0*256.0);           \n"
"   const vec4 bitMsk = vec4(1.0/256.0, 1.0/256.0, 1.0/256.0, 0);                    \n"
"   vec4 comp = vec4(fract(depth * bitSh));                                          \n"
"   comp -= comp.yzww * bitMsk;                                                      \n"
"   return comp.wzyx;//(如果不进行分量反转(unpack对应处理)，显示不正确，目前原因未明)\n"
"}                                                                                   \n"


"#endif \n"

"void main()														\n"
"{																	\n"
"	if(u_depthMode){										\n"
"	    float normalizedDistance  = v_texCoord1.z / v_texCoord1.w;	\n"
"	    normalizedDistance = (normalizedDistance + 1.0) / 2.0;		\n"
"	    // bias (to remove artifacts)	\n"
"	    normalizedDistance += 0.0005;	\n"
"	 	\n"
"	    // pack value into 32-bit RGBA texture	\n"
"	    gl_FragColor = pack(normalizedDistance);	\n"
"	}else{											\n"
"	    vec4 texcoord  = v_texCoord1 / v_texCoord1.w;	\n"
"	    texcoord  = (texcoord + 1.0) / 2.0;	\n"
"	    float d  = texcoord.z;	\n"
"	    vec4 sampledTex   = texture2D(s_texture0,texcoord.st);	\n"
"       float r = unpack(sampledTex);			\n"
"	    float result  = r<(d-0.008)?0.7:1.0;	\n"
"	    vec4 color  = texture2D(s_texture1,v_texCoord0);	\n"
"       gl_FragColor = vec4(result*color.rgb,1.0); \n"
"	}											\n"
"}                                                    \n";


const char* pszVertShader= 
"uniform mat4 u_tgMatrix;					\n"
"uniform mat4 u_mvpMatrix;					\n"
"attribute vec4 a_position;					\n"
"attribute vec2 a_texCoord;					\n"
"varying vec2 v_texCoord0;					\n"
"varying vec4 v_texCoord1;					\n"
"void main()								\n"
"{											\n"
"   gl_Position = u_mvpMatrix * a_position;	\n"
"   v_texCoord0 = a_texCoord;					\n"
"   v_texCoord1 = u_tgMatrix * a_position;       \n"
"}											\n";
const char* pszFragShader=
"precision mediump float;                            \n"
"varying vec2 v_texCoord0;							\n"
"varying vec4 v_texCoord1;							\n"
"uniform sampler2D s_texture0;                        \n"
"uniform sampler2D s_texture1;                        \n"
"void main()                                         \n"
"{                                                   \n"
"     vec4 color0 = texture2D(s_texture0,v_texCoord0);\n"
"     vec4 color1 = v_texCoord1.w<0.0 ? vec4(1.0) : texture2DProj( s_texture1, v_texCoord1 );   \n"//(只对透视投影的投影机有效)
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
		EGL_DEPTH_SIZE, 16,
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

	glShaderSource(uiVertShader,1,(const char**)&pszGenShadowVertShader,NULL);
	glShaderSource(uiFragShader,1,(const char**)&pszGenShadowFragShader,NULL);

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
	iLocTG=glGetUniformLocation(uiProgramObject, "u_tgMatrix");
	iLocDepthMode=glGetUniformLocation(uiProgramObject, "u_depthMode");
	iLocPosition = glGetAttribLocation (uiProgramObject, "a_position" );
	iLocTexCoord = glGetAttribLocation (uiProgramObject, "a_texCoord" );

	// Get the sampler location
	iLocSampler0 = glGetUniformLocation (uiProgramObject, "s_texture0" );
	iLocSampler1 = glGetUniformLocation (uiProgramObject, "s_texture1" );

	// Load the texture
	pTexture0=new ShadowTexture("depth texture",512,512);
	pTexture1=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\24ES2ShadowMapping\\wood.png");
	pTexture2=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\24ES2ShadowMapping\\earth.png");
	pTexture3=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\24ES2ShadowMapping\\fish.png");
	pTexture4=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\24ES2ShadowMapping\\rock.png");


	

	cubeNumIndices = genCube(0.5f, &cubeVertices,NULL, &cubeTexcoords, &cubeIndices );
	sphereNumIndices = genSphere(0.2f,32,16,&sphereVertices,NULL,&sphereTexcoords,&sphereIndices);
	fishNumIndices = genFish(0.15f,&fishVertices,NULL,&fishTexcoords,&fishIndices);
	//glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), fishVertices);
	

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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glUniform1i(iLocSampler0, 0);
	

	glActiveTexture ( GL_TEXTURE1 );
	pTexture1->bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glUniform1i(iLocSampler1, 1);


	glViewport(0,0,uiWidth,uiHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
}

void DrawCube(){
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), cubeVertices);
	glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), cubeTexcoords);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT,cubeIndices);
}
void DrawSphere(){
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), sphereVertices);
	glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), sphereTexcoords);
	glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT,sphereIndices);
}
void DrawFish(){
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), fishVertices);
	glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), fishTexcoords);
	glDrawElements(GL_TRIANGLES, fishNumIndices, GL_UNSIGNED_SHORT,fishIndices);
}
void DrawPlane(){
	glVertexAttribPointer (iLocPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), rectVertices );
	glVertexAttribPointer (iLocTexCoord, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), &rectVertices[3] );
	glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, rectIndices );
}
void DrawFrame()
{
	static ESMatrix perspective;
	static ESMatrix modelview;
	static float aspect = (float)uiWidth/uiHeight;
	

	


	if(first)
	{

		

		glViewport(0,0,pTexture0->getWidth(),pTexture0->getHeight());

		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniform1i(iLocDepthMode,true);
		glDisable(GL_TEXTURE_2D);

		esMatrixLoadIdentity(&mvMatrix);
		esRotate(&mvMatrix,rotY,0,1,0);
		esMatrixMultiply(&modelview,&mvMatrix,&modelview);

		esMatrixLoadIdentity(&tgMatrix);
		esPerspective(&tgMatrix,60.0f,1,1,1000);
		esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
		esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
		glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);

		pTexture0->bind();
		DrawCube();

		esMatrixLoadIdentity(&mvMatrix);
		esRotate(&mvMatrix,rotY,0,1,0);
		esTranslate(&mvMatrix,0.5f,0,0);
		esMatrixMultiply(&modelview,&mvMatrix,&modelview);

		esMatrixLoadIdentity(&tgMatrix);
		esPerspective(&tgMatrix,60.0f,1,1,1000);
		esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
		esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
		glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);

		DrawSphere();

		esMatrixLoadIdentity(&mvMatrix);
		esRotate(&mvMatrix,rotY,0,1,0);
		esTranslate(&mvMatrix,-0.5f,0,0);
		esMatrixMultiply(&modelview,&mvMatrix,&modelview);

		esMatrixLoadIdentity(&tgMatrix);
		esPerspective(&tgMatrix,60.0f,1,1,1000);
		esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
		esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
		glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);

		DrawFish();

		glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,0,pTexture0->getWidth(),pTexture0->getHeight(),0);
		glCullFace(GL_BACK);

		first=true;
	}


#if 1

	

	glUniform1i(iLocDepthMode,false);

	glClearColor(0.6f,0.7f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绘制灯光所在
	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 60.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,2,4,0,0,0,0,1,0);

	esMatrixLoadIdentity(&mvMatrix);
	esTranslate(&mvMatrix,lightPos[0],lightPos[1],lightPos[2]);
	esScale(&mvMatrix,0.5f,0.5f,0.5f);
	esMatrixMultiply(&modelview,&mvMatrix,&modelview);
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);


	esMatrixLoadIdentity(&tgMatrix);
	esPerspective(&tgMatrix,60.0f,1,1,1000);
	esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
	esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);
	glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);
	DrawSphere();


	glEnable(GL_TEXTURE_2D);
	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 60.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,2,4,0,0,0,0,1,0);

	esMatrixLoadIdentity(&mvMatrix);
	esRotate(&mvMatrix,rotY,0,1,0);

	esMatrixMultiply(&modelview,&mvMatrix,&modelview);
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);


	esMatrixLoadIdentity(&tgMatrix);
	esPerspective(&tgMatrix,60.0f,1,1,1000);
	esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
	esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);
	glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);

	pTexture1->bind();//这里需要bind，否则会使用texture0进行贴图（原因不明）==>因为glUniform1i(iLocSampler1, 1);Specifies the location of the uniform variable to be modified.
	DrawCube();

	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 60.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,2,4,0,0,0,0,1,0);

	esMatrixLoadIdentity(&mvMatrix);
	esRotate(&mvMatrix,rotY,0,1,0);
	esTranslate(&mvMatrix,0.5f,0,0);
	esMatrixMultiply(&modelview,&mvMatrix,&modelview);
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);


	esMatrixLoadIdentity(&tgMatrix);
	esPerspective(&tgMatrix,60.0f,1,1,1000);
	esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
	esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);
	glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);
	
	pTexture2->bind();
	DrawSphere();

	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 60.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,2,4,0,0,0,0,1,0);

	esMatrixLoadIdentity(&mvMatrix);
	esRotate(&mvMatrix,rotY,0,1,0);
	esTranslate(&mvMatrix,-0.5f,0,0);

	esMatrixMultiply(&modelview,&mvMatrix,&modelview);
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);

	esMatrixLoadIdentity(&tgMatrix);
	esPerspective(&tgMatrix,60.0f,1,1,1000);
	esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
	esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);
	glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);

	pTexture3->bind();
	DrawFish();

	esMatrixLoadIdentity( &perspective );
	esPerspective( &perspective, 60.0f, aspect, 1.0f, 100.0f );
	esMatrixLoadIdentity( &modelview );
	esLookAt(&modelview,0,2,4,0,0,0,0,1,0);

	esMatrixLoadIdentity(&mvMatrix);
	esRotate(&mvMatrix,rotY,0,1,0);
	esScale(&mvMatrix,2,1,2);

	esMatrixMultiply(&modelview,&mvMatrix,&modelview);
	esMatrixMultiply(&mvpMatrix, &modelview, &perspective);


	esMatrixLoadIdentity(&tgMatrix);
	esPerspective(&tgMatrix,60.0f,1,1,1000);
	esLookAt(&tgMatrix,lightPos[0],lightPos[1],lightPos[2],0,0,0,0,1,0);
	esMatrixMultiply(&tgMatrix,&mvMatrix,&tgMatrix);
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);
	glUniformMatrix4fv(iLocTG, 1, GL_FALSE, (GLfloat*)&tgMatrix.m[0][0]);

	pTexture4->bind();
	DrawPlane();
	glDisable(GL_TEXTURE_2D);

#endif

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

	if(pTexture2)
		delete pTexture2;

	if(pTexture3)
		delete pTexture3;

	if(pTexture4)
		delete pTexture4;

	glUseProgram(NULL);
	glDeleteProgram(uiProgramObject);
	glDeleteShader(uiVertShader);
	glDeleteShader(uiFragShader);

	if(cubeVertices!=NULL)
		free(cubeVertices);
	if(cubeTexcoords!=NULL)
		free(cubeTexcoords);
	if(cubeIndices!=NULL)
		free(cubeIndices);

	if(sphereVertices!=NULL)
		free(sphereVertices);
	if(sphereTexcoords!=NULL)
		free(sphereTexcoords);
	if(sphereIndices!=NULL)
		free(sphereIndices);

	if(fishVertices!=NULL)
		free(fishVertices);
	if(fishTexcoords!=NULL)
		free(fishTexcoords);
	if(fishIndices!=NULL)
		free(fishIndices);
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
			sprintf_s(buffer,"OpenGL ES2 ShadowMapping(fps: %.2f)", (float)(FRAME+1)/total_time);
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