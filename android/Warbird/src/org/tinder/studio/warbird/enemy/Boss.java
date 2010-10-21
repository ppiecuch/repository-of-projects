package org.tinder.studio.warbird.enemy;

import org.tinder.studio.warbird.Effect;
import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.Util;
import org.tinder.studio.warbird.gun.Gun;
import org.tinder.studio.warbird.gun.LevelCGun;
import org.tinder.studio.warbird.gun.LevelDGun;
import org.tinder.studio.warbird.path.Path;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

public class Boss extends Plane {

	private Path path;
	private int gunIndex=0;
	private int gunCounter=30;
	

	public Boss(int x, int y, int health, int camp, int velocity,Path path) {
		super(x, y, health, camp, velocity);
		this.path=path;
	}
	
	@Override
	public void die() {
		int x=position.x+(frames.get(frameIndex).getWidth()-Effect.FRAMES_2.get(0).getWidth())/2;
		int y=position.y+(frames.get(frameIndex).getHeight()-Effect.FRAMES_2.get(0).getHeight())/2;
		Effect.addEffect(new Effect(x,y,Effect.FRAMES_2));
		Effect.addEffect(new Effect(x,y+20,Effect.FRAMES_2,-5));
		Effect.addEffect(new Effect(x,y-20,Effect.FRAMES_2,-11));
		Effect.addEffect(new Effect(x+20,y,Effect.FRAMES_2,-15));
		Effect.addEffect(new Effect(x+20,y+20,Effect.FRAMES_2,-6));
		Effect.addEffect(new Effect(x+20,y-20,Effect.FRAMES_2,-13));
		Effect.addEffect(new Effect(x-20,y,Effect.FRAMES_2,-17));
		Effect.addEffect(new Effect(x-20,y+20,Effect.FRAMES_2,-2));
		Effect.addEffect(new Effect(x-20,y-20,Effect.FRAMES_2,-7));
		Effect.addEffect(new Effect(x,y,Effect.FRAMES_2,-6));
		Effect.addEffect(new Effect(x,y+30,Effect.FRAMES_2,-15));
		Effect.addEffect(new Effect(x,y-30,Effect.FRAMES_2,-22));
		Effect.addEffect(new Effect(x+25,y,Effect.FRAMES_2,-17));
		Effect.addEffect(new Effect(x+25,y+25,Effect.FRAMES_2,-16));
		Effect.addEffect(new Effect(x+15,y-25,Effect.FRAMES_2,-23));
		Effect.addEffect(new Effect(x-15,y,Effect.FRAMES_2,-19));
		Effect.addEffect(new Effect(x-20,y+15,Effect.FRAMES_2,-12));
		Effect.addEffect(new Effect(x-20,y-15,Effect.FRAMES_2,-12));
		destroy=true;
		Log.d("Boss","die:"+this);
	}
	
	private void update(){
		path.getNextPosition(position, velocity);
	}
	
	@Override
	public void draw(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY) {
		update();
		super.draw(canvas, paint,minX,minY,maxX,maxY);
		if(--gunCounter==0)
		{
			gunIndex++;
			if(gunIndex>=guns.size())
				gunIndex=0;
			gunCounter=100;
		}
		guns.get(gunIndex).fire(Gun.PI_3_2);
	}

	@Override
	public int getImpact() {
		return 100;
	}
	
	@Override
	public Boss clone() {
		Boss boss=new Boss(position.x,position.y, health, camp, velocity, path);
		for(Gun g:guns)
		{
			Gun gun=g.clone();
			gun.setHost(boss);
			boss.addGun(gun);
		}
		boss.setFrames(frames);
		resetFrameIndex();
		return boss;
	}

}
