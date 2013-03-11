#include "CircleEmiter.h"


float MultiparC3(float c,float d,float cV)
{

	c=c+d*cV;
	ASSERT_RANGE(c);


	return c;

}



CCircleEmiter::CCircleEmiter(void)
{
	length=30;
	width=30;
	onlyAtLine=false;


	 y2=width*width;
	 x2y2=width*width*length*length;
	 _x2=1.0/(float)(length*length);


	 startRadius=100;
	 startRadiusVar=0;
	 endRadius=100;
	 endRadiusVar=0;
	 rotatePerSecond=0;
	 rotatePerSecondVar=0;
	 startCircleAngle=0;
	 startCircleAngleVar=0;
     startRadiusEqualEnd=false;


	

}

CCircleEmiter::~CCircleEmiter(void)
{
	Particle.set_used(0);
}

void CCircleEmiter::setLengthAndWidth( int length_,int width_ )
{
       length=length_;
		width=width_;


		y2=width*width;
		x2y2=width*width*length*length;
		_x2=1.0/(float)(length*length);

}

void CCircleEmiter::setOnlyAtLine( bool onlyatline/*=true*/ )
{
    
	onlyAtLine=onlyatline;

}

int CCircleEmiter::emit( int now,int timeSinceLastCall,CSparticle*& outArray )
{
 
/*

	time += timeSinceLastCall;

	const s32 pps = (max_per - min_per);
	const f32 perSecond = pps>0 ? (f32)min_per + (rand() % pps) : min_per;
	const f32 everyWhatMillisecond = 1000.0f / perSecond;

	if (time > everyWhatMillisecond)
	{


		int amount=time/everyWhatMillisecond+1;
		time = 0;
		Particle.set_used(0);
		if(width<0)width=0;
		if(length<0)length=0;
		for(int i=0;i<amount;i++)
		{
			CSparticle par;

			//�㷢�����Ŀ���λ��
              
			int pos_angel=rand()%361;
			f32 pos_angel_f=pos_angel*PI/180;
             
		    
			int  length_rand=rand()%length;
			int  width_rand=rand()%width;
           
			if(onlyAtLine)
			{
				length_rand=length;
				width_rand=width;
			}

			f32 x=pos.X+((f32)length_rand)*cos(pos_angel_f);
			f32 y=pos.Y+((f32)width_rand)*sin(pos_angel_f);
			par.pos.set(x,y);


			
		

			//������ʼʱ��,����Ӱ����
			par.startTime = now;

			//���Ʒ���
			par.direct=direct;
			if (angel>0)
			{
				//��תһ���Ƕ�
				//srand(time_t(0));
				int angel_random=-angel+rand()%(angel<<1);
				par.direct.rotateBy(angel_random);

			}

			//��������
			if (max_life-min_life == 0)
				par.endTime = now + min_life;
			else
				par.endTime = now + min_life + (rand() % (max_life-min_life));

			//������ɫ
			par.col = min_col.getInterpolated(
				max_col, (rand() % 100) / 100.0f);

			par.start_col=par.col;


			//���ƴ�С
			if (min_size==max_size)
				par.size=min_size;
			else
				par.size = min_size.getInterpolated(
				max_size, (rand() % 100) / 100.0f);


			par.startSize=par.size;

			Particle.push_back(par);


		}

		outArray = Particle.pointer();
		return  Particle.size();


	}

	return 0;
*/



	timeSinceLastEmite += timeSinceLastCall;      //�Դ��ϴε��õ����ڵ�ʱ��

	//�ôη����������
	s32 pps=0;
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

		/*if(posVar.x)
			temp_pos.x+=-posVar.x+rand()%((int)(2*posVar.x));
		if(posVar.y)
			temp_pos.y+=-posVar.y+rand()%((int)(2*posVar.y));*/
		//@hzb
		if(posVar.x)
			temp_pos.x+=-posVar.x+randomizer::rand(0,INT_MAX)%((int)(2*posVar.x));
		if(posVar.y)
			temp_pos.y+=-posVar.y+randomizer::rand(0,INT_MAX)%((int)(2*posVar.y));



       

		for(int i=0;i<amount;i++)
		{
			CSparticle par;

            //�������ӵ�����ֵ
			par.timeToLive=timeToLive+timeToLiveVar*CCRANDOM_MINUS1_1();
			ASSERT_GREATER_ZERO(par.timeToLive);


			//�㷢�����Ŀ���λ��
			par.pos=temp_pos;

			//�õ�����ķ����
			float radius_start=startRadius+CCRANDOM_MINUS1_1()*startRadiusVar;
			if(radius_start<0)radius_start=0;

			//�õ����յķ����
			float radius_end=endRadius+CCRANDOM_MINUS1_1()*endRadiusVar;
            if(radius_end<0)radius_end=0;

			par.radius=radius_start;
			par.deltaRadius=(radius_end-radius_start)/(float)par.timeToLive;

			//�õ���ת�ٶ�

			par.deltaRotationCircle=rotatePerSecond+CCRANDOM_MINUS1_1()*rotatePerSecondVar;

			//

			par.angleCircle=startCircleAngle+CCRANDOM_MINUS1_1()*startCircleAngleVar;



			float temp_angleCircle=CC_DEGREES_TO_RADIANS(par.angleCircle);
			//����λ��  ����Ϊ 0��
			par.pos.x+=par.radius*cosf(temp_angleCircle);
			par.pos.y-=par.radius*sinf(temp_angleCircle);


	

			//������ʼʱ��,����Ӱ����
			par.startTime = now;


			//�����ٶȴ�С�ͷ���
			par.speed=speed+speedVar*CCRANDOM_MINUS1_1();
			par.angle=angle+angleVar*CCRANDOM_MINUS1_1();
			float  angle_f=CC_DEGREES_TO_RADIANS(par.angle);
			par.direct=position2df(par.speed*sinf(angle_f),par.speed*cosf(angle_f));



			


			//���Ƴ�ʼ��ɫ
			float a=MultiparC3(startColor.getAlpha(),CCRANDOM_MINUS1_1(),startColorVar.getAlpha());
			float r=MultiparC3(startColor.getRed(),CCRANDOM_MINUS1_1(),startColorVar.getRed());
			float g=MultiparC3(startColor.getGreen(),CCRANDOM_MINUS1_1(),startColorVar.getGreen());
			float b=MultiparC3(startColor.getBlue(),CCRANDOM_MINUS1_1(),startColorVar.getBlue());
			par.color.set(a,r,g,b);


			//������ɫ�仯��
			//1)�õ���ֹ��ɫ
			SColorf endColorT;
			a=MultiparC3(endColor.getAlpha(),CCRANDOM_MINUS1_1(),endColorVar.getAlpha());
			r=MultiparC3(endColor.getRed(),CCRANDOM_MINUS1_1(),endColorVar.getRed());
			g=MultiparC3(endColor.getGreen(),CCRANDOM_MINUS1_1(),endColorVar.getGreen());
			b=MultiparC3(endColor.getBlue(),CCRANDOM_MINUS1_1(),endColorVar.getBlue());
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


int CCircleEmiter::GetYByX( int x )
{
        
   float dif=(x2y2-y2*x*x)*_x2;
   if(dif<0)return 0;
   return  sqrt(dif);

}

int CCircleEmiter::getType()
{

	return 2;   //���η�����

}

void CCircleEmiter::load( IReadStream *FileReader )
{

	CParticleEmiter::load(FileReader);

	startRadius=FileReader->readFloat();
	startRadiusVar=FileReader->readFloat();
	endRadius=FileReader->readFloat();
	endRadiusVar=FileReader->readFloat();
	rotatePerSecond=FileReader->readFloat();
	rotatePerSecondVar=FileReader->readFloat();
	startCircleAngle=FileReader->readFloat();
	startCircleAngleVar=FileReader->readFloat();
	startRadiusEqualEnd=FileReader->readBool();


}




