package org.tinder.studio.warbird.path;

import org.tinder.studio.warbird.Point2D;

public interface Path {
	
	/**
	 * 获取下一个位置,如果存在返回true,否则返回false
	 * @param position
	 * @param velocity
	 * @return
	 */
	public boolean getNextPosition(Point2D position,int velocity);
	/*回到起点*/
	public void reset(Point2D position);

}
