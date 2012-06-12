#ifndef _SPEEFFECT_H_
#define _SPEEFFECT_H_


#include "ParticleSystem.h"            //粒子系统
#include "BillBoardAmt.h"              //公告板

using namespace core;


//特效效果类型
enum  SPE_TYPE
{
   AROLA=0,       //欧罗拉动画
   BILLBOARD,     //公告板
   PARTICLE       //粒子特效
   

};

class CSpeEffect
{
public:

	SPE_TYPE  speType; 
	SPE_TYPE  getSpeType(){return speType;}

	//欧罗拉动画，tbd

	//公告板数据 tbd,

	//粒子系统   

	struct   SPE_EFFECT
	{
      
	CBillBoardAmt    * p_BillBoard;
	CParticleSystem  * p_ParticleSystem;
    
	//欧罗拉动画对象  @tbd
	
	}m_SpeEffect;


	//文件路径

	core::stringw file;
	
	//封装各种效果的接口

	void load(c8 * filePath);                 //载入文件，初始化特效 ,内部调用不同特效的加载
//	void load(IXMLReader * XmlReader);        //为在pc端测试用，正式移植时可不用
	void update(int timediff);                //特效的绘制的更新
	void draw();                              //特效的绘制，内部调用公告板，粒子，或欧罗拉动画绘制
    
	IVideoDriver  * driver;      
	scene::IGraphicsAdapter* gfAdapter;
	void setDriver(IVideoDriver * driver);
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter){this->gfAdapter=adapter;}

	void setPos(position2di pos);
	position2df  getPos();


	//临时的位置和循环信息
	//用于文件打开时设置用
	bool tempPlayAlways;
	position2df  tempPos;


	//起始时间
	int startTime;
	int endTime;                              //endTime 如果为0,则以特效对象的长度为准

	void  setStartTime(int time);
	void  setEndTime(int  time);
	void  setLife(int life);
	int   getLife();       
	void  setEnable(bool enable);                 //设置是否禁止  tbd
	void  setPlayAlways(bool playAlways);         //设置是否一直播放
    bool  getPlyaAlways();                        //是否一直播放
	int timeHasStart;                             //技能已播放的时间


	//是否播放
	bool enable; 
	void setDisable(bool enable,bool reset);            //设置
	void reSet();                                       //重置,将技能的已播放时间置为0



   
	void    load(IReadStream *FileReader);

	void    setZ(int Z);
	void    setAngle(f32  angle);                                //设置角度
 
	
	int getFileType( c8 * filePath );




	CSpeEffect(void);
	~CSpeEffect(void);
	
};

#endif