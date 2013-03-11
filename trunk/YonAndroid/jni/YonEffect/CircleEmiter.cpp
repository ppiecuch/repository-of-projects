#include "CircleEmiter.h"


float MultiparC3(float c,float d,float cV)
{

	c=c+d*cV;
	ASSERT_RANGE(c);


	return c;

}



CCircleEmiter::CCircleEmiter(void)
{
	length=30;
	width=30;
	onlyAtLine=false;


	 y2=width*width;
	 x2y2=width*width*length*length;
	 _x2=1.0/(float)(length*length);


	 startRadius=100;
	 startRadiusVar=0;
	 endRadius=100;
	 endRadiusVar=0;
	 rotatePerSecond=0;
	 rotatePerSecondVar=0;
	 startCircleAngle=0;
	 startCircleAngleVar=0;
     startRadiusEqualEnd=false;


	

}

CCircleEmiter::~CCircleEmiter(void)
{
	Particle.set_used(0);
}

void CCircleEmiter::setLengthAndWidth( int length_,int width_ )
{
       length=length_;
		width=width_;


		y2=width*width;
		x2y2=width*width*length*length;
		_x2=1.0/(float)(length*length);

}

void CCircleEmiter::setOnlyAtLine( bool onlyatline/*=true*/ )
{
    
	onlyAtLine=onlyatline;

}

