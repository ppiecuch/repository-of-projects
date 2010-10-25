package org.tinder.studio.warbird.path;

import org.tinder.studio.warbird.Point2D;

public interface Path {
	
	/**
	 * ��ȡ��һ��λ��,������ڷ���true,���򷵻�false
	 * @param position
	 * @param velocity
	 * @return
	 */
	public boolean getNextPosition(Point2D position,int velocity);
	/*�ص����*/
	public void reset(Point2D position);

}
