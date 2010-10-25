package org.tinder.studio.warbird.mode;

import java.util.Date;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class LogoMode implements Mode {
	private Bitmap logo;
	private Long validity=null;
	@Override
	public void draw(Canvas canvas, Paint paint) {
		canvas.drawBitmap(logo,0,0,paint);
	}

	@Override
	public void update(int keyState) {
		if(validity==null)
			validity=new Date().getTime();
		else if(validity-new Date().getTime()>validity)
		{
			logo.recycle();
			logo=null;
			//TODO nextMode
		}
			
	}
}
