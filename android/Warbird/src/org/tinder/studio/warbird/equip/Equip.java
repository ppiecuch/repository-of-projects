package org.tinder.studio.warbird.equip;

import java.util.LinkedList;
import java.util.List;

import org.tinder.studio.warbird.Drawable;
import org.tinder.studio.warbird.Player;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;

/**
 * ×°±¸
 * @author Micheal
 *
 */
public abstract class Equip implements Drawable {

	protected List<Bitmap> frames;
	protected int frameIndex;
	protected Point position;
	protected double dx,dy,direction;
	protected int velocity;
	protected long validity;
	
	public Equip(int x,int y,int velocity,double direction,long validity){
		this.dx=Math.cos(direction)*velocity;
		this.dy=-Math.sin(direction)*velocity;
		this.position=new Point(x,y);
		this.velocity=velocity;
		this.direction=direction;
		this.validity=validity;
		this.frameIndex=0;
		
	}
	
	@Override
	public void draw(Canvas canvas, Paint paint, int minX, int minY, int maxX,
			int maxY) {
		
		
	}
	
	public void addFrame(Bitmap frame){
		if(this.frames==null)
			this.frames=new LinkedList<Bitmap>();
		this.frames.add(frame);
	}
	
	public abstract void equip(Player player);

}
