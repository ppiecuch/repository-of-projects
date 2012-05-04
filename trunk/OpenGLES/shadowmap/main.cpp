/**
*
* @file   shadow.cpp
* @author Jerome Baril
* @email  jerome.baril@labri.fr
*
* @brief Shadow Mapping
*
*/

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#ifdef GL_ARB_shadow    
#define GL_TEXTURE_COMPARE_MODE      GL_TEXTURE_COMPARE_MODE_ARB    
#define GL_TEXTURE_COMPARE_FUNC      GL_TEXTURE_COMPARE_FUNC_ARB    
#define GL_DEPTH_TEXTURE_MODE        GL_DEPTH_TEXTURE_MODE_ARB    
#define GL_COMPARE_R_TO_TEXTURE      GL_COMPARE_R_TO_TEXTURE_ARB    
#endif   

#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "esMatrix.h"

using namespace std;


// window size
int windowWidth  = 512;
int windowHeight = 512;

// global variable
int    shadowMapSize = 512;
GLuint shadowMapTexture;

float lightPosition[4] ={1.5f, 2.f, 0.f, 1.f};

ESMatrix lightProjectionMatrix;
ESMatrix lightViewMatrix;

float cameraPosition[4]={-1.5f, 2.5f, 5.f, 1.f};

ESMatrix cameraProjectionMatrix;
ESMatrix cameraViewMatrix;
ESMatrix textureMatrix;

float white[4]={1.f,1.f,1.f,1.f};
float black[4]={0.f,0.f,0.f,1.f};


void
drawScene ()
{
	glFrontFace(GL_CW);
	glutSolidTeapot(0.5);

	glFrontFace(GL_CCW);
	glPushMatrix();
	glTranslatef(0,0,-1);
	glutSolidCube(0.5f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f,0.5f,0);
	glutSolidSphere(0.5f,16,16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0,1);
	glRotatef(-90,1,0,0);
	glutSolidCone(0.5f,0.5f,16,16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);
	glutSolidSphere(0.1f,4,4);
	glPopMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0.f,1.f,0.f);
	glVertex3f(-4.f, -1.5f, 4.f);
	glVertex3f( 4.f, -1.5f, 4.f);
	glVertex3f( 4.f, -1.5f,-4.f);
	glVertex3f(-4.f, -1.5f,-4.f);
	glEnd();

}


// callback to draw graphic primitives
void
displayFunc ()
{
	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&lightProjectionMatrix.m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&lightViewMatrix.m[0][0]);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);

	// avoid Z-fighting
	glPolygonOffset( 2, 4 );
	glEnable( GL_POLYGON_OFFSET_FILL );

	//Draw the scene
	drawScene();

	//Read the depth buffer into the shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

	//restore states
	glDisable( GL_POLYGON_OFFSET_FILL );
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&cameraProjectionMatrix.m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&cameraViewMatrix.m[0][0]);

	glViewport(0, 0, windowWidth, windowHeight);

	float lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float lightDiffuse[] = { 0.35f, 0.35f, 0.35f, 1.0f };
	float lightSpecular[] = { 0.f, 0.f, 0.f, 0.f };

	// Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	drawScene();


	//3rd pass
	//Draw with bright light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen

	esMatrixLoadIdentity(&textureMatrix);
	esTranslate(&textureMatrix,0.5f,0.5f,0.5f);
	esScale(&textureMatrix,0.5f,0.5f,0.5f);
	esMatrixMultiply(&textureMatrix,&lightProjectionMatrix,&textureMatrix);
	esMatrixMultiply(&textureMatrix,&lightViewMatrix,&textureMatrix);

	//因为我们将投影纹理矩阵用于纹理自动生成，而OpenGL的矩阵是以列优先排列，
	//为了将4个列分别用于{s,t,r,q}纹理坐标生成，这里对矩阵进行转置
	esMatrixTranspose(&textureMatrix,&textureMatrix);
	
	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, &textureMatrix.m[0][0]);
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, &textureMatrix.m[1][0]);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, &textureMatrix.m[2][0]);
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, &textureMatrix.m[3][0]);
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glClear(GL_DEPTH_BUFFER_BIT);

	drawScene();

	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//Restore other states
	glDisable(GL_LIGHTING);

	// swap buffer
	glutSwapBuffers();
}


// callback to resize the size of the viewport
void 
reshapeFunc (int w, int h)
{
	glViewport (0, 0, w, h);
	windowWidth = w; windowHeight = h;

	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth/windowHeight, 0.01f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &cameraProjectionMatrix.m[0][0]);

	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &cameraViewMatrix.m[0][0]);

	glPopMatrix();
}


// mysterious callback function ...
void 
idleFunc ()
{
	glutPostRedisplay();
}


// callback to manage keyboard interactions
void 
keyboardFunc (unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case 27: // Esc
		exit (EXIT_SUCCESS);
		break;
	}
}


// initialize glut framework
void 
initGlut (int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition (10, 10);
	glutInitWindowSize (windowWidth, windowHeight);
	if (glutCreateWindow ("TP8: Shadow Mapping") <= 0) {
		cerr << "Impossible de creer la fenetre Glut." << endl;
		exit (EXIT_FAILURE);
	}

	// callbacks
	glutDisplayFunc (displayFunc);
	glutKeyboardFunc (keyboardFunc);
	glutReshapeFunc (reshapeFunc);
	glutIdleFunc (idleFunc);
}


// initialize OpenGL context
void 
initGl ()
{
	// background color
	glClearColor (0.0, 0.0, 0.0, 1.0);

	// viewport
	glViewport (0, 0, windowWidth, windowHeight);

	// define projetion matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity (); 

	// define viewpoint
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


void
initShadowMap ()
{
	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	GLfloat borderColor[4] = {1,1, 1, 1};
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth/windowHeight, 0.01f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &cameraProjectionMatrix.m[0][0]);

	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &cameraViewMatrix.m[0][0]);

	glLoadIdentity();
	gluPerspective(120.0f, 1.0f, 1.f, 10.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &lightProjectionMatrix.m[0][0]);

	glLoadIdentity();
	gluLookAt(	lightPosition[0], lightPosition[1], lightPosition[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, &lightViewMatrix.m[0][0]);

	glPopMatrix();
}


int
main (int argc, char **argv)
{
	initGlut (argc, argv);
	initGl ();
	initShadowMap();

	glutMainLoop ();
	return (EXIT_SUCCESS);
}


