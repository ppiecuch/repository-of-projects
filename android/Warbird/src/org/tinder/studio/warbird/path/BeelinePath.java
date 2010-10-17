package org.tinder.studio.warbird.path;

import android.graphics.Point;

public class BeelinePath implements Path {
	
	private Point start,end;
	private int dx,dy,length;
	
	public BeelinePath(int startX,int startY,int endX,int endY){
		this.start=new Point(startX,startY);
		this.end=new Point(endX,endY);
		this.dx=endX-startX;
		this.dy=endY-startY;
		this.length=(int) Math.sqrt(dx*dx+dy*dy);
	}

	@Override
	public void getNextPosition(Point position,int velocity) {
		int nx=velocity*dx/length;
		int ny=velocity*dy/length;
		position.x+=nx;
		position.y+=ny;

	}

	@Override
	public void reset(Point position) {
		position.x=start.x;
		position.y=start.y;
	}

}