int CCircleEmiter::emit( int now,int timeSinceLastCall,CSparticle*& outArray )
{
 
/*

	time += timeSinceLastCall;

	const s32 pps = (max_per - min_per);
	const f32 perSecond = pps>0 ? (f32)min_per + (rand() % pps) : min_per;
	const f32 everyWhatMillisecond = 1000.0f / perSecond;

	if (time > everyWhatMillisecond)
	{


		int amount=time/everyWhatMillisecond+1;
		time = 0;
		Particle.set_used(0);
		if(width<0)width=0;
		if(length<0)length=0;
		for(int i=0;i<amount;i++)
		{
			CSparticle par;

			//点发射器的控制位置
              
			int pos_angel=rand()%361;
			f32 pos_angel_f=pos_angel*PI/180;
             
		    
			int  length_rand=rand()%length;
			int  width_rand=rand()%width;
           
			if(onlyAtLine)
			{
				length_rand=length;
				width_rand=width;
			}

			f32 x=pos.X+((f32)length_rand)*cos(pos_angel_f);
			f32 y=pos.Y+((f32)width_rand)*sin(pos_angel_f);
			par.pos.set(x,y);


			
		

			//控制起始时间,用于影响器
			par.startTime = now;

			//控制方向
			par.direct=direct;
			if (angel>0)
			{
				//旋转一定角度
				//srand(time_t(0));
				int angel_random=-angel+rand()%(angel<<1);
				par.direct.rotateBy(angel_random);

			}

			//控制生命
			if (max_life-min_life == 0)
				par.endTime = now + min_life;
			else
				par.endTime = now + min_life + (rand() % (max_life-min_life));

			//控制颜色
			par.col = min_col.getInterpolated(
				max_col, (rand() % 100) / 100.0f);

			par.start_col=par.col;


			//控制大小
			if (min_size==max_size)
				par.size=min_size;
			else
				par.size = min_size.getInterpolated(
				max_size, (rand() % 100) / 100.0f);


			par.startSize=par.size;

			Particle.push_back(par);


		}

		outArray = Particle.pointer();
		return  Particle.size();


	}

	return 0;
*/



	timeSinceLastEmite += timeSinceLastCall;      //自从上次调用到现在的时间

	//该次发射的粒子数
	s32 pps=0;
	f32 perSecond = emissionRate+CCRANDOM_MINUS1_1()*emissionRateVar;
		perSecond= perSecond>0 ?perSecond:0;
	if(perSecond==0)
	{
		timeSinceLastEmite=0;
		return 0;        //如果此时并无粒子发射，则返回.

	}

	const f32 everyWhatMillisecond = 1000.0f / perSecond;

	if (timeSinceLastEmite > everyWhatMillisecond)
	{

		 // float amountf=(float)(timeSinceLastEmite)/(float)(everyWhatMillisecond);
    //  float floatR=CCRANDOM_MINUS1_1()*0.5-0.001;
	//  if(floatR<0)floatR=-floatR;
	    int amount=(float)(timeSinceLastEmite)/(float)(everyWhatMillisecond);//floatR+amountf;
		timeSinceLastEmite = 0;

		Particle.set_used(0);

		//随机生成发射位置
		position2df temp_pos=pos;

		/*if(posVar.x)
			temp_pos.x+=-posVar.x+rand()%((int)(2*posVar.x));
		if(posVar.y)
			temp_pos.y+=-posVar.y+rand()%((int)(2*posVar.y));*/
		//@hzb
		if(posVar.x)
			temp_pos.x+=-posVar.x+randomizer::rand(0,INT_MAX)%((int)(2*posVar.x));
		if(posVar.y)
			temp_pos.y+=-posVar.y+randomizer::rand(0,INT_MAX)%((int)(2*posVar.y));



       

		for(int i=0;i<amount;i++)
		{
			CSparticle par;

            //控制粒子的生命值
			par.timeToLive=timeToLive+timeToLiveVar*CCRANDOM_MINUS1_1();
			ASSERT_GREATER_ZERO(par.timeToLive);


			//点发射器的控制位置
			par.pos=temp_pos;

			//得到最初的发射点
			float radius_start=startRadius+CCRANDOM_MINUS1_1()*startRadiusVar;
			if(radius_start<0)radius_start=0;

			//得到最终的发射点
			float radius_end=endRadius+CCRANDOM_MINUS1_1()*endRadiusVar;
            if(radius_end<0)radius_end=0;

			par.radius=radius_start;
			par.deltaRadius=(radius_end-radius_start)/(float)par.timeToLive;

			//得到旋转速度

			par.deltaRotationCircle=rotatePerSecond+CCRANDOM_MINUS1_1()*rotatePerSecondVar;

			//

			par.angleCircle=startCircleAngle+CCRANDOM_MINUS1_1()*startCircleAngleVar;



			float temp_angleCircle=CC_DEGREES_TO_RADIANS(par.angleCircle);
			//设置位置  假设为 0度
			par.pos.x+=par.radius*cosf(temp_angleCircle);
			par.pos.y-=par.radius*sinf(temp_angleCircle);


	

			//控制起始时间,用于影响器
			par.startTime = now;


			//控制速度大小和方向
			par.speed=speed+speedVar*CCRANDOM_MINUS1_1();
			par.angle=angle+angleVar*CCRANDOM_MINUS1_1();
			float  angle_f=CC_DEGREES_TO_RADIANS(par.angle);
			par.direct=position2df(par.speed*sinf(angle_f),par.speed*cosf(angle_f));



			


			//控制初始颜色
			float a=MultiparC3(startColor.getAlpha(),CCRANDOM_MINUS1_1(),startColorVar.getAlpha());
			float r=MultiparC3(startColor.getRed(),CCRANDOM_MINUS1_1(),startColorVar.getRed());
			float g=MultiparC3(startColor.getGreen(),CCRANDOM_MINUS1_1(),startColorVar.getGreen());
			float b=MultiparC3(startColor.getBlue(),CCRANDOM_MINUS1_1(),startColorVar.getBlue());
			par.color.set(a,r,g,b);


			//控制颜色变化率
			//1)得到终止颜色
			SColorf endColorT;
			a=MultiparC3(endColor.getAlpha(),CCRANDOM_MINUS1_1(),endColorVar.getAlpha());
			r=MultiparC3(endColor.getRed(),CCRANDOM_MINUS1_1(),endColorVar.getRed());
			g=MultiparC3(endColor.getGreen(),CCRANDOM_MINUS1_1(),endColorVar.getGreen());
			b=MultiparC3(endColor.getBlue(),CCRANDOM_MINUS1_1(),endColorVar.getBlue());
			endColorT.set(a,r,g,b);
			//2)得到颜色变化率
			a=((float)endColorT.getAlpha()-(float)par.color.getAlpha())/(float)par.timeToLive;
			r=((float)endColorT.getRed()-(float)par.color.getRed())/(float)par.timeToLive;
			g=((float)endColorT.getGreen()-(float)par.color.getGreen())/(float)par.timeToLive;
			b=((float)endColorT.getBlue()-(float)par.color.getBlue())/(float)par.timeToLive;
			par.deltaColor.set(a,r,g,b);



			//控制初始尺寸  
			float size_w=startDimSize.w+CCRANDOM_MINUS1_1()*startDimSizeVar.w;
			float size_h=startDimSize.h+CCRANDOM_MINUS1_1()*startDimSizeVar.h;
			if(size_w<0)size_w=0;
			if(size_h<0)size_h=0;
			if(shapeOfSquare)par.size.set(size_w,size_w);
		     else
			par.size.set(size_w,size_h);

			//控制尺寸变化率
			//1)得到最终尺寸
			dimension2df endDimSizeT;
			size_w=endDimSize.w+CCRANDOM_MINUS1_1()*endDimSizeVar.w;
			size_h=endDimSize.h+CCRANDOM_MINUS1_1()*endDimSizeVar.h;
			if(size_w<0)size_w=0;
			if(size_h<0)size_h=0;
			
			endDimSizeT.set(size_w,size_h);


			par.deltaSize.w=(endDimSizeT.w-par.size.w)/(float)par.timeToLive;
			if(shapeOfSquare)
			  par.deltaSize.h=par.deltaSize.w;
		    else
			par.deltaSize.h=(endDimSizeT.h-par.size.h)/(float)par.timeToLive;



			//旋转变化率
			float rotations,rotatione;
			rotations=startRotation+CCRANDOM_MINUS1_1()*startRotationVar;
			rotatione=endRotation+CCRANDOM_MINUS1_1()*endRotationVar;

			par.rotation=rotations;
			par.deltaRotation=(rotatione-rotations)/(float)par.timeToLive;



			//射线加速 
			float  radialAccel_=radialAccel+CCRANDOM_MINUS1_1()*radialAccelVar;
			par.radialAccel=radialAccel_;

			//切线加速
			float  tangentialAccel_=tangentialAccel+CCRANDOM_MINUS1_1()*tangentialAccelVar;
			par.tagentialAccel=tangentialAccel_;







			par.startSize=par.size;


			Particle.push_back(par);


		}

		outArray = Particle.pointer();
		return  Particle.size();


	}

	return 0;






}


int CCircleEmiter::GetYByX( int x )
{
        
   float dif=(x2y2-y2*x*x)*_x2;
   if(dif<0)return 0;
   return  sqrt(dif);

}

int CCircleEmiter::getType()
{

	return 2;   //环形发射器

}

void CCircleEmiter::load( IReadStream *FileReader )
{

	CParticleEmiter::load(FileReader);

	startRadius=FileReader->readFloat();
	startRadiusVar=FileReader->readFloat();
	endRadius=FileReader->readFloat();
	endRadiusVar=FileReader->readFloat();
	rotatePerSecond=FileReader->readFloat();
	rotatePerSecondVar=FileReader->readFloat();
	startCircleAngle=FileReader->readFloat();
	startCircleAngleVar=FileReader->readFloat();
	startRadiusEqualEnd=FileReader->readBool();


}




