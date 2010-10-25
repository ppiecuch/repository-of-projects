package org.tinder.studio.warbird.mode;

import org.tinder.studio.warbird.KeyCache;

import android.graphics.Canvas;
import android.graphics.Paint;

public class PauseMode implements Mode {
	
	private static PauseMode instance;
	private Mode parent;
	private int index=0;
	private String[] options={"resume","option","menu"};
	private int[] xs={};//各选项的坐标
	private int[] ys={};
	private int[] pxs={};//各选项指针的坐标
	private int[] pys={};
	private PauseMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
//		canvas.drawBitmap(bg,pxs[index],pys[index],paint);
		for(int i=0;i<options.length;i++)
		{
			canvas.drawText(options[i],xs[i],ys[i],paint);
		}
//		canvas.drawBitmap(pointer,pxs[index],pys[index],paint);
	}

	@Override
	public void update(int keyState) {
		if((keyState&KeyCache.KEY_FIRE)>0)
		{
			switch(index){
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			}
			//TODO view.setMode(parent);
		}

	}
	
	public void setParent(Mode parent) {
		this.parent = parent;
	}

	public static PauseMode getInstance() {
		if(instance==null)
			instance=new PauseMode();
		return instance;
	}
}
