#pragma once
#include "particleemiter.h"

class CCircleEmiter :
	public CParticleEmiter
{
    int   width;        //宽
	int   length;       //长
	bool  onlyAtLine;   //只在线上发射
    int  GetYByX(int x);                 //从x值中返回y值，为方便计算，保存下面的数据
	int  x2y2;                           //width^2*length^2
	int  y2;                             //width^2
	f32  _x2;                           //1/y^2;


   //起始半径
	float startRadius; 

   //起始半径浮动
   float  startRadiusVar;   

   //终止半径
   float endRadius;

   //终止半径
   float endRadiusVar;

   //旋转速度
   float rotatePerSecond;

   //旋转速度浮动

   float rotatePerSecondVar;

   //初始化角度

   float  startCircleAngle;

   //出视角浮动

   float  startCircleAngleVar;

   //是否保存半径不变
   bool   startRadiusEqualEnd;


public:
    
	void setLengthAndWidth(int length_,int width_);
	void setOnlyAtLine(bool onlyatline=true);
   /*
    void setStartRadius(float startRadius_);
	void setStartRadiusVar(float startRadiusVar_);
	void setEndRadius(float endRadius_);
	void setEndRadiusVar(float endRadiusVar_);
	void setRotatePerSecond(float rotatePerSecond_);
	void setRotatePerSecondVar(float rotatePerSecondVar_);
   */

	CC_PROPERTY(float,startRadius,StartRadius);
	CC_PROPERTY(float,startRadiusVar,StartRadiusVar);
	CC_PROPERTY(float,endRadius,EndRadius);
	CC_PROPERTY(float,endRadiusVar,EndRadiusVar);
	CC_PROPERTY(float,rotatePerSecond,RotatePerSecond);
	CC_PROPERTY(float,rotatePerSecondVar,RotatePerSecondVar);
	CC_PROPERTY(float,startCircleAngle,StartCircleAngle);
	CC_PROPERTY(float,startCircleAngleVar,StartCircleAngleVar);
	CC_PROPERTY(bool,startRadiusEqualEnd,StartRadiusEqualEnd);



	//重载读写函数，写入特有数据
   // void Save(irr::io::IXMLWriter * XmlWriter);
	//void Open(irr::io::IXMLReader * XmlReader);


	virtual  void   load(IReadStream *FileReader);







    int  getType();                                                         //得到发射器类型
	int  emit(int now,int timeSinceLastCall,CSparticle*& outArray);
	CCircleEmiter(void);
    ~CCircleEmiter(void);
};
