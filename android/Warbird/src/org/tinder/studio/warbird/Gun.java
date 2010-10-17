package org.tinder.studio.warbird;

public abstract class Gun implements Cloneable {
	
	public static final double PI_1_2=Math.PI/2;
	public static final double PI_3_2=3*Math.PI/2;
	
	protected Feature feature;//子弹特性
	protected Plane host;
	protected int frequence;//子弹发射频率
	protected int velocity;//子弹速度
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
