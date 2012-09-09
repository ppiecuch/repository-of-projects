
#include "BillBoard.h"
#include "math.h"

CBillBoard::CBillBoard(void)
{

   driver=NULL;
   p_material=NULL;
   p_texture=NULL;
   p_texture_alpha=NULL;

   

 
}

CBillBoard::~CBillBoard(void)
{
	if(p_material)
		p_material->drop();
}

void CBillBoard::init()
{
       
	   angle=0;                                   //角度变化
	   ori=ORI_CAMERA_2D;                         //默认朝向镜头
       is_defined=true;                           //默认为自定义uv动画
	   uv_animation=UV_L_TO_R;                    //无动画
	   u_speed=0;
	   v_speed=0;
	   width=100;                                 //宽度 100
	   ratio=1.0;                                 //长宽比例 1:1
	   raduis=sqrt(1+ratio*ratio)*width;          //最小半径
	 
	   //索引初始化
	   indices[0]=0;
	   indices[1]=3;
	   indices[2]=1;
	   indices[3]=1;
	   indices[4]=3;
	   indices[5]=2;




       offset.set(0,0);                          //偏移量为 00
       scale=1;                                  //放缩比例为1
       
       
         
	   //设置纹理坐标
	   or_tord[0].set(1,0);
	   or_tord[1].set(0,0);
	   or_tord[2].set(0,1);
	   or_tord[3].set(1,1);

       
	   org_angle=0.5*PI-atan(ratio);
	 
	   start_angle=org_angle;

	 //  TRACE(L"org_angle %f0.2\n",org_angle);


	   col=video::COLOR_WHITE;


	   mix_type=ENUM_MATERIAL_TYPE_BLEND;

	   src_blend=ENUM_BLEND_FACTOR_ONE;

	   dst_blend=ENUM_BLEND_FACTOR_ZERO;

	   modufun=ENUM_MODULATE_1X;

	   alphasource=ENUM_ALPHA_SOURCE_VERTEX;              //默认是纹理的混合


}


void CBillBoard::update_vec()
{
     
    //将元数据变化后赋给顶点数组
       
	//得到旋转之后的数据
	
	vec[0].pos.set((pos.x+offset.x)+raduis*cos(start_angle+angle)*scale,(pos.y+offset.y)-raduis*sin(start_angle+angle)*scale,0);
	vec[1].pos.set((pos.x+offset.x)+raduis*cos(PI-org_angle+start_angle-org_angle+angle)*scale,(pos.y+offset.y)-raduis*sin(PI-org_angle+start_angle-org_angle+angle)*scale,0);
	vec[2].pos.set((pos.x+offset.x)*2-vec[0].pos.x,(pos.y+offset.y)*2-vec[0].pos.y,0);
	vec[3].pos.set((pos.x+offset.x)*2-vec[1].pos.x,(pos.y+offset.y)*2-vec[1].pos.y,0);

   
	if(ori==ORI_FLOORD_2D)
	{
		vec[0].pos.y*=0.70f;
		vec[1].pos.y*=0.70f;
		vec[2].pos.y*=0.70f;
		vec[3].pos.y*=0.70f;

		vec[0].pos.y+=0.3*(pos.y+offset.y);
		vec[1].pos.y+=0.3*(pos.y+offset.y);
		vec[2].pos.y+=0.3*(pos.y+offset.y);
		vec[3].pos.y+=0.3*(pos.y+offset.y);

		//做移动偏移
	
	}



	if (ori==ORI_PERSON_2D)
	{
      
		vec[0].pos.x*=0.70f;
		vec[1].pos.x*=0.70f;
		vec[2].pos.x*=0.70f;
		vec[3].pos.x*=0.70f;

		vec[0].pos.x+=0.3*(pos.x+offset.x);
		vec[1].pos.x+=0.3*(pos.x+offset.x);
		vec[2].pos.x+=0.3*(pos.x+offset.x);
		vec[3].pos.x+=0.3*(pos.x+offset.x);

        

	}

   
	
		
	 or_tord[0].set(or_tord[0].x+u_speed,or_tord[0].y+v_speed);
	 or_tord[1].set(or_tord[1].x+u_speed,or_tord[1].y+v_speed);
	 or_tord[2].set(or_tord[2].x+u_speed,or_tord[2].y+v_speed);
	 or_tord[3].set(or_tord[3].x+u_speed,or_tord[3].y+v_speed);




     vec[0].texcoords.set(or_tord[0].x,or_tord[0].y);
	 vec[1].texcoords.set(or_tord[1].x,or_tord[1].y);
	 vec[2].texcoords.set(or_tord[2].x,or_tord[2].y);
	 vec[3].texcoords.set(or_tord[3].x,or_tord[3].y);


	 vec[0].color=col;
	 vec[1].color=col;
	 vec[2].color=col;
	 vec[3].color=col;




}



