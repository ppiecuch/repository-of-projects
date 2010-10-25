package org.tinder.studio.warbird.equip;

import org.tinder.studio.warbird.Player;

public class LifeEquip extends Equip {

	public LifeEquip(double x, double y, int velocity, double direction, long validity) {
		super(x, y, velocity, direction, validity);
	}
	
	@Override
	public LifeEquip clone(){
		LifeEquip equip=new LifeEquip(position.x, position.y, velocity, direction, validity);
		equip.frames=frames;
		return equip;
	}

	@Override
	public void equip(Player player) {
		destroy=true;
		player.increateLife();
	}

}
