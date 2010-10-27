package org.tinder.studio.warbird.mode;

import java.util.Date;

import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.R;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;

public class LogoMode implements Mode {
	private Bitmap bmp;
	private Long validity=null;
	private GameView view;
	private long startTime;
	
	public LogoMode(GameView view,long validity) {
		this.view=view;
		this.bmp=((BitmapDrawable)view.getResources().getDrawable(R.drawable.logo)).getBitmap();
		this.validity=validity;
		this.startTime=0;
	}
	
	@Override
	public void draw(Canvas canvas, Paint paint) {
		canvas.drawColor(0xffffffff);
		canvas.drawBitmap(bmp,(view.getWidth()-bmp.getWidth())/2,(view.getHeight()-bmp.getHeight())/2,paint);
	}

	@Override
	public void update(int keyState) {
		if(startTime==0)
			startTime=new Date().getTime();
		else if(new Date().getTime()-startTime>validity)
		{
			bmp.recycle();
			bmp=null;
			MenuMode.getInstance().setView(view);
			view.setMode(MenuMode.getInstance());
		}
			
	}
}
