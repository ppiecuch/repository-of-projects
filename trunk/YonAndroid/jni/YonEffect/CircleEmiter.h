#pragma once
#include "particleemiter.h"

class CCircleEmiter :
	public CParticleEmiter
{
    int   width;        //��
	int   length;       //��
	bool  onlyAtLine;   //ֻ�����Ϸ���
    int  GetYByX(int x);                 //��xֵ�з���yֵ��Ϊ������㣬�������������
	int  x2y2;                           //width^2*length^2
	int  y2;                             //width^2
	f32  _x2;                           //1/y^2;


   //��ʼ�뾶
	float startRadius; 

   //��ʼ�뾶����
   float  startRadiusVar;   

   //��ֹ�뾶
   float endRadius;

   //��ֹ�뾶
   float endRadiusVar;

   //��ת�ٶ�
   float rotatePerSecond;

   //��ת�ٶȸ���

   float rotatePerSecondVar;

   //��ʼ���Ƕ�

   float  startCircleAngle;

   //���ӽǸ���

   float  startCircleAngleVar;

   //�Ƿ񱣴�뾶����
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



	//���ض�д������д����������
   // void Save(irr::io::IXMLWriter * XmlWriter);
	//void Open(irr::io::IXMLReader * XmlReader);


	virtual  void   load(IReadStream *FileReader);







    int  getType();                                                         //�õ�����������
	int  emit(int now,int timeSinceLastCall,CSparticle*& outArray);
	CCircleEmiter(void);
    ~CCircleEmiter(void);
};
