package org.tinder.studio.lwjgl.util;
/**
 *
 * @Author Micheal Hong
 * @Email babala_234@163.com
 * @Version 2010-7-13обнГ11:33:54
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
		vertex=new Point3f[horizonalSteps*portraitSteps*4][];
		for(int i=0;i<portraitSteps;i++)
		{
			for(int j=0;j<horizonalSteps;j++)
			{
				
			}
		}
	}
}
