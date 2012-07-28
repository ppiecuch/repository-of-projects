#include <GL/glut.h>

#include "FPSCounter.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma comment(lib,"utility.lib")
FPSCounter fpsCounter;
GLvoid* bmpbits;
#define Z_MAX 1
GLuint m_texture[Z_MAX];

#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

inline void TRACE(const char * pszFormat, ...)
{
	va_list pArgs;
	char szMessageBuffer[16380]={0};
	va_start( pArgs, pszFormat );
	vsnprintf_s( szMessageBuffer, 16380,16380-1,pszFormat, pArgs );
	va_end( pArgs );   
	OutputDebugStringA(szMessageBuffer);   
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#define GETRAND() ( (rand()/(float)RAND_MAX-0.5)*2 )

float g_points[5000000][2];
float g_pointColor[5000000];

int g_pointNum;

float halfSize=30.0f;

inline float noise(int x,int y)
{
	int n=x+y*57;
	n=(n<<13)^n;
	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0); 
}

inline float smoothedNoise(float x,float y)
{
	float corners = ( noise(x-1, y-1)+noise(x+1, y-1)+noise(x-1, y+1)+noise(x+1, y+1) ) / 16;
	float sides = ( noise(x-1, y) +noise(x+1, y) +noise(x, y-1) +noise(x, y+1) ) / 8;
	float center = noise(x, y) / 4;
	return corners + sides + center;
}

inline float interpolate(float a,float b,float x)
{
	float f=(1-cos(x*3.141592658))*0.5;
	return a*(1-f) + b*f;
	/*return a*(1-x) + b*x;*/
}

inline float interpolatedNoise(float x,float y)
{
	int intX;
	if(x<0) intX=(int)x-1;
	else intX=(int)x;
	float fractionalX=x-intX;
	int intY;
	if(y<0) intY=(int)y-1;
	else intY=(int)y;
	float fractionalY=y-intY;

	float v1=smoothedNoise(intX,intY);
	float v2=smoothedNoise(intX+1,intY);
	float v3=smoothedNoise(intX,intY+1);
	float v4=smoothedNoise(intX+1,intY+1);

	float i1=interpolate(v1,v2,fractionalX);
	float i2=interpolate(v3,v4,fractionalX);

	return interpolate(i1 , i2 , fractionalY);
}

float PerlinNoise_2D(float x,float y)
{
	static float persistence=0.25;
	static int n=4;
	float frequency;
	float amplitude;
	float total=0;
	for (int i=0;i<5;++i)
	{
		frequency=pow(2.0f,(float)i);
		amplitude=pow(persistence,(float)i);
		total += interpolatedNoise(x * frequency, y * frequency) * amplitude;
	}
	return total;
}



GLdouble theta = 0;//旋转角度
void printFPS(){
	//Update frames per second counter
	fpsCounter.update();

	//Print fps
	static char fpsString[32];
	sprintf_s(fpsString, "FPS:%.2f", fpsCounter.getFPS());

	//Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Print text
	glRasterPos2f(-1.0f, 0.9f);
	for(unsigned int i=0; i<strlen(fpsString); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

	//reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void display(void) { 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_POINTS);
	for (int i=0;i<g_pointNum;++i)
	{
		glColor3f(g_pointColor[i],g_pointColor[i],g_pointColor[i]);
		glVertex2fv(g_points[i]);
	}
	glEnd();
	glutSwapBuffers();
}

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
	//Z_MAX=10,m_texture的定义：GLuint m_texture[Z_MAX]
	int i;
	char filename[50];
	BITMAPINFOHEADER bmpheader;
	for(i=0;i<=Z_MAX-1;i++)
	{
		sprintf(filename,"texture%d.bmp",i);
		//sprintf(filename,"test.bmp");
		//图片都放在Data/gl中，依次命名b1.bmp....b10.bmp
		bmpheader=LoadGLBitmap(filename);
		glBindTexture(GL_TEXTURE_2D,m_texture[i]);
		glTexImage2D(GL_TEXTURE_2D,0,3,bmpheader.biWidth,bmpheader.biHeight,0,GL_RGB,GL_UNSIGNED_BYTE,bmpbits);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	}
}
void init()
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);

	int i;
	float t;
	float max=-5,min=5;
	srand(time(NULL));
	g_pointNum=50000;
	for (i=0;i<g_pointNum;++i)
	{
		g_points[i][0]=GETRAND()*halfSize;
		g_points[i][1]=GETRAND()*halfSize;
		t=PerlinNoise_2D(g_points[i][0],g_points[i][1]);
		if(t>max) max=t;
		if(t<min) min=t;
		g_pointColor[i]=0.5+0.5*t;
		//printf("%f\n",t);
	}
	printf("%f %f\n",min,max);
}
void resize(int cx,int cy)
{
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
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-halfSize,halfSize,-halfSize,halfSize);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void idle()
{
	//glutPostRedisplay 标记当前窗口需要重新绘制。通过glutMainLoop下一次循环时，窗口显示将被回调以重新显示窗口的正常面板。
	//多次调用glutPostRedisplay，在下一个显示回调只产生单一的重新显示回调。 
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y)
{
	const static int ESC = 27;
	switch (key)
	{
	case ESC:
		exit(0);
	}
}
void mouse(int btn, int state, int x, int y)
{
}
void motion(int x, int y)
{
}

