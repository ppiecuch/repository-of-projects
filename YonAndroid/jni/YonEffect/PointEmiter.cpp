#include "PointEmiter.h"


/*颜色的乘积 */


float MultiparC1(float c,float d,float cV)
{
	
    c=c+d*cV;
	ASSERT_RANGE(c);


	return c;

}




CPointEmiter::CPointEmiter(void)
{
	//初始化



}

CPointEmiter::~CPointEmiter(void)
{
}

int CPointEmiter::emit( int now,int timeSinceLastCall,CSparticle*& outArray )
{
     

	timeSinceLastEmite += timeSinceLastCall;      //自从上次调用到现在的时间
    
	//该次发射的粒子数
	s32 pps=0;
	//if(emissionRateVar!=0)
   // pps = -emissionRateVar+rand()%(2*emissionRateVar+1);         //这一次随机的浮动

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

	   if(posVar.x)
	   temp_pos.x+=-posVar.x+rand()%((int)(2*posVar.x));
	   if(posVar.y)
	   temp_pos.y+=-posVar.y+rand()%((int)(2*posVar.y));


      


      for(int i=0;i<amount;i++)
	  {
		 
		  CSparticle par;

		 //点发射器的控制位置
		  par.pos=temp_pos; 
		
		 //控制起始时间,用于影响器
		  par.startTime = now;


	    //控制速度大小和方向
		   par.speed=speed+speedVar*CCRANDOM_MINUS1_1();
           par.angle=angle+angleVar*CCRANDOM_MINUS1_1();
		   float  angle_f=CC_DEGREES_TO_RADIANS(par.angle);
		   par.direct=position2df(par.speed*sinf(angle_f),par.speed*cosf(angle_f));
		 

		  
		 //控制粒子的生命值
          par.timeToLive=timeToLive+timeToLiveVar*CCRANDOM_MINUS1_1();
		  ASSERT_GREATER_ZERO(par.timeToLive);

		
		  //控制初始颜色
		  float a=MultiparC1(startColor.getAlpha(),CCRANDOM_MINUS1_1(),startColorVar.getAlpha());
		  float r=MultiparC1(startColor.getRed(),CCRANDOM_MINUS1_1(),startColorVar.getRed());
		  float g=MultiparC1(startColor.getGreen(),CCRANDOM_MINUS1_1(),startColorVar.getGreen());
		  float b=MultiparC1(startColor.getBlue(),CCRANDOM_MINUS1_1(),startColorVar.getBlue());
		  par.color.set(a,r,g,b);
		  



		  //控制颜色变化率
		  //1)得到终止颜色
		  SColorf endColorT;
		   a=MultiparC1(endColor.getAlpha(),CCRANDOM_MINUS1_1(),endColorVar.getAlpha());
		   r=MultiparC1(endColor.getRed(),CCRANDOM_MINUS1_1(),endColorVar.getRed());
		   g=MultiparC1(endColor.getGreen(),CCRANDOM_MINUS1_1(),endColorVar.getGreen());
		   b=MultiparC1(endColor.getBlue(),CCRANDOM_MINUS1_1(),endColorVar.getBlue());
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

int CPointEmiter::getType()
{

	return 0;    //点型发射器
}

void CPointEmiter::setZ( int Z )
{
	
}
