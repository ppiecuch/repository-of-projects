#include "ParticleSystem.h"

CParticleSystem::CParticleSystem(void)
{
	emiter=NULL;
	driver=NULL;
	lastEmitTime=0;

	//设置2D材质
	src_fator=ENUM_BLEND_FACTOR_ONE;
	dst_fator=ENUM_BLEND_FACTOR_ZERO;
	alphasource=1;
	modufun=ENUM_MODULATE_1X;
	timeToLive=-1000;
	//p_material=NULL;
	
	m_emiter_type=0;
	
	followMode=false;


}


CParticleSystem::~CParticleSystem(void)
{
	//if(p_material)p_material->drop();

	if(emiter)
	{
		delete emiter;
		emiter=NULL;
	}
	
}


void CParticleSystem::setEmiter( CParticleEmiter * emiter_ )
{
    if(emiter==emiter_)return;
	if(emiter!=NULL)
	{   
        //保留原先的的数据
	   
		*emiter_=*emiter;
		delete emiter;
		emiter=NULL;
	
	}
	emiter=emiter_;
}



void CParticleSystem::setDriver( IVideoDriver * driver_ )
{
	driver=driver_;
		
	//if(driver)
	//		p_material=driver->createMaterial();

 
}

void CParticleSystem::setTexture( c8 * path_ )
{

	if(driver==NULL)return;
 
    //p_material->setTexture(0,driver->getTexture(path_));
	material.setTexture(0,driver->getTexture(path_));



}

