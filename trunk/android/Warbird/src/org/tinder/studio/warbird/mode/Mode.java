package org.tinder.studio.warbird.mode;

import android.graphics.Canvas;
import android.graphics.Paint;

public interface Mode {
	
	public void draw(Canvas canvas,Paint paint);
	public void update(int keyState);

}
