package org.tinder.studio.warbird;

public interface Hittable {
	
	public int getImpact();
	public boolean intersect(Plane plane);
	public int getHitX();
	public int getHitY();

}
