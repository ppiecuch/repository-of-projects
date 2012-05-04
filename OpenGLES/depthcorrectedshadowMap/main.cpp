#define  _USE_MATH_DEFINES
#include "glsl_common.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>

#define SHADOW_MAP_RESOLUTION	512
#define BUMP_RESOLUTION			256

glLight			light;
glCamera		camera, model;

glShader		vertexShader, fragmentShader;
glProgram		shaderProgram, shaderProgramBump;

glCamera		*curFrame = &camera;
glTexture		depthTex, bumpTex;
int				depthTexUnit = 0, bumpTexUnit = 1;
glMaterial		torusMaterial, backboardMaterial;
glColor			blue(0.7f, 0.7f, 1.0f);
glColor			red(1.0f, 0.7f, 0.7f);

glFramebuffer	fb;
glModel			torus, backboard;

bool draw_depth_texture = false;
bool animate_model = true;
float rot_angle = 180.0f;
float T[16], dt = 0.5f;
bool draw_line = false;

float *makeBumpTexture(int resolution)
{
	float *buf = new float [4 * resolution * resolution];
	float x, y, z, len;
	for ( int i = 0; i < resolution; i++ )
	{
		x = 6.0f * (float)i / (float)resolution - 3.0f;
		for ( int j = 0; j < resolution; j++ )
		{
			y = 6.0f * (float)j / (float)resolution - 3.0f;
			z = exp(-0.5f * (x * x + y * y));

			len = sqrt((x * x + y * y) * z * z + 1.0f);

			buf[4 * (i+j*resolution) + 0] = x * z / len;
			buf[4 * (i+j*resolution) + 1] = y * z / len;
			buf[4 * (i+j*resolution) + 2] = 1.0 / len;
			buf[4 * (i+j*resolution) + 3] = z;
		}
	}
	return buf;
}

void drawQuad(const float sz)
{
	const float texc = 1.0f;
	float data[][5] = {	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, { texc, 0.0f, sz, 0.0f, 0.0f }, { texc, texc, sz, sz, 0.0f }, { 0.0f, texc, 0.0f, sz, 0.0f } };
	glInterleavedArrays(GL_T2F_V3F, 0, data);
	glDrawArrays(GL_QUADS, 0, 4);
}

void initShadowMap(void)
{
	depthTex.setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	depthTex.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	depthTex.init(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 1, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);

	fb.setTexture(depthTex, GL_DEPTH_ATTACHMENT_EXT);
}

void updateShadowMap(void)
{
	fb.bind();

	glEnable(GL_POLYGON_OFFSET_FILL);

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	light.applyProjection();

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glLoadIdentity();
		light.multInverseTransform();
		backboard.call();
		model.multTransform();
		glRotatef(rot_angle, 1.0f, 1.0f, 1.0f);
		torus.call();
	glPopMatrix();

	glPushMatrix();
	light.loadProjectionMatrix();
	light.multInverseTransform();
	camera.multTransform();
	glGetFloatv(GL_MODELVIEW_MATRIX, T);
	glPopMatrix();
	shaderProgram.setUniformMatrix("T", 4, T);
	shaderProgramBump.setUniformMatrix("T", 4, T);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();

	glDisable(GL_POLYGON_OFFSET_FILL);

	fb.release();
}

