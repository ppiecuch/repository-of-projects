package org.tinder.studio.lwjgl.roam;
/**
 * 
 * @author Micheal Hong 
 * @email babala_234@163.com
 * @version 2010-7-23 ����05:47:56
 *
 */
public class Roam {
	
	private short[][] heightWeights;
	private float[] scales;	//�Ŵ�ϵ��     scale>0
	private float delicate;	//ϸ��ϵ��
	
	public Roam(short[][] heightWeights,float[] scales,float delicate)
	{
		this.heightWeights=heightWeights;
		this.scales=scales;
		this.delicate=delicate;
	}

}
