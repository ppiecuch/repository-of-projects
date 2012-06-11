
#include "BillBoardAmt.h"

CBillBoardAmt::CBillBoardAmt(void)
{
	time_length=3000;     //10�� һ�����
	time_since_start=0;    //����ʱ��Ϊ0
	draw_model=-1;         //Ĭ��Ϊѭ������
}

CBillBoardAmt::~CBillBoardAmt(void)
{

}

void CBillBoardAmt::setTimeLength( int time_length_ )
{
       time_length=time_length_;
       
	   //ͬʱ����ǰ�����ݵ�ʱ���ӳ�
	   {
	   
	   }


}

void CBillBoardAmt::update( int time )
{ 

	if(draw_model==0)return;
	 
	//����ʱ��
	time_since_start+=time;

	if(time_since_start>time_length)
	{
		if(draw_model>0)
			draw_model--;                 //������ƶ�Σ�����ٻ��ƴ���
	}


	//��������ʱ���жϸ�������  ����ɫ

	COLORREF col=GetColor(time_since_start);

	int r=(0x00ff0000&col)>>16;
	int b=(0x000000ff&col)<<16;
    COLORREF col2=(col&0x0000ff00)|r|b;
	//billboard.setColor(0xff000000|col);


	//��������ʱ���жϸ�������  ��alphaͨ��

     int a=GetAlpha(time_since_start);
     a=a<<24;
	 a&=0xff000000;
       
	//������ɫ��ͨ��
	 billboard.setColor(a|col);

    //������ת����

	float angel=GetAngel(time_since_start);
    billboard.SetAngle(angel);
  

	//���÷���

	float scale=GetScale(time_since_start);
	billboard.SetScale(scale);

	
	//����ƫ��

	position2di offset=GetOffset(time_since_start);
	billboard.SetOffset(offset);


	billboard.update_vec();


   
}


/*
*�õ���ɫ����ĵ�ǰ��ɫ,���޵�ǰ��ɫ������ ��ɫ
*/
COLORREF CBillBoardAmt::GetColor( int time_current )
{
	
	
	if(color_ary.size()==0)return RGB(255,255,255);

	
    int size=color_ary.size();

	if(time_current>time_length && draw_model!=-1)
	{
		COLORREF col=color_ary[size-1].col;
		int r=(0x00ff0000&col)>>16;
		int b=(0x000000ff&col)<<16;
		COLORREF col2=(col&0x0000ff00)|r|b;

		return  col2;

	}
	int time_now=time_current%(time_length);               //��ȡ��ǰʱ�䣬Ӧ��ѭ�������
	
	for(int i=0;i<size;i++)
	{

	  COLOR_INFO col_info=color_ary[i];
      if(col_info.time_start<time_now&&(col_info.time_start+col_info.time_l)>=time_now)
	  {
		  
		  f32 ratio=(f32)(time_now-col_info.time_start)/(f32)col_info.time_l;


		  //��������֮��Ĳ�ֵ
		  if(i<(size-1))
		  {
		
			 COLORREF col_ref=col_info.col;
             COLORREF col_ref2=color_ary[i+1].col;
             
			 int b=(col_ref&0x00ff0000)>>16;
			 int g=(col_ref&0x0000ff00)>>8;
			 int r=(col_ref&0x000000ff);

			 int b2=(col_ref2&0x00ff0000)>>16;
			 int g2=(col_ref2&0x0000ff00)>>8;
			 int r2=(col_ref2&0x000000ff);

			 return  RGB(b*(1-ratio)+b2*ratio,g*(1-ratio)+g2*ratio,r*(1-ratio)+r2*ratio);
		  
		  }		  
		  
		  {
			  COLORREF col=col_info.col;
			  int r=(0x00ff0000&col)>>16;
			  int b=(0x000000ff&col)<<16;
			  COLORREF col2=(col&0x0000ff00)|r|b;

			  return  col2;
		  }
	


	  
	  }



	}

	//�Ҳ����������һ����ɫ
	COLORREF col=color_ary[size-1].col;
	int r=(0x00ff0000&col)>>16;
	int b=(0x000000ff&col)<<16;
	COLORREF col2=(col&0x0000ff00)|r|b;

	return  col2;


}







