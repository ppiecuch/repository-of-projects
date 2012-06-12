#ifndef _PARTICLEEMITER_H_
#define _PARTICLEEMITER_H_

#include "math.h"
#include  "Sparticle.h"
//#include "FileReader.h"



#define CC_PROPERTY(varType, varName,VarFName)\
public: virtual varType get##VarFName(void){return varName;}\
public: virtual void set##VarFName(varType var){varName=var;}

//#define CCRANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f)                         //-1~1的随机浮点数

 
#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f)                    //角度转换成弧度
 
#define ASSERT_GREATER_ZERO(_NUMBER_)  if(_NUMBER_<0)_NUMBER_=0;                           //确保大于等于0

#define ASSERT_RANGE(_NUMBER_) if(_NUMBER_<0)_NUMBER_=0;if(_NUMBER_>1)_NUMBER_=1;          //确保在范围内。


//粒子的几种朝向模式
enum  ROTATION_MODE
{
	//无定向朝向
	ROTATION_NONE=0,
	//向发射点
	ROTATION_EMITER,
	//向发射速度
	ROTATION_SPEED

};



class CParticleEmiter
{
public:

	
     
	//@hzb
	static IRandomizer* randomizer;
	static void setRandomizer(IRandomizer* randomizer){CParticleEmiter::randomizer=randomizer;}
	inline f32  CCRANDOM_MINUS1_1(){
		return (2.0f*(f32)randomizer->rand(0,INT_MAX)/INT_MAX)-1.0f;
	}

    //发射位置 对应于点发射器的位置，  方形发射器和圆形发射器的中心
	position2df  pos;                             //设置为浮点数结构，为以后粒子运动做好准备。
    //位置的浮动值
	position2df posVar; 
    CC_PROPERTY(position2df,pos,Pos);             //设置位置


	CC_PROPERTY(position2df,posVar,PosVar);       //设置位置浮动值 



	//发射速度大小          speed+speedVar>=0 
	float  speed;                
	//发射速度大小浮动值
	float  speedVar;
    CC_PROPERTY(float,speed,Speed);           //设置发射速度大小
	CC_PROPERTY(float,speedVar,SpeedVar);     //设置发射速度大小浮动值



	//发射方向
	int  angle;       
	//发射方向浮动值
	int  angleVar;
	CC_PROPERTY(int,angle,Angle);                                       //设置发射方向
	CC_PROPERTY(int,angleVar,AngleVar);                                 //设置发射方向



	//每次发送个数
	int  emissionRate;  
	//每次发送个数浮动值
	int  emissionRateVar;
	CC_PROPERTY(int,emissionRate,EmissionRate);                         //设置发射方向
	CC_PROPERTY(int,emissionRateVar,EmissionRateVar);                   //设置发射方向浮动


     
	//最大发送个数
	int maxParticleCount;                                                
    CC_PROPERTY(int,maxParticleCount,MaxParticleCount);



	//生命周期       
	int  timeToLive; 
	//生命周期的浮动值
	int  timeToLiveVar;
	CC_PROPERTY(int,timeToLive,TimeToLive);                            //设置生命值
	CC_PROPERTY(int,timeToLiveVar,TimeToLiveVar);                      //设置生命值浮动



	//初始尺寸
	dimension2df   startDimSize;
	//初始尺寸浮动值
	dimension2df  startDimSizeVar;
    CC_PROPERTY(dimension2df,startDimSize,StartDimSize);               //设置初始尺寸
	CC_PROPERTY(dimension2df,startDimSizeVar,StartDimSizeVar);         //设置初始尺寸浮动



	//最终尺寸
	dimension2df   endDimSize;
	//最终尺寸浮动值
	dimension2df   endDimSizeVar;
	CC_PROPERTY(dimension2df,endDimSize,EndDimSize);               //设置最终尺寸
	CC_PROPERTY(dimension2df,endDimSizeVar,EndDimSizeVar);         //设置最终尺寸浮动



	//是否保持尺寸不变
	bool startEqualEnd;
	CC_PROPERTY(bool,startEqualEnd,StartEqualEnd);

	//是否保持正方形
	bool  shapeOfSquare;        
	CC_PROPERTY(bool,shapeOfSquare,ShapeOfSquare);





