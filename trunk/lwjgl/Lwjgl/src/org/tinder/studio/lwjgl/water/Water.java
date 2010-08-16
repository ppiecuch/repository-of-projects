package org.tinder.studio.lwjgl.water;

import java.nio.DoubleBuffer;

import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.ARBFragmentProgram;
import org.lwjgl.opengl.ARBMultitexture;
import org.lwjgl.opengl.ARBVertexProgram;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL13;
import org.lwjgl.opengl.GL30;
import org.lwjgl.opengl.GL40;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-8-16 ����03:22:41
 * �����ǿ���ˮ��ʱ���������ȿ������Ǳ�ˮ��Ť���ķ���⼰����⣬
 * ���������Ҫһ��du/dv����ƫ�����������������������������s��t���ꡣ
 * �ɷ�������ʽ����֪���ж��ٹⱻ���䣬�ж��ٹⱻ���䡣
 * ���ǽ�ͨ��1��ȥ���ط������������λ�õĹ�һ�������ĵ��������ģ�������Ԫ�أ��������ǾͲ��ü���ÿһ�ֹ�Ĳ������Ӷ���ø��õ�Ч�ʡ�
 * ����ʵ�У���Ϊˮ�����൱�������������ӣ�ʹ���ǿ�������һ��ģ���ľ���
 * Ϊ��ģ����һЧ�������Ƕ�����ʹ������Ȼ��壬��Ȼ��Ҳֻ��һ������ֵ����������������õĻ������ǻ����Կ���һ���־��淴��⡣
 */
public class Water {
	
	private int texSize;
	private int reflection;
	private int refraction;
	private int depth;
	private int myVertexProgram;
	private int myFragmentProgram;
	private int texmove;
	
	/**
	 * ��Ⱦ��Ӱ,ͨ��glScale�ߵ��³�����ʵ��
	 */
	private void RenderReflection()
	{
	    GL11.glViewport(0,0, texSize, texSize);
	    GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);
	    GL11.glLoadIdentity();
//	    GL11.gluLookAt(......) 

	    GL11.glPushMatrix();
	    GL11.glTranslatef(0.0f, 0.0f, 0.0f);
	    GL11.glScalef(1.0f, -1.0f, 1.0f);
	    double plane[] = {0.0, 1.0, 0.0, 0.0}; //water at y=0
	    DoubleBuffer buffer=BufferUtils.createDoubleBuffer(plane.length);
	    buffer.put(plane);
	    GL11.glEnable(GL11.GL_CLIP_PLANE0);
	    GL11.glClipPlane(GL11.GL_CLIP_PLANE0, buffer);
//	    RenderScene();
	    GL11.glDisable(GL11.GL_CLIP_PLANE0);
	    GL11.glPopMatrix();

	    //render reflection to texture
	    GL11.glBindTexture(GL11.GL_TEXTURE_2D, reflection);
	    //glCopyTexSubImage2D copies the frame buffer 
	    //to the bound texture
	    GL11.glCopyTexSubImage2D(GL11.GL_TEXTURE_2D,0,0,0,0,0,texSize, texSize);
	}
	
	/**
	 * ��Ⱦˮ�׼���ȣ������ڵ�Ӱ�Ĵ�����֮ͬ���������赹ת�������Լ�ƽ�淨������Y����ȡ����
	 */
	private void renderRefractionAndDepth()
	{
		GL11.glViewport(0,0, texSize, texSize);
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);
		GL11.glLoadIdentity();
//		GL11.gluLookAt(......) 

		GL11.glPushMatrix();
		GL11.glTranslatef(0.0f, 0.0f, 0.0f);
	    //normal pointing along negative y
	    double plane[] = {0.0, -1.0, 0.0, 0.0}; 
	    DoubleBuffer buffer=BufferUtils.createDoubleBuffer(plane.length);
	    buffer.put(plane);
	    GL11.glEnable(GL11.GL_CLIP_PLANE0);
	    GL11.glClipPlane(GL11.GL_CLIP_PLANE0, buffer);
