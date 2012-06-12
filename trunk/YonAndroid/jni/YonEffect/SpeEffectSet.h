#ifndef  _SPEEFFECT_SET_H_
#define  _SPEEFFECT_SET_H_

///��Ч���ϣ�����Լ��ܵĽӿڵ�λ
#include "SpeEffect.h"
class CSpeEffectSet
{
public:
	::core::array<CSpeEffect *> SpeArray;       //Ч���б�

	
	//���ºͻ���
	void  update(int timediff);
	void  draw();
	void  clear();

	position2di  pos;                        //��Ч����λ�ã�Ĭ��Ϊ������Ч��ƽ��ֵ���������û��ƶ�ʱ,���е���ЧԪ�ض�����Ե�ƫ��
	void         setPos(position2di pos);
	position2di  getPos();
	void         reCalculatePos();           //���¼���λ��     


	IVideoDriver        * driver;           //��ͼ����
	scene::IGraphicsAdapter* gfAdapter;
	void  setVideoDriver(IVideoDriver  * driver){this->driver=driver;}
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter){this->gfAdapter=adapter;}

	void  load(IReadStream  *FileReader);
	void  setZ(int Z);
    
    
	

	CSpeEffectSet(void);
	~CSpeEffectSet(void);

};

#endif