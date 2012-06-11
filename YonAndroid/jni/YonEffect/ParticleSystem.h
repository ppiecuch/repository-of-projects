#pragma once
#include "ParticleEmiter.h"
#include "PointEmiter.h"
#include "BoxEmiter.h"
#include "CircleEmiter.h"




#define CC_PROPERTY_INTERFACE(varType,VarFName)\
public: void set##VarFName(varType var){if(emiter)emiter->set##VarFName(var);};\
public: virtual varType get##VarFName(void){if(emiter)return emiter->get##VarFName(); }\

#define CC_PROPERTY_INTERFACE_CIRCLE(varType,VarFName)\
public: void set##VarFName(varType var){if(emiter&&emiter->getType()==2)((CCircleEmiter *)emiter)->set##VarFName(var);};\
public: virtual varType get##VarFName(void){if(emiter&&emiter->getType()==2)return ((CCircleEmiter *)emiter)->get##VarFName(); }\



class CParticleSystem
{
private:
	video::IVideoDriver    * driver;                     //绘图引擎
     CParticleEmiter * emiter;
	 core::array<CSparticle>Particles;        //粒子集
	 int lastEmitTime;                             //上次发射的时间
	 IMaterial * p_material;                           //材质 
     ENUM_BLEND_FACTOR src_fator;                     //源系数 
	 ENUM_BLEND_FACTOR dst_fator;                     //目标系数
	 u32               alphasource;                   //alpha来源
	 ENUM_MODULATE     modufun;                     //放大系数
	 int               timeToLive;                 //粒子的寿命
	 bool              followMode;                 //跟随模式
	 core::position2df       tempPos;                    //跟随模式下的位置缓存数据


public:
	

	void setDriver(IVideoDriver    * driver_);
	void setEmiter(CParticleEmiter * emiter_);     //设置发射器
	void setTexture(c8 * path_);                   //设置纹理
	//CString texPath;                               //保存纹理路径
	void update(int time_);                        //更新粒子系统，发射器和影响器在这里有作用
	void draw();                                   //绘制粒子集,每次绘制前更新一次
	void recollabuffer();                          //充分配内存
    void clear();                                  //特效本身提供清空或复位的接口
    


    void setFollowMode(bool  followMode);
	void setEmiterType(int m_emiter_type_);        //设置发射类型
	int  m_emiter_type;                            //0：点型， 1：矩形  2：环形
	void setLengthAndWidth(int length,int width);  //设置长和宽，只对12有效

	//设置纹理的混合形态
	void setMixType(ENUM_BLEND_FACTOR src_fator_,ENUM_BLEND_FACTOR dst_fator_);
	void setAlphaSource(u32 alphasource_);
	void setModufunc(ENUM_MODULATE modufun_);
	

	//设置生命值
	void setTimeToLive_(int timeToLive);



	//CC_PROPERTY_INTERFACE(position2df,Pos);                    //位置
	void setPos(core::position2df var)                                 //为提供跟随模式做的支持
	{
		if(emiter)
		{
			tempPos=emiter->getPos();
			emiter->setPos(var);
			
			if (followMode)                                      //如果是跟随状态，在设置位置时同时为每克粒子加上偏移
			{
				core::position2df pos=emiter->getPos();
				core::position2df offsetPos=pos-tempPos;
				if(offsetPos.x!=0||offsetPos.y!=0)
				for (int i=0;i<Particles.size();i++)
					Particles[i].pos+=offsetPos;
				
				tempPos=emiter->getPos();

			}
		
		}

	};           

	core::position2df  getPos(){if(emiter)return emiter->getPos();return core::position2df(0,0);}

	void  setZ(int Z);

