#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include <time.h>


#include<GL/glaux.h>

#   pragma comment (lib, "glaux.lib");
#include<iostream>
using namespace std;

#define GLUT_KEY_ESCAPE 27
GLint winWidth=800,winHeight=600;
GLuint texture[3]; 
float points[129][129][3];
float buffer[128][128]={0};
int contr=0;
GLfloat LightAmbient[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 1.0f, 1.0f, 1.0f, 0.0f };

AUX_RGBImageRec *LoadBMP(char *Filename)
{
	FILE *File=NULL;
	if(!Filename)
		return NULL;
	File=fopen(Filename,"r");
	if(File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

int LoadGLTextures()
{
	int Status=FALSE;
	AUX_RGBImageRec *TextureImage[1];
	memset(TextureImage,0,sizeof(AUX_RGBImageRec *));
	if(TextureImage[0]=LoadBMP("Data.bmp"))
	{
		Status=TRUE;
		glGenTextures(1,&texture[0]);
		glBindTexture(GL_TEXTURE_2D,texture[0]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,TextureImage[0]->sizeX,TextureImage[0]->sizeY,GL_RGB,GL_UNSIGNED_BYTE,TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	if(TextureImage[0])
	{
		if(TextureImage[0]->data)
			free(TextureImage[0]->data);
		free(TextureImage[0]);
	}
	return Status;
}

int init()
{
	if(!LoadGLTextures())
		return FALSE;
	
	glEnable(GL_TEXTURE_2D);
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,0,1,-10,10);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);						
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0.0,0.0,0.0,0,1,-10,0,0,1);
	for(int x=0; x<129; x++)
	{
		for(int y=0; y<129; y++)
		{
			points[x][y][0]=float(x/128.0);
			points[x][y][1]=float(y/128.0);
			points[x][y][2]=0.0;
		}
	}
	return TRUE;
}

void displayFcn()
{
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();*/
	
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	
	float float_x,float_y,float_xb,float_yb;
	glPushMatrix();
	glPolygonMode( GL_BACK, GL_FILL );
	glPolygonMode( GL_FRONT, GL_FILL );
	//glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
	for(int x = 0; x < 128; x++ )
	{
		for(int y = 0; y < 128; y++ )
		{
			float_x = float(x)/128.0;
			float_y = float(y)/128.0;
			float_xb = float(x+1)/128.0;
			float_yb = float(y+1)/128.0;
			glTexCoord2f( float_x, float_y);
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2f( float_x, float_yb );
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2f( float_xb, float_yb );
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2f( float_xb, float_y );
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();
	/*glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,10);
	glEnd();*/
	
	glPopMatrix();
	glutSwapBuffers();
	
	glFlush();
}


void exchange()
{
	srand(time(NULL));
	float tmp;
	if(contr==0)
	{
		int n=rand()%127+1;
		int m=rand()%127+1;
		points[n][m][2]+=2.0;
		for (int i=1; i<128; i++)
			for(int j=1;j<128;j++)
			{
				buffer[i][j]=(points[i-1][j][2]+points[i+1][j][2]+points[i][j-1][2]+points[i][j+1][2])/2.0-buffer[i][j];
				buffer[i][j]=buffer[i][j]-buffer[i][j]/32.0;
			}
		for(i=1;i<128;i++)
			for(int j=1;j<128;j++)
			{
				tmp=buffer[i][j];
				buffer[i][j]=points[i][j][2];
				points[i][j][2]=tmp;
			}
	}
	contr++;
	if(contr==5)
		contr=0;
	glutPostRedisplay();
}



void mouseFcn(GLint button,GLint action,GLint x,GLint y)
{
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			if(action==GLUT_DOWN)
			{
				cout<<x<<"        "<<y<<endl;
				int ex=int(x*128.0/winWidth);
				cout<<ex<<endl;
				int ey=int((winHeight-y)*128.0/winHeight);
				cout<<ey<<endl;
				points[ex][ey][2]+=2.0;
			}
			break;
	}
	//glutPostRedisplay();
}

void SpecialKeys(unsigned char key, int x, int y)
{   
  if(key==(char)GLUT_KEY_ESCAPE)   
	exit(0);   
} 


int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(winWidth,winHeight);
	glutCreateWindow("test for 3D");
	

	init();
	glutDisplayFunc(displayFcn);
	glutIdleFunc(exchange);
	glutMouseFunc(mouseFcn);
	glutKeyboardFunc(SpecialKeys);
	glutMainLoop();
}
