package org.tinder.studio.warbird;

/**
 * »á¹¥»÷µÄ
 * @author Micheal
 *
 */
public interface Hittable {
	
	/*
	 * ×²»÷É±ÉËÁ¦
	 */
	public int getImpact();
	public boolean intersect(Plane plane);
	public int getHitX();
	public int getHitY();

}
