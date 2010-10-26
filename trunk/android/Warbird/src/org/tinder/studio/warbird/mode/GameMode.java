package org.tinder.studio.warbird.mode;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Effect;
import org.tinder.studio.warbird.Feature;
import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.KeyCache;
import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.Player;
import org.tinder.studio.warbird.R;
import org.tinder.studio.warbird.Util;
import org.tinder.studio.warbird.enemy.Boss;
import org.tinder.studio.warbird.enemy.Commander;
import org.tinder.studio.warbird.enemy.Enemy1;
import org.tinder.studio.warbird.equip.Equip;
import org.tinder.studio.warbird.gun.Gun;
import org.tinder.studio.warbird.gun.LevelAGun;
import org.tinder.studio.warbird.gun.LevelBGun;
import org.tinder.studio.warbird.gun.LevelCGun;
import org.tinder.studio.warbird.gun.LevelDGun;
import org.tinder.studio.warbird.gun.LevelGun;
import org.tinder.studio.warbird.gun.TrackingGun;
import org.tinder.studio.warbird.layer.ScrollBgLayer;
import org.tinder.studio.warbird.path.BeelinePath;
import org.tinder.studio.warbird.path.LoopXPath;
import org.tinder.studio.warbird.path.OvalYPath;
import org.tinder.studio.warbird.path.Path;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;

public class GameMode implements LoadableMode {
	
	private GameView view;
	int minX=0;
	int minY=0;
	int maxX=0;
	int maxY=0;
	private int progress=0;
	public static Gun trackingGun;
	public static Gun energyGun;
	private static final int increment=3; //移动速度
	private Player player;
	private ScrollBgLayer bg;
	private Commander commander;
	
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
	private int[][] tb={{287,50,12,29},{313,56,19,19},{337,59,30,12},{372,56,19,19},{407,50,11,29},{432,54,20,20},{458,59,30,12},{493,56,19,18}};
	private int[] lifeEquip={184,108,25,26};
	private int[] energyEquip={185,79,24,24};
	private int[] trackingEquip={213,108,28,26};
	private int[] bombEquip={213,81,25,24};
	
	public GameMode(GameView view){
		this.view=view;
	}

	@Override
	public void draw(Canvas canvas,Paint paint) {
		if(maxX==0)
		{
			this.maxX=view.getWidth();
			this.maxY=view.getHeight();
		}
		bg.draw(canvas, paint, minX, minY, maxX, maxY);
		checkCollision();
		Equip.drawAll(canvas, paint, minX, minY, maxX, maxY);
		Bullet.drawAll(canvas, paint,minX,minY,maxX,maxY);
		Plane.drawAllEnemies(canvas, paint, minX, minY, maxX, maxY);
		player.draw(canvas, paint,minX,minY,maxX,maxY);
		Effect.drawAll(canvas, paint, minX, minY, maxX, maxY);
		player.drawLifeIcon(canvas, paint, minX, minY, maxX, maxY);
	}

	@Override
	public void update(int keyState) {
		int width=view.getWidth();
		int height=view.getHeight();
		player.resetFrameIndex();
		if((keyState&KeyCache.KEY_UP)>0&&(keyState&KeyCache.KEY_LEFT)>0)
		{
			player.upLeft(0, 0);
			return;
		}
		if((keyState&KeyCache.KEY_UP)>0&&(keyState&KeyCache.KEY_RIGHT)>0)
		{
			player.upRight(width, 0);
			return;
		}
		if((keyState&KeyCache.KEY_DOWN)>0&&(keyState&KeyCache.KEY_LEFT)>0)
		{
			player.downLeft(0, height);
			return;
		}
		if((keyState&KeyCache.KEY_DOWN)>0&&(keyState&KeyCache.KEY_RIGHT)>0)
		{
			player.downRight(width, height);
			return;
		}
		if((keyState&KeyCache.KEY_DOWN)>0)
		{
			player.down(height);
			return;
		}
		if((keyState&KeyCache.KEY_UP)>0)
		{
			player.up(0);
			return;
		}
		if((keyState&KeyCache.KEY_LEFT)>0)
		{
			player.left(0);
			return;
		}
		if((keyState&KeyCache.KEY_RIGHT)>0)
		{
			player.right(width);
			return;
		}
		
	}
	
