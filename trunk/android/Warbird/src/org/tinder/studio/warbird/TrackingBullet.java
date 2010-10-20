package org.tinder.studio.warbird;

import org.tinder.studio.warbird.gun.Gun;

import android.util.Log;

public class TrackingBullet extends Bullet {

	protected double deflectionAngleLimit;//ƫת�ǽ��ޣ������ٹ���������Ŀ����ƶ����ӵ�������������ֵ
	protected Plane target;//Ŀ��
	protected int velocity;//�ٶ�
	protected double direction;//����
	
	public TrackingBullet(int x, int y, int velocity, double direction,Feature feature,double deflectionAngleLimit,Plane target) {
		super(x, y, velocity, direction, feature);
		this.deflectionAngleLimit=deflectionAngleLimit;
		this.target=target;
		this.velocity=velocity;
		this.direction=direction;
	}
	
	@Override
	protected void update(int minX, int minY, int maxX, int maxY) {
		/*����λ��*/
		super.update(minX, minY, maxX, maxY);
		Log.d("TrackingBullet","------start-----");
		if(target!=null&&target.isDestroy()==false)
		{
			/*�ӵ���Ŀ�����������*/
			int vx=target.getPosition().x-this.position.x;
			int vy=target.getPosition().y-this.position.y;
			/*ʹ���ڻ��ж������ļн�[0,180]*/
			double dot=Util.dot(dx,dy,vx,vy);
			double cos=dot/velocity*Util.modulus(vx,vy);
			double angle=Math.acos(cos);
			/*ʹ������ж������ļн���˳ʱ�뻹����ʱ��*/
			double cross=Util.cross(dx,dy,vx,vy);
			if(angle!=0&&Math.abs(cross)>0.00001)
			{
				/*�ο�ƫת�ǽ��޽��е���*/
				Point2D result=new Point2D();
				if(angle<deflectionAngleLimit)
				{
					if(cross>0)
					{
						//��ʱ����ת
						direction+=angle;
						Util.rotate(dx,dy, angle, result);
						Log.d("TrackingBullet","ccw angle:"+angle+",direction:"+direction);
					}
					else if(cross<0)
					{
						//˳ʱ����ת
						direction-=angle;
						Util.rotate(dx,dy, -angle, result);
						Log.d("TrackingBullet","cw angle:"+angle+",direction:"+direction);
					}
				}
				else
				{
					if(cross>0)
					{
						//��ʱ����ת
						direction+=deflectionAngleLimit;
						Util.rotate(dx,dy, deflectionAngleLimit, result);
						Log.d("TrackingBullet","ccw limit:"+deflectionAngleLimit+",direction:"+direction);
					}
					else if(cross<0)
					{
						//˳ʱ����ת
						direction-=deflectionAngleLimit;
						Util.rotate(dx,dy, -deflectionAngleLimit, result);
						Log.d("TrackingBullet","cw limit:"+deflectionAngleLimit+",direction:"+direction);
					}
				}
				Log.d("TrackingBullet","dx:"+dx+",dy:"+dy+"--result:"+result.x+","+result.y);
				this.dx=result.x;
				this.dy=result.y;
			}
			/*����direction����frameIndex*/
			while(direction<0)
				direction+=Gun.PI2;
			while(direction>Gun.PI2)
				direction-=Gun.PI2;
			if(direction<=Gun.PI_1_8||direction>Gun.PI_15_8)
				frameIndex=6;
			else if(direction>Gun.PI_1_8&&direction<=Gun.PI_3_8)
				frameIndex=7;
			else if(direction>Gun.PI_3_8&&direction<=Gun.PI_5_8)
				frameIndex=0;
			else if(direction>Gun.PI_5_8&&direction<=Gun.PI_7_8)
				frameIndex=1;
			else if(direction>Gun.PI_7_8&&direction<=Gun.PI_9_8)
				frameIndex=2;
			else if(direction>Gun.PI_9_8&&direction<=Gun.PI_11_8)
				frameIndex=3;
			else if(direction>Gun.PI_11_8&&direction<=Gun.PI_13_8)
				frameIndex=4;
			else if(direction>Gun.PI_13_8&&direction<=Gun.PI_15_8)
				frameIndex=5;
			Log.d("TrackingBullet","frameIndex:"+frameIndex);
		}
		Log.d("TrackingBullet","------end-----");
	}

}
