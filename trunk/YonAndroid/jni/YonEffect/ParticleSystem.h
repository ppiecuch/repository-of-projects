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
	video::IVideoDriver    * driver;                     //��ͼ����
     CParticleEmiter * emiter;
	 core::array<CSparticle>Particles;        //���Ӽ�
	 int lastEmitTime;                             //�ϴη����ʱ��
	 IMaterial * p_material;                           //���� 
     ENUM_BLEND_FACTOR src_fator;                     //Դϵ�� 
	 ENUM_BLEND_FACTOR dst_fator;                     //Ŀ��ϵ��
	 u32               alphasource;                   //alpha��Դ
	 ENUM_MODULATE     modufun;                     //�Ŵ�ϵ��
	 int               timeToLive;                 //���ӵ�����
	 bool              followMode;                 //����ģʽ
	 core::position2df       tempPos;                    //����ģʽ�µ�λ�û�������


public:
	

	void setDriver(IVideoDriver    * driver_);
	void setEmiter(CParticleEmiter * emiter_);     //���÷�����
	void setTexture(c8 * path_);                   //��������
	//CString texPath;                               //��������·��
	void update(int time_);                        //��������ϵͳ����������Ӱ����������������
	void draw();                                   //�������Ӽ�,ÿ�λ���ǰ����һ��
	void recollabuffer();                          //������ڴ�
    void clear();                                  //��Ч�����ṩ��ջ�λ�Ľӿ�
    


    void setFollowMode(bool  followMode);
	void setEmiterType(int m_emiter_type_);        //���÷�������
	int  m_emiter_type;                            //0�����ͣ� 1������  2������
	void setLengthAndWidth(int length,int width);  //���ó��Ϳ�ֻ��12��Ч

	//��������Ļ����̬
	void setMixType(ENUM_BLEND_FACTOR src_fator_,ENUM_BLEND_FACTOR dst_fator_);
	void setAlphaSource(u32 alphasource_);
	void setModufunc(ENUM_MODULATE modufun_);
	

	//��������ֵ
	void setTimeToLive_(int timeToLive);



	//CC_PROPERTY_INTERFACE(position2df,Pos);                    //λ��
	void setPos(core::position2df var)                                 //Ϊ�ṩ����ģʽ����֧��
	{
		if(emiter)
		{
			tempPos=emiter->getPos();
			emiter->setPos(var);
			
			if (followMode)                                      //����Ǹ���״̬��������λ��ʱͬʱΪÿ�����Ӽ���ƫ��
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

	CC_PROPERTY_INTERFACE(core::position2df,PosVar);          //λ�ø���
	CC_PROPERTY_INTERFACE(float,Speed);                 //�ٶȴ�С
	CC_PROPERTY_INTERFACE(float,SpeedVar);              //�ٶȴ�С����
	CC_PROPERTY_INTERFACE(int,Angle);                   //�ٶȷ���
	CC_PROPERTY_INTERFACE(int,AngleVar);                //�ٶȷ��򸡶�
	CC_PROPERTY_INTERFACE(int,EmissionRate);            //���ӷ�����
	CC_PROPERTY_INTERFACE(int,EmissionRateVar);         //���ӷ���������
	CC_PROPERTY_INTERFACE(int,MaxParticleCount);        //��������
	CC_PROPERTY_INTERFACE(int,TimeToLive);              //��������
	CC_PROPERTY_INTERFACE(int,TimeToLiveVar);           //������������
	CC_PROPERTY_INTERFACE(core::dimension2df,StartDimSize);   //���ӳ�ʼ�ߴ�
	CC_PROPERTY_INTERFACE(core::dimension2df,StartDimSizeVar);//���ӳ�ʼ�ߴ縡��
	CC_PROPERTY_INTERFACE(core::dimension2df,EndDimSize);     //������ֹ�ߴ�
	CC_PROPERTY_INTERFACE(core::dimension2df,EndDimSizeVar);  //������ֹ�ߴ縡��
	CC_PROPERTY_INTERFACE(video::SColorf,StartColor);           //���ӵĳ�ʼ��ɫ
	CC_PROPERTY_INTERFACE(video::SColorf,StartColorVar);        //���ӵĳ�ʼ��ɫ����
	CC_PROPERTY_INTERFACE(video::SColorf,EndColor);             //���ӵ�������ɫ
	CC_PROPERTY_INTERFACE(video::SColorf,EndColorVar);          //���ӵ�������ɫ����
	CC_PROPERTY_INTERFACE(float,StartRotation);         //���ӵĳ�ʼ��ɫ
	CC_PROPERTY_INTERFACE(float,StartRotationVar);      //���ӵĳ�ʼ��ɫ����
	CC_PROPERTY_INTERFACE(float,EndRotation);           //���ӵ���ֹ��ɫ
	CC_PROPERTY_INTERFACE(float,EndRotationVar);        //���ӵ���ֹ��ɫ����
	CC_PROPERTY_INTERFACE(int,EmiteLife);               //���ӷ���������ֹʱ��
	CC_PROPERTY_INTERFACE(bool,StartEqualEnd);          //���ô�С�Ƿ񱣳ֲ���
	CC_PROPERTY_INTERFACE(bool,ShapeOfSquare);          //�����Ƿ񱣳�������
	CC_PROPERTY_INTERFACE(bool,StartColorEqualEnd);     //������ɫ�Ƿ񱣳ֲ���
	CC_PROPERTY_INTERFACE(bool,StartRotationEqualEnd);  //�����Ƿ���ת����
	CC_PROPERTY_INTERFACE(ROTATION_MODE,RotationMode);  //���ó���ģʽ
    CC_PROPERTY_INTERFACE(core::position2df,Gravity);         //�������������� 
	CC_PROPERTY_INTERFACE(float,RadialAccel);            //�����ط����ļ��ٶ�
	CC_PROPERTY_INTERFACE(float,RadialAccelVar);         //���ٶȸ���  
	CC_PROPERTY_INTERFACE(float,TangentialAccel);        //���߼��ٶ�
	CC_PROPERTY_INTERFACE(float,TangentialAccelVar);    //�������߼��ٶȸ���
    CC_PROPERTY_INTERFACE(bool,EmiteAlways);            //�����Ƿ�һֱѭ��

	//���η�������������
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartRadius);               //����İ뾶
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartRadiusVar);            //����İ뾶����    
	CC_PROPERTY_INTERFACE_CIRCLE(float,EndRadius);                 //���յİ뾶
	CC_PROPERTY_INTERFACE_CIRCLE(float,EndRadiusVar);              //���յİ뾶����
	CC_PROPERTY_INTERFACE_CIRCLE(float,RotatePerSecond);           //��ת���ٶ�
	CC_PROPERTY_INTERFACE_CIRCLE(float,RotatePerSecondVar);        //��ת���ٶȸ���
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartCircleAngle);          //��ת�ǳ��ٶ�
	CC_PROPERTY_INTERFACE_CIRCLE(float,StartCircleAngleVar);       //��ת�ǳ��ٶȸ���
	CC_PROPERTY_INTERFACE_CIRCLE(bool,StartRadiusEqualEnd);        //�Ƿ񱣴�뾶����
   
	
     


	 //����͵�������ϵͳ
	 // void Save(xc::io::IXMLWriter * XmlWriter);
	 // void Open(xc::io::IXMLReader * XmlReader);

	  //��������
	//  stringw Int2W_char(int data);


	void    load(IReadStream *FileReader);
    
  
	 int   GetParticleAmount();



	/*����������*/
	CParticleEmiter * CreatePointEmiter();        //�����㷢����
	CParticleEmiter * CreateBoxEmiter();          //�����㷢����
	CParticleEmiter * CreateCircleEmiter();       //�������η�����



	core::array<scene::SVertex>draw_buffer_v;     //���ƵĻ���--����
	core::array<u16>draw_buffer_i;           //���ƵĻ���--����
	CParticleSystem(void);
	~CParticleSystem(void);




};
