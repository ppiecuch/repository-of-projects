package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Feature;
import org.tinder.studio.warbird.Plane;

public abstract class Gun implements Cloneable {
	
	public static final double PI_1_2=Math.PI/2;
	public static final double PI_3_2=3*Math.PI/2;
	public static final double PI2=Math.PI*2;
	public static final double PI_1_16=Math.PI/16;
	public static final double PI_1_8=Math.PI/8;
	public static final double PI_3_8=3*Math.PI/8;
	public static final double PI_1_4=Math.PI/4;
	public static final double PI_5_8=5*Math.PI/8;
	public static final double PI_3_4=3*Math.PI/4;
	public static final double PI_7_8=7*Math.PI/8;
	public static final double PI_9_8=9*Math.PI/8;
	public static final double PI_5_4=5*Math.PI/4;
	public static final double PI_11_8=11*Math.PI/8;
	public static final double PI_13_8=13*Math.PI/8;
	public static final double PI_7_4=7*Math.PI/4;
	public static final double PI_15_8=15*Math.PI/8;
	public static final double PI_1_64=Math.PI/64;
	public static final double PI_1_32=Math.PI/32;
	public static final double[] PI_4S_ARRAY=new double[]{PI_1_4,PI_3_4,PI_5_4,PI_7_4};
	
	protected Feature feature;//子弹特性
	protected Plane host;
	protected int frequence;//子弹发射频率
	protected int velocity;//子弹速度
	protected int counter=0;
	protected int level;//火力等级
	
	public Gun(Feature feature,int frequence,int velocity,int level){
		this.feature=feature;
		this.frequence=frequence;
		this.velocity=velocity;
		this.level=level;
	}
	
	public void setHost(Plane host) {
		this.host = host;
	}
	
	public void reset(){
		this.counter=0;
		this.level=1;
	}
	
	public Gun clone() {
		return null;
	}
	
	@Override
	public boolean equals(Object o) {
		return o.getClass()==this.getClass();
	}

	public abstract void fire(double direction);
	public abstract void increaseLevel();
	public abstract void maxLevel();

}
