package org.tinder.studio.warbird;

import org.tinder.studio.warbird.gun.Gun;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

public class Player extends Plane {
	
	private int life;
	private Bitmap lifeIcon;
	
	private int rebirthHealth;
	private int rebirthX,rebirthY;

	public Player(int x,int y, int health, int camp,int velocity,int life,Bitmap liftIcon) {
		super(x, y, health, camp, velocity);
		this.life=life;
		this.rebirthHealth=health;
		this.rebirthX=x;
		this.rebirthY=y;
		this.lifeIcon=liftIcon;
	}
	
	public void rebirth(){
		
		this.destroy=false;
		this.health=rebirthHealth;
		this.position.x=rebirthX;
		this.position.y=rebirthY;
		this.life--;
		Log.d("Player","rebirth!!!!!life:"+life);
	}
	
	@Override
	public void die() {
		super.die();
		if(life>0)
			new Thread(){
			public void run() {
				try {
					Thread.sleep(2000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				rebirth();
			};
		}.start();
	}
	
	@Override
	public void draw(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY) {
		if(destroy)
			return;
		super.draw(canvas, paint,minX,minY,maxX,maxY);
		for(Gun g:guns)
			g.fire(Gun.PI_1_2);
	}
	
	public void drawLifeIcon(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY) {
		for(int i=0;i<life;i++)
		{
			canvas.drawBitmap(lifeIcon,minX+i*lifeIcon.getWidth(),maxY-lifeIcon.getHeight(),paint);
		}
	}

	@Override
	public int getImpact() {
		return 100;
	}
	
	public void increateLife() {
		if(this.life<5)
			this.life++;
	}

	@Override
	public String toString() {
		return "life:"+life+",position:["+position.x+","+position.y+"]";
	}

}
