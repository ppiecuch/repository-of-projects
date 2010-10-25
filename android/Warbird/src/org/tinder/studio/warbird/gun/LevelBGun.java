package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;

public class LevelBGun extends Gun {

	private int times=8;
	public LevelBGun(Feature feature, int frequence, int velocity,int level) {
		super(feature, frequence, velocity,level);
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
				Bullet.addBullet(new Bullet(x-30,y+feature.getFrames().get(0).getHeight()-15,velocity,direction,feature));
				Bullet.addBullet(new Bullet(x+30,y+feature.getFrames().get(0).getHeight()-15,velocity,direction,feature));
				times--;
			}
			return false;
		}
		times=8;
		return true;
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
