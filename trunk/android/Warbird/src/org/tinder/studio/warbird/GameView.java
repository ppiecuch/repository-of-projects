package org.tinder.studio.warbird;

import java.util.LinkedList;
import java.util.List;

import org.tinder.studio.warbird.enemy.Boss;
import org.tinder.studio.warbird.enemy.Command;
import org.tinder.studio.warbird.enemy.Commander;
import org.tinder.studio.warbird.enemy.Enemy1;
import org.tinder.studio.warbird.equip.Equip;
import org.tinder.studio.warbird.gun.Gun;
import org.tinder.studio.warbird.gun.LevelCGun;
import org.tinder.studio.warbird.gun.LevelDGun;
import org.tinder.studio.warbird.gun.LevelGun;
import org.tinder.studio.warbird.gun.LevelAGun;
import org.tinder.studio.warbird.gun.LevelBGun;
import org.tinder.studio.warbird.gun.TrackingGun;
import org.tinder.studio.warbird.layer.ScrollBgLayer;
import org.tinder.studio.warbird.mode.GameMode;
import org.tinder.studio.warbird.mode.Mode;
import org.tinder.studio.warbird.path.BeelinePath;
import org.tinder.studio.warbird.path.LoopXPath;
import org.tinder.studio.warbird.path.OvalYPath;
import org.tinder.studio.warbird.path.Path;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;

public class GameView extends View implements Runnable{
	
	public static final int FPS = 30;	//每秒绘制的帧数
	private Thread thread;
	private Integer width,height;
	private boolean runnable=true;
	private boolean needInit=true;
	private Paint foreground;
	private Mode mode;
	
	private KeyCache keyCache;
	
	public static final int DIRECTION_LEFT=4;
	public static final int DIRECTION_RIGHT=2;
	private int direction=-1;

	public GameView(Context context) {
		super(context);
		
		setFocusable(true);
		setFocusableInTouchMode(true);
		foreground=new Paint(Paint.ANTI_ALIAS_FLAG);
		mode=new GameMode(this);
		
		this.keyCache=new KeyCache();
//		bmp=((BitmapDrawable)getResources().getDrawable(R.drawable.pic)).getBitmap();
//		bmp=Util.alphaColor(bmp, 0xFF040404, (byte)0);
		
	}
	
	private void init(){
		this.width=getWidth();
		this.height=getHeight();
		Log.d("GameView","width:"+width+",height:"+height);
	}
	
	private void logic(){
		int keyState=keyCache.getKeyState();
		mode.update(keyState);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		if(needInit){
			init();
			needInit=false;
		}
//		
		canvas.drawColor(getResources().getColor(R.color.game_background));
		mode.draw(canvas, foreground);
//		foreground.setColor(Color.WHITE);
//		canvas.drawText("游戏测试",50,50,foreground);
		
//		canvas.drawBitmap(players[index],position.x,position.y,foreground);
//		canvas.drawBitmap(bullet,position.x+bulletOffset[0],position.y+bulletOffset[1],foreground);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
//		Log.d("GameView","key down:"+keyCode);
		switch(keyCode){
		case KeyEvent.KEYCODE_DPAD_DOWN:
		case KeyEvent.KEYCODE_DPAD_LEFT:
		case KeyEvent.KEYCODE_DPAD_RIGHT:
		case KeyEvent.KEYCODE_DPAD_UP:
			keyCache.keyPressed(keyCode);
			break;
		default:
			return super.onKeyDown(keyCode, event);
		}
		return true;
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
//		Log.d("GameView","key up:"+keyCode);
		switch(keyCode){
		case KeyEvent.KEYCODE_DPAD_DOWN:
		case KeyEvent.KEYCODE_DPAD_LEFT:
		case KeyEvent.KEYCODE_DPAD_RIGHT:
		case KeyEvent.KEYCODE_DPAD_UP:
			keyCache.keyReleased(keyCode);
			break;
		default:
			return super.onKeyUp(keyCode, event);
		}
		return true;
	}

	@Override
	public void run() {
		while (runnable) {
			long startTime = System.currentTimeMillis();
			
			// 处理游戏逻辑
			if(needInit==false)
				logic();
			this.postInvalidate();
			
			long endTime = System.currentTimeMillis();
            long costTime = endTime - startTime;
            if(costTime<1000/FPS)
            {
                try{
                  Thread.sleep(1000/FPS-costTime);
                }
                catch(Exception e){
                   Log.e("GameView",e.getMessage());
                   Thread.currentThread().interrupt();
                }
            }
		}
		System.out.println("GameView stopped");
		
	}
	
	public void start()
	{
		thread=new Thread(this);
		thread.start();
	}
	
	public void stop()
	{
		this.runnable=false;
		//TODO commander.destroy();
		try {
			thread.join();
		} catch (InterruptedException e) {
			 Log.e("GameView",e.getMessage());
		}
	}

	

	
}
