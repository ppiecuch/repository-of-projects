package org.tinder.studio.warbird.path;

import android.graphics.Point;

public class OvalYPath implements Path {
	
	private int a,b,offsetX,offsetY;
	private boolean cw;//是否顺时针旋转
	private double start;//起点角度跟终点角度
	private static final double DT=Math.PI/180;
	
	public OvalYPath(int a,int b,int offsetX,int offsetY,double start,boolean cw){
		this.a=a;
		this.b=b;
		this.offsetX=offsetX;
		this.offsetY=offsetY;
		this.cw=cw;
		this.start=start;
	}
	

	public void getNextPosition(Point position, int velocity) {
		double theta=getTheta(position);
		if(cw)
			theta=theta+velocity*DT;
		else
			theta=theta-velocity*DT;
		position.x=(int) (a*Math.cos(theta)+offsetX);
		position.y=(int) (b*Math.sin(theta)+offsetY);
	}

	public void reset(Point position) {
		position.x=(int) (a*Math.cos(start)+offsetX);
		position.y=(int) (b*Math.sin(start)+offsetY);
	}
	
	/**
	 * 根据椭圆坐标求出其对应的角度theta[0,2PI]
	 * @param position
	 * @return
	 */
	private double getTheta(Point position){
		double theta=Math.acos((position.x-offsetX)/a);
		if(position.y-offsetY<0)
			theta=Math.PI*2-theta;
		return theta;
	}

}