void CBillBoard::render()
{


	if(driver==NULL)return;

	/**************************
	if(p_texture!=NULL)
	{
		//混合固定通道
		if(p_texture_alpha!=NULL&&mix_type==ENUM_MATERIAL_TYPE_BLEND)
			MixAlphaToRGB(p_texture,p_texture_alpha);
        

	}
    *********************///////
   
   

	//driver->draw2DVertexPrimitiveList(vec,4,indices,2, irr::video::EVT_STANDARD, irr::scene::EPT_TRIANGLES);

#if 0
	if(p_material)
	{
		if(p_texture)
		 p_material->setTexture(0,p_texture);

	    p_material->setMaterialType(mix_type);
		p_material->setBlendSrcFactor(src_blend);
		p_material->setBlendDstFactor(dst_blend);
		p_material->setAlphaSource((ENUM_ALPHA_SOURCE)alphasource);
		//p_material->states.CullFace=false;
		p_material->setModulate(modufun);

		driver->setMaterial(p_material);
	
	}

	driver->setTransform(ENUM_TRANSFORM_WORLD,IDENTITY_MATRIX);
	driver->drawVertexPrimitiveList(vec,4,indices,6,ENUM_PRIMITIVE_TYPE_TRIANGLES,ENUM_VERTEX_TYPE_3V1T1C);

#else
	if(p_material)
	{
		if(p_texture)
			p_material->setTexture(0,p_texture);

		p_material->setMaterialType(mix_type);
		p_material->setBlendSrcFactor(src_blend);
		p_material->setBlendDstFactor(dst_blend);
		p_material->setAlphaSource((ENUM_ALPHA_SOURCE)alphasource);
		//p_material->states.CullFace=false;
		p_material->setModulate(modufun);
	}

	gfAdapter->drawVertexPrimitiveList(p_material,vec,4,indices,6,ENUM_VERTEX_TYPE_3V1T1C);

#endif
  

}

//设置驱动器，应可忽略
void CBillBoard::SetDriver( IVideoDriver * drv )
{
    driver=drv;

	if(driver)
		p_material=driver->createMaterial();

}

//设置纹理
void CBillBoard::SetTexture( ITexture * p_tex )
{
    p_texture=p_tex;
}

//设置播放位置和得到播放位置
void CBillBoard::setPos( position2di origin_ )
{
     pos=origin_;

	

}


position2di CBillBoard::getPos()
{

	return  pos;
}

//设置长宽，大小
void CBillBoard::setDim( int width_,f32 ratio_/*=0*/ )
{
     
	 width=width_;
	 ratio=ratio_;

	 raduis=sqrt(1+ratio*ratio)*width;          //最小半径


    f32 temp_angel=org_angle;

    org_angle=0.5*PI-atan(ratio);
	
	start_angle=org_angle+start_angle-temp_angel;


}
//设置颜色
void CBillBoard::setColor( SColor col_ )
{

      col=col_;

}

//设置当前旋转的角度，用于控制公告板的旋转
void CBillBoard::SetAngle( f32 angel,int flag/*=0*/ )
{

       this->angle+=angel;          
	   if(this->angle>2*PI)
		   this->angle-=2*PI;

}

//设置放缩
void CBillBoard::SetScale( f32 scale_ )
{ 
	scale=scale_;
}

//设置朝向
void CBillBoard::SetOri( ORI ori_ )
{
     
	ori=ori_;

}

//设置初始角度
void CBillBoard::SetOrgAngle( f32 org_angle_ )
{

	start_angle=org_angle+org_angle_;

}

//设置位移，用于测试公告板的移动
void CBillBoard::SetOffset( position2di offset_ )
{
  
	offset=offset_;

}