void destroy()
{

}

int main(int argc, char **argv)
{
	atexit(destroy);
	//初始化GLUT
	//Void glutInit(int*argc,char**argv)； 
	//参数： 
	//Argc：一个指针，指向从main（）函数传递过来的没更改的argc变量。 
	//Argv：一个指针，指向从main（）函数传递过来的没更改的argv变量。
	glutInit(&argc, argv);
	//定义显示方式。 
	//Void glutInitDisplayMode(unsighed int mode) 
	//参数： 
	//Mode――可以指定下列显示模式 
	//Mode参数是一个GLUT库里预定义的可能的布尔组合。你使用mode去指定颜色模式，数量和缓冲区类型。 
	//指定颜色模式的预定义常量有： 
	//1：GLUT_RGBA或者GLUT_RGB。指定一个RGBA窗口，这是一个默认的颜色模式。 
	//2：GLUT_INDEX。指定颜色索引模式。 
	//这个显示模式还允许你选择单缓冲区或双缓冲区窗口。 
	//1：GLUT_SINGLE.单缓冲区窗口。 
	//2：GLUT_BUFFER.双缓冲区窗口，这是产生流畅动画必须选的。 
	//还可以指定更多，如果你想指定一组特殊的缓冲的话，用下面的变量： 
	//1：GLUT_ACCUM.累积缓冲区。 
	//2：GLUT_STENCIL.模板缓冲区。 
	//3：GLUT_DEPTH.深度缓冲区。 
	//用“或“（|）操作符来建立你想要的显示模式。 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//确定窗口位置（它默认的是屏幕左上角），我们使用函数glutInitWindowPosition（）。 
	//Void glutInitWindowPositon(int x,int y); 
	//参数： 
	//X: 距离屏幕左边的像素数。－1是默认值，意思就是由窗口管理程序决定窗口出现在哪里。如果不使用默认值，那你就自己设置一个值。 
	//Y：距离屏幕上边的像素数。和X一样。 
	//注意，参数仅仅是对窗口管理程序的一个建议。尽管你精心的设置了窗口位置，window返回的可能是不同的位置。如果你设置了，一般会得到你想要的结果。
	glutInitWindowPosition(100,100); 
	//设置窗口大小，使用函数glutInitWindowSize（）。 
	//Void glutInitWindowSize(int width,int height); 
	//参数： 
	//Width：窗口的宽度。 
	//Height：窗口的高度。 
	//同样width，height也只是一个参考数字。避免使用负数。 
	glutInitWindowSize(640,480); 
	//调用函数glutCreateWindow()来创建窗口
	//Int glutCreateWindow(char* title); 
	//参数： 
	//Title：设置窗口的标题。 
	//glutCreateWindow（）的返回值是一个窗口标识符。
	(void) glutCreateWindow("GL Framework");

	init();

	//告诉GLUT使用我们上面的函数来进行渲染。这个叫寄存回调。。让我们告诉GLUT这个函数renderScene应该被使用。
	//当需要重画的时候GLUT有一个只传递一个函数名称参数的函数（以函数名为形参的函数）就会被调用。 
	//void glutDisplayFunc(void (*func)(void)); 
	//参数： 
	//func: 当窗口需要被重绘是调用的函数的名称。注意使用NULL作为实参是错误的。
	glutDisplayFunc(display);
	//在glutIdleFunc(void (*func)(void))回调函数中指定一个函数，如果不存在其他尚未完成的事件（例如，当事件循环处于空闲的时候），就执行这个函数。
	//这个回调函数接受一个函数指针作为它的唯一参数。如果向它传递NULL(0),就相当于禁用这个函数。
	glutIdleFunc(idle);
	//窗口发生变化时,视窗需要重新计算.
	//void glutReshapeFunc(void (*func)(int width, int height)); 
	//参数: 
	//func -- The name of the function that will be responsible forsetting the correct perspective when the window changes size. 
	glutReshapeFunc(resize);
	//void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
	// key 为键盘按键的ASCII码
	glutKeyboardFunc(key);
	//检测鼠标Clicks
	//和键盘处理一样，GLUT为你的注册函数（也就是处理鼠标clicks事件的函数）提供了一个方法。函数glutMouseFunc,这个函数一般在程序初始化阶段被调用。函数原型如下：
	//void glutMouseFunc(void(*func)(int button,int state,int x,int y));
	//参数：
	//func：处理鼠标click事件的函数的函数名。
	//从上面可以看到到，处理鼠标click事件的函数，一定有4个参数。
	//第一个参数表明哪个鼠标键被按下或松开，这个变量可以是下面的三个值中的一个：
	//GLUT_LEFT_BUTTON
	//GLUT_MIDDLE_BUTTON
	//GLUT_RIGHT_BUTTON
	//第二个参数表明，函数被调用发生时，鼠标的状态，也就是是被按下，或松开，可能取值如下：
	//GLUT_DOWN
	//GLUT_UP
	//当函数被调用时，state的值是GLUT_DOWN，那么程序可能会假定将会有个GLUT_UP事件，甚至鼠标移动到窗口外面，也如此。
	//然而，如果程序调用glutMouseFunc传递NULL作为参数，那么GLUT将不会改变鼠标的状态。
	//剩下的两个参数（x,y）提供了鼠标当前的窗口坐标（以左上角为原点）。
	glutMouseFunc(mouse);
	//检测动作（motion）
	//GLUT提供鼠标motion检测能力。有两种GLUT处理的motion：active motion和passive motion。
	//Active motion是指鼠标移动并且有一个鼠标键被按下。（拖动鼠标）
	//Passive motion是指当鼠标移动时，并有没鼠标键按下。（移动鼠标）
	//如果一个程序正在追踪鼠标，那么鼠标移动期间，没一帧将产生一个结果。
	//和以前一样，你必须注册将处理鼠标事件的函数（定义函数）。GLUT让我们可以指定两个不同的函数，一个追踪passive motion，另一个追踪active motion
	//它们的函数原型，如下：
	//void glutMotionFunc(void(*func)(int x,int y));
	//void glutPassiveMotionFunc(void (*func)(int x,int y));
	//参数：
	//Func：处理各自类型motion的函数名。
	//处理motion的参数函数的参数（x,y）是鼠标在窗口的坐标。以左上角为原点。
	glutMotionFunc(motion);

	//检测鼠标进入或离开窗口
	//GLUT还能检测鼠标鼠标离开，进入窗口区域。一个回调函数可以被定义去处理这两个事件。GLUT里，调用这个函数的是glutEntryFunc,函数原型如下：
	//void glutEntryFunc(void(*func)（int state）);
	//参数：
	//Func：处理这些事件的函数名。
	//上面函数的参数中，state有两个值：
	//GLUT_LEFT      鼠标离开窗口
	//GLUT_ENTERED  鼠标进入窗口
	//表明，是离开，还是进入窗口。
	//glutEntryFunc(entry);
	//glutVisibilityFunc(vis);
	//glutCreateMenu(menu);
	//glutAddMenuEntry("Toggle showing projection", 's');
	//glutAddMenuEntry("Switch texture", 't');
	//glutAddMenuEntry("Switch object", 'o');
	//glutAddMenuEntry("Toggle filtering", 'l');
	//glutAddMenuEntry("Quit", 666);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
	//如果用定时器的话，初始的时候注册一个定时器的回调函数，原型是
	//glutTimerFunc(unsigned int millis, void (*func)(int value), int value);
	//这样使用glutTimerFunc(毫秒数, 回调函数指针, 区别值);
	//写自己的回调函数
	//void OnTimer(int value);
	//用value区分是哪个定时器
	//在函数里改变和位置有关的变量，然后调用glutPostRedisplay();用来重绘
	//最后再次调用glutTimerFunc，因为glut的定时器是调用一次才产生一次定时，所以如果要持续产生定时的话，在定时函数末尾再次调用glutTimerFunc


	//告诉GLUT我们准备进入应用程序事件处理循环。GLUT提供了一个函数让程序进入一个永不结束的循环。一直等待处理下一个事件。函数是glutMainLoop（）。 
	//glutMainLoop()一旦进入循环就再也不返回
	//glutMainLoop()  阻断了系统的正常流程，在关闭glutCreateWindow()创建的窗口后，glutMainLoop()直接用exit(0)退出程序，而不会继续执行glutMainLoop()后的语句，
	//这就使一些类的释放及资源回收工作无法进行，从而造成内存泄漏。纠正此问题，有3个办法：
	//1、使用glutLeaveMainLoop()代替glutMainLoop()。
	//2、处理代码中exit(0)的部分。 
	//3、在glutMainLoop之前先设置： 
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
	//然后再用glutMainLoop();则 glutMainLoop()会在退出后，继续执行其后的代码。
	//4、创建单例类对象，并在其销毁函数中调用你的资源释放处理
	//使用以上方法，可以有效处理glutMainLoop()造成的内存泄漏！
	//void glutMainLoop(void) 
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}