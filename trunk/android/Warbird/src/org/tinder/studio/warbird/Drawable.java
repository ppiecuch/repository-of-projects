package org.tinder.studio.warbird;

import android.graphics.Canvas;
import android.graphics.Paint;

public interface Drawable {
	
	public void draw(Canvas canvas,Paint paint,int minX,int minY,int maxX,int maxY);

}
