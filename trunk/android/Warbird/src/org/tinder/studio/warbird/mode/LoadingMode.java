package org.tinder.studio.warbird.mode;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

public class LoadingMode implements Mode {
	
	private LoadableMode needLoad;
	private Thread thread;
	private static LoadingMode instance;
	
	private LoadingMode(){}

	@Override
	public void draw(Canvas canvas, Paint paint) {
		Log.d("LoadingMode","progress:"+needLoad.getProgress());
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
		}
		if(needLoad.getProgress()==100){
			//TODO GameView.setMode(...)
		}

	}

	public void reset(LoadableMode needLoad) {
		this.needLoad = needLoad;
		this.thread=null;
	}

	public static LoadingMode getInstance() {
		if(instance==null)
			instance=new LoadingMode();
		return instance;
	}
}