void init(void)
{
	glewInit();

	glPolygonOffset(1.0f, 1.0f);

	light.enable();

	glEnable(GL_DEPTH_TEST);

	camera.lookAt(5.0f, -2.0f, 2.0f, 70.0f, 45.0f, -2.0f);
	camera.setFOV(45.0f);
	camera.setNearFar(2.0f, 10.0f);

	light.lookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f);
	light.setFOV(30.0f);
	light.setNearFar(8.0f, 15.0f);
	light.setAspectRatio(1.0f);

	model.lookAt(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);

	light.setAmbient(glColor(0.3f, 0.3f, 0.3f));
	light.setDiffuse(glColor(0.5f, 0.5f, 0.5f));
	light.setSpecular(glColor(0.5f, 0.5f, 0.5f));
	
	torusMaterial.setSpecular(blue);
	torusMaterial.setDiffuse(blue);
	torusMaterial.setAmbient(blue);
	torusMaterial.setShininess(5.0f);

	backboardMaterial.setSpecular(red);
	backboardMaterial.setAmbient(red);
	backboardMaterial.setShininess(1.0f);

	bumpTex.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	bumpTex.init(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, BUMP_RESOLUTION, BUMP_RESOLUTION, 1, 0, GL_RGBA, GL_FLOAT, makeBumpTexture(BUMP_RESOLUTION));
	initShadowMap();

	torus.newList();
	drawTorus(1.0f, 0.3f, 32, 64);
	torus.endList();
	torus.setProgram(&shaderProgram);
	torus.setMaterial(&torusMaterial);
	torus.setTexture(&depthTex);

	backboard.newList();
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -2.5f);
		drawBox(5.0f, 5.0f, 0.1f, 1.0f / dt);
		glPopMatrix();
	backboard.endList();

	backboard.setProgram(&shaderProgramBump);
	backboard.setMaterial(&backboardMaterial);
	backboard.setTexture(&depthTex, depthTexUnit);
	backboard.setTexture(&bumpTex, bumpTexUnit);

	vertexShader.addDefinition("#define BUMP");
	vertexShader.loadShaderSource(GL_VERTEX_SHADER, "depth corrected shadowMap.vert");
	cout << vertexShader.getInfoLog() << endl;

	fragmentShader.addDefinition("#define BUMP");
	fragmentShader.loadShaderSource(GL_FRAGMENT_SHADER, "depth corrected shadowMap.frag");
	cout << fragmentShader.getInfoLog() << endl;
	
	shaderProgram.attach(&vertexShader, &fragmentShader);
	cout << shaderProgram.getInfoLog() << endl;

	shaderProgramBump.attach(&vertexShader, &fragmentShader);
	shaderProgramBump.setMask(&vertexShader, 1);
	shaderProgramBump.setMask(&fragmentShader, 1);
	cout << shaderProgramBump.getInfoLog() << endl;

	shaderProgram.setUniform("depthTexture", depthTexUnit);
	shaderProgramBump.setUniform("depthTexture", depthTexUnit);
	shaderProgramBump.setUniform("bumpTexture", bumpTexUnit);
	shaderProgramBump.setUniform("dt", dt);
}

void drawTexture(glTexture &tex)
{
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	
	bool is_light_turned_on = light.isEnabled();
	light.disable();

	glPushAttrib(GL_VIEWPORT_BIT);

	glViewport(0, camera.getWindowHeight()  >> 1, camera.getWindowHeight() >> 1, camera.getWindowHeight() >> 1);
	
	tex.bind(0);
	glDisable(GL_DEPTH_TEST);
	drawQuad(1.0f);
	glEnable(GL_DEPTH_TEST);
	depthTex.disable(depthTexUnit);

	if ( is_light_turned_on ) light.enable();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	

	glPopAttrib();
}

void displayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	camera.multInverseTransform();
	
	light.update();

	updateShadowMap();

	glPushMatrix();
	if ( draw_line )
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			backboard.call();
			model.multTransform();
			glRotatef(rot_angle, 1.0f, 1.0f, 1.0f);
			torus.call();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else
	{
		backboard.draw();
		model.multTransform();
		glRotatef(rot_angle, 1.0f, 1.0f, 1.0f);
		torus.draw();
	}
	glPopMatrix();
	

	if ( draw_depth_texture )
	{
		depthTex.setParameter(GL_TEXTURE_COMPARE_MODE, GL_NONE);
		drawTexture(depthTex);
		depthTex.setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		depthTex.bind(depthTexUnit);
	}
	depthTex.disable(depthTexUnit);
	
	glDisable(GL_DEPTH_TEST);
		glText::print(10, 30, "%i fps", (int)glTimer::getFPS());
	glEnable(GL_DEPTH_TEST);
	
	glutSwapBuffers();
}

void specialFunc(int key, int x, int y)
{
	if ( key != GLUT_KEY_LEFT && key != GLUT_KEY_RIGHT && key != GLUT_KEY_UP && key != GLUT_KEY_DOWN ) return;
}

void idleFunc(void)
{
	if ( animate_model ) rot_angle = 100.0f * glTimer::toc();
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	static bool flag = true;
	switch ( key )
	{
	case 27 :
		exit(1);
		break;
	case 'c' :
	case 'C' :
		curFrame = &camera;
		break;
	case 'l' :
	case 'L' :
		curFrame = &light;
		break;
	case 'm' :
	case 'M' :
		curFrame = &model;
		break;
	case 'd' :
	case 'D' :
		draw_line = !draw_line;
		break;
	case ' ' :
		animate_model = !animate_model;
		if ( !animate_model ) glTimer::suspend();
		else glTimer::resume();
		break;
	case 't' :
	case 'T' :
		draw_depth_texture = !draw_depth_texture;
		break;
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	curFrame->mouseFunc((GLSL_COMMON::MouseButtonType)button, state, x, y);
}

void motionFunc(int x, int y)
{
	curFrame->motionFunc(x, y);
}

void reshapeFunc(int w, int h)
{
	camera.reshapeFunc(w, h);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB | GLUT_ALPHA);

	glutInitWindowSize(512, 512);
	glutCreateWindow("depth corrected shadowMap");

	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
	glutDisplayFunc(displayFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(idleFunc);

	init();

	glutMainLoop();

	return 0;
}
