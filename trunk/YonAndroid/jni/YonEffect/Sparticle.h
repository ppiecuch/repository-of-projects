#pragma once
#include "framework.h"
class CSparticle
{
public:

	 //一个粒子的绘制数据
     //位置
	 core::position2df pos;
	 //速度 包括方向和大小
	 core::position2df direct;
	 //由发射器控制最短生命和最长生命周期
     int  endTime;
	 //开始时间,用于影响器的操作
	 int  startTime;
	 //绘制的颜色
	 //
	  SColor col; 
	 //大小
	  //core::dimension2di size;
	  core::dimension2di startSize;
        // 
	  //开始的颜色
	   SColor start_col;



     //新版粒子数据

	 //速度大小
	  float speed;
	  //速度方向
	  int  angle;
	  //剩余生命周期
	  int timeToLive;
	  //颜色
	  SColorf color;
      //颜色变化率
      SColorf deltaColor;
	  //尺寸
	  dimension2df size;
	  //尺寸变化率
	  dimension2df deltaSize; 
	  //旋转角
	  float rotation;
	  //旋转角变化率
	  float deltaRotation;

	  //射线加速度
      float radialAccel;
	  float tagentialAccel;

      
	  //环形发射器特有的粒子特性
	  //当前的半径
	  float  radius;
	  //半径速度
	  float  deltaRadius;
	  //旋转速度
	  float  deltaRotationCircle;
	  //初始角度
	  float  angleCircle;
     



	  //距离上次调用的时间

	  //影响器作用变量
      
	
      


	CSparticle(void);
	~CSparticle(void);
};
