#ifndef _SPEEFFECT_H_
#define _SPEEFFECT_H_


#include "ParticleSystem.h"            //����ϵͳ
#include "BillBoardAmt.h"              //�����

using namespace core;


//��ЧЧ������
enum  SPE_TYPE
{
   AROLA=0,       //ŷ��������
   BILLBOARD,     //�����
   PARTICLE       //������Ч
   

};

class CSpeEffect
{
public:

	SPE_TYPE  speType; 
	SPE_TYPE  getSpeType(){return speType;}

	//ŷ����������tbd

	//��������� tbd,

	//����ϵͳ   

	struct   SPE_EFFECT
	{
      
	CBillBoardAmt    * p_BillBoard;
	CParticleSystem  * p_ParticleSystem;
    
	//ŷ������������  @tbd
	
	}m_SpeEffect;


	//�ļ�·��

	core::stringw file;
	
	//��װ����Ч���Ľӿ�

	void load(c8 * filePath);                 //�����ļ�����ʼ����Ч ,�ڲ����ò�ͬ��Ч�ļ���
//	void load(IXMLReader * XmlReader);        //Ϊ��pc�˲����ã���ʽ��ֲʱ�ɲ���
	void update(int timediff);                //��Ч�Ļ��Ƶĸ���
	void draw();                              //��Ч�Ļ��ƣ��ڲ����ù���壬���ӣ���ŷ������������
    
	IVideoDriver  * driver;      
	scene::IGraphicsAdapter* gfAdapter;
	void setDriver(IVideoDriver * driver);
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter){this->gfAdapter=adapter;}

	void setPos(position2di pos);
	position2df  getPos();


	//��ʱ��λ�ú�ѭ����Ϣ
	//�����ļ���ʱ������
	bool tempPlayAlways;
	position2df  tempPos;


	//��ʼʱ��
	int startTime;
	int endTime;                              //endTime ���Ϊ0,������Ч����ĳ���Ϊ׼

	void  setStartTime(int time);
	void  setEndTime(int  time);
	void  setLife(int life);
	int   getLife();       
	void  setEnable(bool enable);                 //�����Ƿ��ֹ  tbd
	void  setPlayAlways(bool playAlways);         //�����Ƿ�һֱ����
    bool  getPlyaAlways();                        //�Ƿ�һֱ����
	int timeHasStart;                             //�����Ѳ��ŵ�ʱ��


	//�Ƿ񲥷�
	bool enable; 
	void setDisable(bool enable,bool reset);            //����
	void reSet();                                       //����,�����ܵ��Ѳ���ʱ����Ϊ0



   
	void    load(IReadStream *FileReader);

	void    setZ(int Z);
	void    setAngle(f32  angle);                                //���ýǶ�
 
	
	int getFileType( c8 * filePath );




	CSpeEffect(void);
	~CSpeEffect(void);
	
};

#endif