//	    RenderScene();
	    GL11.glDisable(GL11.GL_CLIP_PLANE0);
	    GL11.glPopMatrix();

	    //render color buffer to texture
	    GL11.glBindTexture(GL11.GL_TEXTURE_2D, refraction);
	    GL11.glCopyTexSubImage2D(GL11.GL_TEXTURE_2D,0,0,0,0,0,texSize, texSize);

	    //render depth to texture
	    GL11.glBindTexture(GL11.GL_TEXTURE_2D, depth);
	    GL11.glCopyTexImage2D(GL11.GL_TEXTURE_2D,0,GL11.GL_DEPTH_COMPONENT, 0,0, texSize,texSize, 0);
	}
	
	void RenderWater()
	{

	    //bind & enable shader programs
		GL11.glEnable(ARBVertexProgram.GL_VERTEX_PROGRAM_ARB);
		GL11.glEnable(ARBFragmentProgram.GL_FRAGMENT_PROGRAM_ARB);
		GL11.glBindProgramARB(ARBVertexProgram.GL_VERTEX_PROGRAM_ARB, myVertexProgram);
		GL11.glBindProgramARB(ARBFragmentProgram.GL_FRAGMENT_PROGRAM_ARB, myFragmentProgram);

	    //move texture across water surface
		GL11.glProgramLocalParameter4fARB(ARBVertexProgram.GL_VERTEX_PROGRAM_ARB, 0, texmove,texmove,texmove, 1.0f);
		GL11.glProgramLocalParameter4fARB(ARBVertexProgram.GL_VERTEX_PROGRAM_ARB, 1, -texmove,-texmove,-texmove, 1.0f);
	    //Set viewposition and lightposition
		GL11.glProgramLocalParameter4fARB(ARBVertexProgram.GL_VERTEX_PROGRAM_ARB, 2, viewpos.x,viewpos.y,viewpos.z, 1.0f);
		GL11.glProgramLocalParameter4fARB(ARBVertexProgram.GL_VERTEX_PROGRAM_ARB, 3, lightpos.x,lightpos.y,lightpos.z, 1.0f);

	    //Set watercolor
		GL11.glProgramLocalParameter4fARB(ARBFragmentProgram.GL_FRAGMENT_PROGRAM_ARB, 0, water.red,water.green,water.blue, 1.0f);

	    //bind all textures
		ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE0_ARB);
		GL11.glEnable(GL11.GL_TEXTURE_2D); 
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, reflection);

		ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE1_ARB);
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, refraction);

		ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE2_ARB);
	    GL11.glEnable(GL11.GL_TEXTURE_2D); 
	    GL11.glBindTexture(GL11.GL_TEXTURE_2D, normalmap);

	    ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE3_ARB);
	    GL11.glEnable(GL11.GL_TEXTURE_2D);
	    GL11.glBindTexture(GL11.GL_TEXTURE_2D, dudvmap);

	    ARBMultitexture.glActiveTextureARB(ARBMultitexture.GL_TEXTURE4_ARB);
	    GL11.glEnable(GL11.GL_TEXTURE_2D); 
	    GL11.glBindTexture(GL11.GL_TEXTURE_2D, depth);

	    //Render the water surface 
	    GL11.glBegin(GL11.GL_QUADS);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE0_ARB, 0.0f, 5.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE1_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE2_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE3_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE4_ARB, 0.0f, 1.0f);
	    GL11.glVertex3f(-5.0f, 0.0f, 5.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE0_ARB, 0.0f, 0.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE1_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE2_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE3_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE4_ARB, 0.0f, 1.0f);
	    GL11.glVertex3f(-5.0f, 0.0f, -5.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE0_ARB, 5.0f, 0.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE1_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE2_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE3_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE4_ARB, 0.0f, 1.0f);
	    GL11.glVertex3f(5.0f, 0.0f, -5.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE0_ARB, 5.0f, 5.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE1_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE2_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE3_ARB, 0.0f, 1.0f);
	    ARBMultitexture.glMultiTexCoord2fARB(ARBMultitexture.GL_TEXTURE4_ARB, 0.0f, 1.0f);
	    GL11.glVertex3f(5.0f, 0.0f, 5.0f);
	    GL11.glEnd();

	    //Disable texture units and shader programs
	    .
	    .
	    .
	}

}
