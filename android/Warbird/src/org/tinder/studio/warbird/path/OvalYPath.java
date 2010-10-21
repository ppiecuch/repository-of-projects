package org.tinder.studio.warbird.path;

import android.graphics.Point;

public class OvalYPath implements Path {
	
	private int a,b,offsetX,offsetY;
	private boolean cw;
	private double start,end;
	
	public OvalYPath(int a,int b,int offsetX,int offsetY,double start,double end,boolean cw){
		this.a=a;
		this.b=b;
		this.offsetX=offsetX;
		this.offsetY=offsetY;
		this.cw=cw;
		this.start=start;
		this.end=end;
	}
	

	@Override
	public void getNextPosition(Point position, int velocity) {
		

	}

	@Override
	public void reset(Point position) {
		position.x=(int) (a*Math.cos(start)+offsetX);
		position.y=(int) (b*Math.sin(start)+offsetY);

	}

}
