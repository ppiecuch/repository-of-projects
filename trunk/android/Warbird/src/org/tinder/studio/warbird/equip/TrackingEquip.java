package org.tinder.studio.warbird.equip;

import java.util.List;

import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.Player;
import org.tinder.studio.warbird.gun.Gun;

public class TrackingEquip implements Equip {

	@Override
	public void equip(Player player) {
		List<Gun> guns=player.getGuns();
		for(Gun g:guns)
		{
			if(g.equals(GameView.trackingGun))
			{
				g.increaseLevel();
				return;
			}
		}
		player.addGun(GameView.trackingGun);
	}

}
