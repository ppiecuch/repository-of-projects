/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <windows.h>		// Header File For Windows
#include "glee.h"
#include <gl/gl.h>			// Header File For The OpenGL32 Library
#include <gl/glu.h>			// Header File For The GLu32 Library
#include <gl/glaux.h>		// Header File For The Glaux Library
#include <gl/glut.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>


#include "TextureLoader.h"

using namespace std;


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

struct Triple
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

GLfloat rtri;
GLfloat rquad;

GLfloat texmove;

GLenum error;

const GLsizei texSize = 512;
Triple ViewPos;

GLsizei m_width, m_height;

GLuint reflectionTexture;
GLuint refractionTexture;
GLuint depthTexture;

glTexture dudvmap;
glTexture normalmap;

TextureLoader *pTextureLoader = new TextureLoader();

GLenum my_vertex_shader;
GLenum my_fragment_shader;
GLenum my_program;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void get_error()
{

	switch(glGetError())
	{
		case GL_NO_ERROR :
			fprintf(stderr, "GL_NO_ERROR");
		break;
		case GL_INVALID_ENUM :
			fprintf(stderr, "GL_INVALID_ENUM");
		break;
		case GL_INVALID_VALUE :
			fprintf(stderr, "GL_INVALID_VALUE");
		break;
		case GL_INVALID_OPERATION :
			fprintf(stderr, "GL_INVALID_OPERATION");
		break;
		case GL_STACK_OVERFLOW :
			fprintf(stderr, "GL_STACK_OVERFLOW");
		break;
		case GL_STACK_UNDERFLOW :
			fprintf(stderr, "GL_STACK_UNDERFLOW");
		break;
		case GL_OUT_OF_MEMORY :
			fprintf(stderr, "GL_OUT_OF_MEMORY");
		break;
		
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void get_arb_error(GLhandleARB object)
{
	GLsizei noOfCharsWritten = 0;
	GLcharARB infoLog[255];
	
	glGetInfoLogARB(object, 255, &noOfCharsWritten, infoLog);

	fstream file_op("dreck.log",ios::out);
	file_op<<infoLog;
	file_op.close();
}



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const char* ReadTextFile(const char* file)
{	
	char c;
	char* buffer;
	int i = 0;

	ifstream myfile (file);				

	buffer = (char *) calloc(i + 1, sizeof(char));

	if (myfile.is_open())				
	{
		while (! myfile.eof() )				
		{		  
		  c = myfile.get();
		  buffer[i] = c;
		  ++i;
		  buffer = (char*) realloc (buffer, (i + 1) * sizeof(char));
		}

		myfile.close(); 
	}
	else cout << "Unable to open file";
	
	return buffer;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	m_width = width;
	m_height = height;

	glViewport(0,0,m_width,m_height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SetWorldParams()
{
	ViewPos.x = 0.0f;
	ViewPos.y = 2.0f;
	ViewPos.z = 4.0f;
	rtri = 0.0f;
	rquad = 0.0f;
	texmove = 1.0f;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// for debugging stuff...
void RenderQuadWithTexture(GLuint texture)
{
	glClearColor(0.4f, 0.3f, 0.2f, 0.5f);			
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-2.0f,1.5f,0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f, 1.5f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f, 0.5f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-2.0f, 0.5f, 0.0f);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void drawSomeThings()
{	
	glPushMatrix();
		glTranslatef(-0.5f,0.25f,0.0f);						// Move Left 1.5 Units And Into The Screen 6.0
		glScalef(0.25f,0.25f,0.25f);
		glRotatef(rtri,0.0f,1.0f,0.0f);						// Rotate The Triangle On The Y axis ( NEW )
		glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
			glColor3f(1.0f,0.0f,0.0f);						// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
			glColor3f(0.0f,1.0f,0.0f);						// Green
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Left Of Triangle (Front)
			glColor3f(0.0f,0.0f,1.0f);						// Blue
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Right Of Triangle (Front)
			glColor3f(1.0f,0.0f,0.0f);						// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
			glColor3f(0.0f,0.0f,1.0f);						// Blue
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Left Of Triangle (Right)
			glColor3f(0.0f,1.0f,0.0f);						// Green
			glVertex3f( 1.0f,-1.0f, -1.0f);					// Right Of Triangle (Right)
			glColor3f(1.0f,0.0f,0.0f);						// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
			glColor3f(0.0f,1.0f,0.0f);						// Green
			glVertex3f( 1.0f,-1.0f, -1.0f);					// Left Of Triangle (Back)
			glColor3f(0.0f,0.0f,1.0f);						// Blue
			glVertex3f(-1.0f,-1.0f, -1.0f);					// Right Of Triangle (Back)
			glColor3f(1.0f,0.0f,0.0f);						// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
			glColor3f(0.0f,0.0f,1.0f);						// Blue
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Left Of Triangle (Left)
			glColor3f(0.0f,1.0f,0.0f);						// Green
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Right Of Triangle (Left)
		glEnd();											// Done Drawing The Pyramid
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.5f,0.25f,0.0f);						// Move Right 1.5 Units And Into The Screen 7.0
		glScalef(0.25f,0.25f,0.25f);
		glRotatef(rquad,1.0f,1.0f,1.0f);					// Rotate The Quad On The X axis ( NEW )
		glBegin(GL_QUADS);									// Draw A Quad
			glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
			glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
			glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
			glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)
			glColor3f(1.0f,0.5f,0.0f);						// Set The Color To Orange
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
			glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)
			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
			glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)
			glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
			glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
			glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
			glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)
			glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
			glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)
			glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
			glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
			glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
			glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
		glEnd();											// Done Drawing The Quad
	glPopMatrix();
	
	//rtri+=0.2f;											// Increase The Rotation Variable For The Triangle ( NEW )
	//rquad-=0.15f;										// Decrease The Rotation Variable For The Quad ( NEW )
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GenerateTextures()
{
	
	// Prepare reflection texture
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D,reflectionTexture);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Prepare refraction texture
	glGenTextures(1, &refractionTexture);
	glBindTexture(GL_TEXTURE_2D,refractionTexture);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	// Prepare depth texture
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D,depthTexture);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// prepare normalmap texture
	pTextureLoader->LoadTextureFromDisk("w_normalmap.jpg", &normalmap);
	
	// prepare dudvmap texture
	pTextureLoader->LoadTextureFromDisk("dudv_map.jpg", &dudvmap);
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Render Scene scaled by(1.0, -1.0, 1.0) to texture
void RenderReflection()
{
	glViewport(0,0,texSize,texSize);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //!  || durch | ersetzt
    glLoadIdentity();
    gluLookAt(ViewPos.x,ViewPos.y,ViewPos.z,0,0,0,0,1,0);

    glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScalef(1.0, -1.0, 1.0);
		double plane[4] = {0.0, 1.0, 0.0, 0.0}; //water at y=0
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);
		drawSomeThings();
		glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    //render reflection to texture
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);

    //glCopyTexSubImage2D copies the frame buffer
    //to the bound texture    
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,texSize, texSize);

	//RenderQuadWithTexture(reflectionTexture);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void RenderRefractionAndDepth()
{
    glViewport(0,0,texSize,texSize);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    gluLookAt(ViewPos.x,ViewPos.y,ViewPos.z,0,0,0,0,1,0);

    glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		//normal pointing along negative y
		double plane[4] = {0.0, -1.0, 0.0, 0.0};
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);
		drawSomeThings();
		glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    //render color buffer to texture
    glBindTexture(GL_TEXTURE_2D, refractionTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,texSize, texSize);
	//RenderQuadWithTexture(refractionTexture);

    //render depth to texture
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT, 0,0, texSize,texSize, 0);
	//RenderQuadWithTexture(depthTexture);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InitShaders()
{

	const char * my_fragment_shader_source;
	const char * my_vertex_shader_source;


	// Get Vertex And Fragment Shader Sources
	my_vertex_shader_source		= ReadTextFile("vwater.sh");
	my_fragment_shader_source	= ReadTextFile("fwater.sh");
	//my_fragment_shader_source = ReadTextFile("test.txt");
	
	const char *vv = my_vertex_shader_source;
	const char *ff = my_fragment_shader_source;

	// Create Shader And Program Objects
	my_program = glCreateProgramObjectARB();

	my_vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	my_fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// Load Shader Sources
	glShaderSourceARB(my_vertex_shader, 1, &vv, NULL);
	glShaderSourceARB(my_fragment_shader, 1, &ff, NULL);

	// Compile The Shaders
	glCompileShaderARB(my_vertex_shader);
	glCompileShaderARB(my_fragment_shader);
	
	// Attach The Shader Objects To The Program Object
	glAttachObjectARB(my_program, my_vertex_shader);
	glAttachObjectARB(my_program, my_fragment_shader);

	// Link The Program Object
	glLinkProgramARB(my_program);

	//get_arb_error(my_program);		
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void RenderWater()
{
    // Use The Program Object Instead Of Fixed Function OpenGL
	glUseProgramObjectARB(my_program);
	
	int uniform = glGetUniformLocationARB(my_program, "viewpos");
	glUniform4fARB(uniform, ViewPos.x, ViewPos.y, ViewPos.z, 1.0);

	uniform = glGetUniformLocationARB(my_program, "lightpos");
	glUniform4fARB(uniform, 5.0f,25.0f,6.0f, 1.0f);

	uniform = glGetUniformLocationARB(my_program, "time");
	glUniform1fARB(uniform, texmove);

	uniform = glGetUniformLocationARB(my_program, "time2");
	glUniform1fARB(uniform, -texmove);

	texmove += 0.01;
	
	uniform = glGetUniformLocationARB(my_program, "water_reflection");
	glUniform1iARB(uniform, 0);

	uniform = glGetUniformLocationARB(my_program, "water_refraction");
	glUniform1iARB(uniform, 1);
	
	uniform = glGetUniformLocationARB(my_program, "water_normalmap");
	glUniform1iARB(uniform, 2);	

	uniform = glGetUniformLocationARB(my_program, "water_dudvmap");
	glUniform1iARB(uniform, 3);

	uniform = glGetUniformLocationARB(my_program, "water_depthmap");
	glUniform1iARB(uniform, 4);

	uniform = glGetUniformLocationARB(my_program, "waterColor");
	glUniform4fARB(uniform, 1.0f,1.0f,1.0f, 1.0f);

	uniform = glGetUniformLocationARB(my_program, "waterDepth");
	glUniform4fARB(uniform, 1.0f,1.0f,1.0f, 1.0f);


//bind all textures
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);

    glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);

    glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, normalmap.TextureID);

    glActiveTextureARB(GL_TEXTURE3_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, dudvmap.TextureID);

    glActiveTextureARB(GL_TEXTURE4_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

    //Render the water surface
    glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 5.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
		glVertex3f(-5.0f, 0.0f, 5.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
		glVertex3f(-5.0f, 0.0f, -5.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 5.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
		glVertex3f(5.0f, 0.0f, -5.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 5.0f, 5.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 1.0f);
		glVertex3f(5.0f, 0.0f, 5.0f);
    glEnd();

    //Disable texture units and shader programs
	glActiveTextureARB(GL_TEXTURE0_ARB); glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE1_ARB); glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE2_ARB); glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE3_ARB); glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE4_ARB); glDisable(GL_TEXTURE_2D);
	glUseProgramObjectARB(0);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int DrawGLScene(GLvoid)
{		
	RenderReflection();
	RenderRefractionAndDepth();	
	glViewport(0,0,800,600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
	gluLookAt(ViewPos.x,ViewPos.y,ViewPos.z,0,0,0,0,1,0);
	drawSomeThings();
	RenderWater();
	return TRUE;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ClearApp()
{
	pTextureLoader->FreeTexture(&normalmap);
	pTextureLoader->FreeTexture(&dudvmap);		
}

bool Init()
{	
	return TRUE;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.53f, 0.85f, 1.0f, 0.9f);				// Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	GLeeInit();
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	SetWorldParams();
	GenerateTextures();
	InitShaders();


	return TRUE;										// Initialization Went OK
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}



/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{	
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			// LoWord Can Be WA_INACTIVE, WA_ACTIVE, WA_CLICKACTIVE,
			// The High-Order Word Specifies The Minimized State Of The Window Being Activated Or Deactivated.
			// A NonZero Value Indicates The Window Is Minimized.
			if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
				active=TRUE;						// Program Is Active
			else
				active=FALSE;						// Program Is No Longer Active

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			ClearApp();
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE			
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's Solid Object Tutorial",800,600,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && Init() && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's Solid Object Tutorial",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