void CParticleSystem::update( int time_ )
{
	
	if(emiter==NULL)return;
    

	/*if(lastEmitTime == 0)
	{
		lastEmitTime=time_;
		return  ;
	}
  */

	//u32 now = time_;
	u32 timediff = time_;
	//lastEmitTime = time_;


	int maxParticleCount=emiter->maxParticleCount;
	//更新粒子集 
	if((emiter->emiteLife>0||emiter->emiteAlways)&&(maxParticleCount>Particles.size()||maxParticleCount<=0))
	{
	
	CSparticle * ary=0;
	int newparticles=emiter->emit(time_,timediff,ary);

	if (newparticles && ary)
	{
		s32 j=Particles.size();
		if (newparticles > 16250-j)
			newparticles=16250-j;
		Particles.set_used(j+newparticles);
		for (s32 i=j; i<j+newparticles; ++i)
		{
			Particles[i]=ary[i-j];
		}

	}

	}
	
	if(emiter->emiteLife>0)
		emiter->emiteLife-=timediff;


	//根据速度改变位置
	for (int i=0;i<Particles.size();i++)
	{
		

		CSparticle & particle=Particles[i];

		//改变颜色
		if(!emiter->startColorEqualEnd)          //是否改变颜色
		{
		float a=particle.color.getAlpha()+timediff*particle.deltaColor.getAlpha();
		ASSERT_RANGE(a);
     

		float r=particle.color.getRed()+timediff*particle.deltaColor.getRed();
		ASSERT_RANGE(r);
	

		float g=particle.color.getGreen()+timediff*particle.deltaColor.getGreen();
		ASSERT_RANGE(g);
	

		float b=particle.color.getBlue()+timediff*particle.deltaColor.getBlue();
		ASSERT_RANGE(b);
		particle.color.set(a,r,g,b);
		
		}

        //改变大小
		//
		if(!emiter->startEqualEnd)          //是否改变大小
		{
		particle.size.w+=particle.deltaSize.w*timediff;
		particle.size.h+=particle.deltaSize.h*timediff;
		
		}


       
		//利用重力影响加速  其他的诸如  直线加速和 切线加速
		{
		
			position2df gravity=emiter->getGravity();

			position2df pos_offset;     //位移偏向


			//这段时间的总改变速度

			position2df  speed_add;
        
          
			//加上射线加速
			//求出射线位置
			position2df radial=(particle.pos-emiter->pos).normalize();   
			//求出切线
			position2df tangential;
			tangential.set(-radial.y,radial.x);
			
			//射线加速度
			radial*=particle.radialAccel;
			
			//切线加速度
			tangential*=particle.tagentialAccel;

                
			speed_add=gravity+radial+tangential;

			speed_add.y=-speed_add.y;


			speed_add*=timediff*0.001;


			
			particle.direct+=speed_add;
         
		  
				
		   //改变位置
		    particle.pos.set(particle.pos.x+particle.direct.x*timediff*0.001,particle.pos.y-particle.direct.y*timediff*0.001);       //毫秒化成秒单位
		
		
		
		}
		



		



        
		//如果是环形发射器
		
		if(emiter->getType()==2)
		{
			  
			//设置位置变化
            //得到初始位置
			position2df oriPos,newPos;
			float temp_angleCircle=CC_DEGREES_TO_RADIANS(particle.angleCircle);
			oriPos.x=cosf(temp_angleCircle)*particle.radius;
			oriPos.y-=sinf(temp_angleCircle)*particle.radius;

			//旋转变化后
			particle.angleCircle+=particle.deltaRotationCircle*timediff*0.001;
			
			if(!((CCircleEmiter *)emiter)->getStartRadiusEqualEnd())
			particle.radius+=particle.deltaRadius*timediff;

		    temp_angleCircle=CC_DEGREES_TO_RADIANS(particle.angleCircle);

			newPos.x=cosf(temp_angleCircle)*particle.radius;
			newPos.y-=sinf(temp_angleCircle)*particle.radius;

			//		 
			particle.pos+=newPos-oriPos;
		
						
		}







		//改变旋转角度
		if(emiter->rotationMode==ROTATION_NONE&&!emiter->startRotationEqualEnd)
		{
		
			particle.rotation+=particle.deltaRotation*timediff;
		
		
		}


		//朝发射点
		if(emiter->rotationMode==ROTATION_EMITER)
		{
			
			//反正切求出角度
              
			float rotation;
			if(particle.pos.x==emiter->pos.x)
				rotation=0;                    //90度 竖直朝向
			else
			{
			
				float  y1=emiter->pos.y;
				float  y2=particle.pos.y;
				float  x1=emiter->pos.x;
				float  x2=particle.pos.x;
			
			    rotation=atanf((y1-y2)/(x1-x2));

				//转成角度
				rotation=90.f-rotation*180/PI;
                 
				
			
			}


			particle.rotation=rotation;

		}


		//朝发射速度

		if(emiter->rotationMode==ROTATION_SPEED)
		{
			
			//反正切求出角度
              
			float rotation;
			if(particle.direct.x==0)
				rotation=0;                    //90度 竖直朝向
			else
			{
			
			
			
				rotation=atanf(particle.direct.y/particle.direct.x);

				//转成角度
				rotation=90+rotation*180/PI;
                 
				
			
			}


			particle.rotation=rotation;

		}





	}




	//删除寿命已尽的粒子
	for (u32 i=0; i<Particles.size();)
	{
          
		if(Particles[i].timeToLive<0)
			Particles.erase(i);
		else
		{
			Particles[i].timeToLive-=timediff;
				++i;
		}



	}


	recollabuffer();        //重新分配内存，如果原先的缓存太小


	//更新绘制集
	s32 idx = 0;
	for(int i=0;i<Particles.size();i++)
	{
		const CSparticle& particle = Particles[i];
        
		dimension2df size=particle.size;

		//顶点
		//位置加上旋转 为效率算，写两份代码
 
		if(particle.rotation==0)      //没有旋转的情况
		{

		draw_buffer_v[0+idx].pos .set(particle.pos.x-size.w*0.5,particle.pos.y-size.h*0.5,0);
		
	

		draw_buffer_v[1+idx].pos .set(particle.pos.x-size.w*0.5,particle.pos.y+size.h*0.5,0);

		
    	draw_buffer_v[2+idx].pos .set(particle.pos.x+size.w*0.5,particle.pos.y+size.h*0.5,0);

		
	    draw_buffer_v[3+idx].pos .set(particle.pos.x+size.w*0.5,particle.pos.y-size.h*0.5,0);

	
		}

		else         //旋转的情况
		{


		
		float sizeW_2=size.w*0.5;
		float sizeH_2=size.h*0.5;

		float x1 = -sizeW_2;;
		float y1 = -sizeH_2;

		float x2 = sizeW_2;
		float y2 = sizeH_2;
		float x = particle.pos.x;
		float y = particle.pos.y;


        float r = (float)-CC_DEGREES_TO_RADIANS(particle.rotation);
		float cr = cosf(r);
		float sr = sinf(r);
		float ax = x1 * cr - y1 * sr + x;
		float ay = x1 * sr + y1 * cr + y;
		float bx = x2 * cr - y1 * sr + x;
		float by = x2 * sr + y1 * cr + y;
		float cx = x2 * cr - y2 * sr + x;
		float cy = x2 * sr + y2 * cr + y;
		float dx = x1 * cr - y2 * sr + x;
		float dy = x1 * sr + y2 * cr + y;



        draw_buffer_v[0+idx].pos .set(ax,ay,0);
        draw_buffer_v[3+idx].pos .set(bx,by,0);
        draw_buffer_v[2+idx].pos .set(cx,cy,0);
        draw_buffer_v[1+idx].pos .set(dx,dy,0);

		

	
		}
       
		//颜色
		draw_buffer_v[0+idx].color = particle.color.toSColor();
		draw_buffer_v[1+idx].color = particle.color.toSColor();
		draw_buffer_v[2+idx].color = particle.color.toSColor();
		draw_buffer_v[3+idx].color = particle.color.toSColor();
		
		
		idx +=4;



	}
      


	


	
    
}