void CBillBoardAmt::draw()
{


	if(draw_model!=0)
     billboard.render();


}

void CBillBoardAmt::initBillboard()
{
   
	billboard.init();


}


void CBillBoardAmt::setDriver( IVideoDriver * driver )
{
     
      billboard.SetDriver(driver);

}



void CBillBoardAmt::setTexture( ITexture * texture )
{

      billboard.SetTexture(texture);

}




void CBillBoardAmt::init()
{
       
    //��ʼ��������
	billboard.init();
    texture_ary.clear();             //������������
	uv_ary.clear();                  //����UV��������
	scale_ary.clear();               //���������������
	offset_ary.clear();              //����λ��ƫ������
	angel_ary.clear();               //������ת����
	color_ary.clear();               //�����ɫ����
	alpha_ary.clear();               //���alphaͨ������




	//test!!//   ����

	//���Է���
	SCALE_INFO scaleInfo;
	scaleInfo.scale=2;
	scaleInfo.time_l=1500;
	scaleInfo.time_start=0;
	scale_ary.push_back(scaleInfo);

    scaleInfo.scale=0.2;
	scaleInfo.time_l=1500;
	scaleInfo.time_start=1500;

	scale_ary.push_back(scaleInfo);


	scaleInfo.scale=2;
	scaleInfo.time_l=0;
	scaleInfo.time_start=3000;

	scale_ary.push_back(scaleInfo);

	//������ɫ

	COLOR_INFO  colorInfo;
	colorInfo.col=RGB(0,0,255);
	colorInfo.time_start=0;
	colorInfo.time_l=3000;

	color_ary.push_back(colorInfo);

	colorInfo.col=RGB(0,255,0);
	colorInfo.time_l=0;
	colorInfo.time_start=3000;

	color_ary.push_back(colorInfo);


	//������ת

	ROTATE_INFO rotateInfo;
	rotateInfo.angel=20;
	rotateInfo.time_start=0;
	rotateInfo.time_l=3000;

	angel_ary.push_back(rotateInfo);

	rotateInfo.angel=10;
	rotateInfo.time_l=1500;
	rotateInfo.time_start=1500;

//	angel_ary.push_back(rotateInfo);

	rotateInfo.angel=20;
	rotateInfo.time_start=3000;
	rotateInfo.time_l=0;

	angel_ary.push_back(rotateInfo);



	

	


}

void CBillBoardAmt::setPos( position2di origin )
{
      billboard.setPos(origin);
}



position2di  CBillBoardAmt::getPos()
{

return	billboard.getPos();

}

//������ɫ����
void CBillBoardAmt::SetColorAry( core::array<COLOR_INFO>color_ary_ )
{
	color_ary=color_ary_;
       
}

//����ͨ���仯����
void CBillBoardAmt::SetAlphaAry( core::array<ALPHA_INFO>alpha_ary_ )
{
      
	alpha_ary=alpha_ary_;

}

int CBillBoardAmt::GetAlpha( int time_current )
{

	if(alpha_ary.size()==0)return 255;

	
	int size=alpha_ary.size();


	if(time_current>time_length && draw_model!=-1)
		return alpha_ary[size-1].alpha;

	int time_now=time_current%time_length;                //��ȡ��ǰʱ�䣬Ӧ��ѭ�������

	for(int i=0;i<size;i++)
	{

		ALPHA_INFO alpha_info=alpha_ary[i];
		if(alpha_info.time_start<time_now&&(alpha_info.time_start+alpha_info.time_l)>=time_now)
		{

			f32 ratio=(f32)(time_now-alpha_info.time_start)/(f32)alpha_info.time_l;


			//��������֮��Ĳ�ֵ

				int  alpha1=alpha_info.alpha;
				int  alpha2;
				if(i<(size-1))
		        alpha2=alpha_ary[i+1].alpha;
				else 
				alpha2=alpha_ary[0].alpha;
		
				return  alpha1*(1-ratio)+alpha2*ratio;



		}


	}

	//�Ҳ����������һ����ɫ
	return alpha_ary[size-1].alpha;



}

