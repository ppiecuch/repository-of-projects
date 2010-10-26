package org.tinder.studio.warbird.mode;

import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.R;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;

public class LoadingMode implements Mode {
	
	private LoadableMode needLoad;
	private Thread thread;
	private static LoadingMode instance;
	private GameView view;
	private Bitmap bmp;
	private Bitmap bg;
	private int x,y;
	
	private LoadingMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
		if(x==0||y==0)
		{
			this.x=(view.getWidth()-bmp.getWidth())/2;
			this.y=view.getHeight()-bmp.getHeight()*2;
		}
		int progress=needLoad.getProgress();
		Log.d("LoadingMode","progress:"+progress);
		canvas.drawBitmap(bg,x,y,paint);
		int width=progress*bmp.getWidth()/100;
		if(width==0)width=1;
		canvas.drawBitmap(Bitmap.createBitmap(bmp, 0, bmp.getHeight()/2,width , bmp.getHeight()-bmp.getHeight()/2),x,y, paint);
	}

	@Override
	public void update(int keyState) {
		if(this.thread==null)
		{
			this.thread=new Thread(){
				@Override
				public void run() {
					needLoad.init();
				}
			};
			this.thread.start();
		}
		if(needLoad.getProgress()==100){
			view.setMode(needLoad);
		}

	}

	public void reset(GameView view,LoadableMode needLoad) {
		this.needLoad = needLoad;
		this.thread=null;
		this.view=view;
		if(bmp==null)
		{
			this.bmp=((BitmapDrawable)view.getResources().getDrawable(R.drawable.progress)).getBitmap();
			this.bg=Bitmap.createBitmap(bmp, 0, 0, bmp.getWidth(), bmp.getHeight()/2);
		}
	}

	public static LoadingMode getInstance() {
		if(instance==null)
			instance=new LoadingMode();
		return instance;
	}
}
