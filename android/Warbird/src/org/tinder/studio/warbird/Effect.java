package org.tinder.studio.warbird;

import java.util.LinkedList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.util.Log;

public class Effect implements Drawable {
	
	private static List<Effect> effects;
	private static List<Effect> temp=new LinkedList<Effect>();
	
	public static final List<Bitmap> FRAMES_1=new LinkedList<Bitmap>();
	public static final List<Bitmap> FRAMES_2=new LinkedList<Bitmap>();
	
	
	protected List<Bitmap> frames;
	protected int frameIndex;
	private Point position;
	
	public Effect(int x,int y,List<Bitmap> frames)
	{
		this.frames=frames;
		this.position=new Point(x, y);
		this.frameIndex=0;
	}
	
	public Effect(int x,int y,List<Bitmap> frames,int frameIndex)
	{
		this.frames=frames;
		this.position=new Point(x, y);
		this.frameIndex=frameIndex;
	}

	@Override
	public void draw(Canvas canvas, Paint paint, int minX, int minY, int maxX,
			int maxY) {
		if(frameIndex>=0)
			canvas.drawBitmap(frames.get(frameIndex),position.x,position.y,paint);
		frameIndex++;

	}
	
	private void update(){
		if(frameIndex>=frames.size())
		{
			this.position=null;
			this.frames=null;
		}
	}
	
	public void addFrame(Bitmap frame){
		if(this.frames==null)
			this.frames=new LinkedList<Bitmap>();
		this.frames.add(frame);
	}
	
	
	
	public static List<Effect> getEffects() {
		return effects;
	}

	public static void drawAll(Canvas canvas, Paint paint,int minX,int minY,int maxX,int maxY){
		if(effects!=null)
		{
			temp.clear();
			for(Effect e:effects)
			{
				e.update();
				if(e.position==null)
				{
					temp.add(e);
//					Log.d("Effect","remove:"+e);
					continue;
				}
//				Log.d("Effect","draw:"+e);
				e.draw(canvas, paint,minX,minY,maxX,maxY);
			}
			effects.removeAll(temp);
		}
	}
	
	public static void addEffect(Effect effect){
		if(effects==null)
			effects=new LinkedList<Effect>();
		effects.add(effect);
	}

	
	
	

}