//������ת�ٶ�����
void CBillBoardAmt::SetAngelAry( core::array<ROTATE_INFO>angel_ary_ )
{
    
	 angel_ary=angel_ary_;
 
}

float  CBillBoardAmt::GetAngel( int time_current )
{



	if(angel_ary.size()==0)return 0.0;

	

	int size=angel_ary.size();

	if(time_current>time_length && draw_model!=-1)
		return 0;

	int time_now=time_current%time_length;        //��ȡ��ǰʱ�䣬Ӧ��ѭ�������

	for(int i=0;i<size;i++)
	{

		ROTATE_INFO rotate_info=angel_ary[i];
		if(rotate_info.time_start<time_now&&(rotate_info.time_start+rotate_info.time_l)>=time_now)
		{

			f32 ratio=(f32)(time_now-rotate_info.time_start)/(f32)rotate_info.time_l;


			//��������֮��Ĳ�ֵ

			
			f32  angel1=rotate_info.angel;
			
			//return angel1;
			f32  angel2;
			if(i<(size-1))
				angel2=angel_ary[i+1].angel;
			else 
				angel2=angel_ary[0].angel;

			return  angel1*(1-ratio)+angel2*ratio;


		}

	}

	//�Ҳ����������һ����ת��
	 return angel_ary[size-1].angel;


}




//�õ���������
void CBillBoardAmt::SetScaleAry( core::array<SCALE_INFO>scale_ary_ )
{
        
	scale_ary=scale_ary_;

}

float CBillBoardAmt::GetScale( int time_current )
{

	if(scale_ary.size()==0)return 1;

	
	int size=scale_ary.size();

	if(time_current>time_length && draw_model!=-1)
		return scale_ary[size-1].scale;


	int time_now=time_current%time_length;        //��ȡ��ǰʱ�䣬Ӧ��ѭ�������

	for(int i=0;i<size;i++)
	{

		SCALE_INFO scale_info=scale_ary[i];
		if(scale_info.time_start<time_now&&(scale_info.time_start+scale_info.time_l)>=time_now)
		{

			f32 ratio=(f32)(time_now-scale_info.time_start)/(f32)scale_info.time_l;


			//��������֮��Ĳ�ֵ


			f32  scale1=scale_info.scale;

			//return angel1;
			f32  scale2;
			if(i<(size-1))
				scale2=scale_ary[i+1].scale;
			else 
				scale2=scale_ary[0].scale;

			return  scale1*(1-ratio)+scale2*ratio;


		}

	}

	//�Ҳ����������һ������
	return scale_ary[size-1].scale;




}

void CBillBoardAmt::SetOri( ORI ori )
{
       billboard.SetOri(ori);
}

void CBillBoardAmt::setDim( int width,f32 ratio )
{
    
	billboard.setDim(width,ratio);

}

void CBillBoardAmt::setOrgAngel( f32 ori_angel )
{
  
    billboard.SetOrgAngle(ori_angel);

}

void CBillBoardAmt::SetOffsetAry( core::array<OFFSET_INFO>offset_ary_ )
{
    
	offset_ary=offset_ary_;

}

