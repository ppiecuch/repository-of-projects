package org.tinder.studio.lwjgl.roam;
/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-23 下午05:47:56
 *
 */
public class Roam {
	
	private short[][] heightWeights;
	private float[] scales;	//放大系数     scale>0
	private float delicate;	//细致系数
	
	public Roam(short[][] heightWeights,float[] scales,float delicate)
	{
		this.heightWeights=heightWeights;
		this.scales=scales;
		this.delicate=delicate;
	}

}
