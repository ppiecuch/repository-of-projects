package org.tinder.studio.warbird.equip;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import org.tinder.studio.warbird.Drawable;
import org.tinder.studio.warbird.Hittable;
import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.Player;
import org.tinder.studio.warbird.Point2D;
import org.tinder.studio.warbird.Util;
import org.tinder.studio.warbird.gun.Gun;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;

/**
 * 装备
 * @author Micheal
 *
 */
public abstract class Equip implements Drawable,Hittable,Cloneable {
	
	private static List<Equip> equips;
	private static List<Equip> temp=new LinkedList<Equip>();
	public static final LifeEquip LIFE_EQUIP=new LifeEquip(0,0,5,Gun.PI_1_4,15000);
	public static final EnergyEquip ENERGY_EQUIP=new EnergyEquip(0,0,5,Gun.PI_1_4,15000);
	public static final TrackingEquip TRACKING_EQUIP=new TrackingEquip(0,0,5,Gun.PI_1_4,15000);
	public static final BombEquip BOMB_EQUIP=new BombEquip(0,0,5,Gun.PI_1_4,15000);

	protected List<Bitmap> frames;
	protected int frameIndex;
	protected Point2D position;
	protected double dx,dy,direction;//direction以X右Y下坐标为准
	protected int velocity;
	protected long validity;
	protected long startTime;
	protected boolean destroy;
	
	public Equip(double x,double y,int velocity,double direction,long validity){
		this.dx=Math.cos(direction)*velocity;
		this.dy=Math.sin(direction)*velocity;
		this.position=new Point2D(x,y);
		this.velocity=velocity;
		this.validity=validity;
		this.frameIndex=0;
		this.destroy=false;
		this.direction=direction;
	}
	
	@Override
	public void draw(Canvas canvas, Paint paint, int minX, int minY, int maxX, int maxY) {
		/*判断是否在有效期内*/
		if(new Date().getTime()-startTime<=validity)
		{
			if(position.x+dx<minX||position.x+dx+frames.get(frameIndex).getWidth()>maxX)
			{
				this.dx=-dx;
			}
			if(position.y+dy<minY||position.y+dy+frames.get(frameIndex).getHeight()>maxY)
			{
				this.dy=-dy;
			}
		}
		else
		{
			/*判断是否在有效范围*/
			if(position.x<minX+frames.get(frameIndex).getWidth()||position.x>maxX)
			{
				destroy=true;
				return;
			}
			if(position.y<minY+frames.get(frameIndex).getHeight()||position.y>maxY)
			{
				destroy=true;
				return;
			}
		}
		position.x+=dx;
		position.y+=dy;
		canvas.drawBitmap(frames.get(frameIndex),(float)position.x,(float)position.y,paint);
		frameIndex++;
		if(frameIndex>=frames.size())
			frameIndex=0;
	}
	
	public static void drawAll(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY){
		if(equips!=null)
		{
//			Log.d("Equip","temp:"+temp);
			temp.clear();
			for(Equip e:equips)
			{
				if(e.destroy)
				{
					temp.add(e);
					continue;
				}
				e.draw(canvas, paint,minX,minY,maxX,maxY);
			}
			equips.removeAll(temp);
		}
	}
	
	public void addFrame(Bitmap frame){
		if(this.frames==null)
			this.frames=new LinkedList<Bitmap>();
		this.frames.add(frame);
	}
	
	public static void addEquip(Equip equip){
		if(equips==null)
			equips=new LinkedList<Equip>();
		equip.startTime=new Date().getTime();
		equips.add(equip);
	}
	
	@Override
	public Equip clone() {
		return null;
	}
	
	@Override
	public boolean intersect(Plane plane) {
		Bitmap bitmap=plane.getFrame();
		int max=Math.max(bitmap.getWidth()+frames.get(frameIndex).getWidth(),bitmap.getHeight()+frames.get(frameIndex).getHeight());
		double distanceX=plane.getPosition().x-this.position.x;
		double distanceY=plane.getPosition().y-this.position.y;
		if(distanceX*distanceX+distanceY*distanceY>max*max)
			return false;
		BitmapDrawable b1=new BitmapDrawable(frames.get(frameIndex));
		BitmapDrawable b2=new BitmapDrawable(bitmap);
		b1.setBounds((int)this.position.x,(int)this.position.y,(int)this.position.x+b1.getIntrinsicWidth(),(int)this.position.y+b1.getIntrinsicHeight());
		b2.setBounds((int)plane.getPosition().x,(int)plane.getPosition().y,(int)plane.getPosition().x+b2.getIntrinsicWidth(),(int)plane.getPosition().y+b2.getIntrinsicHeight());
//		Log.d("Equip",b1.getBounds().toShortString()+",b2:"+b2.getBounds().toShortString()+"="+b1.getBounds().intersect(b2.getBounds()));
		return b1.getBounds().intersect(b2.getBounds());
	}
	
	public void setPosition(double x,double y){
		this.position.x=x;
		this.position.y=y;
	}
	
	public void setDirection(double direction) {
		this.direction = direction;
		this.dx=Math.cos(direction)*velocity;
		this.dy=Math.sin(direction)*velocity;
	}

	@Override
	public int getImpact() {
		return 0;
	}
	
	@Override
	public int getHitX() {
		return (int) position.x;
	}
	
	@Override
	public int getHitY() {
		return (int) position.y;
	}
	
	public boolean isDestroy() {
		return destroy;
	}

	public static List<Equip> getEquips() {
		return equips;
	}
	
	public static Equip generateRandomEquip(){
		int index=Util.random(1, 3);
		switch(index)
		{
		case 1:
			return ENERGY_EQUIP.clone();
		case 2:
			return TRACKING_EQUIP.clone();
		case 3:
			return LIFE_EQUIP.clone();
		}
		return null;
	}

	public abstract void equip(Player player);

}
