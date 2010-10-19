package org.tinder.studio.warbird;

import android.util.Log;

public class TrackingGun extends Gun {

	public TrackingGun(Feature feature,int frequence,int velocity) {
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
			Plane target=null;
			if(Plane.getEnemies().size()>0)
			{
				int index=Util.random(0,Plane.getEnemies().size());
				Log.d("TrackingGun","target enemy:"+index);
				target=Plane.getEnemies().get(index);
			}
			
			Bullet.addBullet(new TrackingBullet(x,y,velocity,direction,feature,Gun.PI_1_32,target));
		}
			
	}
	
	@Override
	public TrackingGun clone(){
		return new TrackingGun(feature, frequence, velocity);
	}

}
