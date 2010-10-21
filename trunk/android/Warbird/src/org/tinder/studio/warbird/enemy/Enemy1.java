package org.tinder.studio.warbird.enemy;

import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.Util;
import org.tinder.studio.warbird.equip.Equip;
import org.tinder.studio.warbird.gun.Gun;
import org.tinder.studio.warbird.path.Path;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;

public class Enemy1 extends Plane {
	
	
	private Path path;
	

	public Enemy1(int x, int y, int health, int camp, int velocity,Path path) {
		super(x, y, health, camp, velocity);
		this.path=path;
	}
	
	private void update(){
		path.getNextPosition(position, velocity);
	}
	
	@Override
	public void die() {
		super.die();
		int random=Util.random(1,10);
		if(random==1)
		{
			Equip equip=Equip.generateRandomEquip();
			equip.setPosition(position.x,position.y);
			random=Util.random(0, 3);
			equip.setDirection(Gun.PI_4S_ARRAY[random]);
			Equip.addEquip(equip);
		}
	}
	
	@Override
	public void draw(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY) {
		update();
		super.draw(canvas, paint,minX,minY,maxX,maxY);
		for(Gun g:guns)
			g.fire(Gun.PI_3_2);
	}

	@Override
	public int getImpact() {
		return 100;
	}
	
	@Override
	public Enemy1 clone() {
		Enemy1 enemy1=new Enemy1(position.x,position.y, health, camp, velocity, path);
		for(Gun g:guns)
		{
			Gun gun=g.clone();
			gun.setHost(enemy1);
			enemy1.addGun(gun);
		}
		enemy1.setFrames(frames);
		resetFrameIndex();
		return enemy1;
	}

}
