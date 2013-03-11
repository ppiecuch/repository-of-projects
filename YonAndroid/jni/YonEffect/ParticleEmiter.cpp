#include "ParticleEmiter.h"

//IRandomizer* CParticleEmiter::randomizer;

CParticleEmiter::CParticleEmiter(void)
{


	//初始化发射属性，即以下属性为默认属性
    
	//位置
	pos.set(0,0);
	//位置浮动
	posVar.set(0,0);

	//速度大小 
	speed=0.1f;
	//速度大小浮动
	speedVar=0.00f;

	//速度方向
	angle=0;                  //0度代表向上
	angleVar=0;               //速度方向浮动

	//发射个数
	emissionRate=100;      
	//发射个数浮动
	emissionRateVar=0;     


	//最大发射个数

	maxParticleCount=100;

	//生命周期
	timeToLive=1000; 
	//生命周期浮动
	timeToLiveVar=0;

	//初始尺寸
	startDimSize.set(16,16);
	startDimSizeVar.set(0,0);

	//终止尺寸
	endDimSize.set(16,16);
	endDimSizeVar.set(0,0);

	//是否保存尺寸不变
	startEqualEnd=false;

	//是否保持为正方形
	shapeOfSquare=false;

	//初始颜色
    startColor.set(1,1,1,1);
	startColorVar.set(0,0,0,0);

    //终止颜色
	endColor.set(1,1,1,1);
	endColorVar.set(0.0,0.0,0.0,0.0);


	//是否颜色保持不变
	startColorEqualEnd=false;

	//起始旋转角  @tbd;
     startRotation=0;
	 startRotationVar=0;

	 //终止旋转角
	 endRotation=0;
	 endRotationVar=0;

	 startRotationEqualEnd=false;


	 //朝向模式
	 rotationMode=ROTATION_NONE;


	 //重力加速
	 gravity.set(0,0);
     
	 //射线加速
	 radialAccel=0;
     radialAccelVar=0;

	 //切线加速
     tangentialAccel=0;
	 tangentialAccelVar=0;
 


	 timeSinceLastEmite=0;  
	 emiteLife=3000;
     emiteAlways=true;




}

CParticleEmiter::~CParticleEmiter(void)
{

    
}


int CParticleEmiter::emit( int now,int timeSinceLastCall,CSparticle*& outarray )
{


	return 0;

}


int CParticleEmiter::getType()
{

    return 0;                    //默认为点型

}



void CParticleEmiter::load( IReadStream *FileReader )
{

	emiteLife=FileReader->readInt();
	emiteLifeTemp=emiteLife;
	emiteAlways=FileReader->readBool();
	pos.x=FileReader->readFloat();
	pos.y=FileReader->readFloat();
	posVar.x=FileReader->readFloat();
	posVar.y=FileReader->readFloat();
	speed=FileReader->readFloat();
	speedVar=FileReader->readFloat();
	angle=FileReader->readInt();
	angleVar=FileReader->readInt();
	emissionRate=FileReader->readInt();
	emissionRateVar=FileReader->readInt();
	maxParticleCount=FileReader->readInt();
	timeToLive=FileReader->readInt();
	timeToLiveVar=FileReader->readInt();
	startDimSize.w=FileReader->readFloat();
	startDimSize.h=FileReader->readFloat();
	startDimSizeVar.w=FileReader->readFloat();
	startDimSizeVar.h=FileReader->readFloat();
	endDimSize.w=FileReader->readFloat();
	endDimSize.h=FileReader->readFloat();
	endDimSizeVar.w=FileReader->readFloat();
	endDimSizeVar.h=FileReader->readFloat();
	startEqualEnd=FileReader->readBool();
	shapeOfSquare=FileReader->readBool();
	float  a,r,g,b;
	a=FileReader->readFloat();
	r=FileReader->readFloat();
	g=FileReader->readFloat();
	b=FileReader->readFloat();
	startColor.set(a,b,g,r);
	a=FileReader->readFloat();
	r=FileReader->readFloat();
	g=FileReader->readFloat();
	b=FileReader->readFloat();
	startColorVar.set(a,b,g,r);
	a=FileReader->readFloat();
	r=FileReader->readFloat();
	g=FileReader->readFloat();
	b=FileReader->readFloat();
	endColor.set(a,b,g,r);
	a=FileReader->readFloat();
	r=FileReader->readFloat();
	g=FileReader->readFloat();
	b=FileReader->readFloat();
	endColorVar.set(a,b,g,r);
	startColorEqualEnd=FileReader->readBool();
	startRotation=FileReader->readFloat();
	startRotationVar=FileReader->readFloat();
	endRotation=FileReader->readFloat();
	endRotationVar=FileReader->readFloat();
	startRotationEqualEnd=FileReader->readBool();
	rotationMode=(ROTATION_MODE)FileReader->readInt();
	gravity.x=FileReader->readFloat();
	gravity.y=FileReader->readFloat();
	radialAccel=FileReader->readFloat();
	radialAccelVar=FileReader->readFloat();
	tangentialAccel=FileReader->readFloat();
	tangentialAccelVar=FileReader->readFloat();





}

void CParticleEmiter::setZ( int Z )
{
     
}


