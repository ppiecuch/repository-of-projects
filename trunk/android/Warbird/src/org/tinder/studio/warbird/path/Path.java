package org.tinder.studio.warbird.path;

import android.graphics.Point;

public interface Path {
	
	public void getNextPosition(Point position,int velocity);
	/*回到起点*/
	public void reset(Point position);

}
