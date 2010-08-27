package org.tinder.studio.lwjgl.water;

import java.util.Random;

import org.lwjgl.opengl.GL11;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-8-26 下午03:22:41
 */
public class Water {
	
	private float[] sides;		//边长
	private int[] steps;		//步数
	private float[][][] points;
	private float[][] buffer;
	private Random random;
	private int count=0;
	private float[][][] texCoors;
	
	public Water(float[] sides,int[] steps) {
		this.sides=sides;
		this.steps=steps;
		this.random=new Random();
	}
	
	public void init(){
		float dx=sides[0]/steps[0];
		float dy=sides[1]/steps[1];
		
		float du=1/steps[0];
		float dv=1/steps[1];
		
		points=new float[steps[1]+1][][];
		texCoors=new float[steps[1]+1][][];
		for(int y = 0; y < steps[1]+1; y++ )
		{
			points[y]=new float[steps[0]+1][];
			texCoors[y]=new float[steps[0]+1][];
			for(int x = 0; x < steps[0]+1; x++ )
			{
				points[y][x]=new float[3];
				points[y][x][0]=x*dx;
				points[y][x][1]=y*dy;
				points[y][x][2]=0;
				
				texCoors[y][x]=new float[2];
				texCoors[y][x][0]=x*du;
				texCoors[y][x][1]=(steps[1]-y)*dv;
			}
		}
		
		buffer=new float[steps[1]][];
		for(int y = 0; y < steps[1]; y++ )
			buffer[y]=new float[steps[0]];
	}
	
	public void render(){
		if (count++==3) {
			count=0;
			exchange();
		}
		//glLoadIdentity();
//		glBindTexture(GL11.GL_TEXTURE_2D,texture[0]);
		
//		float float_x,float_y,float_xb,float_yb;
		GL11.glPushMatrix();
		GL11.glPolygonMode(GL11.GL_BACK,GL11.GL_FILL );
		GL11.glPolygonMode(GL11.GL_FRONT,GL11.GL_FILL );
		GL11.glColor3f(1.0f,1.0f,1.0f);
		GL11.glBegin(GL11.GL_LINE_LOOP);
		for(int y = 0; y < steps[1]; y++ )
		{
			for(int x = 0; x < steps[0]; x++ )
			{
//				GL11.glTexCoord2f( float_x, float_y);
				GL11.glVertex3f( points[y][x][0], points[y][x][2], points[y][x][1]);

//				GL11.glTexCoord2f( float_x, float_yb );
				GL11.glVertex3f( points[y][x+1][0], points[y][x+1][2], points[y][x+1][1] );

//				GL11.glTexCoord2f( float_xb, float_yb );
				GL11.glVertex3f( points[y+1][x+1][0], points[y+1][x+1][2], points[y+1][x+1][1] );

//				GL11.glTexCoord2f( float_xb, float_y );
				GL11.glVertex3f( points[y+1][x][0], points[y+1][x][2] , points[y+1][x][1]);
			}
		}
		GL11.glEnd();
	}
	
	private void exchange(){
		float tmp;
		int n=Math.abs(random.nextInt())%(steps[1]-1)+1;
		int m=Math.abs(random.nextInt())%(steps[0]-1)+1;
		points[n][m][2]+=2.0;
		for (int i=1; i<steps[1]; i++)
			for(int j=1;j<steps[0];j++)
			{
				buffer[i][j]=(points[i-1][j][2]+points[i+1][j][2]+points[i][j-1][2]+points[i][j+1][2])/2-buffer[i][j];
				buffer[i][j]=buffer[i][j]-buffer[i][j]/32;
			}
		for(int i=1;i<steps[1];i++)
			for(int j=1;j<steps[0];j++)
			{
				tmp=buffer[i][j];
				buffer[i][j]=points[i][j][2];
				points[i][j][2]=tmp;
			}
	}
}
