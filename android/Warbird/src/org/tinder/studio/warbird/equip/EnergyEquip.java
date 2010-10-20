package org.tinder.studio.warbird.equip;

import java.util.List;

import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.Player;
import org.tinder.studio.warbird.gun.Gun;

public class EnergyEquip extends Equip {

	public EnergyEquip(int x, int y, int velocity, double direction,
			long validity) {
		super(x, y, velocity, direction, validity);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void equip(Player player) {
		List<Gun> guns=player.getGuns();
		for(Gun g:guns)
		{
			if(g.equals(GameView.energyGun))
			{
				g.increaseLevel();
				return;
			}
		}
	}

}
