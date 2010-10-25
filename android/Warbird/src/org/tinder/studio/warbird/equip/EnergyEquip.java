package org.tinder.studio.warbird.equip;

import java.util.List;

import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.Player;
import org.tinder.studio.warbird.gun.Gun;
import org.tinder.studio.warbird.mode.GameMode;

public class EnergyEquip extends Equip {

	public EnergyEquip(double x, double y, int velocity, double direction,long validity) {
		super(x, y, velocity, direction, validity);
	}
	
	@Override
	public EnergyEquip clone(){
		EnergyEquip equip=new EnergyEquip(position.x,position.y, velocity, direction, validity);
		equip.frames=frames;
		return equip;
	}

	@Override
	public void equip(Player player) {
		destroy=true;
		List<Gun> guns=player.getGuns();
		for(Gun g:guns)
		{
			if(g.equals(GameMode.energyGun))
			{
				g.increaseLevel();
				return;
			}
		}
	}

}
