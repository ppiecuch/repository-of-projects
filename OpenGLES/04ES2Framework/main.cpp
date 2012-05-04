
//加载OpenGL ES2需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#include <EGL/egl.h>
#include <GLES2/gl2.h>

//加载Window程序需要的头文件
#include<windows.h>
#include <stdio.h>

TCHAR szWindowClass[100] = TEXT("GLES2");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;

//业务相关
/* 3D data. Vertex range -0.5..0.5 in all axes.
* Z -0.5 is near, 0.5 is far. */
const float aVertices[] =
{
	/* Front face. */
	/* Bottom left */
	-0.5,  0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	/* Top right */
	-0.5,  0.5, -0.5,
	0.5,  0.5, -0.5,
	0.5, -0.5, -0.5,
	/* Left face */
	/* Bottom left */
	-0.5,  0.5,  0.5,
	-0.5, -0.5, -0.5,
	-0.5, -0.5,  0.5,
	/* Top right */
	-0.5,  0.5,  0.5,
	-0.5,  0.5, -0.5,
	-0.5, -0.5, -0.5,
	/* Top face */
	/* Bottom left */
	-0.5,  0.5,  0.5,
	0.5,  0.5, -0.5,
	-0.5,  0.5, -0.5,
	/* Top right */
	-0.5,  0.5,  0.5,
	0.5,  0.5,  0.5,
	0.5,  0.5, -0.5,
	/* Right face */
	/* Bottom left */
	0.5,  0.5, -0.5,
	0.5, -0.5,  0.5,
	0.5, -0.5, -0.5,
	/* Top right */
	0.5,  0.5, -0.5,
	0.5,  0.5,  0.5,
	0.5, -0.5,  0.5,
	/* Back face */
	/* Bottom left */
	0.5,  0.5,  0.5,
	-0.5, -0.5,  0.5,
	0.5, -0.5,  0.5,
	/* Top right */
	0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5, -0.5,  0.5,
	/* Bottom face */
	/* Bottom left */
	-0.5, -0.5, -0.5,
	0.5, -0.5,  0.5,
	-0.5, -0.5,  0.5,
	/* Top right */
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5,  0.5,
};

const float aColours[] =
{
	/* Front face */
	/* Bottom left */
	1.0, 0.0, 0.0, /* red */
	0.0, 0.0, 1.0, /* blue */
	0.0, 1.0, 0.0, /* green */
	/* Top right */
	1.0, 0.0, 0.0, /* red */
	1.0, 1.0, 0.0, /* yellow */
	0.0, 0.0, 1.0, /* blue */
	/* Left face */
	/* Bottom left */
	1.0, 1.0, 1.0, /* white */
	0.0, 1.0, 0.0, /* green */
	0.0, 1.0, 1.0, /* cyan */
	/* Top right */
	1.0, 1.0, 1.0, /* white */
	1.0, 0.0, 0.0, /* red */
	0.0, 1.0, 0.0, /* green */
	/* Top face */
	/* Bottom left */
	1.0, 1.0, 1.0, /* white */
	1.0, 1.0, 0.0, /* yellow */
	1.0, 0.0, 0.0, /* red */
	/* Top right */
	1.0, 1.0, 1.0, /* white */
	0.0, 0.0, 0.0, /* black */
	1.0, 1.0, 0.0, /* yellow */
	/* Right face */
	/* Bottom left */
	1.0, 1.0, 0.0, /* yellow */
	1.0, 0.0, 1.0, /* magenta */
	0.0, 0.0, 1.0, /* blue */
	/* Top right */
	1.0, 1.0, 0.0, /* yellow */
	0.0, 0.0, 0.0, /* black */
	1.0, 0.0, 1.0, /* magenta */
	/* Back face */
	/* Bottom left */
	0.0, 0.0, 0.0, /* black */
	0.0, 1.0, 1.0, /* cyan */
	1.0, 0.0, 1.0, /* magenta */
	/* Top right */
	0.0, 0.0, 0.0, /* black */
	1.0, 1.0, 1.0, /* white */
	0.0, 1.0, 1.0, /* cyan */
	/* Bottom face */
	/* Bottom left */
	0.0, 1.0, 0.0, /* green */
	1.0, 0.0, 1.0, /* magenta */
	0.0, 1.0, 1.0, /* cyan */
	/* Top right */
	0.0, 1.0, 0.0, /* green */
	0.0, 0.0, 1.0, /* blue */
	1.0, 0.0, 1.0, /* magenta */
};

GLint iLocPosition = 0;
GLint iLocColour, iLocTexCoord, iLocNormal, iLocMVP;
int iXangle = 0, iYangle = 0, iZangle = 0;
float aRotate[16], aModelView[16], aPerspective[16], aMVP[16];
const unsigned int uiWidth  = 640;
const unsigned int uiHeight = 480;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg,end;
#define FRAME 10
unsigned int frame=0;

