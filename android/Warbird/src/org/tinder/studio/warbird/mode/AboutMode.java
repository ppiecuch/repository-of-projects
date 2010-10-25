package org.tinder.studio.warbird.mode;

import android.graphics.Canvas;
import android.graphics.Paint;

public class AboutMode implements Mode {
	
	private static AboutMode instance;
	private AboutMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
		// TODO Auto-generated method stub

	}

	@Override
	public void update(int keyState) {
		// TODO Auto-generated method stub

	}

	public static AboutMode getInstance() {
		return instance;
	}
	
	

}
