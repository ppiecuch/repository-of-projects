#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

/*
*
*�������
*����������
*���𵥸����������ݹ���ͻ���
*�������irr������������irr��أ�
*���Ҫ�ı���Ʒ������Ķ����ࡣ
*
*/

#include "framework.h"
//#include "FileReader.h"
//#include "FileWriter.h"


enum ORI                                          //����ģʽ
{
   ORI_CAMERA_2D,                                 //����ͷ
   ORI_FLOORD_2D,                                 //�������
   ORI_PERSON_2D                                  //�����ˣ���ֱ����
 
};

enum UVANIMATION                                  //UV����ģʽ   ��ʱû�õ�
{
	UV_L_TO_R,                                    //������
	UV_R_TO_L,                                    //���ҵ���
	UV_T_TO_B,
	UV_B_TO_T,
	UV_LT_TO_RB,
	UV_RB_TO_LT,
	UV_RT_TO_LB,
	UV_LB_TO_RT,
	UV_RANDOM,                                   //����UV����
	UV_NONE                                      //����

};


enum MIX_MODE                                    //���ģʽ       ��ʱû�õ�
{
    MIX_SCALE,                                   //�������        Ĭ��Ϊ�������
 
	MIX_NO_SCALE                                 //���������

};



class CBillBoard
{
public:

	
	//IMaterial *           p_material;
	SMaterial material;

	ENUM_MATERIAL_TYPE   mix_type;                                                    //��Ϸ�ʽ
	
	ENUM_BLEND_FACTOR    src_blend;                                                   //Դ�Ļ��

	ENUM_BLEND_FACTOR    dst_blend;                                                   //Ŀ����

	void  SetMixType(ENUM_BLEND_FACTOR src_blend_,ENUM_BLEND_FACTOR dst_blend_);      // ���û��ģʽ

	ENUM_MODULATE        modufun;                                                     //��Ϻ�Ŵ�ı���
       
	void  SetModulate(ENUM_MODULATE modufun_);                                        //���û�Ϸ���ϵ��

	u32                  alphasource;                                                 //�������ͨ�����ʱ��ͨ������Դ

	void  SetAlphaSource(u32 alphasource);                                            //����Alpha��Դ
	
	ORI      ori;                                                                     //���� 0 ��ͷ��1 ���棬 2���˲�

	void     SetOri(ORI ori_);                                                        //���ó���

	int   width;                                                                      //������

	f32   ratio;                                                                      //�����

	int   raduis;                                                                     //��С�İ�ΧԲ�뾶

	void  setDim(int width_,f32 ratio_=0);                                            //������С, ֻ�����������Ӱ�쵽ԭ����λ��

	f32     org_angle;                                                                //��ʼ�Ƕȣ� �ɳ����������

	f32     start_angle;                                                              //��ʼ�Ƕ�   �������õĳ�ʼ�ǶȺͳ�ʼ�Ƕ�֮�� 

	void    SetOrgAngle(f32 org_angle_);                                              //���õĳ�ʼ�Ƕ�

	position2df  or_tord[4];                                                          //ԭʼ�������λ�ã�����UV����

	void SetUV(float lt_u,float lt_v,float lb_u,float lb_v,float rt_u,float rt_v,float rb_u,float rb_v);      //���ó�ʼuv      
   
	float  u_speed;                                                                   //ˮƽU�ٶ�

	float  v_speed;                                                                   //ˮƽV�ٶ�       Ĭ�Ͼ�Ϊ 0

	void   SetUVSpeed(float u_speed_,float v_speed_);                                 //����UV�ٶ�
  
	ITexture      *  p_texture;                                                       //��������õ�����

	void  SetTexture(ITexture * p_tex);                                               //�������õ�����
	
	ITexture      *  p_texture_alpha;                                                 //ͨ������

	void  SetTextureAlpha(ITexture * p_texture_alpha_);                               //����ͨ������

	void  MixAlphaToRGB(ITexture * RGB_,ITexture * Alpha_);                           //��������ͨ������

	position2di pos;                                                               //����λ��       ���棬����Ч�༭�п��ر�

	void  setPos(position2di origin_);                                             //����λ��

	position2di  getPos();            

	void   setZ(int Z); 


	/*
	 ��ʱ��δ�õ���رյĽӿڡ�������	
	
	*/
   
	IVideoDriver  * driver;                                                           //��ͼ����
	scene::IGraphicsAdapter* gfAdapter;
  
	void   SetDriver(IVideoDriver * drv);                                             //���û�������
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter){this->gfAdapter=adapter;}

  
	//�ڲ�����

	SVertex   vec[4];                                                                 //�������ĸ�����
    	  
	u16 indices[6];                                                                   //��Ԫ��������


	//֧�ֶ�̬�仯������

	f32     angle;                             //��ת�Ƕ�

	void    SetAngle(f32 angel,int flag=0);    //flag 0 ���ӽǶȣ�1,���ٽǶȣ�2�����ýǶ�

	
	position2di  offset;                      //2dƫ�ƣ��������� 4�����ƫ�����ݡ�Ĭ��Ϊ��0 0��

	void SetOffset(position2di offset_);     //����ƫ����

    f32   scale;                             //��������

	void  SetScale(f32 scale_);              //���÷���


    UVANIMATION uv_animation;                //UV�Զ��嶯��

    bool   is_defined;                       //�Ƿ�Ԥ���壬Ĭ��Ϊ�ǣ��������Ԥ���壬��̬���ö���������Ч

	SColor  col;                             //��ɫ�仯

    void    setColor(SColor col_);           //������ɫ





	void    update(position2di offset_,f32 scale_,int angle_,f32 ratio_,ITexture * p_texture_);                          //ÿ�λ���ʱ���ø���
    
    void    update(int lt,int lb,int rb,int rt);                                                                         //������Զ�����������������ĸ������������

	void    render();                                                                                                    //���ƹ����

    
	void    update_vec();                                                                                              //���¶�������

	void    init();																					                  //��ʼ������






     //�����ı���͵���
	//void  save(IXMLWriter * XmlWriter);
	//void  open(IXMLReader * XmlReader);

	void    load(IReadStream  * FileReader);



	 //��������
	stringw Float2W_char(float data);
	stringw Int2W_char(int data);
  

    CBillBoard(void);
    ~CBillBoard(void);




};
#endif