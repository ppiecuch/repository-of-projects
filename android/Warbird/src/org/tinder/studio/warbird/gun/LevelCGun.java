package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;

public class LevelCGun extends Gun {

	public LevelCGun(Feature feature, int frequence, int velocity, int level) {
		super(feature, frequence, velocity, level);
	}

	@Override
	public void fire(double direction) {
		if(counter++%frequence==0)
		{
			int x=host.getPosition().x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
			int y=host.getPosition().y;
			if(direction==Gun.PI_3_2)
				y+=+host.getFrame().getHeight();
			for(float i=0.0f;i<Gun.PI2;i+=Gun.PI_1_8)
				Bullet.addBullet(new Bullet(x,y+feature.getFrames().get(0).getHeight(),velocity,i,feature));
		}
			
	}
	
	@Override
	public LevelCGun clone(){
		return new LevelCGun(feature, frequence, velocity, level);
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
