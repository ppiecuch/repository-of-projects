package org.tinder.studio.warbird.equip;

import org.tinder.studio.warbird.Player;

public class BombEquip extends Equip {

	public BombEquip(double x, double y, int velocity, double direction,long validity) {
		super(x, y, velocity, direction, validity);
	}
	
	@Override
	protected BombEquip clone() {
		BombEquip equip=new BombEquip(position.x, position.y, velocity, direction, validity);
		equip.frames=frames;
		return equip;
	}

	@Override
	public void equip(Player player) {
		destroy=true;
		player.addBomb(this);
	}

}
