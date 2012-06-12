#ifndef _BILLBOARDAMT_H_
#define _BILLBOARDAMT_H_

/*

����������������
����������
��Ҫ���𹫸��Ķ�������

*/

#include "BillBoard.h"



class CBillBoardAmt
{
public:

	
	CBillBoard billboard;                           //���������Ķ���


    //��ʼ��
	void   init();                                  //��ʼ������嶯��
    
 


       

	//��װbillboard�Ľӿڣ��Ա����
	void  setDriver(video::IVideoDriver  * driver);        //��������
	void  setGraphicsAdapter(scene::IGraphicsAdapter* adapter);
	void  setTexture(video::ITexture * texture);           //��������
	void  setTextureAlpha(video::ITexture * texture_a);    //����ͨ������
	void  setPos(position2di origin);            //����ԭ��
	position2di  getPos();                              //����λ��
    void  setDim(int width,f32 ratio);              //���óߴ�
	void  initBillboard();                          //��ʼ�������
	void  setOrgAngel(f32 ori_angel);               //���ó�ʼ��
	void  setUV(float lt_u,float lt_v,float lb_u,float lb_v,float rt_u,float rt_v,float rb_u,float rb_v);
	void  setUVSpeed(float u_speed,float v_speed);
	void  SetMixType(ENUM_BLEND_FACTOR src_blend_,ENUM_BLEND_FACTOR dst_blend_);   
	void  SetAlphaSource(u32 alphasource);
	void  SetModulate(ENUM_MODULATE modufun_); 
    




    void SetOri(ORI ori);                                //���ó���





	int   time_length;                              //������ʱ���ܳ�
	void  setTimeLength(int time_length_);          //���ö���ʱ��

	int  time_since_start;                          //�Ѿ�������ʱ��

    int  draw_model;                                //����ģʽ 0 �������ƣ�n>0 ������ƶ��ٴ� -1 �����ظ�
	
	
	//��������仯
	struct TEXTURE_INFO
	{
		video::ITexture * p_textrue;                        //��������
       int   time_l;                                //����ռ�ݵ�ʱ�䳤��
	   int   time_start;                            //����ʼ�����ŵ�ʱ��
	
	};

	core::array<TEXTURE_INFO>texture_ary;      //����������



	//�Զ���UV����ģʽ�µ�uv����
	struct UV_INFO
	{
		int time_l;                                //����ʱ�� 
	    int time_start;                            //������ʼʱ��
		
		int lt,lb,rb,rt;                           //�ĸ��������������
	
	};

	core::array<UV_INFO>uv_ary;               //uv��������


	



	//λ��ƫ����Ϣ
	struct OFFSET_INFO
	{
        position2di pos;
		int time_l;
        int time_start;
	};

	core::array<OFFSET_INFO>offset_ary;       //ƫ������

	position2di GetOffset(int time_current);       //�õ���ǰ��ƫ����ֵ

	void  SetOffsetAry(core::array<OFFSET_INFO>offset_ary_);//���õ�ǰ��ƫ������

	




	//������������
	struct SCALE_INFO
	{
		int time_l;
		int time_start;                    

		f32 scale;              

	};

	core::array<SCALE_INFO>scale_ary;          //��������


	float  GetScale(int time_current);             //�õ���������
	
	void   SetScaleAry(core::array<SCALE_INFO>scale_ary_);  //��������





	//��ת����Ϣ
	struct ROTATE_INFO
	{
		f32 angel;
		int time_l;
		int time_start;

	};

	core::array<ROTATE_INFO>angel_ary;                                               //��ת����

	f32  GetAngel(int time_current);                                                     //����ʱ��ڵ�õ���ת���ٶ�
    
	void SetAngelAry(core::array<ROTATE_INFO>angel_ary_);                           //������ת����


	

	//��ɫ�仯����
	struct COLOR_INFO
	{
		u32 col;
		int time_l;          //����ʱ��
		int time_start;      //�仯ʱ��
	
	};

	core::array<COLOR_INFO>color_ary;        //��ɫ�仯����

    u32 GetColor(int time_current);
    
	void  SetColorAry(core::array<COLOR_INFO>color_ary_);



	//ͨ���仯���� 
	struct ALPHA_INFO
	{
		int  alpha;
		int  time_l;  
		int  time_start;   

	};

	core::array<ALPHA_INFO> alpha_ary;       //ͨ���仯����

	int  GetAlpha(int time_current);          

	void  SetAlphaAry(core::array<ALPHA_INFO>alpha_ary_);







	void  update(int time);                      //����ʱ��
    void  draw();                                //����
 

	 //�����ı���͵���
	//void  save(IXMLWriter * XmlWriter);
	//void  open(IXMLReader * XmlReader);
    
	//void  readColorAry(IXMLReader * XmlReader);    //����ɫ����
	//void  readAlphaAry(IXMLReader * XmlReader);    //����Alpha����
	//void  readAngleAry(IXMLReader * XmlReader);    //������ת����
	//void  readScaleAry(IXMLReader * XmlReader);    //�����������

	void    load(IReadStream * FileReader);
	void    setZ(int Z);


	//��������
	 stringw Int2W_char(int data);
	 stringw Float2W_char(float data);


	 CBillBoardAmt(void);
	 ~CBillBoardAmt(void);




};
#endif