//设置公告板的UV
void CBillBoard::SetUV( float lt_u,float lt_v,float lb_u,float lb_v,float rt_u,float rt_v,float rb_u,float rb_v )
{
  

	 or_tord[0].set(rt_u,rt_v);
	 or_tord[1].set(lt_u,lt_v);
	 or_tord[2].set(lb_u,lb_v);
	 or_tord[3].set(rb_u,rb_v);



}

//设置UV流动速度
void CBillBoard::SetUVSpeed( float u_speed_,float v_speed_ )
{
    

	u_speed=u_speed_;

	v_speed=v_speed_;



}


//设置通道纹理贴图

void CBillBoard::SetTextureAlpha( ITexture * p_texture_alpha_ )
{     
	p_texture_alpha=p_texture_alpha_;

}


//用于通道纹理和纹理贴图的混合，暂不开放。
void CBillBoard::MixAlphaToRGB( ITexture * RGB_,ITexture * Alpha_ )
{


/*
	u32 *p = (u32*)RGB_->lock();
	u32 *p_a=(u32*)Alpha_->lock();
	core::dimension2d<u32>  dim = RGB_->getSize();
	core::dimension2d<u32>  dim_a = Alpha_->getSize();

	float w=dim.w;      //宽
	float h=dim.h;     //高

	u32 pitch = RGB_->getPitch()  / 4;
	u32 pitch_a=Alpha_->getPitch() /4;

	const u32 pixels = pitch * dim.Height;
	const u32 pixels_a = pitch_a * dim_a.Height;


	float r_s2b_w=float(dim_a.Width)/float(dim.Width);
	float r_s2b_h=float(dim_a.Height)/float(dim.Height);

	//
	float re_h=1.0/float(dim.Height);
	float re_w=1.0/float(dim.Width);
	
	float  start_x=or_tord[1].X*dim.Width;
	float  start_y=or_tord[1].Y*dim.Height;


	int  dim_a_b=(dim_a.Height-1)*dim_a.Width;


     
    if(start_x>dim.Width)
	while(start_x>dim.Width)
		  start_x-=dim.Width;

	if(start_x<0)
		while(start_x<0)
			start_x+=dim.Width;



	if(start_y>dim.Height)
		while(start_y>dim.Height)
			start_y-=dim.Height;

	if(start_y<0)
		while(start_y<0)
			start_y+=dim.Height;



	for(int pixel=0;pixel<pixels;pixel++)
	{

		//求出像素对应的宽或高的比例，减去uv的初始值

		float  w_ratio;
		float  h_ratio;

	
	
		int  x=pixel%dim.Width;
		int  y=pixel/dim.Width;
		
		/*float x_ratio=float(x)*re_w;           //水平比例
		float y_ratio=float(y)*re_h;           //竖直比例

		x_ratio-=start_x;
		y_ratio-=start_y;

		if(x_ratio>0)
		while(x_ratio>1)
			x_ratio-=1;
		if(x_ratio<0)
		while(x_ratio<0)
	    x_ratio+=1;

		if(y_ratio>0)
		while(y_ratio>1)
			y_ratio-=1;
		if(y_ratio<0)
		while(y_ratio<0)
		   y_ratio+=1;


        u32 a=p_a[int(x_ratio*dim_a.Width+y_ratio*dim_a.Height*dim_a.Width)];
       */ 

		//求出像素差

	/*	int x_dis=x-start_x;
		int y_dis=y-start_y;

		if(x_dis<0)
				x_dis+=dim.Width;

		if(y_dis<0)
			y_dis+=dim.Height;

          
		int    test_x=x_dis*r_s2b_w;
		int    test_y=y_dis*r_s2b_h;
		test_y *=dim_a.Width;


//          
		//取平均值
        int p_l=0;
		if(test_x>0)
			p_l=test_x+test_y-1;
		else
			p_l=dim_a.Width-1+test_y;


		int p_r=0;
		if(test_x<dim_a.Width-1)
			p_r=test_x+test_y+1;
		else
			p_r=0+test_y;

	
		int p_t=0;
		if(test_y>0)
			p_t=test_x+test_y-dim_a.Width;
		else
			p_t=dim_a_b+test_x;


		int p_b=0;
		if(test_y+1<dim_a.Height)
			p_b=test_x+test_y+dim_a.Width;
		else
			p_b=test_x;

// 
	//	u32 a=((p_a[p_l]+p_a[p_r]+p_a[p_t]+p_a[p_b])*0.05+p_a[test_x+test_y]*0.8);   //getAlpha(p_a,dim_a,w_ratio,h_ratio);
// 
//      

      
		u32 a=p_a[test_x+test_y];

// 		u32 a1=MixU32(p_a[p_l],p_a[p_r]);
// 		u32 a2=MixU32(p_a[p_t],p_a[p_b]);
// 
// 		u32 a=MixU32(a1,a2);
// 
// 		a=MixU32(a,p_a[test_x+test_y]);

		a&=0x000000ff;              //
		a<<=24;      
		(*p)&=0x00ffffff;
		(*p)|=a;
		p++;


	}


	Alpha_->unlock();
	RGB_->unlock();

*/



}



