package org.tinder.studio.warbird.mode;

import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.KeyCache;
import org.tinder.studio.warbird.R;
import org.tinder.studio.warbird.Warbird;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;

public class MenuMode implements Mode {
	
	private int index=0;
	private String[] options={"开始游戏","选项","退出"};
	private int[] xs={200,200,200};//各选项的坐标
	private int[] ys={150,180,210};
	private int[] pxs={180,180,180};//各选项指针的坐标
	private int[] pys={138,168,198};
	private Bitmap pointer;
	private Bitmap bg;
	private static MenuMode instance;
	private GameView view;
	private MenuMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
		canvas.drawColor(Color.BLACK);
		canvas.drawBitmap(bg,(view.getWidth()-bg.getWidth())/2,(view.getHeight()-bg.getHeight())/2,paint);
		for(int i=0;i<options.length;i++)
		{
			canvas.drawText(options[i],xs[i],ys[i],paint);
		}
		canvas.drawBitmap(pointer,pxs[index],pys[index],paint);
	}

	@Override
	public void update(int keyState) {
		if((keyState&KeyCache.KEY_DOWN)>0)
		{
			index++;
			if(index>=options.length)index=0;
			KeyCache.getInstance().clearKeyState();
			return;
		}
		if((keyState&KeyCache.KEY_UP)>0)
		{
			index--;
			if(index<0)index=options.length-1;
			KeyCache.getInstance().clearKeyState();
			return;
		}
		if((keyState&KeyCache.KEY_FIRE)>0)
		{
			switch(index){
			case 0:
				LoadingMode.getInstance().reset(view,new GameMode(view));
				view.setMode(LoadingMode.getInstance());
				break;
			case 1:
				break;
			case 2:
				break;
				
			}
			return;
		}

	}
	
	public void reset(){
		this.index=0;
	}

	public void setView(GameView view) {
		this.view = view;
		this.pointer=((BitmapDrawable)view.getResources().getDrawable(R.drawable.pointer)).getBitmap();
		this.bg=((BitmapDrawable)view.getResources().getDrawable(R.drawable.bg2)).getBitmap();
	}
	

	public static MenuMode getInstance() {
		if(instance==null)
			instance=new MenuMode();
		return instance;
	}

}
