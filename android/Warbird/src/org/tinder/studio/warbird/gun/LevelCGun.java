package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;
import org.tinder.studio.warbird.Util;

public class LevelCGun extends Gun {

	private int times=5;
	private double[] ANGLES_OFFSET={Gun.PI_1_16,Gun.PI_1_32,Gun.PI_1_64};
	public LevelCGun(Feature feature, int frequence, int velocity, int level) {
		super(feature, frequence, velocity, level);
	}

	@Override
	public boolean fire(double direction) {
		if(times>0)
		{
			if(counter++%frequence==0)
			{
				double x=host.getPosition().x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
				double y=host.getPosition().y;
				if(direction==Gun.PI_3_2)
					y+=+host.getFrame().getHeight();
				for(float i=0.0f;i<Gun.PI2;i+=Gun.PI_1_8)
					Bullet.addBullet(new Bullet(x,y+feature.getFrames().get(0).getHeight(),velocity,i+ANGLES_OFFSET[Util.random(0,2)],feature));
				times--;
			}
			return false;
		}
		times=5;
		return true;
			
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
