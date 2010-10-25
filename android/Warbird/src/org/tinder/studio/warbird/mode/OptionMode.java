package org.tinder.studio.warbird.mode;

import android.graphics.Canvas;
import android.graphics.Paint;

public class OptionMode implements Mode {
	
	private static OptionMode instance;
	private String[] options={"音量"};
	private int[] xs={};//各选项的坐标
	private int[] ys={};
	private int[][] oxs={};
	private int[][] oys={};
	private int[][] pxs={};
	private int[][] pys={};
	private int[] indexs={0,0};
	private OptionMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
		// TODO Auto-generated method stub

	}

	@Override
	public void update(int keyState) {
		// TODO Auto-generated method stub

	}

	public static OptionMode getInstance() {
		if(instance==null)
			instance=new OptionMode();
		return instance;
	}
	
	

}
