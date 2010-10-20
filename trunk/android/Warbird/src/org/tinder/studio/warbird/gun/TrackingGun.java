package org.tinder.studio.warbird.gun;


import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Feature;
import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.TrackingBullet;
import org.tinder.studio.warbird.Util;

import android.util.Log;

public class TrackingGun extends Gun {

	public TrackingGun(Feature feature,int frequence,int velocity,int level) {
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
			Plane target1=null;
			Plane target2=null;
			Plane target3=null;
			
			
			switch(level)
			{
			case 1:
				if(Plane.getEnemies().size()>0)
				{
					int index=Util.random(0,Plane.getEnemies().size());
					Log.d("TrackingGun","target enemy:"+index);
					target1=Plane.getEnemies().get(index);
				}
				Bullet.addBullet(new TrackingBullet(x,y,velocity,direction,feature,Gun.PI_1_32,target1));
				break;
			case 2:
				if(Plane.getEnemies().size()>0)
				{
					int index1=Util.random(0,Plane.getEnemies().size());
					int index2=Util.random(0,Plane.getEnemies().size());
					Log.d("TrackingGun","target enemy:"+index1+","+index2);
					target1=Plane.getEnemies().get(index1);
					target2=Plane.getEnemies().get(index2);
				}
				Bullet.addBullet(new TrackingBullet(x-10,y,velocity,direction,feature,Gun.PI_1_32,target1));
				Bullet.addBullet(new TrackingBullet(x+10,y,velocity,direction,feature,Gun.PI_1_32,target2));
				break;
			case 3:
				if(Plane.getEnemies().size()>0)
				{
					int index1=Util.random(0,Plane.getEnemies().size());
					int index2=Util.random(0,Plane.getEnemies().size());
					int index3=Util.random(0,Plane.getEnemies().size());
					Log.d("TrackingGun","target enemy:"+index1+","+index2+","+index3);
					target1=Plane.getEnemies().get(index1);
					target2=Plane.getEnemies().get(index2);
					target2=Plane.getEnemies().get(index3);
				}
				Bullet.addBullet(new TrackingBullet(x-15,y,velocity,direction,feature,Gun.PI_1_32,target1));
				Bullet.addBullet(new TrackingBullet(x,y,velocity,direction,feature,Gun.PI_1_32,target2));
				Bullet.addBullet(new TrackingBullet(x+15,y,velocity,direction,feature,Gun.PI_1_32,target3));
			}
		}
			
	}
	
	@Override
	public TrackingGun clone(){
		return new TrackingGun(feature, frequence, velocity,level);
	}

	@Override
	public void increaseLevel() {
		if(level>=3)
			return;
		level++;
	}

	@Override
	public void maxLevel() {
		this.level=3;
	}

}
