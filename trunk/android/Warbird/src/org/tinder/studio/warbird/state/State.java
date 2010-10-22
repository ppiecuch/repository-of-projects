package org.tinder.studio.warbird.state;

import org.tinder.studio.warbird.Plane;

public interface State {
	
	/**
	 * ¸³Óè×´Ì¬
	 * @param plane
	 */
	public void bestow(Plane plane);
	/**
	 * ¾»»¯×´Ì¬
	 * @param plane
	 */
	public void purge(Plane plane);

}
