#include <windows.h>
#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

#include "ddsimage.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

void initCube(void);
static void redraw(void);
int main(int argc, char **argv);

struct 
{
	struct
	{
		float pos[3];
		float col[3];
	}ver[8];

	struct 
	{
		unsigned int ver[4];
	} quad[6];

	struct 
	{
		float s;
		float t;
	} tex[24];
}cube;

GLuint texobj = 0;


int main(int argc, char **argv) 
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Spinning cube");
	glewInit();
	glutDisplayFunc(redraw);	

	glMatrixMode(GL_PROJECTION);						//hello
	gluPerspective(45,	//view angle
		1.0,			//aspect ratio
		10.0,			//near clip
		200.0);			//far clip
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_CULL_FACE);

	initCube();

	//for (int i = 0; i < 1000; i++)
	{
		BuildDDS("aura00.dds", texobj);
	}
	
	glutMainLoop();

	return 0; 
}

void initCube(void)
{
	//defines the position of each vertex
	float vertexPosDat[8][3]=
	{
		{-10, 10, 10}, //left,top,front
		{10,  10, 10}, //right,top,front
		{10,  10,-10}, //right,top,back
		{-10, 10,-10}, //left, top,back
		{-10,-10, 10}, //left,bottom,front
		{10, -10, 10}, //right,bottom,front
		{10, -10,-10}, //right,bottom,back
		{-10,-10,-10}  //left,bottom,back
	};

	//defines the colour of each vertex
	float vertexColDat[8][3]=
	{
		{0.5,0  ,0  }, //dark red
		{1  ,1  ,0.3}, //yellow
		{1  ,0  ,0  }, //red
		{0.5,1  ,0.2}, //dull yellow??
		{1  ,1  ,0  }, //yellow
		{0.9,0.5,0  }, //orange
		{1  ,0.9,0.1}, //yellow
		{1  ,0  ,0  }, //red
	};

	//defines the vertexes of each quad in anti-clockwise order
	unsigned int quadVerDat[6][4]=
	{
		{0,1,2,3}, //top
		{0,3,7,4}, //left
		{3,2,6,7}, //back
		{2,1,5,6}, //right
		{0,4,5,1}, //front
		{4,7,6,5}, //bottom
	};

	int a,b;
	//put the vertex data into the cube struct
	for (a=0;a<8;++a)
	{
		for (b=0;b<3;++b)
		{
			cube.ver[a].pos[b]=vertexPosDat[a][b];
			cube.ver[a].col[b]=vertexColDat[a][b];
		}
	}

	//put the quad data into the cube struct
	for (a=0;a<6;++a)
	{
		for (b=0;b<4;++b)
		{
			cube.quad[a].ver[b]=quadVerDat[a][b];
		}
		cube.tex[a * 4].s = 0;
		cube.tex[a * 4].t = 0;
		cube.tex[a * 4 + 1].s = 1;
		cube.tex[a * 4 + 1].t = 0;
		cube.tex[a * 4 + 2].s = 1;
		cube.tex[a * 4 + 2].t = 1;
		cube.tex[a * 4 + 3].s = 0;
		cube.tex[a * 4 + 3].t = 1;
	}
}

static void redraw(void)
{
	static float rotateBy=0;
	int a,b;
	unsigned int currentVer;

	rotateBy+=1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0, 0, -50);
	glRotatef(rotateBy, 1, 1, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texobj);

	glBegin(GL_QUADS);
	for (a=0;a<6;++a)
	{
		for (b=0;b<4;++b)
		{
			currentVer=cube.quad[a].ver[b];

			//glColor3fv(cube.ver[ currentVer ].col);
			glVertex3fv(cube.ver[ currentVer ].pos);
			glTexCoord2f(cube.tex[a * 4 + b].s, cube.tex[a * 4 + b].t);
		}
	}
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}