	CC_PROPERTY_INTERFACE(core::position2df,PosVar);          //位置浮动
	CC_PROPERTY_INTERFACE(float,Speed);                 //速度大小
	CC_PROPERTY_INTERFACE(float,SpeedVar);              //速度大小浮动
	CC_PROPERTY_INTERFACE(int,Angle);                   //速度方向
	CC_PROPERTY_INTERFACE(int,AngleVar);                //速度方向浮动
	CC_PROPERTY_INTERFACE(int,EmissionRate);            //粒子发射量
	CC_PROPERTY_INTERFACE(int,EmissionRateVar);         //粒子发射量浮动
	CC_PROPERTY_INTERFACE(int,MaxParticleCount);        //最大发射个数
	CC_PROPERTY_INTERFACE(int,TimeToLive);              //粒子生命
	CC_PROPERTY_INTERFACE(int,TimeToLiveVar);           //粒子生命浮动
	CC_PROPERTY_INTERFACE(core::dimension2df,StartDimSize);   //粒子初始尺寸
	CC_PROPERTY_INTERFACE(core::dimension2df,StartDimSizeVar);//粒子初始尺寸浮动
	CC_PROPERTY_INTERFACE(core::dimension2df,EndDimSize);     //粒子终止尺寸
	CC_PROPERTY_INTERFACE(core::dimension2df,EndDimSizeVar);  //粒子终止尺寸浮动
	CC_PROPERTY_INTERFACE(video::SColorf,StartColor);           //粒子的初始颜色
	CC_PROPERTY_INTERFACE(video::SColorf,StartColorVar);        //粒子的初始颜色浮动
	CC_PROPERTY_INTERFACE(video::SColorf,EndColor);             //粒子的最终颜色
	CC_PROPERTY_INTERFACE(video::SColorf,EndColorVar);          //粒子的最终颜色浮动
	CC_PROPERTY_INTERFACE(float,StartRotation);         //粒子的初始颜色
	CC_PROPERTY_INTERFACE(float,StartRotationVar);      //粒子的初始颜色浮动
	CC_PROPERTY_INTERFACE(float,EndRotation);           //粒子的终止颜色
	CC_PROPERTY_INTERFACE(float,EndRotationVar);        //粒子的终止颜色浮动
	CC_PROPERTY_INTERFACE(int,EmiteLife);               //粒子发射器的终止时间
	CC_PROPERTY_INTERFACE(bool,StartEqualEnd);          //设置大小是否保持不变
	CC_PROPERTY_INTERFACE(bool,ShapeOfSquare);          //设置是否保持正方形
	CC_PROPERTY_INTERFACE(bool,StartColorEqualEnd);     //设置颜色是否保持不变
	CC_PROPERTY_INTERFACE(bool,StartRotationEqualEnd);  //设置是否旋转不变
	CC_PROPERTY_INTERFACE(ROTATION_MODE,RotationMode);  //设置朝向模式
    CC_PROPERTY_INTERFACE(core::position2df,Gravity);         //设置重力加速器 
	CC_PROPERTY_INTERFACE(float,RadialAccel);            //设置沿发射点的加速度
	CC_PROPERTY_INTERFACE(float,RadialAccelVar);         //加速度浮动  
	CC_PROPERTY_INTERFACE(float,TangentialAccel);        //切线加速度
	CC_PROPERTY_INTERFACE(float,TangentialAccelVar);    //设置切线加速度浮动
    CC_PROPERTY_INTERFACE(bool,EmiteAlways);            //设置是否一直循环

	//环形发射器特有数据
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartRadius);               //最初的半径
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartRadiusVar);            //最初的半径浮动    
	CC_PROPERTY_INTERFACE_CIRCLE(float,EndRadius);                 //最终的半径
	CC_PROPERTY_INTERFACE_CIRCLE(float,EndRadiusVar);              //最终的半径浮动
	CC_PROPERTY_INTERFACE_CIRCLE(float,RotatePerSecond);           //旋转角速度
	CC_PROPERTY_INTERFACE_CIRCLE(float,RotatePerSecondVar);        //旋转角速度浮动
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartCircleAngle);          //旋转角初速度
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartCircleAngleVar);       //旋转角初速度浮动
	CC_PROPERTY_INTERFACE_CIRCLE(bool,StartRadiusEqualEnd);        //是否保存半径不变
   
	
     


	 //保存和导入粒子系统
	 // void Save(xc::io::IXMLWriter * XmlWriter);
	 // void Open(xc::io::IXMLReader * XmlReader);

	  //辅助函数
	//  stringw Int2W_char(int data);


	void    load(IReadStream *FileReader);
    
  
	 int   GetParticleAmount();



	/*创建发射器*/
	CParticleEmiter * CreatePointEmiter();        //创建点发射器
	CParticleEmiter * CreateBoxEmiter();          //创建点发射器
	CParticleEmiter * CreateCircleEmiter();       //创建环形发射器



	core::array<scene::SVertex>draw_buffer_v;     //绘制的缓存--顶点
	core::array<u16>draw_buffer_i;           //绘制的缓存--索引
	CParticleSystem(void);
	~CParticleSystem(void);




};