core::position2di CBillBoardAmt::GetOffset( int time_current )
{
 
	if(offset_ary.size()==0)return position2di(0,0);

	if(time_current>time_length && draw_model!=-1)
		return position2di(0,0);

	int size=offset_ary.size();

	int time_now=time_current%time_length;        //��ȡ��ǰʱ�䣬Ӧ��ѭ�������

	for(int i=0;i<size;i++)
	{

		OFFSET_INFO offset_info=offset_ary[i];
		if(offset_info.time_start<time_now&&(offset_info.time_start+offset_info.time_l)>=time_now)
		{

			f32 ratio=(f32)(time_now-offset_info.time_start)/(f32)offset_info.time_l;


			//��������֮��Ĳ�ֵ

            int x1,y1,x2,y2;

			x1=offset_info.pos.x;
			y1=offset_info.pos.y;

			if(i<(size-1))
			{
				x2=offset_ary[i+1].pos.x;
				y2=offset_ary[i+1].pos.y;
			
			}
			else 
			{
				x2=offset_ary[0].pos.x;
				y2=offset_ary[0].pos.y;
			
			}

			return  position2di((1-ratio)*x1+ratio*x2,(1-ratio)*y1+ratio*y2);


		}

	}

	//�Ҳ����������һ������
	return offset_ary[size-1].pos;


}

void CBillBoardAmt::setUV( float lt_u,float lt_v,float lb_u,float lb_v,float rt_u,float rt_v,float rb_u,float rb_v )
{


    billboard.SetUV(lt_u,lt_v,lb_u,lb_v,rt_u,rt_v,rb_u,rb_v);



}

void CBillBoardAmt::setUVSpeed( float u_speed,float v_speed )
{
        billboard.SetUVSpeed(u_speed,v_speed);
}




void CBillBoardAmt::setTextureAlpha( ITexture * texture_a )
{
      billboard.SetTextureAlpha(texture_a);

}




void CBillBoardAmt::SetMixType( ENUM_BLEND_FACTOR src_blend_,ENUM_BLEND_FACTOR dst_blend_ )
{

	 billboard.SetMixType(src_blend_,dst_blend_);

}

void CBillBoardAmt::SetAlphaSource( u32 alphasource )
{

	billboard.SetAlphaSource(alphasource);

}

void CBillBoardAmt::SetModulate( ENUM_MODULATE modufun_ )
{
    billboard.SetModulate(modufun_);

}