void CParticleSystem::recollabuffer()
{


	if (Particles.size() * 4 > draw_buffer_v.size() ||
		Particles.size() * 6 > draw_buffer_i.size())
	{
		u32 oldSize = draw_buffer_v.size();
        draw_buffer_v.set_used(Particles.size() * 4);

		u32 i;

		// fill remaining vertices
		for (i=oldSize; i<draw_buffer_v.size(); i+=4)
		{
			//@hzb-OGL-UV
		    //draw_buffer_v[0+i].texcoords.set(0.0f, 0.0f);
			//draw_buffer_v[1+i].texcoords.set(0.0f, 1.0f);
			//draw_buffer_v[2+i].texcoords.set(1.0f, 1.0f);
			//draw_buffer_v[3+i].texcoords.set(1.0f, 0.0f);
			//DX-UV
			draw_buffer_v[0+i].texcoords.set(0.0f, 0.0f);
			draw_buffer_v[1+i].texcoords.set(0.0f, 0.0f);
			draw_buffer_v[2+i].texcoords.set(1.0f, 1.0f);
			draw_buffer_v[3+i].texcoords.set(1.0f, 1.0f);
		}

		// fill remaining indices
		u32 oldIdxSize =draw_buffer_i.size();
		u32 oldvertices = oldSize;
		draw_buffer_i.set_used(Particles.size() * 6);

		for (i=oldIdxSize; i<draw_buffer_i.size(); i+=6)
		{
			draw_buffer_i[0+i] = (u16)0+oldvertices;
			draw_buffer_i[1+i] = (u16)2+oldvertices;
			draw_buffer_i[2+i] = (u16)1+oldvertices;
			draw_buffer_i[3+i] = (u16)0+oldvertices;
			draw_buffer_i[4+i] = (u16)3+oldvertices;
			draw_buffer_i[5+i] = (u16)2+oldvertices;
			oldvertices += 4;
		}
	}





}

void CParticleSystem::draw()
{
     
	if(driver==NULL)return ;
	if(draw_buffer_v.size()==0)return ;
	
#if 0
	if(p_material)
	{
		p_material->setMaterialType(ENUM_MATERIAL_TYPE_BLEND);
		p_material->setBlendSrcFactor(src_fator);
		p_material->setBlendDstFactor(dst_fator);
		p_material->setAlphaSource((ENUM_ALPHA_SOURCE)alphasource);
		//p_material->states.CullFace=false;
		p_material->setModulate(modufun);

		driver->setMaterial(p_material);

	}

	driver->setTransform(ENUM_TRANSFORM_WORLD,IDENTITY_MATRIX);

	driver->drawVertexPrimitiveList(&draw_buffer_v[0],Particles.size()*4,&draw_buffer_i[0],Particles.size()*6,ENUM_PRIMITIVE_TYPE_TRIANGLES,ENUM_VERTEX_TYPE_3V1T1C);
#else
	/*if(p_material)
	{
		p_material->setMaterialType(ENUM_MATERIAL_TYPE_BLEND);
		p_material->setBlendSrcFactor(src_fator);
		p_material->setBlendDstFactor(dst_fator);
		p_material->setAlphaSource((ENUM_ALPHA_SOURCE)alphasource);
		p_material->setFrontFace(ENUM_FRONT_FACE_CW);
		//p_material->states.CullFace=false;
		p_material->setModulate(modufun);
	}*/
	material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
	material.BlendSrc=MATERIAL_BLEND_FACTORS[src_fator];
	material.BlendDst=MATERIAL_BLEND_FACTORS[dst_fator];
	material.AlphaSource=(ENUM_ALPHA_SOURCE)alphasource;
	material.FrontFace=ENUM_FRONT_FACE_CW;
	material.Modulate=modufun;
	gfAdapter->drawVertexPrimitiveList(material,&draw_buffer_v[0],Particles.size()*4,&draw_buffer_i[0],Particles.size()*6,ENUM_VERTEX_TYPE_3V1T1C);

#endif
	
}

