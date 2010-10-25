package org.tinder.studio.warbird.path;

import org.tinder.studio.warbird.Point2D;

import android.graphics.Point;

public class LoopXPath implements Path {
	
	private Point2D start,end;
	private int direction=1;
	
	public LoopXPath(int startX,int endX,int y){
		this.start=new Point2D(startX,y);
		this.end=new Point2D(endX,y);
	}

	@Override
	public boolean getNextPosition(Point2D position, int velocity) {
		position.x+=direction*velocity;
		if(position.x>end.x||position.x<start.x)
			direction=-direction;
		return true;
			
	}

	@Override
	public void reset(Point2D position) {
		position.x=start.x;
		position.y=start.y;
	}

}
