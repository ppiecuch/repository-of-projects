package org.tinder.studio.warbird;

/**
 * �ṥ����
 * @author Micheal
 *
 */
public interface Hittable {
	
	/*
	 * ײ��ɱ����
	 */
	public int getImpact();
	public boolean intersect(Plane plane);
	public int getHitX();
	public int getHitY();

}