// //����ͼ��غ���
// void  CBillBoardAmt::save(io::IXMLWriter *XmlWriter)
// {
// 
// 
// 	  XmlWriter->writeXMLHeader();  //д��ͷ
// 
//       XmlWriter->writeElement(L"SpeEffect");
// 
//       XmlWriter->writeElement(L"EffectType",true,L"effectType",L"1");
// 	  
// 	  XmlWriter->writeComment(L"����屣���ļ�");
// 	  XmlWriter->writeElement(L"BillBoardAmt");
//       
// 	  //���ȵ��þ�̬������
// 	  billboard.save(XmlWriter);
// 
// 
// 	  //д�붯̬ʱ��
// 	  XmlWriter->writeComment(L"д�붯̬���ƵĻ�����Ϣ");
// 	  XmlWriter->writeElement(L"AmtInfo",true,L"timeLength",Int2W_char(time_length).c_str(),L"drawMode",Int2W_char(draw_model).c_str());
// 
//        
// 	  //д�붯̬����
// 	  XmlWriter->writeComment(L"����嶯̬��������");
// 	  
// 	  XmlWriter->writeElement(L"Amt");
// 
// 	  
// 	  //д����ɫ����
// 	  XmlWriter->writeComment(L"��ɫ�仯����");
// 	  XmlWriter->writeElement(L"ColorAry",false,L"size",Int2W_char(color_ary.size()).c_str());
// 	  for(int i=0;i<color_ary.size();i++)
// 	  {
// 		  COLOR_INFO colorInfo=color_ary[i];
// 		  //д�� ��ɫֵ����ʼʱ�䣬ʱ�䳤��
// 		  XmlWriter->writeElement(L"ColorInfo",true,L"colorRef",Int2W_char((int)colorInfo.col).c_str(),L"startTime",Int2W_char(colorInfo.time_start).c_str(),L"timeLength",Int2W_char(\
// 			  colorInfo.time_l).c_str());
// 		  
// 
// 	  
// 	  }
// 	  XmlWriter->writeClosingTag(L"ColorAry");
// 
//      
// 	  //д��Alpha����  
// 	  XmlWriter->writeComment(L"Alpha�仯����");
// 	  XmlWriter->writeElement(L"AlphaAry",false,L"size",Int2W_char(alpha_ary.size()).c_str());
// 	  for(int i=0;i<alpha_ary.size();i++)
// 	  {
// 		  ALPHA_INFO alphaInfo=alpha_ary[i];
// 		  //д�� ��ɫֵ����ʼʱ�䣬ʱ�䳤��
// 		  XmlWriter->writeElement(L"AlphaInfo",true,L"alpha",Int2W_char(alphaInfo.alpha).c_str(),L"startTime",Int2W_char(alphaInfo.time_start).c_str(),L"timeLength",Int2W_char(\
// 			  alphaInfo.time_l).c_str());
// 		  
// 
// 	  
// 	  }
// 	  XmlWriter->writeClosingTag(L"AlphaAry");
// 
// 
// 
// 
// 	  //д����ת����
// 	  XmlWriter->writeComment(L"��ת�仯����");
//       XmlWriter->writeElement(L"AngleAry",false,L"size",Int2W_char(angel_ary.size()).c_str());
// 	  for(int i=0;i<angel_ary.size();i++)
// 	  {
// 		  ROTATE_INFO angleInfo=angel_ary[i];
// 		  //д�� ��ɫֵ����ʼʱ�䣬ʱ�䳤��
// 		  XmlWriter->writeElement(L"AngleInfo",true,L"angle",Float2W_char(angleInfo.angel).c_str(),L"startTime",Int2W_char(angleInfo.time_start).c_str(),L"timeLength",Int2W_char(\
// 			  angleInfo.time_l).c_str());
// 		  
// 
// 	  
// 	  }
// 	  XmlWriter->writeClosingTag(L"AngleAry");
// 
// 
// 	  //д���������
// 	  XmlWriter->writeComment(L"�����仯����");
// 	  XmlWriter->writeElement(L"ScaleAry",false,L"size",Int2W_char(scale_ary.size()).c_str());
// 	  for(int i=0;i<scale_ary.size();i++)
// 	  {
// 		  SCALE_INFO scaleInfo=scale_ary[i];
// 		  //д�� ��ɫֵ����ʼʱ�䣬ʱ�䳤��
// 		  XmlWriter->writeElement(L"ScaleInfo",true,L"scale",Float2W_char(scaleInfo.scale).c_str(),L"startTime",Int2W_char(scaleInfo.time_start).c_str(),L"timeLength",Int2W_char(\
// 			  scaleInfo.time_l).c_str());
// 		  
// 
// 	  
// 	  }
// 	  XmlWriter->writeClosingTag(L"ScaleAry");
// 
// 
//      
// 	  //�ƶ����鲻д�룬������Ϊ��Чʱд�� 
// 
// 
// 
// 
// 
// 	  XmlWriter->writeClosingTag(L"Amt");
// 
// 
// 	  XmlWriter->writeClosingTag(L"BillBoardAmt");
// 
// 	  XmlWriter->writeClosingTag(L"SpeEffect");
// 
// 
// 
// 
// 
// }
// 
//����

