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
	private Point3f[][][] vertex;
	private Point2f[][][] texCoords;
	
	public Dome(float radius, int horizonalSteps, int portraitSteps) {
		this.horizonalSteps=horizonalSteps;
		this.portraitSteps=portraitSteps;
		this.radius=radius;
		float dtheta=(float)360/horizonalSteps;
		float dphi=(float)90/portraitSteps;
		vertex=new Point3f[portraitSteps][][];
		texCoords=new Point2f[portraitSteps][][];
//		Point2f[] temp=new Point2f[3];
//		int index=0;
		for(int i=0,phi=0;i<portraitSteps;i++,phi+=dphi)
		{
			vertex[i]=new Point3f[horizonalSteps][];
			texCoords[i]=new Point2f[horizonalSteps][];
			for(int j=0,theta=0;j<horizonalSteps;j++,theta+=dtheta)
			{
				vertex[i][j]=new Point3f[4];
				texCoords[i][j]=new Point2f[4];
				Point3f point=new Point3f();
				Point2f uv=new Point2f();
				point.x = (float) (radius * Math.sin(phi*DTOR) * Math.cos(DTOR*theta));
				point.y = (float) (radius * Math.sin(phi*DTOR) * Math.sin(DTOR*theta));
				point.z = (float) (radius * Math.cos(phi*DTOR));
				vertex[i][j][0]=point;
				Vector3f vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				//tu = asin(Nx)/PI + 0.5 
				//tv = asin(Ny)/PI + 0.5 
				uv.x=(float)(Math.atan2(vector.x, vector.z)/(Math.PI*2)) + 0.5f;
				uv.y=(float)(Math.asin(vector.y) / Math.PI) + 0.5f;
//				uv.x=(float) (Math.asin(vector.x)/Math.PI+0.5f);
//				uv.y=(float) (Math.asin(vector.y)/Math.PI+0.5f);
				texCoords[i][j][0]=uv;
				System.out.print("point:"+point+",uv:"+uv);
//				temp[0]=temp[1];
//				temp[1]=temp[2];
//				temp[2]=uv;
//				if(index++>=2)revise(temp[0], temp[1], temp[2]);
				
				point=new Point3f();
				uv=new Point2f();
				point.x = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.cos(DTOR*theta));
				point.y = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.sin(DTOR*theta));
				point.z = (float) (radius * Math.cos((phi+dphi)*DTOR));
				vertex[i][j][1]=point;
				vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)(Math.atan2(vector.x, vector.z)/(Math.PI*2)) + 0.5f;
				uv.y=(float)(Math.asin(vector.y) / Math.PI) + 0.5f;
//				uv.x=(float) (Math.asin(vector.x)/Math.PI+0.5f);
//				uv.y=(float) (Math.asin(vector.y)/Math.PI+0.5f);
				texCoords[i][j][1]=uv;
				System.out.print("point:"+point+",uv:"+uv);
//				temp[0]=temp[1];
//				temp[1]=temp[2];
//				temp[2]=uv;
//				if(index++>=2)revise(temp[0], temp[1], temp[2]);
				
				point=new Point3f();
				uv=new Point2f();
				point.x = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.cos(DTOR*(theta+dtheta)));
				point.y = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.sin(DTOR*(theta+dtheta)));
				point.z = (float) (radius * Math.cos((phi+dphi)*DTOR));
				vertex[i][j][2]=point;
				vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)(Math.atan2(vector.x, vector.z)/(Math.PI*2)) + 0.5f;
				uv.y=(float)(Math.asin(vector.y) / Math.PI) + 0.5f;
//				uv.x=(float) (Math.asin(vector.x)/Math.PI+0.5f);
//				uv.y=(float) (Math.asin(vector.y)/Math.PI+0.5f);
				texCoords[i][j][2]=uv;
				System.out.print("point:"+point+",uv:"+uv);
//				temp[0]=temp[1];
//				temp[1]=temp[2];
//				temp[2]=uv;
//				if(index++>=2)revise(temp[0], temp[1], temp[2]);
				
				point=new Point3f();
				uv=new Point2f();
				point.x = (float) (radius * Math.sin(phi*DTOR) * Math.cos(DTOR*(theta+dtheta)));
				point.y = (float) (radius * Math.sin(phi*DTOR) * Math.sin(DTOR*(theta+dtheta)));
				point.z = (float) (radius * Math.cos(phi*DTOR));
				vertex[i][j][3]=point;
				vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)(Math.atan2(vector.x, vector.z)/(Math.PI*2)) + 0.5f;
				uv.y=(float)(Math.asin(vector.y) / Math.PI) + 0.5f;
//				uv.x=(float) (Math.asin(vector.x)/Math.PI+0.5f);
//				uv.y=(float) (Math.asin(vector.y)/Math.PI+0.5f);
				texCoords[i][j][3]=uv;
				System.out.println("point:"+point+",uv:"+uv);
//				temp[0]=temp[1];
//				temp[1]=temp[2];
//				temp[2]=uv;
//				if(index++>=2)revise(temp[0], temp[1], temp[2]);
			}
			System.out.println();
		}
	}
	
//	private void revise(Point2f p0,Point2f p1,Point2f p2)
//	{
//		if (p0.x - p1.x > 0.9f)
//			p1.x += 1.0f;
//
//		if (p1.x - p0.x > 0.9f)
//			p0.x += 1.0f;
//
//		if (p0.x - p2.x > 0.9f)
//			p2.x += 1.0f;
//
//		if (p2.x - p0.x > 0.9f)
//			p0.x += 1.0f;
//
//		if (p1.x - p2.x > 0.9f)
//			p2.x += 1.0f;
//
//		if (p2.x - p1.x > 0.9f)
//			p1.x += 1.0f;
//
//		if (p0.y - p1.y > 0.8f)
//			p1.y += 1.0f;
//
//		if (p1.y - p0.y > 0.8f)
//			p0.y += 1.0f;
//
//		if (p0.y - p2.y > 0.8f)
//			p2.y += 1.0f;
//
//		if (p2.y - p0.y > 0.8f)
//			p0.y += 1.0f;
//
//		if (p1.y - p2.y > 0.8f)
//			p2.y += 1.0f;
//
//		if (p2.y - p1.y > 0.8f)
//			p1.y += 1.0f;
//	}

	public int getHorizonalSteps() {
		return horizonalSteps;
	}

	public int getPortraitSteps() {
		return portraitSteps;
	}

	public float getRadius() {
		return radius;
	}

	public Point3f[][][] getVertex() {
		return vertex;
	}
	
	public void render()
	{
		GL11.glBegin(GL11.GL_QUADS);
		for(int i=0;i<portraitSteps;i++)
		{
			for(int j=0;j<horizonalSteps;j++)
			{
//				GL11.glBegin(GL11.GL_LINE_LOOP);
				for(int k=0;k<4;k++)
				{
					GL11.glTexCoord2f(texCoords[i][j][k].x,texCoords[i][j][k].y);
					GL11.glVertex3f(vertex[i][j][k].x, vertex[i][j][k].z,vertex[i][j][k].y);
				}
//				GL11.glEnd();
			}
		}
		GL11.glEnd();
	}
	
	
}
