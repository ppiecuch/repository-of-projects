package org.tinder.studio.warbird;



public class Level1Gun extends Gun {

	public Level1Gun(Feature feature,int frequence,int velocity) {
		super(feature,frequence,velocity);
	}

	@Override
	public void fire(double direction) {
		if(counter++%frequence==0)
		{
			int x=host.position.x+host.getFrame().getWidth()/2-feature.getFrames().get(0).getWidth()/2;
			int y=host.position.y;
			if(direction==Gun.PI_3_2)
				y+=+host.getFrame().getHeight();
			Bullet.addBullet(new Bullet(x,y,velocity,direction,feature));
		}
			
	}
	
	@Override
	public Level1Gun clone(){
		return new Level1Gun(feature, frequence, velocity);
	}

}