	//初始颜色
	SColorf   startColor; 
	//初始颜色浮动值
	SColorf   startColorVar;   
	CC_PROPERTY(SColorf,startColor,StartColor);                     //设置初始颜色
	CC_PROPERTY(SColorf,startColorVar,StartColorVar);               //设置初始颜色浮动





	//终止颜色
	SColorf   endColor;
	//终止颜色浮动值
	SColorf   endColorVar;
	CC_PROPERTY(SColorf,endColor,EndColor);                     //设置终止颜色
	CC_PROPERTY(SColorf,endColorVar,EndColorVar);               //设置终止颜色浮动


   
	//是否保持颜色不变
	bool startColorEqualEnd;
	CC_PROPERTY(bool,startColorEqualEnd,StartColorEqualEnd);



	//旋转初始角度   如果为0则代表不旋转
	float  startRotation;
	//旋转初始角度浮动值
	float  startRotationVar;
	CC_PROPERTY(float,startRotation,StartRotation);                     //设置初始旋转角
	CC_PROPERTY(float,startRotationVar,StartRotationVar);               //设置初始旋转角浮动
    
    


	//旋转终止角度
	float  endRotation;
	//旋转终止角度浮动值
	float  endRotationVar;
	CC_PROPERTY(float,endRotation,EndRotation);                      //设置终止旋转角
	CC_PROPERTY(float,endRotationVar,EndRotationVar);                //设置终止旋转角浮动


    //  几种朝向模式
	bool startRotationEqualEnd;                                       //保存旋转角不变
	CC_PROPERTY(bool,startRotationEqualEnd,StartRotationEqualEnd);    //保存旋转角不变
     
	ROTATION_MODE  rotationMode;
	CC_PROPERTY(ROTATION_MODE,rotationMode,RotationMode);             //保存旋转角不变

    



	//发射器的作用时间
     int emiteLife;    
	 // CC_PROPERTY(int,emiteLife,EmiteLife);                           //设置生命值
     public: virtual int getEmiteLife(){return emiteLife;}
     public: virtual void setEmiteLife(int var){emiteLife=var;emiteLifeTemp=var;}



     
	 //是否循环
	 bool emiteAlways;                 
	 CC_PROPERTY(bool,emiteAlways,EmiteAlways);                    //设置是否一直循环

	 //为保存而设置生命值变量
	 int  emiteLifeTemp;
	 	 

	 int  timeSinceLastEmite;                                        //自从上次发射依赖的时间间隔  用于决定这一次发射的粒子数量


	 //发射器的影响器

	 //重力模式下运动影响器

	 position2df gravity;                                         //重力的两个方向 
     CC_PROPERTY(position2df,gravity,Gravity);                    //设置重力加速
	 
	 //沿发射点的加速度
	 float  radialAccel;    
	 CC_PROPERTY(float,radialAccel,RadialAccel);                     //设置沿发射点的加速度
       
	 //发射加速度浮动
	 float  radialAccelVar;
	 CC_PROPERTY(float,radialAccelVar,RadialAccelVar);                            

     //切线加速度
	 float  tangentialAccel;                                        //沿发射点切线加速度
	 CC_PROPERTY(float,tangentialAccel,TangentialAccel);             //设置切线加速度
     
	 //切线加速度浮动
	 float  tangentialAccelVar;
	 CC_PROPERTY(float,tangentialAccelVar,TangentialAccelVar);             //设置切线加速度浮动


        
	 //保存和导入粒子发射器的基本属性
	// virtual void Save(xc::io::IXMLWriter * XmlWriter);
	// virtual void Open(xc::io::IXMLReader * XmlReader);

	 //辅助函数
	// stringw Float2W_char(float data);
	// stringw Int2W_char(int data);
	// int     SColorf2Int(SColorf data);
	// SColorf  Int2SColorf(int data);


	 virtual  void   load(IReadStream *FileReader);

	 virtual  void     setZ(int Z);




 


    virtual int  getType();                                                         //得到发射器类型
    virtual	int  emit(int now,int timeSinceLastCall,CSparticle*& outArray);         //发射函数  当前时间，用于设置粒子的过期时间，距离上次发射时间间隔
    core::array<CSparticle> Particle;                                          //粒子集


	CParticleEmiter(void);
	virtual ~CParticleEmiter(void);

};
#endif