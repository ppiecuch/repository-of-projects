package org.tinder.studio.warbird;

import java.util.LinkedList;
import java.util.List;

import org.tinder.studio.warbird.enemy.Command;
import org.tinder.studio.warbird.enemy.Commander;
import org.tinder.studio.warbird.enemy.Enemy1;
import org.tinder.studio.warbird.layer.ScrollBgLayer;
import org.tinder.studio.warbird.mode.GameMode;
import org.tinder.studio.warbird.mode.Mode;
import org.tinder.studio.warbird.path.BeelinePath;
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
	private static final int increment=3; //移动速度
	private Thread thread;
	private Integer width,height;
	private boolean runnable=true;
	private boolean needInit=true;
	private Paint foreground;
	private Mode mode;
	private Player player;
	private ScrollBgLayer bg;
	private Commander commander;
//	private List<Plane> enemies;
//	private List<Bullet> bullets;
	
	private Bitmap bmp;
	
	private int[][] p1={{0,315,50,48},{52,315,46,48},{96,315,46,48}};
	private int[][] p2={{0,367,50,40},{52,367,46,40},{96,367,46,40}};
	private int[] b={169,6,9,9};
	private int[] e1={65,79,90,60};
	private int[] e3={0,193,34,32};
	private int[][] explode1={{315,4,50,38},{365,4,50,38},{415,4,50,38},{465,4,50,38}};
	private int[][] explode2={{0,256,65,60},{65,256,65,60},{130,256,65,60},{195,256,65,60},
		{260,256,65,60},{325,256,65,60},{390,256,65,60},{455,256,65,60}};
	private int[] p2Icon={0,166,26,20};
	private int[] bossCoor={347,132,167,116};
	
//	private Bitmap[] players;
//	private Bitmap bullet;
//	private int[] bulletOffset;
//	private int index;
//	private Point position;
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
		List<Plane> enemies=new LinkedList<Plane>();
//		bullets=new LinkedList<Bullet>();
		
		this.keyCache=new KeyCache();
		bmp=((BitmapDrawable)getResources().getDrawable(R.drawable.pic)).getBitmap();
		bmp=Util.alphaColor(bmp, 0xFF040404, (byte)0);
		
		
