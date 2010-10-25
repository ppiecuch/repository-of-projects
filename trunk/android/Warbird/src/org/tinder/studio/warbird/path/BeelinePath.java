package org.tinder.studio.warbird.path;

import org.tinder.studio.warbird.Point2D;

import android.graphics.Point;

public class BeelinePath implements Path {
	
	private Point2D start,end;
	private int dx,dy,length;
	
	public BeelinePath(int startX,int startY,int endX,int endY){
		this.start=new Point2D(startX,startY);
		this.end=new Point2D(endX,endY);
		this.dx=endX-startX;
		this.dy=endY-startY;
		this.length=(int) Math.sqrt(dx*dx+dy*dy);
	}

	@Override
	public boolean getNextPosition(Point2D position,int velocity) {
		int nx=velocity*dx/length;
		int ny=velocity*dy/length;
		position.x+=nx;
		position.y+=ny;
		if(end.x>start.x&&position.x>end.x||
				end.x<start.x&&position.x<start.x)
			return false;
		if(end.y>start.y&&position.y>end.y||
			end.y<start.y&&position.y<start.y)
			return false;
		
		return true;

	}

	@Override
	public void reset(Point2D position) {
		position.x=start.x;
		position.y=start.y;
	}

}