	private void checkCollision(){
		/*检测物品与飞机的碰撞*/
		if(Equip.getEquips()!=null)
		{
			for(Equip e:Equip.getEquips())
			{
				if(e.isDestroy()||player.isDestroy())
					continue;
				if(e.intersect(player))
				{
					e.equip(player);
				}
			}
		}
		/*检测子弹与飞机的碰撞*/
		for(Bullet b:Bullet.getBullets())
		{
			if(b.getPosition()!=null)
			{
				switch(b.getFeature().getCamp())
				{
				case Feature.CAMP_BLACK:
					if(player.isDestroy()||player.isVisible()==false)
						continue;
					if(b.intersect(player))
					{
						player.hitted(b);
						b.destroy();
					}
					break;
				case Feature.CAMP_WHITE:
					synchronized (Plane.LOCK_ENEMY) {
						for(Plane p:Plane.getEnemies())
						{
							if(p.isDestroy())
								continue;
							if(b.intersect(p))
							{
								p.hitted(b);
								b.destroy();
							}
						}
					}
				}
				
			}
		}
		/*检测飞机之间的碰撞*/
		synchronized (Plane.LOCK_ENEMY) {
			for(Plane p:Plane.getEnemies())
			{
				if(player.isDestroy()||player.isVisible()==false)
					break;
				if(p.isDestroy())
					continue;
				if(p.intersect(player))
				{
					player.hitted(p);
					p.hitted(player);
				}
			}
		}
	}

	@Override
	public int getProgress() {
		return progress;
	}