//数学函数
#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif /* M_PI */
/* 
* Simulates desktop's glRotatef. The matrix is returned in column-major 
* order. 
*/
void rotate_matrix(double angle, double x, double y, double z, float *R) {
	double radians, c, s, c1, u[3], length;
	int i, j;

	radians = (angle * M_PI) / 180.0;

	c = cos(radians);
	s = sin(radians);

	c1 = 1.0 - cos(radians);

	length = sqrt(x * x + y * y + z * z);

	u[0] = x / length;
	u[1] = y / length;
	u[2] = z / length;

	for (i = 0; i < 16; i++) {
		R[i] = 0.0;
	}

	R[15] = 1.0;

	for (i = 0; i < 3; i++) {
		R[i * 4 + (i + 1) % 3] = u[(i + 2) % 3] * s;
		R[i * 4 + (i + 2) % 3] = -u[(i + 1) % 3] * s;
	}

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			R[i * 4 + j] += c1 * u[i] * u[j] + (i == j ? c : 0.0);
		}
	}
}
/* 
* Simulates gluPerspectiveMatrix 
*/
void perspective_matrix(double fovy, double aspect, double znear, double zfar, float *P) {
	int i;
	double f;

	f = 1.0/tan(fovy * 0.5);

	for (i = 0; i < 16; i++) {
		P[i] = 0.0;
	}

	P[0] = f / aspect;
	P[5] = f;
	P[10] = (znear + zfar) / (znear - zfar);
	P[11] = -1.0;
	P[14] = (2.0 * znear * zfar) / (znear - zfar);
	P[15] = 0.0;
}
/* 
* Multiplies A by B and writes out to C. All matrices are 4x4 and column
* major. In-place multiplication is supported.
*/
void multiply_matrix(float *A, float *B, float *C) {
	int i, j, k;
	float aTmp[16];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			aTmp[j * 4 + i] = 0.0;

			for (k = 0; k < 4; k++) {
				aTmp[j * 4 + i] += A[k * 4 + i] * B[j * 4 + k];
			}
		}
	}

	for (i = 0; i < 16; i++) {
		C[i] = aTmp[i];
	}
}

//shader
const char* pszVertShader= "\
		   attribute highp vec4 av4position;\
		   attribute vec3 av3colour;\
		   uniform mediump mat4 mvp;\
		   varying vec3 vv3colour;\
		   void main(void)\
		   {\
				vv3colour = av3colour;\
				gl_Position = mvp * av4position; \
			}";
const char* pszFragShader="\
			precision lowp float;\
			varying vec3 vv3colour;\
			void main(void)\
			{\
				gl_FragColor = vec4(vv3colour, 1.0);\
			}";
GLuint uiVertShader;
GLuint uiFragShader;
GLuint uiProgramObject;


//消息处理函数
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

