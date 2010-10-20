package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;

public class LevelBGun extends Gun {

	public LevelBGun(Feature feature, int frequence, int velocity,int level) {
		super(feature, frequence, velocity,level);
	}

	@Override
	public void fire(double direction) {
		if(counter++%frequence==0)
		{
			int x=host.getPosition().x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
			int y=host.getPosition().y;
			if(direction==Gun.PI_3_2)
				y+=+host.getFrame().getHeight();
			Bullet.addBullet(new Bullet(x-10,y+feature.getFrames().get(0).getHeight()*2,velocity,direction,feature));
			Bullet.addBullet(new Bullet(x+10,y+feature.getFrames().get(0).getHeight()*2,velocity,direction,feature));
		}
	}
	
	@Override
	public LevelBGun clone(){
		return new LevelBGun(feature, frequence, velocity,level);
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