	@Override
	public void init() {
		
		int width=view.getWidth();
		int height=view.getHeight();
		
		bmp=((BitmapDrawable)view.getResources().getDrawable(R.drawable.pic)).getBitmap();
		bmp=Util.alphaColor(bmp, 0xFF040404, (byte)0);
		progress=5;
		
		/*初始化背景*/
		bg=new ScrollBgLayer(0, -1, ((BitmapDrawable)view.getResources().getDrawable(R.drawable.bg)).getBitmap());
		progress=10;
		
		/*初始化Player*/
		Bitmap[] players=new Bitmap[3];
		for(int i=0;i<p2.length;i++)
		{
			players[i]=Bitmap.createBitmap(bmp,p2[i][0],p2[i][1],p2[i][2],p2[i][3]);
		}
		int x=(width-players[0].getWidth())/2;
		int y=height-players[0].getHeight();
		Bitmap lifeIcon=Bitmap.createBitmap(bmp,p2Icon[0],p2Icon[1],p2Icon[2],p2Icon[3]);
		player=new Player(x, y, 1, Feature.CAMP_WHITE,3,3,lifeIcon);
		for(int i=0;i<p2.length;i++)
		{
			player.addFrame(players[i]);
		}
		progress=30;
		
		/*初始化Path*/
		Path beelinePath1=new BeelinePath(20, 0,20, height);
		Path beelinePath2=new BeelinePath(width-100, 0,width-100, height);
		Path beelinePath3=new BeelinePath(10, 0,10, height);
		Path beelinePath4=new BeelinePath(width-50, 0,width-50, height);
		Path beelinePath5=new BeelinePath(60, 0,60, height);
		Path beelinePath6=new BeelinePath(width-100, 0,width-100, height);
		Path beelinePath7=new BeelinePath(100, 0,100, height);
		Path beelinePath8=new BeelinePath(width-150, 0,width-150, height);
		Path loopXPath=new LoopXPath(0, width-180,2);
		Path ovalPath1=new OvalYPath(width/4,height/4, width/4+5, 6,Math.PI,0,false);
		Path ovalPath2=new OvalYPath(width/4,height/4, width*3/4-30, 6,0,Math.PI,true);
		progress=40;
		
		/*初始化Gun*/
		energyGun=new LevelGun(Feature.FEATURE_1,5,15,1);
		Gun gunE1=new LevelGun(Feature.FEATURE_E1,15,5,1);
		Gun gunE2=new LevelAGun(Feature.FEATURE_E1,50,5,1);
		Gun gunE22=new LevelAGun(Feature.FEATURE_E1,50,5,2);
		Gun gunE23=new LevelAGun(Feature.FEATURE_E1,50,5,3);
		Gun gunE3=new LevelBGun(Feature.FEATURE_E1,10,5,1);
		trackingGun=new TrackingGun(Feature.FEATURE_2,20,10,1);
		Gun gunE4=new LevelCGun(Feature.FEATURE_E1,15,8,1);
		Gun gunE5=new LevelDGun(Feature.FEATURE_E1,80,8,1);
		progress=50;
		
		
		/*初始化Enemy*/
		Bitmap enemy1Image=Bitmap.createBitmap(bmp,e1[0],e1[1],e1[2],e1[3]);
		Bitmap enemy3Image=Bitmap.createBitmap(bmp,e3[0],e3[1],e3[2],e3[3]);
		
		Enemy1 enemy1=new Enemy1(20,0,50, Feature.CAMP_BLACK,2,beelinePath1);
		enemy1.addFrame(enemy1Image);
		enemy1.addGun(gunE23);
		beelinePath1.reset(enemy1.getPosition());
		
		Enemy1 enemy2=new Enemy1(0,0,50, Feature.CAMP_BLACK,2,beelinePath2);
		enemy2.addFrame(enemy1Image);
		enemy2.addGun(gunE23);
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
		Enemy1 enemy9=new Enemy1(20,0,10, Feature.CAMP_BLACK,3,ovalPath1);
		enemy9.addFrame(enemy3Image);
		enemy9.addGun(gunE1);
		ovalPath1.reset(enemy9.getPosition());
		Enemy1 enemy10=new Enemy1(20,0,10, Feature.CAMP_BLACK,2,ovalPath2);
		enemy10.addFrame(enemy3Image);
		enemy10.addGun(gunE1);
		ovalPath2.reset(enemy10.getPosition());
		
		/*初始化BOSS*/
		Bitmap bossImage=Bitmap.createBitmap(bmp,bossCoor[0],bossCoor[1],bossCoor[2],bossCoor[3]);
		Boss boss=new Boss(0,0,1000,Feature.CAMP_BLACK,1,loopXPath);
		boss.addFrame(bossImage);
		boss.addGun(gunE4);
		boss.addGun(gunE3);
		boss.addGun(gunE5);
		progress=60;
		
		
		/*初始化Feature*/
		Bitmap bullet1=Bitmap.createBitmap(bmp,b[0],b[1],b[2],b[3]);
		Feature.FEATURE_1.addFrame(bullet1);
		Feature.FEATURE_E1.addFrame(bullet1);
		
		for(int i=0;i<tb.length;i++)
		{
			Feature.FEATURE_2.addFrame(Bitmap.createBitmap(bmp,tb[i][0],tb[i][1],tb[i][2],tb[i][3]));
		}
		progress=70;
		
		/*初始化Equip*/
		Bitmap life=Bitmap.createBitmap(bmp,lifeEquip[0],lifeEquip[1],lifeEquip[2],lifeEquip[3]);
		Equip.LIFE_EQUIP.addFrame(life);
		Bitmap energy=Bitmap.createBitmap(bmp,energyEquip[0],energyEquip[1],energyEquip[2],energyEquip[3]);
		Equip.ENERGY_EQUIP.addFrame(energy);
		Bitmap tracking=Bitmap.createBitmap(bmp,trackingEquip[0],trackingEquip[1],trackingEquip[2],trackingEquip[3]);
		Equip.TRACKING_EQUIP.addFrame(tracking);
		Bitmap bomb=Bitmap.createBitmap(bmp,bombEquip[0],bombEquip[1],bombEquip[2],bombEquip[3]);
		Equip.BOMB_EQUIP.addFrame(bomb);
		progress=80;
		
		
		/*初始化Effect*/
		for(int i=0;i<explode1.length;i++)
			Effect.FRAMES_1.add(Bitmap.createBitmap(bmp,explode1[i][0],explode1[i][1],explode1[i][2],explode1[i][3]));
		for(int i=0;i<explode2.length;i++)
			Effect.FRAMES_2.add(Bitmap.createBitmap(bmp,explode2[i][0],explode2[i][1],explode2[i][2],explode2[i][3]));
		
		player.addGun(energyGun);
		player.addGun(trackingGun);
		progress=90;
		
		
		
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
		commander.register("e9",enemy9);
		commander.register("e10",enemy10);
		commander.register("b1",boss);
		
		
		
		commander.addCommand("e3",1000,true);
		commander.addCommand("e4",0,false);
		commander.addCommand("e5",4000,false);
		commander.addCommand("e6",0,true);
		commander.addCommand("e7",4000,true);
		commander.addCommand("e8",0,false);
		commander.addCommand("e7",4000,false);
		commander.addCommand("e8",0,true);
		commander.addCommand("e5",4000,true);
		commander.addCommand("e6",0,false);
		commander.addCommand("e3",4000,false);
		commander.addCommand("e4",0,true);
		
		commander.addCommand("e1",10000,true);
		commander.addCommand("e2",0,true);
		commander.addCommand("e1",10000,true);
		commander.addCommand("e2",0,true);
		commander.addCommand("e9",5000,true);
		commander.addCommand("e10",0,true);
		commander.addCommand("e9",1000,true);
		commander.addCommand("e10",0,true);
		commander.addCommand("e9",1000,true);
		commander.addCommand("e10",0,true);
		commander.addCommand("b1",20000,false).addAward(Equip.generateRandomEquip());
		progress=99;
		
		
		commander.start();
		progress=100;
		Log.i("GameMode", "Initialize finished1!!!");
	}
	
	public Player getPlayer() {
		return player;
	}

	public ScrollBgLayer getBg() {
		return bg;
	}

	public Commander getCommander() {
		return commander;
	}
	
	

}
