package org.tinder.studio.warbird;

public abstract class Gun implements Cloneable {
	
	public static final double PI_1_2=Math.PI/2;
	public static final double PI_3_2=3*Math.PI/2;
	
	protected Feature feature;//�ӵ�����
	protected Plane host;
	protected int frequence;//�ӵ�����Ƶ��
	protected int velocity;//�ӵ��ٶ�
	protected int counter=0;
	
	public Gun(Feature feature,int frequence,int velocity){
		this.feature=feature;
		this.frequence=frequence;
		this.velocity=velocity;
	}
	
	public void setHost(Plane host) {
		this.host = host;
	}
	
	public Gun clone() {
		return null;
	}


	public abstract void fire(double direction);

}