// void  CBillBoardAmt::open(IXMLReader * XmlReader)
// {
// 
//             
//     //���ļ�������BillBoard�ڵ�ʱ�ȵ���Billboard�ļ��غ������ع����ľ�̬����
// 
// 
// 	//��������ǰ����������
// 	
// 	texture_ary.clear();             //������������
// 	uv_ary.clear();                  //����UV��������
// 	scale_ary.clear();               //���������������
// 	offset_ary.clear();              //����λ��ƫ������
// 	angel_ary.clear();               //������ת����
// 	color_ary.clear();               //�����ɫ����
// 	alpha_ary.clear();               //���alphaͨ������
// 
// 
// 
// 
// 
// 	const stringw BillBoard=L"BillBoard";
// 	const stringw AmtInfo = L"AmtInfo";
// 	const stringw ColorAry= L"ColorAry";
// 	const stringw AlphaAry= L"AlphaAry";
//     const stringw AngleAry= L"AngleAry";
//     const stringw ScaleAry= L"ScaleAry";
// 	
// 
// 	int  size;     //��̬����Ĵ�С
// 
// 	while(XmlReader->read())
// 	{
// 
// 
// 		if(XmlReader->getNodeType()==::io::EXN_ELEMENT)
// 		{
// 
// 			 
// 			//���뾲̬�ڵ㣬ת�뾲̬�����Ķ��������
// 			if(BillBoard==XmlReader->getNodeName())
// 			{
// 				billboard.open(XmlReader);
// 	
// 			}
//           
// 
// 			if(AmtInfo==XmlReader->getNodeName())
// 			{
// 
// 				time_length=XmlReader->getAttributeValueAsInt(L"timeLength");
// 				draw_model=XmlReader->getAttributeValueAsInt(L"drawMode");
// 			
// 			}
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 			//���붯̬����  ------��ɫ����
// 		    if(ColorAry==XmlReader->getNodeName())
// 			{
// 
// 				int size=XmlReader->getAttributeValueAsInt(L"size");
// 				if(size==0)
// 				{
// 					//ֱ��������������
// 					COLOR_INFO colorInfo;
// 					colorInfo.col=RGB(255,255,255);
// 					colorInfo.time_l=time_length;
// 					colorInfo.time_start=0;
// 					color_ary.push_back(colorInfo);
// 
// 					colorInfo.time_start=time_length;
// 				 	colorInfo.time_l=0;
// 					color_ary.push_back(colorInfo);
//                     
//                 			
// 				
// 				}
// 				else
// 					readColorAry(XmlReader);
// 
// 			}
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 		             
// 			 //���붯̬����  ------alpha ����
// 			if(AlphaAry==XmlReader->getNodeName())
// 			 {
// 
// 				int size=XmlReader->getAttributeValueAsInt(L"size");
// 				if(size==0)
// 				{
// 					//ֱ��������������
// 					ALPHA_INFO alphaInfo;
// 					alphaInfo.alpha=255;
// 					alphaInfo.time_l=time_length;
// 					alphaInfo.time_start=0;
// 					alpha_ary.push_back(alphaInfo);
// 
// 					alphaInfo.time_start=time_length;
// 				 	alphaInfo.time_l=0;
// 				    alpha_ary.push_back(alphaInfo);
//                     
//                 			
// 				
// 				}
// 				else
// 					readAlphaAry(XmlReader);
// 			
// 			}
// 		
// 
// 				
// 
// 
// 		    //���붯̬����  ------��ת ����
// 			if(AngleAry==XmlReader->getNodeName())
// 			 {
// 
// 				int size=XmlReader->getAttributeValueAsInt(L"size");
// 				if(size==0)
// 				{
// 					//ֱ��������������
// 					ROTATE_INFO rotateInfo;
// 					rotateInfo.angel=0;
// 					rotateInfo.time_l=time_length;
// 					rotateInfo.time_start=0;
// 					angel_ary.push_back(rotateInfo);
// 
// 					rotateInfo.time_start=time_length;
// 				 	rotateInfo.time_l=0;
// 				    angel_ary.push_back(rotateInfo);
//                     
//                 			
// 				
// 				}
// 				else
// 					readAngleAry(XmlReader);
// 			
// 			}
// 
// 
// 
// 
// 
// 			//���붯̬���� ----------��������
// 			if(ScaleAry==XmlReader->getNodeName())
// 			 {
// 
// 				int size=XmlReader->getAttributeValueAsInt(L"size");
// 				if(size==0)
// 				{
// 					//ֱ��������������
// 					SCALE_INFO scaleInfo;
// 					scaleInfo.scale=1;
// 					scaleInfo.time_l=time_length;
// 					scaleInfo.time_start=0;
// 					scale_ary.push_back(scaleInfo);
// 
// 					scaleInfo.time_start=time_length;
// 				 	scaleInfo.time_l=0;
// 				    scale_ary.push_back(scaleInfo);
//                     
//                 			
// 				
// 				}
// 				else
// 					readScaleAry(XmlReader);
// 			
// 			}
// 
// 
// 
// // / 	
// 		
// 		
// 		}
// 	
// 	
// 	
// 	 	
// 	
// 	
// 	
// 	
// 	}
// 
// 
// 
// 
// }
// 
// 
// 



