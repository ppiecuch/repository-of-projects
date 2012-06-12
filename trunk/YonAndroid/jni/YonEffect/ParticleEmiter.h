#ifndef _PARTICLEEMITER_H_
#define _PARTICLEEMITER_H_

#include "math.h"
#include  "Sparticle.h"
//#include "FileReader.h"



#define CC_PROPERTY(varType, varName,VarFName)\
public: virtual varType get##VarFName(void){return varName;}\
public: virtual void set##VarFName(varType var){varName=var;}

//#define CCRANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f)                         //-1~1�����������

 
#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f)                    //�Ƕ�ת���ɻ���
 
#define ASSERT_GREATER_ZERO(_NUMBER_)  if(_NUMBER_<0)_NUMBER_=0;                           //ȷ�����ڵ���0

#define ASSERT_RANGE(_NUMBER_) if(_NUMBER_<0)_NUMBER_=0;if(_NUMBER_>1)_NUMBER_=1;          //ȷ���ڷ�Χ�ڡ�


//���ӵļ��ֳ���ģʽ
enum  ROTATION_MODE
{
	//�޶�����
	ROTATION_NONE=0,
	//�����
	ROTATION_EMITER,
	//�����ٶ�
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

    //����λ�� ��Ӧ�ڵ㷢������λ�ã�  ���η�������Բ�η�����������
	position2df  pos;                             //����Ϊ�������ṹ��Ϊ�Ժ������˶�����׼����
    //λ�õĸ���ֵ
	position2df posVar; 
    CC_PROPERTY(position2df,pos,Pos);             //����λ��


	CC_PROPERTY(position2df,posVar,PosVar);       //����λ�ø���ֵ 



	//�����ٶȴ�С          speed+speedVar>=0 
	float  speed;                
	//�����ٶȴ�С����ֵ
	float  speedVar;
    CC_PROPERTY(float,speed,Speed);           //���÷����ٶȴ�С
	CC_PROPERTY(float,speedVar,SpeedVar);     //���÷����ٶȴ�С����ֵ



	//���䷽��
	int  angle;       
	//���䷽�򸡶�ֵ
	int  angleVar;
	CC_PROPERTY(int,angle,Angle);                                       //���÷��䷽��
	CC_PROPERTY(int,angleVar,AngleVar);                                 //���÷��䷽��



	//ÿ�η��͸���
	int  emissionRate;  
	//ÿ�η��͸�������ֵ
	int  emissionRateVar;
	CC_PROPERTY(int,emissionRate,EmissionRate);                         //���÷��䷽��
	CC_PROPERTY(int,emissionRateVar,EmissionRateVar);                   //���÷��䷽�򸡶�


     
	//����͸���
	int maxParticleCount;                                                
    CC_PROPERTY(int,maxParticleCount,MaxParticleCount);



	//��������       
	int  timeToLive; 
	//�������ڵĸ���ֵ
	int  timeToLiveVar;
	CC_PROPERTY(int,timeToLive,TimeToLive);                            //��������ֵ
	CC_PROPERTY(int,timeToLiveVar,TimeToLiveVar);                      //��������ֵ����



	//��ʼ�ߴ�
	dimension2df   startDimSize;
	//��ʼ�ߴ縡��ֵ
	dimension2df  startDimSizeVar;
    CC_PROPERTY(dimension2df,startDimSize,StartDimSize);               //���ó�ʼ�ߴ�
	CC_PROPERTY(dimension2df,startDimSizeVar,StartDimSizeVar);         //���ó�ʼ�ߴ縡��



	//���ճߴ�
	dimension2df   endDimSize;
	//���ճߴ縡��ֵ
	dimension2df   endDimSizeVar;
	CC_PROPERTY(dimension2df,endDimSize,EndDimSize);               //�������ճߴ�
	CC_PROPERTY(dimension2df,endDimSizeVar,EndDimSizeVar);         //�������ճߴ縡��



	//�Ƿ񱣳ֳߴ粻��
	bool startEqualEnd;
	CC_PROPERTY(bool,startEqualEnd,StartEqualEnd);

	//�Ƿ񱣳�������
	bool  shapeOfSquare;        
	CC_PROPERTY(bool,shapeOfSquare,ShapeOfSquare);





	//��ʼ��ɫ
	SColorf   startColor; 
	//��ʼ��ɫ����ֵ
	SColorf   startColorVar;   
	CC_PROPERTY(SColorf,startColor,StartColor);                     //���ó�ʼ��ɫ
	CC_PROPERTY(SColorf,startColorVar,StartColorVar);               //���ó�ʼ��ɫ����





