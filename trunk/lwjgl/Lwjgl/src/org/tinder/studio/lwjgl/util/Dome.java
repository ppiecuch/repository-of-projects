package org.tinder.studio.lwjgl.util;

import org.lwjgl.opengl.GL11;

/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-13下午11:33:54
 *
 */
public class Dome {
	
	private static final double DTOR=Math.PI/180;
	private int horizonalSteps,portraitSteps;
	private float radius;					//半径
	private Point3f[][][] vertex;			//顶点坐标
	private Point2f[][][] texCoords;		//纹理坐标
	
	/**
	 * 以原点为球心，绘制半球形天空
	 * @param radius 球的半径
	 * @param horizonalSteps 水平方向分割步数
	 * @param portraitSteps 垂直方向分割步数
	 */
	public Dome(float radius, int horizonalSteps, int portraitSteps) {
		this.horizonalSteps=horizonalSteps;
		this.portraitSteps=portraitSteps;
		this.radius=radius;
		float dtheta=(float)360/horizonalSteps;	//水平方向步增
		float dphi=(float)90/portraitSteps;		//垂直方向步增
		vertex=new Point3f[portraitSteps][][];
		texCoords=new Point2f[portraitSteps][][];
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
				point.y = (float) (radius * Math.sin(phi*DTOR) * Math.cos(DTOR*theta));
				point.x = (float) (radius * Math.sin(phi*DTOR) * Math.sin(DTOR*theta));
				point.z = (float) (radius * Math.cos(phi*DTOR));
				vertex[i][j][0]=point;
				Vector3f vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)j/horizonalSteps;
				uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
				texCoords[i][j][0]=uv;
				
				point=new Point3f();
				uv=new Point2f();
				point.y = (float) (radius * Math.sin(phi*DTOR) * Math.cos(DTOR*(theta+dtheta)));
				point.x = (float) (radius * Math.sin(phi*DTOR) * Math.sin(DTOR*(theta+dtheta)));
				point.z = (float) (radius * Math.cos(phi*DTOR));
				vertex[i][j][1]=point;
				vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)(j+1)/horizonalSteps;
				uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
				texCoords[i][j][1]=uv;
				
				point=new Point3f();
				uv=new Point2f();
				point.y = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.cos(DTOR*(theta+dtheta)));
				point.x = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.sin(DTOR*(theta+dtheta)));
				point.z = (float) (radius * Math.cos((phi+dphi)*DTOR));
				vertex[i][j][2]=point;
				vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)(j+1)/horizonalSteps;
				uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
				texCoords[i][j][2]=uv;
				
				point=new Point3f();
				uv=new Point2f();
				point.y = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.cos(DTOR*theta));
				point.x = (float) (radius * Math.sin((phi+dphi)*DTOR) * Math.sin(DTOR*theta));
				point.z = (float) (radius * Math.cos((phi+dphi)*DTOR));
				vertex[i][j][3]=point;
				vector=new Vector3f(point.x, point.y, point.z);
				vector.normalize();
				uv.x=(float)j/horizonalSteps;
				uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
				texCoords[i][j][3]=uv;
			}
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
