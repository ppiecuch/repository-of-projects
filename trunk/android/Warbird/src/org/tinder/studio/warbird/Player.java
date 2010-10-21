package org.tinder.studio.warbird;

import java.util.LinkedList;
import java.util.List;

import org.tinder.studio.warbird.equip.BombEquip;
import org.tinder.studio.warbird.gun.Gun;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

public class Player extends Plane {
	
	private int life;
	private Bitmap lifeIcon;
	private List<BombEquip> bombs;
	
	
	private int rebirthHealth;
	private int rebirthX,rebirthY;
	private int invisiblecounter=0;

	public Player(int x,int y, int health, int camp,int velocity,int life,Bitmap liftIcon) {
		super(x, y, health, camp, velocity);
		this.life=life;
		this.rebirthHealth=health;
		this.rebirthX=x;
		this.rebirthY=y;
		this.lifeIcon=liftIcon;
		invisiblecounter=30;
	}
	
	public void rebirth(){
		
		this.destroy=false;
		this.health=rebirthHealth;
		this.position.x=rebirthX;
		this.position.y=rebirthY;
		for(Gun g:guns){
			g.reset();
		}
		this.life--;
		this.invisiblecounter=30;
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
		if(invisiblecounter%2==0)
			super.draw(canvas, paint,minX,minY,maxX,maxY);
		if(invisiblecounter>0)
			invisiblecounter--;
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
	
	public void addBomb(BombEquip equip)
	{
		if(this.bombs==null)
			this.bombs=new LinkedList<BombEquip>();
		if(this.bombs.size()>=4)
			this.bombs.set(3,equip);
		else
			this.bombs.add(equip);
	}
	
	public boolean isVisible(){
		return invisiblecounter==0;
	}

	@Override
	public String toString() {
		return "life:"+life+",position:["+position.x+","+position.y+"]";
	}

}
