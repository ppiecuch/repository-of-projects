package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;

public class LevelDGun extends Gun {

	private double offset=Gun.PI4;
	public LevelDGun(Feature feature, int frequence, int velocity, int level) {
		super(feature, frequence, velocity, level);
	}

	@Override
	public boolean fire(double direction) {
		if(offset>0)
		{
			double x=host.getPosition().x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
			double y=host.getPosition().y;
			if(direction==Gun.PI_3_2)
				y+=+host.getFrame().getHeight();
			Bullet.addBullet(new Bullet(x,y+feature.getFrames().get(0).getHeight()-15,velocity,offset-Gun.PI_1_4,feature));
			offset-=Gun.PI_1_8;
			return false;
		}
		else
		{
			offset=Gun.PI4;
			return true;
		}
			
	}
	
	@Override
	public LevelDGun clone(){
		return new LevelDGun(feature, frequence, velocity, level);
	}

	@Override
	public void increaseLevel() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void maxLevel() {
		// TODO Auto-generated method stub
		
	}

}
