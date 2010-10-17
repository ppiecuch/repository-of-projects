package org.tinder.studio.warbird;

import java.util.LinkedList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.util.Log;

public abstract class Plane implements Hittable,Drawable,Cloneable {
	
	private static List<Plane> enemies=new LinkedList<Plane>();
	public static final byte[] LOCK_ENEMY=new byte[0];
	private static List<Plane> temp=new LinkedList<Plane>();
	
	protected Point position;
	protected int health;
	protected List<Gun> guns;
	protected int camp;
	protected List<Bitmap> frames;
	protected int velocity;
	protected boolean destroy;
	
	protected int frameIndex;
	protected int dv,sdv;
	
	public Plane(int x,int y,int health,int camp,int velocity){
		this.position=new Point(x,y);
		this.health=health;
		this.camp=camp;
		this.destroy=false;
		this.frameIndex=0;
		this.dv=velocity;
		this.velocity=velocity;
		this.sdv=(int) (velocity/1.414f);
	}
	
	public void resetFrameIndex(){
		this.frameIndex=0;
	}
	
	public void left(int limit){
		this.frameIndex=1;
		if(this.position.x>limit)
			this.position.x-=dv;
		if(this.position.x<limit)
			this.position.x=limit;
	}
	
	public void right(int limit){
		this.frameIndex=2;
		if(this.position.x+frames.get(frameIndex).getWidth()<limit)
			this.position.x+=dv;
		if(this.position.x>limit)
			this.position.x=limit-frames.get(frameIndex).getWidth();
	}
	
	public void up(int limit){
		this.frameIndex=0;
		if(this.position.y>limit)
			this.position.y-=dv;
		if(this.position.y<limit)
			this.position.y=limit;
	}
	
	public void down(int limit){
		this.frameIndex=0;
		if(this.position.y+frames.get(frameIndex).getHeight()<limit)
			this.position.y+=dv;
		if(this.position.y>limit)
			this.position.y=limit-frames.get(frameIndex).getHeight();
		
	}
	
	public void upLeft(int limitX,int limitY){
		this.frameIndex=1;
		if(this.position.x>limitX)
			this.position.x-=sdv;
		if(this.position.x<limitX)
			this.position.x=limitX;
		if(this.position.y>limitY)
			this.position.y-=sdv;
		if(this.position.y<limitY)
			this.position.y=limitY;
	}
	
	public void upRight(int limitX,int limitY){
		this.frameIndex=2;
		if(this.position.x+frames.get(frameIndex).getWidth()<limitX)
			this.position.x+=sdv;
		if(this.position.x>limitX)
			this.position.x=limitX-frames.get(frameIndex).getWidth();
		if(this.position.y>limitY)
			this.position.y-=sdv;
		if(this.position.y<limitY)
			this.position.y=limitY;
	}
	
	public void downLeft(int limitX,int limitY){
		this.frameIndex=1;
		if(this.position.x>limitX)
			this.position.x-=sdv;
		if(this.position.x<limitX)
			this.position.x=limitX;
		if(this.position.y+frames.get(frameIndex).getHeight()<limitY)
			this.position.y+=sdv;
		if(this.position.y>limitY)
			this.position.y=limitY-frames.get(frameIndex).getHeight();
	}
	
	public void downRight(int limitX,int limitY){
		this.frameIndex=2;
		if(this.position.x+frames.get(frameIndex).getWidth()<limitX)
			this.position.x+=sdv;
		if(this.position.x>limitX)
			this.position.x=limitX-frames.get(frameIndex).getWidth();
		if(this.position.y+frames.get(frameIndex).getHeight()<limitY)
			this.position.y+=sdv;
		if(this.position.y>limitY)
			this.position.y=limitY-frames.get(frameIndex).getHeight();
	}
	
	public void hitted(Hittable hittable){
		
		int x=hittable.getHitX()-Effect.FRAMES_1.get(0).getWidth()/2;
		int y=hittable.getHitY()-Effect.FRAMES_1.get(0).getHeight()/2;
		Effect.addEffect(new Effect(x,y,Effect.FRAMES_1));
		this.health-=hittable.getImpact();
		Log.d("Plane","hitten:"+this+",impact:"+hittable.getImpact()+",health:"+health);
		if(this.health<=0)
			die();
	}
	
	public void addGun(Gun gun)
	{
		if(this.guns==null)
			this.guns=new LinkedList<Gun>();
		
		Gun newGun = gun.clone();
		newGun.setHost(this);
		this.guns.add(newGun);
		
	}
	
	public void addFrame(Bitmap frame){
		if(this.frames==null)
			this.frames=new LinkedList<Bitmap>();
		this.frames.add(frame);
	}
	
	@Override
	public void draw(Canvas canvas,Paint paint,int minX,int minY,int maxX,int maxY) {
		canvas.drawBitmap(frames.get(frameIndex),position.x,position.y,paint);
	}
	
	
	
	public Point getPosition() {
		return position;
	}

	public void die(){
		int x=position.x+(frames.get(frameIndex).getWidth()-Effect.FRAMES_2.get(0).getWidth())/2;
		int y=position.y+(frames.get(frameIndex).getHeight()-Effect.FRAMES_2.get(0).getHeight())/2;
		Effect.addEffect(new Effect(x,y,Effect.FRAMES_2));
		destroy=true;
		Log.d("Plane","die:"+this);
	}
	
	public Bitmap getFrame()
	{
		return frames.get(frameIndex);
	}

	public int getCamp() {
		return camp;
	}
	
	@Override
	public int getHitX() {
		return position.x;
	}
	
	@Override
	public int getHitY() {
		return position.y;
	}

	
	
	public boolean isDestroy() {
		return destroy;
	}

	public static void drawAllEnemies(Canvas canvas,Paint paint,int minX,int minY,int maxX,int maxY){
		temp.clear();
		for(Plane p:enemies){
			if(p.destroy)
			{
				temp.add(p);
				Log.d("Plane","remove:"+p);
				continue;
			}
			p.draw(canvas, paint, minX, minY, maxX, maxY);
		}
		synchronized (Plane.LOCK_ENEMY) {
			enemies.removeAll(temp);
		}
	}

	public static List<Plane> getEnemies() {
		return enemies;
	}
	
	@Override
	public boolean intersect(Plane plane) {
		return false;
	}
	
	

	public void setFrames(List<Bitmap> frames) {
		this.frames = frames;
	}

	@Override
	public Plane clone(){
		return null;
	}
	
}
