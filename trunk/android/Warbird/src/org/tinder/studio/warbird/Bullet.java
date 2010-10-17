package org.tinder.studio.warbird;

import java.util.LinkedList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;

public class Bullet implements Hittable,Drawable,Cloneable {
	
	private static List<Bullet> bullets=new LinkedList<Bullet>();
	
	protected int dx,dy;
	protected Point position;
	protected Feature feature;
	protected boolean destroy;
	
	protected int frameIndex;
	
	private static List<Bullet> temp=new LinkedList<Bullet>();
	
	public Bullet(int x,int y,int velocity,double direction,Feature feature){
		this.dx=(int) (Math.cos(direction)*velocity);
		this.dy=-(int) (Math.sin(direction)*velocity);
		this.position=new Point(x,y);
		this.feature=feature;
		this.frameIndex=0;
	}
	

	@Override
	public int getImpact() {
		return feature.getExecution();
	}

	@Override
	public void draw(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY) {
		canvas.drawBitmap(feature.getFrames().get(frameIndex),position.x,position.y,paint);
		
	}
	
	public void destroy(){
		this.destroy=true;
	}


	private void update(int minX,int minY,int maxX,int maxY) {
		if(this.position!=null)
		{
			this.position.x+=dx;
			this.position.y+=dy;
			if(this.position.x+this.feature.getFrames().get(frameIndex).getWidth()>maxX||
					this.position.x<minX||this.position.y<minY||
					this.position.y+this.feature.getFrames().get(frameIndex).getHeight()>maxY)
				destroy();
		}
	}
	
	public static void addBullet(Bullet bullet){
		if(Bullet.bullets==null)
			Bullet.bullets=new LinkedList<Bullet>();
		Log.d("Bullet","add:"+bullet);
		Bullet.bullets.add(bullet);
	}


	public static List<Bullet> getBullets() {
		return bullets;
	}
	
	public Point getPosition(){
		return position;
	}
	
	public int getX(){
		return position.x;
	}
	
	public int getY()
	{
		return position.y;
	}
	
	public static void drawAll(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY){
		if(bullets!=null)
		{
			temp.clear();
			for(Bullet b:bullets)
			{
				b.update(minX,minY,maxX,maxY);
				if(b.destroy)
				{
					temp.add(b);
					Log.d("Bullet","remove:"+b);
					continue;
				}
				b.draw(canvas, paint,minX,minY,maxX,maxY);
			}
			
			bullets.removeAll(temp);
		}
	}
	
	public boolean intersect(Plane plane){
		if(this.feature.getCamp()==plane.getCamp())
			return false;
//		return false;
		Bitmap bitmap=plane.getFrame();
		BitmapDrawable b1=new BitmapDrawable(this.feature.getFrames().get(frameIndex));
		BitmapDrawable b2=new BitmapDrawable(bitmap);
		b1.setBounds(this.position.x,this.position.y,this.position.x+b1.getIntrinsicWidth(),this.position.y+b1.getIntrinsicHeight());
		b2.setBounds(plane.getPosition().x,plane.getPosition().y,plane.getPosition().x+b2.getIntrinsicWidth(),plane.getPosition().y+b2.getIntrinsicHeight());
//		Log.d("Bullet",b1.getBounds().toShortString()+",b2:"+b2.getBounds().toShortString()+"="+b1.getBounds().intersect(b2.getBounds()));
		return b1.getBounds().intersect(b2.getBounds());
	}


	@Override
	public int getHitX() {
		return position.x+feature.getFrames().get(frameIndex).getWidth()/2;
	}


	@Override
	public int getHitY() {
		return position.y+feature.getFrames().get(frameIndex).getHeight()/2;
	}
	
}
