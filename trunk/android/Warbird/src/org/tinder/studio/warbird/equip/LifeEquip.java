package org.tinder.studio.warbird.equip;

import org.tinder.studio.warbird.Player;

public class LifeEquip implements Equip {

	@Override
	public void equip(Player player) {
		player.increateLife();
	}

}
