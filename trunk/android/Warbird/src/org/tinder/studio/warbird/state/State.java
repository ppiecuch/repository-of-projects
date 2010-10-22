package org.tinder.studio.warbird.state;

import org.tinder.studio.warbird.Plane;

public interface State {
	
	/**
	 * ����״̬
	 * @param plane
	 */
	public void bestow(Plane plane);
	/**
	 * ����״̬
	 * @param plane
	 */
	public void purge(Plane plane);

}
