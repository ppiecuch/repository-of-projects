package org.tinder.studio.lwjgl.util;

import java.io.Serializable;

import org.lwjgl.opengl.GL11;

/**
 * 
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-6-30ÏÂÎç08:46:09
 * 
 */
public class Bounding implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public float maxX;
	public float maxY;
	public float maxZ;
	public float minX;
	public float minY;
	public float minZ;
	
	public Bounding(){
		maxX = 0.0f;
	    maxY = 0.0f;
	    maxZ = 0.0f;
	    minX = 0.0f;
	    minY = 0.0f;
	    minZ = 0.0f;
	}
	
	public Bounding(float minX,float minY,float minZ,float maxX,float maxY, float maxZ)
	{
		this.minX=minX;
		this.minY=minY;
		this.minZ=minZ;
		this.maxX=maxX;
		this.maxY=maxY;
		this.maxZ=maxZ;
	}
	
	public void renderLine()
	{
		GL11.glBegin(GL11.GL_LINES);
		//Bottom
		GL11.glVertex3f(minX, minY, minZ);GL11.glVertex3f(maxX, minY, minZ);
		GL11.glVertex3f(maxX, minY, minZ);GL11.glVertex3f(maxX, minY, maxZ);
		GL11.glVertex3f(maxX, minY, maxZ);GL11.glVertex3f(minX, minY, maxZ);
		GL11.glVertex3f(minX, minY, maxZ);GL11.glVertex3f(minX, minY, minZ);
		//Top
		GL11.glVertex3f(minX, maxY, minZ);GL11.glVertex3f(maxX, maxY, minZ);
		GL11.glVertex3f(maxX, maxY, minZ);GL11.glVertex3f(maxX, maxY, maxZ);
		GL11.glVertex3f(maxX, maxY, maxZ);GL11.glVertex3f(minX, maxY, maxZ);
		GL11.glVertex3f(minX, maxY, maxZ);GL11.glVertex3f(minX, maxY, minZ);
		//Front
		GL11.glVertex3f(minX, minY, maxZ);GL11.glVertex3f(maxX, minY, maxZ);
		GL11.glVertex3f(maxX, minY, maxZ);GL11.glVertex3f(maxX, maxY, maxZ);
		GL11.glVertex3f(maxX, maxY, maxZ);GL11.glVertex3f(minX, maxY, maxZ);
		GL11.glVertex3f(minX, maxY, maxZ);GL11.glVertex3f(minX, minY, maxZ);
		//Back
		GL11.glVertex3f(minX, minY, minZ);GL11.glVertex3f(maxX, minY, minZ);
		GL11.glVertex3f(maxX, minY, minZ);GL11.glVertex3f(maxX, maxY, minZ);
		GL11.glVertex3f(maxX, maxY, minZ);GL11.glVertex3f(minX, maxY, minZ);
		GL11.glVertex3f(minX, maxY, minZ);GL11.glVertex3f(minX, minY, minZ);
		//Left
		GL11.glVertex3f(minX, minY, minZ);GL11.glVertex3f(minX, minY, maxZ);
		GL11.glVertex3f(minX, minY, maxZ);GL11.glVertex3f(minX, minY, maxZ);
		GL11.glVertex3f(minX, maxY, maxZ);GL11.glVertex3f(minX, maxY, minZ);
		GL11.glVertex3f(minX, maxY, minZ);GL11.glVertex3f(minX, minY, minZ);
		//Right
		GL11.glVertex3f(maxX, minY, minZ);GL11.glVertex3f(maxX, minY, maxZ);
		GL11.glVertex3f(maxX, minY, maxZ);GL11.glVertex3f(maxX, maxY, maxZ);
		GL11.glVertex3f(maxX, maxY, maxZ);GL11.glVertex3f(maxX, maxY, minZ);
		GL11.glVertex3f(maxX, maxY, minZ);GL11.glVertex3f(maxX, minY, minZ);
		GL11.glEnd();
	}

}