	//��ֹ��ɫ
	SColorf   endColor;
	//��ֹ��ɫ����ֵ
	SColorf   endColorVar;
	CC_PROPERTY(SColorf,endColor,EndColor);                     //������ֹ��ɫ
	CC_PROPERTY(SColorf,endColorVar,EndColorVar);               //������ֹ��ɫ����


   
	//�Ƿ񱣳���ɫ����
	bool startColorEqualEnd;
	CC_PROPERTY(bool,startColorEqualEnd,StartColorEqualEnd);



	//��ת��ʼ�Ƕ�   ���Ϊ0�������ת
	float  startRotation;
	//��ת��ʼ�Ƕȸ���ֵ
	float  startRotationVar;
	CC_PROPERTY(float,startRotation,StartRotation);                     //���ó�ʼ��ת��
	CC_PROPERTY(float,startRotationVar,StartRotationVar);               //���ó�ʼ��ת�Ǹ���
    
    


	//��ת��ֹ�Ƕ�
	float  endRotation;
	//��ת��ֹ�Ƕȸ���ֵ
	float  endRotationVar;
	CC_PROPERTY(float,endRotation,EndRotation);                      //������ֹ��ת��
	CC_PROPERTY(float,endRotationVar,EndRotationVar);                //������ֹ��ת�Ǹ���


    //  ���ֳ���ģʽ
	bool startRotationEqualEnd;                                       //������ת�ǲ���
	CC_PROPERTY(bool,startRotationEqualEnd,StartRotationEqualEnd);    //������ת�ǲ���
     
	ROTATION_MODE  rotationMode;
	CC_PROPERTY(ROTATION_MODE,rotationMode,RotationMode);             //������ת�ǲ���

    



	//������������ʱ��
     int emiteLife;    
	 // CC_PROPERTY(int,emiteLife,EmiteLife);                           //��������ֵ
     public: virtual int getEmiteLife(){return emiteLife;}
     public: virtual void setEmiteLife(int var){emiteLife=var;emiteLifeTemp=var;}



     
	 //�Ƿ�ѭ��
	 bool emiteAlways;                 
	 CC_PROPERTY(bool,emiteAlways,EmiteAlways);                    //�����Ƿ�һֱѭ��

	 //Ϊ�������������ֵ����
	 int  emiteLifeTemp;
	 	 

	 int  timeSinceLastEmite;                                        //�Դ��ϴη���������ʱ����  ���ھ�����һ�η������������


	 //��������Ӱ����

	 //����ģʽ���˶�Ӱ����

	 position2df gravity;                                         //�������������� 
     CC_PROPERTY(position2df,gravity,Gravity);                    //������������
	 
	 //�ط����ļ��ٶ�
	 float  radialAccel;    
	 CC_PROPERTY(float,radialAccel,RadialAccel);                     //�����ط����ļ��ٶ�
       
	 //������ٶȸ���
	 float  radialAccelVar;
	 CC_PROPERTY(float,radialAccelVar,RadialAccelVar);                            

     //���߼��ٶ�
	 float  tangentialAccel;                                        //�ط�������߼��ٶ�
	 CC_PROPERTY(float,tangentialAccel,TangentialAccel);             //�������߼��ٶ�
     
	 //���߼��ٶȸ���
	 float  tangentialAccelVar;
	 CC_PROPERTY(float,tangentialAccelVar,TangentialAccelVar);             //�������߼��ٶȸ���


        
	 //����͵������ӷ������Ļ�������
	// virtual void Save(xc::io::IXMLWriter * XmlWriter);
	// virtual void Open(xc::io::IXMLReader * XmlReader);

	 //��������
	// stringw Float2W_char(float data);
	// stringw Int2W_char(int data);
	// int     SColorf2Int(SColorf data);
	// SColorf  Int2SColorf(int data);


	 virtual  void   load(IReadStream *FileReader);

	 virtual  void     setZ(int Z);




 


    virtual int  getType();                                                         //�õ�����������
    virtual	int  emit(int now,int timeSinceLastCall,CSparticle*& outArray);         //���亯��  ��ǰʱ�䣬�����������ӵĹ���ʱ�䣬�����ϴη���ʱ����
    core::array<CSparticle> Particle;                                          //���Ӽ�


	CParticleEmiter(void);
	virtual ~CParticleEmiter(void);

};
#endif