//		players=new Bitmap[3];
//		for(int i=0;i<p2.length;i++)
//		{
//			players[i]=Bitmap.createBitmap(bmp,p2[i][0],p2[i][1],p2[i][2],p2[i][3]);
//		}
//		bullet=Bitmap.createBitmap(bmp,b[0],b[1],b[2],b[3]);
	}
	
	private void init(){
		this.width=getWidth();
		this.height=getHeight();
		Log.d("GameView","width:"+width+",height:"+height);
		
		/*初始化背景*/
		bg=new ScrollBgLayer(0, -1, ((BitmapDrawable)getResources().getDrawable(R.drawable.bg)).getBitmap());
		
		/*初始化Player*/
		Bitmap[] players=new Bitmap[3];
		for(int i=0;i<p2.length;i++)
		{
			players[i]=Bitmap.createBitmap(bmp,p2[i][0],p2[i][1],p2[i][2],p2[i][3]);
		}
		int x=(getWidth()-players[0].getWidth())/2;
		int y=getHeight()-players[0].getHeight();
		Bitmap lifeIcon=Bitmap.createBitmap(bmp,p2Icon[0],p2Icon[1],p2Icon[2],p2Icon[3]);
		player=new Player(x, y, 1, Feature.CAMP_WHITE,3,3,lifeIcon);
		for(int i=0;i<p2.length;i++)
		{
			player.addFrame(players[i]);
		}
		
		/*初始化Path*/
		Path beelinePath1=new BeelinePath(20, 0,20, height);
		Path beelinePath2=new BeelinePath(width-100, 0,width-100, height);
		Path beelinePath3=new BeelinePath(10, 0,10, height);
		Path beelinePath4=new BeelinePath(width-50, 0,width-50, height);
		Path beelinePath5=new BeelinePath(60, 0,60, height);
		Path beelinePath6=new BeelinePath(width-100, 0,width-100, height);
		Path beelinePath7=new BeelinePath(100, 0,100, height);
		Path beelinePath8=new BeelinePath(width-150, 0,width-150, height);
		
		/*初始化Gun*/
		Gun gun=new Level1Gun(Feature.FEATURE_1,5,10);
		Gun gunE1=new Level1Gun(Feature.FEATURE_E1,15,5);
		Gun gunE2=new LevelAGun(Feature.FEATURE_E1,50,5);
		Gun gunE3=new LevelBGun(Feature.FEATURE_E1,50,5);
		
		
		/*初始化Enemy*/
		Bitmap enemy1Image=Bitmap.createBitmap(bmp,e1[0],e1[1],e1[2],e1[3]);
		Bitmap enemy3Image=Bitmap.createBitmap(bmp,e3[0],e3[1],e3[2],e3[3]);
		
		Enemy1 enemy1=new Enemy1(20,0,50, Feature.CAMP_BLACK,2,beelinePath1);
		enemy1.addFrame(enemy1Image);
		enemy1.addGun(gunE1);
		beelinePath1.reset(enemy1.getPosition());
		
		Enemy1 enemy2=new Enemy1(0,0,50, Feature.CAMP_BLACK,2,beelinePath2);
		enemy2.addFrame(enemy1Image);
		enemy2.addGun(gunE1);
		beelinePath2.reset(enemy2.getPosition());
		
		Enemy1 enemy3=new Enemy1(0,0,10, Feature.CAMP_BLACK,2,beelinePath3);
		enemy3.addFrame(enemy3Image);
		enemy3.addGun(gunE2);
		beelinePath3.reset(enemy3.getPosition());
		Enemy1 enemy4=new Enemy1(0,0,10, Feature.CAMP_BLACK,2,beelinePath4);
		enemy4.addFrame(enemy3Image);
		enemy4.addGun(gunE2);
		beelinePath4.reset(enemy4.getPosition());
		Enemy1 enemy5=new Enemy1(0,0,10, Feature.CAMP_BLACK,2,beelinePath5);
		enemy5.addFrame(enemy3Image);
		enemy5.addGun(gunE2);
		beelinePath5.reset(enemy5.getPosition());
		Enemy1 enemy6=new Enemy1(0,0,10, Feature.CAMP_BLACK,2,beelinePath6);
		enemy6.addFrame(enemy3Image);
		enemy6.addGun(gunE2);
		beelinePath6.reset(enemy6.getPosition());
		Enemy1 enemy7=new Enemy1(0,0,10, Feature.CAMP_BLACK,2,beelinePath7);
		enemy7.addFrame(enemy3Image);
		enemy7.addGun(gunE2);
		beelinePath7.reset(enemy7.getPosition());
		Enemy1 enemy8=new Enemy1(0,0,10, Feature.CAMP_BLACK,2,beelinePath8);
		enemy8.addFrame(enemy3Image);
		enemy8.addGun(gunE2);
		beelinePath8.reset(enemy8.getPosition());
		
		/*初始化BOSS*/
		Bitmap bossImage=Bitmap.createBitmap(bmp,bossCoor[0],bossCoor[1],bossCoor[2],bossCoor[3]);
		Enemy1 boss=new Enemy1(0,0,100,Feature.CAMP_BLACK,1,beelinePath5);
		boss.addFrame(bossImage);
		boss.addGun(gunE3);
		
//		Plane.getEnemies().add(enemy1);
		
		
		/*初始化Feature*/
		Bitmap bullet1=Bitmap.createBitmap(bmp,b[0],b[1],b[2],b[3]);
		Feature.FEATURE_1.addFrame(bullet1);
		Feature.FEATURE_E1.addFrame(bullet1);
		
		/*初始化Effect*/
		for(int i=0;i<explode1.length;i++)
			Effect.FRAMES_1.add(Bitmap.createBitmap(bmp,explode1[i][0],explode1[i][1],explode1[i][2],explode1[i][3]));
		for(int i=0;i<explode2.length;i++)
			Effect.FRAMES_2.add(Bitmap.createBitmap(bmp,explode2[i][0],explode2[i][1],explode2[i][2],explode2[i][3]));
		
		player.addGun(gun);
		
		
		
		/*初始化Commander*/
		commander=new Commander();
		
		commander.register("e1",enemy1);
		commander.register("e2",enemy2);
		commander.register("e3",enemy3);
		commander.register("e4",enemy4);
		commander.register("e5",enemy5);
		commander.register("e6",enemy6);
		commander.register("e7",enemy7);
		commander.register("e8",enemy8);
		commander.register("b1",boss);
		
		
		
		commander.addCommand("e3",1000);
		commander.addCommand("e4",0);
		commander.addCommand("e5",4000);
		commander.addCommand("e6",0);
		commander.addCommand("e7",4000);
		commander.addCommand("e8",0);
		commander.addCommand("e7",4000);
		commander.addCommand("e8",0);
		commander.addCommand("e5",4000);
		commander.addCommand("e6",0);
		commander.addCommand("e3",4000);
		commander.addCommand("e4",0);
		
		commander.addCommand("e1",10000);
		commander.addCommand("e2",0);
		commander.addCommand("e1",20000);
		commander.addCommand("e2",0);
		commander.addCommand("b1",20000);
		
		
		commander.start();
		
		
		Log.i("GameView", "Initialize finished1!!!");
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
		this.commander.destroy();
		try {
			thread.join();
		} catch (InterruptedException e) {
			 Log.e("GameView",e.getMessage());
		}
	}

	public Player getPlayer() {
		return player;
	}

	public ScrollBgLayer getBg() {
		return bg;
	}

	
}
