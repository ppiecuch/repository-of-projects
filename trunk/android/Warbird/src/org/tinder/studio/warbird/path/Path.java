package org.tinder.studio.warbird.path;

import android.graphics.Point;

public interface Path {
	
	public void getNextPosition(Point position,int velocity);
	/*�ص����*/
	public void reset(Point position);

}
