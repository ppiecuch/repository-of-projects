package org.tinder.studio.warbird;

public class LevelBGun extends Gun {

	public LevelBGun(Feature feature, int frequence, int velocity) {
		super(feature, frequence, velocity);
	}

	@Override
	public void fire(double direction) {
		if(counter++%frequence==0)
		{
			int x=host.position.x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
			int y=host.position.y;
			if(direction==Gun.PI_3_2)
				y+=+host.getFrame().getHeight();
			Bullet.addBullet(new Bullet(x-10,y+feature.getFrames().get(0).getHeight()*2,velocity,direction,feature));
			Bullet.addBullet(new Bullet(x+10,y+feature.getFrames().get(0).getHeight()*2,velocity,direction,feature));
		}
	}
	
	@Override
	public LevelAGun clone(){
		return new LevelAGun(feature, frequence, velocity);
	}

}
