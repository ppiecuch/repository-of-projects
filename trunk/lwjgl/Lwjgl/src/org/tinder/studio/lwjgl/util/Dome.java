package org.tinder.studio.lwjgl.util;

import org.lwjgl.opengl.GL11;

/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-13ÏÂÎç11:33:54
 *
 */
public class Dome {
	
	private static final double DTOR=Math.PI/180;
	private int horizonalSteps,portraitSteps;
	private float radius;
	private Point3f[][] vertex;
	
	public Dome(float radius, int horizonalSteps, int portraitSteps) {
		this.horizonalSteps=horizonalSteps;
		this.portraitSteps=portraitSteps;
		this.radius=radius;
		float dtheta=(float)360/horizonalSteps;
		float dphi=(float)90/portraitSteps;
		vertex=new Point3f[portraitSteps][];
		for(int i=0,phi=0;i<portraitSteps;i++,phi+=dphi)
		{
			vertex[i]=new Point3f[horizonalSteps];
			for(int j=0,theta=0;j<horizonalSteps;j++,theta+=dtheta)
			{
				Point3f point=new Point3f();
				point.x = (float) (radius * Math.sin(phi*DTOR) * Math.cos(DTOR*theta));
				point.y = (float) (radius * Math.sin(phi*DTOR) * Math.sin(DTOR*theta));
				point.z = (float) (radius * Math.cos(phi*DTOR));
				vertex[i][j]=point;
				System.out.print(point);
			}
			System.out.println();
		}
	}

	public int getHorizonalSteps() {
		return horizonalSteps;
	}

	public int getPortraitSteps() {
		return portraitSteps;
	}

	public float getRadius() {
		return radius;
	}

	public Point3f[][] getVertex() {
		return vertex;
	}
	
	public void render()
	{
		GL11.glBegin(GL11.GL_TRIANGLE_STRIP);
		for(int i=0;i<portraitSteps;i++)
		{
			for(int j=0;j<horizonalSteps;j++)
			{
				GL11.glVertex3f(vertex[i][j].x, vertex[i][j].z,vertex[i][j].y);
			}
		}
		GL11.glEnd();
	}
	
	
}