//初始化窗口
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

	//通常，在您呼叫GetDC或BeginPaint时，Windows用默认值建立一个新的设备内容，
	//您对属性所做的一切改变在设备内容用 ReleaseDC或EndPaint呼叫释放时，都会丢失。
	//虽然在通常情况下这种方法已经很令人满意了，但是您还可能想要在释放设备内容之后，
	//仍然保存程序中对设备内容属性所做的改变，以便在下一次呼叫GetDC 和BeginPaint时
	//它们仍然能够起作用。为此，可在登录窗口类别时，将CS_OWNDC旗标纳入窗口类别的一部分：
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

	//该函数依据所需客户矩形大小，计算需要的窗口矩形的大小。计算出的窗口矩形随后可以传送给CreateWindowEx函数，用于创建一个客户区所需大小的窗口。 
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
	
	//EGL_CONTEXT_CLIENT_VERSION determines which version of an OpenGL ES context to create.An Attribute
	//value of 1 specifies creation of an OpenGL ES 1.x context. An Attribute value of 2 specifies creation
	// of an OpenGL 2.x context. The default value for EGL_CONTEXT_CLIENT_VERSION is 1.
	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);

	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

	//打印版本信息
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

	//Create the shader object:
	uiVertShader = glCreateShader(GL_VERTEX_SHADER);
	uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Load the source code into the shader objects
	//void glShaderSource( GLuint   shader,  GLsizei   count,  const GLchar **  string,  const GLint *  length); 
	//If length is NULL, each string is assumed to be null terminated.
	glShaderSource(uiVertShader,1,(const char**)&pszVertShader,NULL);
	glShaderSource(uiFragShader,1,(const char**)&pszFragShader,NULL);

	//Compile the shader
	glCompileShader(uiVertShader);
	glCompileShader(uiFragShader);

	//Check both the shaders compiled successfully.glGetShaderiv and glGetShaderInfoLog are used to query the shader object.
	GLint iShaderCompiled;
	glGetShaderiv(uiVertShader,GL_COMPILE_STATUS,&iShaderCompiled);
	if(!iShaderCompiled)
	{
		//Retrieve the length of the error message
		int i32LogLength,i32CharsWritten;
		glGetShaderiv(uiVertShader,GL_INFO_LOG_LENGTH,&i32LogLength);

		//Allocate enough space for the message and retrieve it
		char* pszLog = new char[i32LogLength];
		glGetShaderInfoLog(uiVertShader,i32LogLength,&i32CharsWritten,pszLog);

		//Display the error
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

	//Create the shader program object and attach the shader to it.
	uiProgramObject = glCreateProgram();

	glAttachShader(uiProgramObject,uiFragShader);
	glAttachShader(uiProgramObject,uiVertShader);

	//Link the program.This creates the actual executable binaries that will be run on the hardware
	glLinkProgram(uiProgramObject);

	//Check the program object was linked successfully.This is done similarly to the way the shader objects were checked.
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

	/* Get attribute locations of non-fixed attributes like colour and texture coordinates. */
	iLocPosition = glGetAttribLocation(uiProgramObject, "av4position");
	iLocColour = glGetAttribLocation(uiProgramObject, "av3colour");
	printf("iLocPosition = %i\n", iLocPosition);
	printf("iLocColour   = %i\n", iLocColour);
	/* Get uniform locations */
	iLocMVP = glGetUniformLocation(uiProgramObject, "mvp");
	printf("iLocMVP      = %i\n", iLocMVP);

	//Now te loading and initialisation of the shader is now complete.
	//glUseProgram is used to set the program object as part of the current render state,before drawing:
	glUseProgram(uiProgramObject);

	/*Enable attributes for position, colour and texture coordinates etc.*/
	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocColour);
	/* Populate attributes for position, colour and texture coordinates etc. */
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, aVertices);
	glVertexAttribPointer(iLocColour, 3, GL_FLOAT, GL_FALSE, 0, aColours);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void DrawFrame()
{
	/* 
	* Do some rotation with Euler angles. It is not a fixed axis as
	* quaterions would be, but the effect is cool. 
	*/
	rotate_matrix(iXangle, 1.0, 0.0, 0.0, aModelView);
	rotate_matrix(iYangle, 0.0, 1.0, 0.0, aRotate);

	multiply_matrix(aRotate, aModelView, aModelView);

	rotate_matrix(iZangle, 0.0, 1.0, 0.0, aRotate);

	multiply_matrix(aRotate, aModelView, aModelView);

	/* Pull the camera back from the cube */
	aModelView[14] -= 2.5;

	perspective_matrix(45.0, (double)uiWidth/(double)uiHeight, 0.01, 100.0, aPerspective);
	multiply_matrix(aPerspective, aModelView, aMVP);

	//void glUniformMatrix4fv(GLint location, GLsizei count,GLboolean transpose,const GLfloat* value); 
	//The commands glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv are used to modify a matrix 
	//or an array of matrices. The numbers in the command name are interpreted as the dimensionality of the matrix. 
	//The number 2 indicates a 2 × 2 matrix (i.e., 4 values), the number 3 indicates a 3 × 3 matrix (i.e., 9 values), 
	//and the number 4 indicates a 4 × 4 matrix (i.e., 16 values). Non-square matrix dimensionality is explicit, 
	//with the first number representing the number of columns and the second number representing the number of rows. 
	//For example, 2x4 indicates a 2 × 4 matrix with 2 columns and 4 rows (i.e., 8 values). 
	//If transpose is GL_FALSE, each matrix is assumed to be supplied in column major order. 
	//If transpose is GL_TRUE, each matrix is assumed to be supplied in row major order. 
	//The count argument indicates the number of matrices to be passed. A count of 1 should be used if modifying the value 
	//of a single matrix, and a count greater than 1 can be used to modify an array of matrices.
	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP);

	iXangle += 3;
	iYangle += 2;
	iZangle += 1;

	if(iXangle >= 360) iXangle -= 360;
	if(iXangle < 0) iXangle += 360;
	if(iYangle >= 360) iYangle -= 360;
	if(iYangle < 0) iYangle += 360;
	if(iZangle >= 360) iZangle -= 360;
	if(iZangle < 0) iZangle += 360;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	if (!eglSwapBuffers(m_eglDisplay, m_eglSurface)) {
		printf("Failed to swap buffers.\n");
	}
}

void DestroyGL()
{
	//After rendering is finished the resources must be cleaned-up
	glUseProgram(NULL);
	glDeleteProgram(uiProgramObject);
	glDeleteShader(uiVertShader);
	glDeleteShader(uiFragShader);
}

int main(void) {

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