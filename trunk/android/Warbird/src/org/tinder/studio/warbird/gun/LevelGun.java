package org.tinder.studio.warbird.gun;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;



public class LevelGun extends Gun {

	public LevelGun(Feature feature,int frequence,int velocity,int level) {
		super(feature,frequence,velocity,level);
	}

	@Override
	public void fire(double direction) {
		if(counter++%frequence==0)
		{
			int x=host.getPosition().x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
			int y=host.getPosition().y;
			if(direction==Gun.PI_3_2)
				y+=+host.getFrame().getHeight();
			switch(level)
			{
			case 5:
				Bullet.addBullet(new Bullet(x-20,y,velocity,direction,feature));
				Bullet.addBullet(new Bullet(x+20,y,velocity,direction,feature));
			case 3:
				Bullet.addBullet(new Bullet(x-10,y,velocity,direction,feature));
				Bullet.addBullet(new Bullet(x+10,y,velocity,direction,feature));
			case 1:
				Bullet.addBullet(new Bullet(x,y,velocity,direction,feature));
				break;
			case 4:
				Bullet.addBullet(new Bullet(x-15,y,velocity,direction,feature));
				Bullet.addBullet(new Bullet(x+15,y,velocity,direction,feature));
			case 2:
				Bullet.addBullet(new Bullet(x-5,y,velocity,direction,feature));
				Bullet.addBullet(new Bullet(x+5,y,velocity,direction,feature));
			}
		}
			
	}
	
	@Override
	public LevelGun clone(){
		return new LevelGun(feature, frequence, velocity,level);
	}

	@Override
	public void increaseLevel() {
		if(level>=5)
			return;
		level++;
	}

	@Override
	public void maxLevel() {
		this.level=5;
	}

}
