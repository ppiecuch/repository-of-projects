package org.tinder.studio.warbird.path;

import org.tinder.studio.warbird.Point2D;

import android.graphics.Point;
import android.util.Log;

public class OvalYPath implements Path {
	
	private int a,b,offsetX,offsetY;
	private boolean cw;//是否顺时针旋转
	private double start,end;//起点角度跟终点角度
	private static final double DT=Math.PI/180;
	
	public OvalYPath(int a,int b,int offsetX,int offsetY,double start,double end,boolean cw){
		this.a=a;
		this.b=b;
		this.offsetX=offsetX;
		this.offsetY=offsetY;
		this.cw=cw;
		this.start=start;
		this.end=end;
	}
	

	public boolean getNextPosition(Point2D position, int velocity) {
		double theta=getTheta(position);
		
		if(cw)
		{
			if(theta<start||theta>end||theta<start&&theta>end)
				return false;
			theta=theta+velocity*DT;
		}
		else
		{
			if(theta>start||theta<end||theta>start&&theta<end)
				return false;
			theta=theta-velocity*DT;
		}	
		position.x=a*Math.cos(theta)+offsetX;
		position.y=b*Math.sin(theta)+offsetY;
		Log.d("OvalYPath",position.toString());
		return true;
	}

	public void reset(Point2D position) {
		position.x=a*Math.cos(start)+offsetX;
		position.y=b*Math.sin(start)+offsetY;
		Log.d("OvalYPath","reset:"+position.toString());
	}
	
	/**
	 * 根据椭圆坐标求出其对应的角度theta[0,2PI]
	 * @param position
	 * @return
	 */
	private double getTheta(Point2D position){
		double theta=Math.acos((position.x-offsetX)/a);
		if(position.y-offsetY<0)
			theta=Math.PI*2-theta;
		return theta;
	}

}
