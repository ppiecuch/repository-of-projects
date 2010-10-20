package org.tinder.studio.warbird.path;

import android.graphics.Point;

public class LoopXPath implements Path {
	
	private Point start,end;
	private int direction=1;
	
	public LoopXPath(int startX,int endX,int y){
		this.start=new Point(startX,y);
		this.end=new Point(endX,y);
	}

	@Override
	public void getNextPosition(Point position, int velocity) {
		position.x+=direction*velocity;
		if(position.x>end.x||position.x<start.x)
			direction=-direction;
			
	}

	@Override
	public void reset(Point position) {
		position.x=start.x;
		position.y=start.y;
	}

}
