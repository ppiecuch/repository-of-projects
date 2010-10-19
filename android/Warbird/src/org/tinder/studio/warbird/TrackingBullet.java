package org.tinder.studio.warbird;

import android.graphics.Point;

public class TrackingBullet extends Bullet {

	protected double deflectionAngleLimit;//偏转角界限，即跟踪过程中随着目标的移动，子弹方向调整的最大值
	protected Plane target;//目标
	protected int velocity;//速度
	protected double direction;//方向
	
	public TrackingBullet(int x, int y, int velocity, double direction,Feature feature,double deflectionAngleLimit,Plane target) {
		super(x, y, velocity, direction, feature);
		this.deflectionAngleLimit=deflectionAngleLimit;
		this.target=target;
		this.velocity=velocity;
		this.direction=direction;
	}
	
	@Override
	protected void update(int minX, int minY, int maxX, int maxY) {
		/*更新位置*/
		super.update(minX, minY, maxX, maxY);
		/*调整角度*/
		int vx=target.getPosition().x-this.position.x;
		int vy=target.getPosition().y-this.position.y;
		/*使用内积判断向量的夹角[0,180]*/
		int dot=Util.dot(dx,dy,vx,vy);
		float cos=dot/velocity*Util.modulus(vx,vy);
		double angle=Math.acos(cos);
		/*使用外积判断向量的夹角是顺时针还是逆时针*/
		int cross=Util.cross(dx,dy,vx,vy);
		if(angle==0||cross==0)
			return;
		/*参考偏转角界限进行调整*/
		Point result=new Point();
		if(angle<deflectionAngleLimit)
		{
			if(cross>0)
			{
				//逆时针旋转
				direction+=angle;
				Util.rotate(dx,dy, angle, result);
			}
			else if(cross<0)
			{
				//顺时针旋转
				direction-=angle;
				Util.rotate(dx,dy, -angle, result);
			}
		}
		else
		{
			if(cross>0)
			{
				//逆时针旋转
				direction+=deflectionAngleLimit;
				Util.rotate(dx,dy, deflectionAngleLimit, result);
			}
			else if(cross<0)
			{
				//顺时针旋转
				direction-=deflectionAngleLimit;
				Util.rotate(dx,dy, -deflectionAngleLimit, result);
			}
		}
		this.dx=result.x;
		this.dy=result.y;
		/*根据direction设置frameIndex*/
		while(direction<0)
			direction+=Gun.PI2;
		while(direction>Gun.PI2)
			direction-=Gun.PI2;
		if(direction<=Gun.PI_1_16||direction>Gun.PI_15_16)
			frameIndex=0;
		else if(direction>Gun.PI_1_16&&direction<=Gun.PI_3_16)
			frameIndex=1;
		else if(direction>Gun.PI_3_16&&direction<=Gun.PI_5_16)
			frameIndex=2;
		else if(direction>Gun.PI_5_16&&direction<=Gun.PI_7_16)
			frameIndex=3;
		else if(direction>Gun.PI_7_16&&direction<=Gun.PI_9_16)
			frameIndex=4;
		else if(direction>Gun.PI_9_16&&direction<=Gun.PI_11_16)
			frameIndex=5;
		else if(direction>Gun.PI_11_16&&direction<=Gun.PI_13_16)
			frameIndex=6;
		else if(direction>Gun.PI_13_16&&direction<=Gun.PI_15_16)
			frameIndex=7;
	}

}
