package org.tinder.studio.lwjgl.water;

/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-8-26 ÏÂÎç03:22:41
 */
public class Water {
	
	private float[] sides;
	private int[] steps;		//²½Êı
	private float[][][] points;
	
	public Water(float[] sides) {
		this.sides=sides;
	}
	
	public void init(){
		float dx=sides[0]/steps[0];
		float dy=sides[1]/steps[1];
		
		points=new float[steps[1]][][];
		for(int y = 0; y < steps[1]; y++ )
		{
			points[y]=new float[steps[0]][];
			for(int x = 0; x < steps[1]; x++ )
			{
				points[y][x]=new float[4];
				points[y][x][0]=x*dx;
				points[y][x][1]=y*dy;
				points[y][x][2]=(x+1)*dx;
				points[y][x][3]=(y+1)*dy;
			}
		}
	}
}
