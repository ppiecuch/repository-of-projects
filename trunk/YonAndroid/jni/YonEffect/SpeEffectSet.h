#ifndef  _SPEEFFECT_SET_H_
#define  _SPEEFFECT_SET_H_

///特效集合，是面对技能的接口单位
#include "SpeEffect.h"
class CSpeEffectSet
{
public:
	::core::array<CSpeEffect *> SpeArray;       //效果列表

	
	//更新和绘制
	void  update(int timediff);
	void  draw();
	void  clear();

	position2di  pos;                        //特效集的位置，默认为所有特效的平均值，当被设置或移动时,所有的特效元素都做相对的偏移
	void         setPos(position2di pos);
	position2di  getPos();
	void         reCalculatePos();           //重新计算位置     


	IVideoDriver        * driver;           //绘图引擎
	scene::IGraphicsAdapter* gfAdapter;
	void  setVideoDriver(IVideoDriver  * driver){this->driver=driver;}
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter){this->gfAdapter=adapter;}

	void  load(IReadStream  *FileReader);
	void  setZ(int Z);
    
    
	

	CSpeEffectSet(void);
	~CSpeEffectSet(void);

};

#endif