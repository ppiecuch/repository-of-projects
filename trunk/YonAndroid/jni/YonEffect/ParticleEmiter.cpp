#include "ParticleEmiter.h"

//IRandomizer* CParticleEmiter::randomizer;

CParticleEmiter::CParticleEmiter(void)
{


	//��ʼ���������ԣ�����������ΪĬ������
    
	//λ��
	pos.set(0,0);
	//λ�ø���
	posVar.set(0,0);

	//�ٶȴ�С 
	speed=0.1f;
	//�ٶȴ�С����
	speedVar=0.00f;

	//�ٶȷ���
	angle=0;                  //0�ȴ�������
	angleVar=0;               //�ٶȷ��򸡶�

	//�������
	emissionRate=100;      
	//�����������
	emissionRateVar=0;     


	//��������

	maxParticleCount=100;

	//��������
	timeToLive=1000; 
	//�������ڸ���
	timeToLiveVar=0;

	//��ʼ�ߴ�
	startDimSize.set(16,16);
	startDimSizeVar.set(0,0);

	//��ֹ�ߴ�
	endDimSize.set(16,16);
	endDimSizeVar.set(0,0);

	//�Ƿ񱣴�ߴ粻��
	startEqualEnd=false;

	//�Ƿ񱣳�Ϊ������
	shapeOfSquare=false;

	//��ʼ��ɫ
    startColor.set(1,1,1,1);
	startColorVar.set(0,0,0,0);

    //��ֹ��ɫ
	endColor.set(1,1,1,1);
	endColorVar.set(0.0,0.0,0.0,0.0);


	//�Ƿ���ɫ���ֲ���
	startColorEqualEnd=false;

	//��ʼ��ת��  @tbd;
     startRotation=0;
	 startRotationVar=0;

	 //��ֹ��ת��
	 endRotation=0;
	 endRotationVar=0;

	 startRotationEqualEnd=false;


	 //����ģʽ
	 rotationMode=ROTATION_NONE;


	 //��������
	 gravity.set(0,0);
     
	 //���߼���
	 radialAccel=0;
     radialAccelVar=0;

	 //���߼���
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

    return 0;                    //Ĭ��Ϊ����

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