//����ɫ����
// void  CBillBoardAmt::readColorAry(io::IXMLReader *XmlReader)
// {
// 
// 	const stringw ColorInfo= L"ColorInfo";
// 	const stringw ColorAry= L"ColorAry";
// 
// 
// 
// 	while(XmlReader->read())
// 	{
// 	
// 		if(XmlReader->getNodeType()==::io::EXN_ELEMENT)
// 		{
//               
//             if(ColorInfo==XmlReader->getNodeName())
// 			{
// 				COLOR_INFO colorInfo;
// 				colorInfo.col=(COLORREF)(XmlReader->getAttributeValueAsInt(L"colorRef"));
// 				colorInfo.time_start=XmlReader->getAttributeValueAsInt(L"startTime");
// 				colorInfo.time_l=XmlReader->getAttributeValueAsInt(L"timeLength");
// 				color_ary.push_back(colorInfo);
// 			
// 			}
// 
// 
// 		
// 
// 
// 		}
// 
// 
// 		if(XmlReader->getNodeType()== ::io::EXN_ELEMENT_END&&ColorAry==XmlReader->getNodeName())
//         
// 			return;
// 	
// 	
// 	
// 	}
// 
// 
// 
// }
// 
// 
//��Alpha����
// void  CBillBoardAmt::readAlphaAry(IXMLReader * XmlReader)
// {
// 
// 
// 
// 
// 	const stringw AlphaInfo= L"AlphaInfo";
// 	const stringw AlphaAry= L"AlphaAry";
// 
// 
// 
// 	while(XmlReader->read())
// 	{
// 	
// 		if(XmlReader->getNodeType()==::io::EXN_ELEMENT)
// 		{
//               
//             if(AlphaInfo==XmlReader->getNodeName())
// 			{
// 				ALPHA_INFO alphaInfo;
// 				alphaInfo.alpha=(XmlReader->getAttributeValueAsInt(L"alpha"));
// 				alphaInfo.time_start=XmlReader->getAttributeValueAsInt(L"startTime");
// 				alphaInfo.time_l=XmlReader->getAttributeValueAsInt(L"timeLength");
// 				alpha_ary.push_back(alphaInfo);
// 			
// 			}
// 
// 	
// 
// 		}
// 
// 		if(XmlReader->getNodeType()== ::io::EXN_ELEMENT_END&&AlphaAry==XmlReader->getNodeName())
//         
// 			return;
// 	
// 	
// 	}
// 
// 
// 
// 
// 
// }
// 
// 
// 
//������ת����
// void  CBillBoardAmt::readAngleAry(io::IXMLReader *XmlReader)
// {
// 	 
// 
// 
// 	const stringw AngleInfo= L"AngleInfo";
// 	const stringw AngleAry= L"AngleAry";
// 
// 
// 
// 	while(XmlReader->read())
// 	{
// 	
// 		if(XmlReader->getNodeType()==::io::EXN_ELEMENT)
// 		{
//               
//             if(AngleInfo==XmlReader->getNodeName())
// 			{
// 				ROTATE_INFO rotateInfo;
// 				rotateInfo.angel=(XmlReader->getAttributeValueAsFloat(L"angle"));
// 				rotateInfo.time_start=XmlReader->getAttributeValueAsInt(L"startTime");
// 				rotateInfo.time_l=XmlReader->getAttributeValueAsInt(L"timeLength");
// 				angel_ary.push_back(rotateInfo);
// 			
// 			}
// 
// 	
// 
// 		}
// 
// 		if(XmlReader->getNodeType()== ::io::EXN_ELEMENT_END&&AngleAry==XmlReader->getNodeName())
//         
// 			return;
// 	
// 	
// 	}
// 
// 
// 
// 
// }
// 
// 
// 
// 
// 
//�����������
// void  CBillBoardAmt::readScaleAry(IXMLReader * XmlReader)
// {
// 
//     const stringw ScaleInfo= L"ScaleInfo";
// 	const stringw ScaleAry= L"ScaleAry";
// 
// 
// 
// 	while(XmlReader->read())
// 	{
// 	
// 		if(XmlReader->getNodeType()==::io::EXN_ELEMENT)
// 		{
//               
//             if(ScaleInfo==XmlReader->getNodeName())
// 			{
// 				SCALE_INFO scaleInfo;
// 				scaleInfo.scale =(XmlReader->getAttributeValueAsFloat(L"scale"));
// 				scaleInfo.time_start=XmlReader->getAttributeValueAsInt(L"startTime");
// 				scaleInfo.time_l=XmlReader->getAttributeValueAsInt(L"timeLength");
// 				scale_ary.push_back(scaleInfo);
// 			
// 			}
// 
// 	
// 
// 		}
// 
// 		if(XmlReader->getNodeType()== ::io::EXN_ELEMENT_END&&ScaleAry==XmlReader->getNodeName())
//         
// 			return;
// 	
// 	
// 	}
// 
// 
// 
// 
// 
// 
// 
// 
// }
// 
// 
//��������
stringw  CBillBoardAmt::Int2W_char(int data)
{
//        CString s;
// 	   s.Format(L"%d",data);
//        core::stringw s_data=s.GetString();
// 	   return s_data;

	return L"";

}

