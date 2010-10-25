package org.tinder.studio.warbird.mode;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class MenuMode implements Mode {
	
	private int index=0;
	private String[] options={"����1","����2"};
	private int[] xs={};//��ѡ�������
	private int[] ys={};
	private int[] pxs={};//��ѡ��ָ�������
	private int[] pys={};
	private Bitmap pointer;
	private static MenuMode instance;
	private MenuMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
		for(int i=0;i<options.length;i++)
		{
			canvas.drawText(options[i],xs[i],ys[i],paint);
		}
		canvas.drawBitmap(pointer,pxs[index],pys[index],paint);
	}

	@Override
	public void update(int keyState) {
		// TODO Auto-generated method stub

	}
	
	public void reset(){
		this.index=0;
	}

	public static MenuMode getInstance() {
		if(instance==null)
			instance=new MenuMode();
		return instance;
	}

}
