package org.tinder.studio.warbird.equip;

import org.tinder.studio.warbird.Player;

public class LifeEquip extends Equip {

	public LifeEquip(int x, int y, int velocity, double direction, long validity) {
		super(x, y, velocity, direction, validity);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void equip(Player player) {
		player.increateLife();
	}

}