//��������
stringw CBillBoardAmt::Float2W_char(float data)
{

//        CString s;
// 	   s.Format(L"%0.4f",data);
//        core::stringw s_data=s.GetString();
// 	   return s_data;

	return L"";

}

void CBillBoardAmt::load( IReadStream * FileReader )
{
    


	//��������ǰ����������

	texture_ary.clear();             //������������
	uv_ary.clear();                  //����UV��������
	scale_ary.clear();               //���������������
	offset_ary.clear();              //����λ��ƫ������
	angel_ary.clear();               //������ת����
	color_ary.clear();               //�����ɫ����
	alpha_ary.clear();               //���alphaͨ������



	billboard.load(FileReader);

	//����ʱ��ͻ���ģʽ
	time_length=FileReader->readInt();
	draw_model=FileReader->readInt();



	int  size=0;
	//////////////////////////////////////////////////////////////////////////\
	������̬��������                                                           \
	��ɫ����
	size=FileReader->readInt();
	for(int i=0;i<size;i++)
	{
		COLOR_INFO colorInfo;
		colorInfo.col=FileReader->readInt();
		colorInfo.time_start=FileReader->readInt();
		colorInfo.time_l=FileReader->readInt();

		color_ary.push_back(colorInfo);


	}




	//����alpha����
	size=FileReader->readInt();
	for (int i=0;i<size;i++)
	{
		ALPHA_INFO alphaInfo;
		alphaInfo.alpha=FileReader->readInt();
		alphaInfo.time_start=FileReader->readInt();
		alphaInfo.time_l=FileReader->readInt();

		alpha_ary.push_back(alphaInfo);
	}





	//������ת����
	size=FileReader->readInt();
	for (int i=0;i<size;i++)
	{

		ROTATE_INFO rotateInfo;
		rotateInfo.angel=FileReader->readFloat();
		rotateInfo.time_start=FileReader->readInt();
		rotateInfo.time_l=FileReader->readInt();

		angel_ary.push_back(rotateInfo);
	}




	//�����������
	size=FileReader->readInt();
	for (int i=0;i<size;i++)
	{
		SCALE_INFO scaleInfo;
		scaleInfo.scale=FileReader->readFloat();
		scaleInfo.time_start=FileReader->readInt();
		scaleInfo.time_l=FileReader->readInt();

		scale_ary.push_back(scaleInfo);
	}






}

void CBillBoardAmt::setZ( int Z )
{
	billboard.setZ(Z);

}

