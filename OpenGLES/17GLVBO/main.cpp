// in order to get function prototypes from glext.h, define GL_GLEXT_PROTOTYPES before including glext.h
#define GL_GLEXT_PROTOTYPES


#include <GL/glut.h>
#include "glext.h"
#include "glInfo.h"  

#include <iostream>
#include <sstream>
using namespace std;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg, end;
#define FRAME 100
unsigned int frame=0;
char buffer[100];

GLdouble theta = 0;//旋转角度

GLuint vboId;                              // ID of VBO
bool vboSupported, vboUsed;
// global variables
void *font = GLUT_BITMAP_8_BY_13;

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array
GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
-1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
-1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

// normal array
GLfloat normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
-1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

// color array
GLfloat colors[] = {1,1,1,  1,1,0,  1,0,0,  1,0,1,              // v0-v1-v2-v3
1,1,1,  1,0,1,  0,0,1,  0,1,1,              // v0-v3-v4-v5
1,1,1,  0,1,1,  0,1,0,  1,1,0,              // v0-v5-v6-v1
1,1,0,  0,1,0,  0,0,0,  1,0,0,              // v1-v6-v7-v2
0,0,0,  0,0,1,  1,0,1,  1,0,0,              // v7-v4-v3-v2
0,0,1,  0,0,0,  0,1,0,  0,1,1};             // v4-v7-v6-v5

// function pointers for VBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
#ifdef _WIN32
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif


///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);     // need to disable lighting for proper text color

	glColor4fv(color);          // set text color
	glRasterPos2i(x, y);        // place text position

	// loop all characters in the string
	while(*str)
	{
		glutBitmapCharacter(font, *str);
		++str;
	}

	glEnable(GL_LIGHTING);
	glPopAttrib();
}
///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
	// backup current model-view matrix
	glPushMatrix();                     // save current modelview matrix
	glLoadIdentity();                   // reset modelview matrix

	// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);     // switch to projection matrix
	glPushMatrix();                  // save current projection matrix
	glLoadIdentity();                // reset projection matrix
	gluOrtho2D(0, 400, 0, 300);  // set to orthogonal projection

	float color[4] = {1, 1, 1, 1};

	stringstream ss;
	ss << "VBO: " << (vboUsed ? "on" : "off") << " " << buffer <<ends;  // add 0(ends) at the end
	drawString(ss.str().c_str(), 1, 286, color, font);
	ss.str(""); // clear buffer

	ss << "Press SPACE key to toggle VBO on/off." << ends;
	drawString(ss.str().c_str(), 1, 1, color, font);

	// restore projection matrix
	glPopMatrix();                   // restore to previous projection matrix

	// restore modelview matrix
	glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
	glPopMatrix();                   // restore to previous modelview matrix
}

void display(void) { 

	if(frame==0)
		_ftime64_s(&beg); 
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();
	glRotatef(theta, 1, 0, 1);

	if(vboUsed) // draw cube using VBO
	{
		for(int i=0;i<100;++i)
		{
			for(int j=0;j<100;++j)
			{
				// bind VBOs with IDs and set the buffer offsets of the bound VBOs
				// When buffer object is bound with its ID, all pointers in gl*Pointer()
				// are treated as offset instead of real pointer.
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

				// enable vertex arrays
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);

				// before draw, specify vertex and index arrays with their offsets
				glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
				glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)+sizeof(normals)));
				//void glVertexPointer(GLint size,GLenum type,GLsizei stride,const GLvoid* pointer);
				glVertexPointer(3, GL_FLOAT, 0, 0);

		
				glTranslatef(i/10.f,j/10.f,0);
				glDrawArrays(GL_QUADS, 0, 24);
			}
		}

		glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		// it is good idea to release VBOs with ID 0 after use.
		// Once bound with 0, all pointers in gl*Pointer() behave as real
		// pointer, so, normal vertex array operations are re-activated
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	else        // draw a cube using vertex array method
		// notice that only difference between VBO and VA is binding buffers and offsets
	{

		for(int i=0;i<100;++i)
		{
			for(int j=0;j<100;++j)
			{
				// enable vertex arrays
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);

				// before draw, specify vertex arrays
				glNormalPointer(GL_FLOAT, 0, normals);
				glColorPointer(3, GL_FLOAT, 0, colors);
				glVertexPointer(3, GL_FLOAT, 0, vertices);

		
				glTranslatef(i/10.f,j/10.f,0);
				glDrawArrays(GL_QUADS, 0, 24);
			}
		}


		glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	// draw info messages
	showInfo();

	glPopMatrix();
	theta ++;
	if(theta >= 360)
		theta = 0;
	glutSwapBuffers();

	if(frame==FRAME)
	{
		_ftime64_s(&end); 
		total_time = (float)(end.time - beg.time) + (float)(end.millitm - beg.millitm) / 1000;
		sprintf_s(buffer,"fps: %.2f", (float)FRAME/total_time);
		frame=0;
	}
	else
	{
		++frame;
	}
}
void init()
{
	// get OpenGL info
	glInfo glInfo;
	glInfo.getInfo();
	//glInfo.printSelf();

#ifdef _WIN32
	// check VBO is supported by your video card
	if(glInfo.isExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
		// get pointers to GL functions
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

		// check once again VBO extension
		if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
			glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
		{
			vboSupported = vboUsed = true;
			cout << "Video card supports GL_ARB_vertex_buffer_object." << endl;
		}
		else
		{
			vboSupported = vboUsed = false;
			cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << endl;
		}
	}
	else
	{
		vboSupported = vboUsed = false;
		cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << endl;
	}


#else // for linux, do not need to get function pointers, it is up-to-date
	if(glInfo.isExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
		vboSupported = vboUsed = true;
		cout << "Video card supports GL_ARB_vertex_buffer_object." << endl;
	}
	else
	{
		vboSupported = vboUsed = false;
		cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << endl;
	}
#endif
	if(vboSupported)
	{
		int bufferSize;

		// create vertex buffer objects, you need to delete them when program exits
		// Try to put both vertex coords array, vertex normal array and vertex color in the same buffer object.
		// glBufferDataARB with NULL pointer reserves only memory space.
		// Copy actual data with 2 calls of glBufferSubDataARB, one for vertex coords and one for normals.
		// target flag is GL_ARRAY_BUFFER_ARB, and usage flag is GL_STATIC_DRAW_ARB
		glGenBuffersARB(1, &vboId);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals)+sizeof(colors), 0, GL_STATIC_DRAW_ARB);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals), sizeof(colors), colors);  // copy colours after normals
	}
}
void resize(int width,int height)
{
}
void destroy()
{
	if(vboSupported)
	{
		// delete VBO when program terminated
		glDeleteBuffersARB(1, &vboId);
	}
}
void idle()
{
	//glutPostRedisplay 标记当前窗口需要重新绘制。通过glutMainLoop下一次循环时，窗口显示将被回调以重新显示窗口的正常面板。
	//多次调用glutPostRedisplay，在下一个显示回调只产生单一的重新显示回调。 
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27: // ESCAPE
		exit(0);
		break;

	case ' ':
		if(vboSupported)
			vboUsed = !vboUsed;
		break;
	}
	glutPostRedisplay();
}
void mouse(int btn, int state, int x, int y)
{
}
void motion(int x, int y)
{
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(640,480); 
	(void) glutCreateWindow("GL VBO");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();

	destroy();
	return 0; 
}