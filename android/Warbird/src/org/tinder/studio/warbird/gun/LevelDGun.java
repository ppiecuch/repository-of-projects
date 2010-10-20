package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;

public class LevelDGun extends Gun {

	private float offset=0.0f;
	public LevelDGun(Feature feature, int frequence, int velocity, int level) {
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
			for(float i=0.0f;i<Gun.PI2;i+=Gun.PI_1_16)
				Bullet.addBullet(new Bullet(x,y+feature.getFrames().get(0).getHeight()*2,velocity,i+offset,feature));
			offset+=Gun.PI_1_64;
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
