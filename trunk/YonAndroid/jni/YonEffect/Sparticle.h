#pragma once
#include "framework.h"
class CSparticle
{
public:

	 //һ�����ӵĻ�������
     //λ��
	 core::position2df pos;
	 //�ٶ� ��������ʹ�С
	 core::position2df direct;
	 //�ɷ���������������������������
     int  endTime;
	 //��ʼʱ��,����Ӱ�����Ĳ���
	 int  startTime;
	 //���Ƶ���ɫ
	 //
	  SColor col; 
	 //��С
	  //core::dimension2di size;
	  core::dimension2di startSize;
        // 
	  //��ʼ����ɫ
	   SColor start_col;



     //�°���������

	 //�ٶȴ�С
	  float speed;
	  //�ٶȷ���
	  int  angle;
	  //ʣ����������
	  int timeToLive;
	  //��ɫ
	  SColorf color;
      //��ɫ�仯��
      SColorf deltaColor;
	  //�ߴ�
	  dimension2df size;
	  //�ߴ�仯��
	  dimension2df deltaSize; 
	  //��ת��
	  float rotation;
	  //��ת�Ǳ仯��
	  float deltaRotation;

	  //���߼��ٶ�
      float radialAccel;
	  float tagentialAccel;

      
	  //���η��������е���������
	  //��ǰ�İ뾶
	  float  radius;
	  //�뾶�ٶ�
	  float  deltaRadius;
	  //��ת�ٶ�
	  float  deltaRotationCircle;
	  //��ʼ�Ƕ�
	  float  angleCircle;
     



	  //�����ϴε��õ�ʱ��

	  //Ӱ�������ñ���
      
	
      


	CSparticle(void);
	~CSparticle(void);
};
