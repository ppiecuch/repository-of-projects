package org.tinder.studio.warbird.mode;

import org.tinder.studio.warbird.Bullet;
import org.tinder.studio.warbird.Effect;
import org.tinder.studio.warbird.Feature;
import org.tinder.studio.warbird.GameView;
import org.tinder.studio.warbird.KeyCache;
import org.tinder.studio.warbird.Plane;
import org.tinder.studio.warbird.equip.Equip;

import android.graphics.Canvas;
import android.graphics.Paint;

public class GameMode implements Mode {
	
	private GameView view;
	int minX=0;
	int minY=0;
	int maxX=0;
	int maxY=0;
	
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
		view.getBg().draw(canvas, paint, minX, minY, maxX, maxY);
		checkCollision();
		Equip.drawAll(canvas, paint, minX, minY, maxX, maxY);
		Bullet.drawAll(canvas, paint,minX,minY,maxX,maxY);
		Plane.drawAllEnemies(canvas, paint, minX, minY, maxX, maxY);
		view.getPlayer().draw(canvas, paint,minX,minY,maxX,maxY);
		Effect.drawAll(canvas, paint, minX, minY, maxX, maxY);
		view.getPlayer().drawLifeIcon(canvas, paint, minX, minY, maxX, maxY);
	}

	@Override
	public void update(int keyState) {
		int width=view.getWidth();
		int height=view.getHeight();
		view.getPlayer().resetFrameIndex();
		if((keyState&KeyCache.KEY_UP)>0&&(keyState&KeyCache.KEY_LEFT)>0)
		{
			view.getPlayer().upLeft(0, 0);
			return;
		}
		if((keyState&KeyCache.KEY_UP)>0&&(keyState&KeyCache.KEY_RIGHT)>0)
		{
			view.getPlayer().upRight(width, 0);
			return;
		}
		if((keyState&KeyCache.KEY_DOWN)>0&&(keyState&KeyCache.KEY_LEFT)>0)
		{
			view.getPlayer().downLeft(0, height);
			return;
		}
		if((keyState&KeyCache.KEY_DOWN)>0&&(keyState&KeyCache.KEY_RIGHT)>0)
		{
			view.getPlayer().downRight(width, height);
			return;
		}
		if((keyState&KeyCache.KEY_DOWN)>0)
		{
			view.getPlayer().down(height);
			return;
		}
		if((keyState&KeyCache.KEY_UP)>0)
		{
			view.getPlayer().up(0);
			return;
		}
		if((keyState&KeyCache.KEY_LEFT)>0)
		{
			view.getPlayer().left(0);
			return;
		}
		if((keyState&KeyCache.KEY_RIGHT)>0)
		{
			view.getPlayer().right(width);
			return;
		}
		
	}
	
	private void checkCollision(){
		/*检测物品与飞机的碰撞*/
		if(Equip.getEquips()!=null)
		{
			for(Equip e:Equip.getEquips())
			{
				if(e.isDestroy()||view.getPlayer().isDestroy())
					continue;
				if(e.intersect(view.getPlayer()))
				{
					e.equip(view.getPlayer());
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
					if(view.getPlayer().isDestroy()||view.getPlayer().isVisible()==false)
						continue;
					if(b.intersect(view.getPlayer()))
					{
						view.getPlayer().hitted(b);
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
				if(view.getPlayer().isDestroy()||view.getPlayer().isVisible()==false)
					break;
				if(p.isDestroy())
					continue;
				if(p.intersect(view.getPlayer()))
				{
					view.getPlayer().hitted(p);
					p.hitted(view.getPlayer());
				}
			}
		}
	}
	
	

}
