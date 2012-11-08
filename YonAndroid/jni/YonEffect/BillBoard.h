#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

/*
*
*公告板类
*功能描述：
*负责单个公告板的数据管理和绘制
*本类基于irr制作，数据与irr相关，
*如果要改变绘制方法，改动此类。
*
*/

#include "framework.h"
//#include "FileReader.h"
//#include "FileWriter.h"


enum ORI                                          //朝向模式
{
   ORI_CAMERA_2D,                                 //面向镜头
   ORI_FLOORD_2D,                                 //朝向地面
   ORI_PERSON_2D                                  //朝向人，竖直方向
 
};

enum UVANIMATION                                  //UV动画模式   暂时没用到
{
	UV_L_TO_R,                                    //从左到右
	UV_R_TO_L,                                    //从右到左
	UV_T_TO_B,
	UV_B_TO_T,
	UV_LT_TO_RB,
	UV_RB_TO_LT,
	UV_RT_TO_LB,
	UV_LB_TO_RT,
	UV_RANDOM,                                   //随意UV流动
	UV_NONE                                      //不动

};


enum MIX_MODE                                    //混合模式       暂时没用到
{
    MIX_SCALE,                                   //放缩混合        默认为放缩混合
 
	MIX_NO_SCALE                                 //不放缩混合

};



class CBillBoard
{
public:

	
	//IMaterial *           p_material;
	SMaterial material;

	ENUM_MATERIAL_TYPE   mix_type;                                                    //混合方式
	
	ENUM_BLEND_FACTOR    src_blend;                                                   //源的混合

	ENUM_BLEND_FACTOR    dst_blend;                                                   //目标混合

	void  SetMixType(ENUM_BLEND_FACTOR src_blend_,ENUM_BLEND_FACTOR dst_blend_);      // 设置混合模式

	ENUM_MODULATE        modufun;                                                     //混合后放大的倍数
       
	void  SetModulate(ENUM_MODULATE modufun_);                                        //设置混合放缩系数

	u32                  alphasource;                                                 //如果加上通道混合时，通道的来源

	void  SetAlphaSource(u32 alphasource);                                            //设置Alpha来源
	
	ORI      ori;                                                                     //朝向 0 镜头，1 地面， 2，人侧

	void     SetOri(ORI ori_);                                                        //设置朝向

	int   width;                                                                      //公告板宽

	f32   ratio;                                                                      //长宽比

	int   raduis;                                                                     //最小的包围圆半径

	void  setDim(int width_,f32 ratio_=0);                                            //调整大小, 只有这个函数才影响到原来的位置

	f32     org_angle;                                                                //初始角度， 由长宽比例决定

	f32     start_angle;                                                              //开始角度   包括设置的初始角度和初始角度之和 

	void    SetOrgAngle(f32 org_angle_);                                              //设置的初始角度

	position2df  or_tord[4];                                                          //原始点的纹理位置，用于UV动画

	void SetUV(float lt_u,float lt_v,float lb_u,float lb_v,float rt_u,float rt_v,float rb_u,float rb_v);      //设置初始uv      
   
	float  u_speed;                                                                   //水平U速度

	float  v_speed;                                                                   //水平V速度       默认均为 0

	void   SetUVSpeed(float u_speed_,float v_speed_);                                 //设置UV速度
  
	ITexture      *  p_texture;                                                       //公告板所用的纹理

	void  SetTexture(ITexture * p_tex);                                               //设置所用的纹理
	
	ITexture      *  p_texture_alpha;                                                 //通道纹理

	void  SetTextureAlpha(ITexture * p_texture_alpha_);                               //设置通道纹理

	void  MixAlphaToRGB(ITexture * RGB_,ITexture * Alpha_);                           //混合纹理和通道纹理

	position2di pos;                                                               //绘制位置       保存，在特效编辑中可重编

	void  setPos(position2di origin_);                                             //设置位置

	position2di  getPos();            

	void   setZ(int Z); 


	/*
	 暂时并未用到或关闭的接口、函数。	
	
	*/
   
	IVideoDriver  * driver;                                                           //绘图引擎
	scene::IGraphicsAdapter* gfAdapter;
  
	void   SetDriver(IVideoDriver * drv);                                             //设置绘制引擎
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter){this->gfAdapter=adapter;}

  
	//内部数据

	SVertex   vec[4];                                                                 //公告板的四个顶点
    	  
	u16 indices[6];                                                                   //基元排列索引


	//支持动态变化的数据

	f32     angle;                             //旋转角度

	void    SetAngle(f32 angel,int flag=0);    //flag 0 增加角度，1,减少角度，2，设置角度

	
	position2di  offset;                      //2d偏移，用于设置 4个点的偏移数据。默认为（0 0）

	void SetOffset(position2di offset_);     //设置偏移量

    f32   scale;                             //放缩比例

	void  SetScale(f32 scale_);              //设置放缩


    UVANIMATION uv_animation;                //UV自定义动画

    bool   is_defined;                       //是否预定义，默认为是，如果不是预定义，则动态设置动画纹理生效

	SColor  col;                             //颜色变化

    void    setColor(SColor col_);           //设置颜色





	void    update(position2di offset_,f32 scale_,int angle_,f32 ratio_,ITexture * p_texture_);                          //每次绘制时设置更新
    
    void    update(int lt,int lb,int rb,int rt);                                                                         //如果是自定义纹理动画，则更新四个点的纹理坐标

	void    render();                                                                                                    //绘制公告板

    
	void    update_vec();                                                                                              //更新顶点数据

	void    init();																					                  //初始化数据






     //公告板的保存和导入
	//void  save(IXMLWriter * XmlWriter);
	//void  open(IXMLReader * XmlReader);

	void    load(IReadStream  * FileReader);



	 //辅助函数
	stringw Float2W_char(float data);
	stringw Int2W_char(int data);
  

    CBillBoard(void);
    ~CBillBoard(void);




};
#endif