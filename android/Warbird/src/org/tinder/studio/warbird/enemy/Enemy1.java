package org.tinder.studio.warbird.enemy;

import org.tinder.studio.warbird.Gun;
import org.tinder.studio.warbird.Plane;
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
	public void draw(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY) {
		update();
		super.draw(canvas, paint,minX,minY,maxX,maxY);
		for(Gun g:guns)
			g.fire(Gun.PI_3_2);
	}

	@Override
	public int getImpact() {
		// TODO Auto-generated method stub
		return 100;
	}
	
	@Override
	public boolean intersect(Plane plane) {
		if(plane.getCamp()==this.camp)
			return false;
		Bitmap bitmap=plane.getFrame();
		BitmapDrawable b1=new BitmapDrawable(getFrame());
		BitmapDrawable b2=new BitmapDrawable(bitmap);
		b1.setBounds(this.position.x,this.position.y,this.position.x+b1.getIntrinsicWidth(),this.position.y+b1.getIntrinsicHeight());
		b2.setBounds(plane.getPosition().x,plane.getPosition().y,plane.getPosition().x+b2.getIntrinsicWidth(),plane.getPosition().y+b2.getIntrinsicHeight());
//		Log.d("Enemy1",b1.getBounds().toShortString()+",b2:"+b2.getBounds().toShortString()+"="+b1.getBounds().intersect(b2.getBounds()));
		return b1.getBounds().intersect(b2.getBounds());
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