void CBillBoard::SetMixType( ENUM_BLEND_FACTOR src_blend_,ENUM_BLEND_FACTOR dst_blend_ )
{
     
	 src_blend=src_blend_;
	 dst_blend=dst_blend_;


}

//设置混合类型 ^^^^^^^
void CBillBoard::SetAlphaSource( u32 alphasource )
{
     
	this->alphasource=alphasource;

}

//设置Alpha来源 ^^^^^^^^^^^^
//设置相乘系数 
void CBillBoard::SetModulate( ENUM_MODULATE  modufun_ )
{
	
	modufun=modufun_;

}




stringw CBillBoard::Float2W_char(float data)
{
     /*  CString s;
	   s.Format(L"%0.4f",data);
       core::stringw s_data=s.GetString();
	   return s_data;
     */
	return L"";

}

stringw  CBillBoard::Int2W_char(int data)
{
      /*
	   CString s;
	   s.Format(L"%d",data);
       core::stringw s_data=s.GetString();
	   return s_data;

	  */
	return L"";
	  



}


//文件读写器
void CBillBoard::load( IReadStream  * FileReader )
{



	angle=0;  

	//读入混合参数
	src_blend=(ENUM_BLEND_FACTOR)FileReader->readInt(); 
	dst_blend=(ENUM_BLEND_FACTOR)FileReader->readInt(); 
	modufun=(ENUM_MODULATE)FileReader->readInt();
	alphasource=FileReader->readInt();

	//读入UV速度
	u_speed=FileReader->readFloat();
	v_speed=FileReader->readFloat();

	//读入宽，长宽比
	width=FileReader->readInt();
	ratio=FileReader->readFloat();

	//读取位置和朝向
	pos.x=FileReader->readInt();
	pos.y=FileReader->readInt();

	ori=(ORI)FileReader->readInt();


	//四个初始化纹理坐标
	or_tord[0].x=FileReader->readFloat();
	or_tord[0].y=FileReader->readFloat();
	or_tord[1].x=FileReader->readFloat();
	or_tord[1].y=FileReader->readFloat();
	or_tord[2].x=FileReader->readFloat();
	or_tord[2].y=FileReader->readFloat();
	or_tord[3].x=FileReader->readFloat();
	or_tord[3].y=FileReader->readFloat();


	start_angle=FileReader->readFloat();


	//读取纹理
	char *chs=NULL;
	int length=FileReader->readInt();
	if(length!=0)
	{


		length=1000;
		chs=new char[length+1];

		for(int i=0;i<length+1;i++)
		{        
			chs[i]=FileReader->readByte();

			if(chs[i]=='\0')
				break;             

		}
		//chs[length]='\0';



	}

	//设置初始化



	//求出长和最小半径
	raduis=sqrt(1+ratio*ratio)*width;          //最小半径

	//初始化索引
	indices[0]=0;
	indices[1]=3;
	indices[2]=1;
	indices[3]=1;
	indices[4]=3;
	indices[5]=2;

	offset.set(0,0);                          //偏移量为 00
	scale=1;                                  //放缩比例为1

	org_angle=0.5*PI-atan(ratio);

	col=video::COLOR_WHITE;
	mix_type=ENUM_MATERIAL_TYPE_BLEND;


	//求出纹理路径并加载

	if(chs)
	{
		
		
		stringc filePath="resource/";
		stringc fileName=chs;
		filePath=filePath+fileName;

		SetTexture(driver->getTexture(filePath));

		delete []chs;



	}





}

void CBillBoard::setZ( int Z )
{
	for (int i=0;i<4;i++)
	  vec[i].pos.z=Z;

}
