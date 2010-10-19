package org.tinder.studio.warbird;

import android.graphics.Point;

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
		/*�����Ƕ�*/
		int vx=target.getPosition().x-this.position.x;
		int vy=target.getPosition().y-this.position.y;
		/*ʹ���ڻ��ж������ļн�[0,180]*/
		int dot=Util.dot(dx,dy,vx,vy);
		float cos=dot/velocity*Util.modulus(vx,vy);
		double angle=Math.acos(cos);
		/*ʹ������ж������ļн���˳ʱ�뻹����ʱ��*/
		int cross=Util.cross(dx,dy,vx,vy);
		if(angle==0||cross==0)
			return;
		/*�ο�ƫת�ǽ��޽��е���*/
		Point result=new Point();
		if(angle<deflectionAngleLimit)
		{
			if(cross>0)
			{
				//��ʱ����ת
				direction+=angle;
				Util.rotate(dx,dy, angle, result);
			}
			else if(cross<0)
			{
				//˳ʱ����ת
				direction-=angle;
				Util.rotate(dx,dy, -angle, result);
			}
		}
		else
		{
			if(cross>0)
			{
				//��ʱ����ת
				direction+=deflectionAngleLimit;
				Util.rotate(dx,dy, deflectionAngleLimit, result);
			}
			else if(cross<0)
			{
				//˳ʱ����ת
				direction-=deflectionAngleLimit;
				Util.rotate(dx,dy, -deflectionAngleLimit, result);
			}
		}
		this.dx=result.x;
		this.dy=result.y;
		/*����direction����frameIndex*/
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