void  CParticleSystem ::clear()
{

	Particles.clear();
	emiter->emiteLife=emiter->emiteLifeTemp;
	//emiteLife=emiteLifeTemp;

}

/*
*创建发射器
*/
CParticleEmiter * CParticleSystem::CreatePointEmiter()
{

	return  new CPointEmiter();

}


CParticleEmiter * CParticleSystem::CreateBoxEmiter()
{
	
	return  new CBoxEmiter();

}


CParticleEmiter * CParticleSystem::CreateCircleEmiter()
{
       return new CCircleEmiter();
}





void CParticleSystem::setEmiterType( int m_emiter_type_ )
{
 
	if(m_emiter_type_==m_emiter_type)
		return ;                       //类型已经是一样的
	CParticleEmiter *emiter_=NULL;
	if(m_emiter_type_==0)
		emiter_=CreatePointEmiter();
	if(m_emiter_type_==1)
		emiter_=CreateBoxEmiter();
	if(m_emiter_type_==2)
		emiter_=CreateCircleEmiter();

	if(emiter_!=NULL)
		setEmiter(emiter_);
	 m_emiter_type=m_emiter_type_;


}



void CParticleSystem::setFollowMode( bool followMode )
{
	this->followMode=followMode;

}

void CParticleSystem::setLengthAndWidth( int length,int width )
{

	if(emiter==NULL)return;
    if(m_emiter_type==1)
		((CBoxEmiter *)emiter)->setLengthAndWidth(length,width);



}



void CParticleSystem::setMixType( ENUM_BLEND_FACTOR src_fator_,ENUM_BLEND_FACTOR dst_fator_ )
{
         src_fator=src_fator_;
		 dst_fator=dst_fator_;

}


void CParticleSystem::setAlphaSource( u32 alphasource_ )
{
      alphasource=alphasource_;

}

void CParticleSystem::setModufunc( ENUM_MODULATE  modufun_ )
{
     modufun=modufun_;
}


//设置生命值
void CParticleSystem::setTimeToLive_(int timeToLive)
{

	this->timeToLive=timeToLive;
}



int CParticleSystem::GetParticleAmount()
{
       return Particles.size();
}


void CParticleSystem::load( IReadStream *FileReader )
{

	
    //读入类型
	m_emiter_type=-1;
	int   emiterType=FileReader->readInt();
	setEmiterType(emiterType);

	//读入纹理
	int  length=0;
	length=FileReader->readInt();
	char *chs=NULL;
	if (length!=0)        //不等于0时加载纹理
	{
		length=1000;
		chs=new char[length+1];

		for(int i=0;i<length+1;i++)
		{        
			chs[i]=FileReader->readByte();

			if(chs[i]=='\0')
				break;             

		}


		stringc filePath="resource/";
		stringc fileName=chs;
		filePath=filePath+fileName;
        
		//setTexture(filePath.c_str());

		//if(driver&&p_material)
        //     p_material->setTexture(0,driver->getTexture(filePath));
		if(driver)
			material.setTexture(0,driver->getTexture(filePath));


        delete  []chs;




	}

	//读入混合参数
	src_fator=(ENUM_BLEND_FACTOR)FileReader->readInt();
	dst_fator=(ENUM_BLEND_FACTOR)FileReader->readInt();
	modufun=(ENUM_MODULATE)FileReader->readInt();
	alphasource=FileReader->readInt();

	setMixType(src_fator,dst_fator);
	setModufunc(modufun);
	setAlphaSource(alphasource);

	followMode=FileReader->readBool();


	if(emiter)
		emiter->load(FileReader);


}

void CParticleSystem::setZ( int Z )
{
	
	
	for (int i=0;i<draw_buffer_v.size();i++)
	{
		
		draw_buffer_v[i].pos.z=Z;

	}



}


