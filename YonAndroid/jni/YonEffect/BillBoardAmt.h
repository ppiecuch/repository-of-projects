#ifndef _BILLBOARDAMT_H_
#define _BILLBOARDAMT_H_

/*

动画公告板管理数据
功能描述：
主要负责公告板的动画数据

*/

#include "BillBoard.h"



class CBillBoardAmt
{
public:

	
	CBillBoard billboard;                           //保留公告板的对象


    //初始化
	void   init();                                  //初始化公告板动画
    
 


       

	//封装billboard的接口，以便代理
	void  setDriver(video::IVideoDriver  * driver);        //设置引擎
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter);
	void  setTexture(video::ITexture * texture);           //设置纹理
	void  setTextureAlpha(video::ITexture * texture_a);    //设置通道纹理
	void  setPos(position2di origin);            //设置原点
	position2di  getPos();                              //返回位置
    void  setDim(int width,f32 ratio);              //设置尺寸
	void  initBillboard();                          //初始化公告板
	void  setOrgAngel(f32 ori_angel);               //设置初始角
	void  setUV(float lt_u,float lt_v,float lb_u,float lb_v,float rt_u,float rt_v,float rb_u,float rb_v);
	void  setUVSpeed(float u_speed,float v_speed);
	void  SetMixType(ENUM_BLEND_FACTOR src_blend_,ENUM_BLEND_FACTOR dst_blend_);   
	void  SetAlphaSource(u32 alphasource);
	void  SetModulate(ENUM_MODULATE modufun_); 
    




    void SetOri(ORI ori);                                //设置朝向





	int   time_length;                              //动画的时间总长
	void  setTimeLength(int time_length_);          //设置动画时间

	int  time_since_start;                          //已经启动的时间

    int  draw_model;                                //绘制模式 0 代表不会制，n>0 代表绘制多少次 -1 代表重复
	
	
	//纹理数组变化
	struct TEXTURE_INFO
	{
		video::ITexture * p_textrue;                        //纹理数据
       int   time_l;                                //纹理占据的时间长度
	   int   time_start;                            //纹理开始被播放的时间
	
	};

	core::array<TEXTURE_INFO>texture_ary;      //纹理动画数据



	//自定义UV动画模式下的uv数组
	struct UV_INFO
	{
		int time_l;                                //作用时长 
	    int time_start;                            //作用起始时间
		
		int lt,lb,rb,rt;                           //四个顶点的纹理坐标
	
	};

	core::array<UV_INFO>uv_ary;               //uv动画数据


	



	//位置偏移信息
	struct OFFSET_INFO
	{
        position2di pos;
		int time_l;
        int time_start;
	};

	core::array<OFFSET_INFO>offset_ary;       //偏移数组

	position2di GetOffset(int time_current);       //得到当前的偏移数值

	void  SetOffsetAry(core::array<OFFSET_INFO>offset_ary_);//设置当前的偏移数组

	




	//放缩动画数据
	struct SCALE_INFO
	{
		int time_l;
		int time_start;                    

		f32 scale;              

	};

	core::array<SCALE_INFO>scale_ary;          //放缩数组


	float  GetScale(int time_current);             //得到放缩数据
	
	void   SetScaleAry(core::array<SCALE_INFO>scale_ary_);  //放缩数据





	//旋转的信息
	struct ROTATE_INFO
	{
		f32 angel;
		int time_l;
		int time_start;

	};

	core::array<ROTATE_INFO>angel_ary;                                               //旋转数组

	f32  GetAngel(int time_current);                                                     //根据时间节点得到旋转角速度
    
	void SetAngelAry(core::array<ROTATE_INFO>angel_ary_);                           //设置旋转数组


	

	//颜色变化数据
	struct COLOR_INFO
	{
		u32 col;
		int time_l;          //持续时间
		int time_start;      //变化时间
	
	};

	core::array<COLOR_INFO>color_ary;        //颜色变化数组

    u32 GetColor(int time_current);
    
	void  SetColorAry(core::array<COLOR_INFO>color_ary_);



	//通道变化数组 
	struct ALPHA_INFO
	{
		int  alpha;
		int  time_l;  
		int  time_start;   

	};

	core::array<ALPHA_INFO> alpha_ary;       //通道变化数组

	int  GetAlpha(int time_current);          

	void  SetAlphaAry(core::array<ALPHA_INFO>alpha_ary_);







	void  update(int time);                      //更新时间
    void  draw();                                //绘制
 

	 //公告板的保存和导入
	//void  save(IXMLWriter * XmlWriter);
	//void  open(IXMLReader * XmlReader);
    
	//void  readColorAry(IXMLReader * XmlReader);    //读颜色数组
	//void  readAlphaAry(IXMLReader * XmlReader);    //读入Alpha数组
	//void  readAngleAry(IXMLReader * XmlReader);    //读入旋转数组
	//void  readScaleAry(IXMLReader * XmlReader);    //读入放缩数据

	void    load(IReadStream * FileReader);
	void    setZ(int Z);


	//辅助函数
	 stringw Int2W_char(int data);
	 stringw Float2W_char(float data);


	 CBillBoardAmt(void);
	 ~CBillBoardAmt(void);




};
#endif