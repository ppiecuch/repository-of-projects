#include "PointEmiter.h"


/*��ɫ�ĳ˻� */


float MultiparC1(float c,float d,float cV)
{
	
    c=c+d*cV;
	ASSERT_RANGE(c);


	return c;

}




CPointEmiter::CPointEmiter(void)
{
	//��ʼ��



}

CPointEmiter::~CPointEmiter(void)
{
}

int CPointEmiter::emit( int now,int timeSinceLastCall,CSparticle*& outArray )
{
     

	timeSinceLastEmite += timeSinceLastCall;      //�Դ��ϴε��õ����ڵ�ʱ��
    
	//�ôη����������
	s32 pps=0;
	//if(emissionRateVar!=0)
   // pps = -emissionRateVar+rand()%(2*emissionRateVar+1);         //��һ������ĸ���

	f32 perSecond = emissionRate+CCRANDOM_MINUS1_1()*emissionRateVar;
	perSecond= perSecond>0 ?perSecond:0;
	if(perSecond==0)
	{
       timeSinceLastEmite=0;
		return 0;        //�����ʱ�������ӷ��䣬�򷵻�.
	
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




	  //������ɷ���λ��
       position2df temp_pos=pos;

	   if(posVar.x)
	   temp_pos.x+=-posVar.x+rand()%((int)(2*posVar.x));
	   if(posVar.y)
	   temp_pos.y+=-posVar.y+rand()%((int)(2*posVar.y));


      


      for(int i=0;i<amount;i++)
	  {
		 
		  CSparticle par;

		 //�㷢�����Ŀ���λ��
		  par.pos=temp_pos; 
		
		 //������ʼʱ��,����Ӱ����
		  par.startTime = now;


	    //�����ٶȴ�С�ͷ���
		   par.speed=speed+speedVar*CCRANDOM_MINUS1_1();
           par.angle=angle+angleVar*CCRANDOM_MINUS1_1();
		   float  angle_f=CC_DEGREES_TO_RADIANS(par.angle);
		   par.direct=position2df(par.speed*sinf(angle_f),par.speed*cosf(angle_f));
		 

		  
		 //�������ӵ�����ֵ
          par.timeToLive=timeToLive+timeToLiveVar*CCRANDOM_MINUS1_1();
		  ASSERT_GREATER_ZERO(par.timeToLive);

		
		  //���Ƴ�ʼ��ɫ
		  float a=MultiparC1(startColor.getAlpha(),CCRANDOM_MINUS1_1(),startColorVar.getAlpha());
		  float r=MultiparC1(startColor.getRed(),CCRANDOM_MINUS1_1(),startColorVar.getRed());
		  float g=MultiparC1(startColor.getGreen(),CCRANDOM_MINUS1_1(),startColorVar.getGreen());
		  float b=MultiparC1(startColor.getBlue(),CCRANDOM_MINUS1_1(),startColorVar.getBlue());
		  par.color.set(a,r,g,b);
		  



		  //������ɫ�仯��
		  //1)�õ���ֹ��ɫ
		  SColorf endColorT;
		   a=MultiparC1(endColor.getAlpha(),CCRANDOM_MINUS1_1(),endColorVar.getAlpha());
		   r=MultiparC1(endColor.getRed(),CCRANDOM_MINUS1_1(),endColorVar.getRed());
		   g=MultiparC1(endColor.getGreen(),CCRANDOM_MINUS1_1(),endColorVar.getGreen());
		   b=MultiparC1(endColor.getBlue(),CCRANDOM_MINUS1_1(),endColorVar.getBlue());
         endColorT.set(a,r,g,b);
         //2)�õ���ɫ�仯��
		  a=((float)endColorT.getAlpha()-(float)par.color.getAlpha())/(float)par.timeToLive;
		  r=((float)endColorT.getRed()-(float)par.color.getRed())/(float)par.timeToLive;
		  g=((float)endColorT.getGreen()-(float)par.color.getGreen())/(float)par.timeToLive;
		  b=((float)endColorT.getBlue()-(float)par.color.getBlue())/(float)par.timeToLive;
          par.deltaColor.set(a,r,g,b);


          
		  //���Ƴ�ʼ�ߴ�  
		  float size_w=startDimSize.w+CCRANDOM_MINUS1_1()*startDimSizeVar.w;
		  float size_h=startDimSize.h+CCRANDOM_MINUS1_1()*startDimSizeVar.h;
		  if(size_w<0)size_w=0;
		  if(size_h<0)size_h=0;
		  if(shapeOfSquare)par.size.set(size_w,size_w);
		  else
		  par.size.set(size_w,size_h);



		  //���Ƴߴ�仯��
		  //1)�õ����ճߴ�
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


 

		  //��ת�仯��
		  float rotations,rotatione;
          rotations=startRotation+CCRANDOM_MINUS1_1()*startRotationVar;
		  rotatione=endRotation+CCRANDOM_MINUS1_1()*endRotationVar;

		  par.rotation=rotations;
		  par.deltaRotation=(rotatione-rotations)/(float)par.timeToLive;



          //���߼��� 
		 float  radialAccel_=radialAccel+CCRANDOM_MINUS1_1()*radialAccelVar;
		 par.radialAccel=radialAccel_;

		 //���߼���
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

	return 0;    //���ͷ�����
}

void CPointEmiter::setZ( int Z )
{
	
}
