#include <GL/glut.h>

#include "FPSCounter.h"
#include <string.h>
#include <stdio.h>
#pragma comment(lib,"utility.lib")
FPSCounter fpsCounter;


GLdouble theta = 0;//��ת�Ƕ�
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
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(theta, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(-0.5,-0.5,0.0);
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.5,0.0,0.0);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(0.0,0.5,0.0);
	glEnd();
	glPopMatrix();

	theta ++;
	if(theta >= 360)
		theta = 0;

	printFPS();

	glutSwapBuffers();
}
void init()
{
}
void resize(int width,int height)
{
}
void idle()
{
	//glutPostRedisplay ��ǵ�ǰ������Ҫ���»��ơ�ͨ��glutMainLoop��һ��ѭ��ʱ��������ʾ�����ص���������ʾ���ڵ�������塣
	//��ε���glutPostRedisplay������һ����ʾ�ص�ֻ������һ��������ʾ�ص��� 
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y)
{
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
	//��ʼ��GLUT
	//Void glutInit(int*argc,char**argv)�� 
	//������ 
	//Argc��һ��ָ�룬ָ���main�����������ݹ�����û���ĵ�argc������ 
	//Argv��һ��ָ�룬ָ���main�����������ݹ�����û���ĵ�argv������
	glutInit(&argc, argv);
	//������ʾ��ʽ�� 
	//Void glutInitDisplayMode(unsighed int mode) 
	//������ 
	//Mode�D�D����ָ��������ʾģʽ 
	//Mode������һ��GLUT����Ԥ����Ŀ��ܵĲ�����ϡ���ʹ��modeȥָ����ɫģʽ�������ͻ��������͡� 
	//ָ����ɫģʽ��Ԥ���峣���У� 
	//1��GLUT_RGBA����GLUT_RGB��ָ��һ��RGBA���ڣ�����һ��Ĭ�ϵ���ɫģʽ�� 
	//2��GLUT_INDEX��ָ����ɫ����ģʽ�� 
	//�����ʾģʽ��������ѡ�񵥻�������˫���������ڡ� 
	//1��GLUT_SINGLE.�����������ڡ� 
	//2��GLUT_BUFFER.˫���������ڣ����ǲ���������������ѡ�ġ� 
	//������ָ�����࣬�������ָ��һ������Ļ���Ļ���������ı����� 
	//1��GLUT_ACCUM.�ۻ��������� 
	//2��GLUT_STENCIL.ģ�建������ 
	//3��GLUT_DEPTH.��Ȼ������� 
	//�á��򡰣�|������������������Ҫ����ʾģʽ�� 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//ȷ������λ�ã���Ĭ�ϵ�����Ļ���Ͻǣ�������ʹ�ú���glutInitWindowPosition������ 
	//Void glutInitWindowPositon(int x,int y); 
	//������ 
	//X: ������Ļ��ߵ�����������1��Ĭ��ֵ����˼�����ɴ��ڹ������������ڳ�������������ʹ��Ĭ��ֵ��������Լ�����һ��ֵ�� 
	//Y��������Ļ�ϱߵ�����������Xһ���� 
    //ע�⣬���������ǶԴ��ڹ�������һ�����顣�����㾫�ĵ������˴���λ�ã�window���صĿ����ǲ�ͬ��λ�á�����������ˣ�һ���õ�����Ҫ�Ľ����
	glutInitWindowPosition(100,100); 
	//���ô��ڴ�С��ʹ�ú���glutInitWindowSize������ 
	//Void glutInitWindowSize(int width,int height); 
	//������ 
	//Width�����ڵĿ�ȡ� 
	//Height�����ڵĸ߶ȡ� 
	//ͬ��width��heightҲֻ��һ���ο����֡�����ʹ�ø����� 
	glutInitWindowSize(640,480); 
	//���ú���glutCreateWindow()����������
	//Int glutCreateWindow(char* title); 
	//������ 
	//Title�����ô��ڵı��⡣ 
	//glutCreateWindow�����ķ���ֵ��һ�����ڱ�ʶ����
	(void) glutCreateWindow("GL Framework");

	init();

	//����GLUTʹ����������ĺ�����������Ⱦ������мĴ�ص����������Ǹ���GLUT�������renderSceneӦ�ñ�ʹ�á�
	//����Ҫ�ػ���ʱ��GLUT��һ��ֻ����һ���������Ʋ����ĺ������Ժ�����Ϊ�βεĺ������ͻᱻ���á� 
	//void glutDisplayFunc(void (*func)(void)); 
	//������ 
	//func: ��������Ҫ���ػ��ǵ��õĺ��������ơ�ע��ʹ��NULL��Ϊʵ���Ǵ���ġ�
	glutDisplayFunc(display);
	//��glutIdleFunc(void (*func)(void))�ص�������ָ��һ�����������������������δ��ɵ��¼������磬���¼�ѭ�����ڿ��е�ʱ�򣩣���ִ�����������
	//����ص���������һ������ָ����Ϊ����Ψһ�����������������NULL(0),���൱�ڽ������������
	glutIdleFunc(idle);
	//���ڷ����仯ʱ,�Ӵ���Ҫ���¼���.
	//void glutReshapeFunc(void (*func)(int width, int height)); 
	//����: 
	//func -- The name of the function that will be responsible forsetting the correct perspective when the window changes size. 
	glutReshapeFunc(resize);
	//void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
	// key Ϊ���̰�����ASCII��
	glutKeyboardFunc(key);
	//������Clicks
	//�ͼ��̴���һ����GLUTΪ���ע�ắ����Ҳ���Ǵ������clicks�¼��ĺ������ṩ��һ������������glutMouseFunc,�������һ���ڳ����ʼ���׶α����á�����ԭ�����£�
	//void glutMouseFunc(void(*func)(int button,int state,int x,int y));
	//������
	//func���������click�¼��ĺ����ĺ�������
	//��������Կ��������������click�¼��ĺ�����һ����4��������
	//��һ�����������ĸ����������»��ɿ�������������������������ֵ�е�һ����
	//GLUT_LEFT_BUTTON
	//GLUT_MIDDLE_BUTTON
	//GLUT_RIGHT_BUTTON
	//�ڶ����������������������÷���ʱ������״̬��Ҳ�����Ǳ����£����ɿ�������ȡֵ���£�
	//GLUT_DOWN
	//GLUT_UP
	//������������ʱ��state��ֵ��GLUT_DOWN����ô������ܻ�ٶ������и�GLUT_UP�¼�����������ƶ����������棬Ҳ��ˡ�
	//Ȼ��������������glutMouseFunc����NULL��Ϊ��������ôGLUT������ı�����״̬��
	//ʣ�µ�����������x,y���ṩ����굱ǰ�Ĵ������꣨�����Ͻ�Ϊԭ�㣩��
	glutMouseFunc(mouse);
	//��⶯����motion��
	//GLUT�ṩ���motion���������������GLUT�����motion��active motion��passive motion��
	//Active motion��ָ����ƶ�������һ�����������¡����϶���꣩
	//Passive motion��ָ������ƶ�ʱ������û�������¡����ƶ���꣩
	//���һ����������׷����꣬��ô����ƶ��ڼ䣬ûһ֡������һ�������
	//����ǰһ���������ע�Ὣ��������¼��ĺ��������庯������GLUT�����ǿ���ָ��������ͬ�ĺ�����һ��׷��passive motion����һ��׷��active motion
	//���ǵĺ���ԭ�ͣ����£�
	//void glutMotionFunc(void(*func)(int x,int y));
	//void glutPassiveMotionFunc(void (*func)(int x,int y));
	//������
	//Func�������������motion�ĺ�������
	//����motion�Ĳ��������Ĳ�����x,y��������ڴ��ڵ����ꡣ�����Ͻ�Ϊԭ�㡣
	glutMotionFunc(motion);

	//�����������뿪����
	//GLUT���ܼ���������뿪�����봰������һ���ص��������Ա�����ȥ�����������¼���GLUT����������������glutEntryFunc,����ԭ�����£�
	//void glutEntryFunc(void(*func)��int state��);
	//������
	//Func��������Щ�¼��ĺ�������
	//���溯���Ĳ����У�state������ֵ��
	//GLUT_LEFT      ����뿪����
	//GLUT_ENTERED  �����봰��
	//���������뿪�����ǽ��봰�ڡ�
	//glutEntryFunc(entry);
	//glutVisibilityFunc(vis);
	//glutCreateMenu(menu);
	//glutAddMenuEntry("Toggle showing projection", 's');
	//glutAddMenuEntry("Switch texture", 't');
	//glutAddMenuEntry("Switch object", 'o');
	//glutAddMenuEntry("Toggle filtering", 'l');
	//glutAddMenuEntry("Quit", 666);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
	//����ö�ʱ���Ļ�����ʼ��ʱ��ע��һ����ʱ���Ļص�������ԭ����
	//glutTimerFunc(unsigned int millis, void (*func)(int value), int value);
	//����ʹ��glutTimerFunc(������, �ص�����ָ��, ����ֵ);
	//д�Լ��Ļص�����
	//void OnTimer(int value);
	//��value�������ĸ���ʱ��
	//�ں�����ı��λ���йصı�����Ȼ�����glutPostRedisplay();�����ػ�
	//����ٴε���glutTimerFunc����Ϊglut�Ķ�ʱ���ǵ���һ�βŲ���һ�ζ�ʱ���������Ҫ����������ʱ�Ļ����ڶ�ʱ����ĩβ�ٴε���glutTimerFunc


	//����GLUT����׼������Ӧ�ó����¼�����ѭ����GLUT�ṩ��һ�������ó������һ������������ѭ����һֱ�ȴ�������һ���¼���������glutMainLoop������ 
	//glutMainLoop()һ������ѭ������Ҳ������
	//glutMainLoop()  �����ϵͳ���������̣��ڹر�glutCreateWindow()�����Ĵ��ں�glutMainLoop()ֱ����exit(0)�˳����򣬶��������ִ��glutMainLoop()�����䣬
	//���ʹһЩ����ͷż���Դ���չ����޷����У��Ӷ�����ڴ�й©�����������⣬��3���취��
	//1��ʹ��glutLeaveMainLoop()����glutMainLoop()��
	//2�����������exit(0)�Ĳ��֡� 
	//3����glutMainLoop֮ǰ�����ã� 
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
	//Ȼ������glutMainLoop();�� glutMainLoop()�����˳��󣬼���ִ�����Ĵ��롣
	//4��������������󣬲��������ٺ����е��������Դ�ͷŴ���
	//ʹ�����Ϸ�����������Ч����glutMainLoop()��ɵ��ڴ�й©��
	//void glutMainLoop(void) 
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}