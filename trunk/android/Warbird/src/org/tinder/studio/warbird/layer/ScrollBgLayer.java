package org.tinder.studio.warbird.layer;

import org.tinder.studio.warbird.Drawable;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class ScrollBgLayer implements Drawable {
	
	private Bitmap bg;
	private Bitmap rbg;
	private int dx,dy;
	
	private int x,y;
	private int bgWidth,bgHeight;
	
	public ScrollBgLayer(int dx,int dy,Bitmap bg){
		this.dx=dx;
		this.dy=dy;
		this.bg=bg;
		this.x=this.y=0;
		this.bgWidth=bg.getWidth();
		this.bgHeight=bg.getHeight();
		this.rbg=bg;
	}

	@Override
	public void draw(Canvas canvas, Paint paint, int minX, int minY, int maxX,
			int maxY) {
		int width=maxX-minX;
		int height=maxY-minY;
		if(width+x<=bgWidth&&height+y<=bgHeight)
		{
			Bitmap top=Bitmap.createBitmap(bg,x,y,width,height);
			canvas.drawBitmap(top,0,0,paint);
		}
		else if(height+y>bgHeight)
		{
			height=bgHeight-y;
			Bitmap top=Bitmap.createBitmap(bg,x,y,width,height);
			canvas.drawBitmap(top,0,0,paint);
			Bitmap bottom=Bitmap.createBitmap(rbg,0,0,width,maxY-minY-height);
			canvas.drawBitmap(bottom,0,height,paint);
		}
		this.x+=dx;
		this.y+=dy;
		if(this.x>bgWidth)
			this.x-=bgWidth;
		if(this.x<0)
			this.x+=bgWidth;
		if(this.y>bgHeight)
			this.y-=bgHeight;
		if(this.y<0)
			this.y+=bgHeight;
		
		
		
	}

}
