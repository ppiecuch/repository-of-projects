
#include "CSkyBox.h"
#include "CMaterial.h"
#include "ICamera.h"
#include "ISceneManager.h"

namespace yon{
namespace scene{
namespace sky{

	CSkyBox::CSkyBox(IModel* parent,video::ITexture* front, video::ITexture* back, video::ITexture* left,
		video::ITexture* right, video::ITexture* top, video::ITexture* bottom):
		IModel(parent),m_pMaterial(NULL)
	{
		m_pTextures[ENUM_FACE_FRONT]=front;
		m_pTextures[ENUM_FACE_BACK]=back;
		m_pTextures[ENUM_FACE_LEFT]=left;
		m_pTextures[ENUM_FACE_RIGHT]=right;
		m_pTextures[ENUM_FACE_TOP]=top;
		m_pTextures[ENUM_FACE_BOTTOM]=bottom;

		//setAutomaticCulling(scene::EAC_OFF);
		//Box.MaxEdge.set(0,0,0);
		//Box.MinEdge.set(0,0,0);

		// create indices
		m_indices[0] = 0;
		m_indices[1] = 1;
		m_indices[2] = 3;
		m_indices[3] = 2;

		// create material
		/*video::SMaterial mat;
		mat.Lighting = false;
		mat.ZBuffer = video::ECFN_NEVER;
		mat.ZWriteEnable = false;
		mat.AntiAliasing=0;
		mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;*/
		m_pMaterial=new video::CMaterial();
		m_pMaterial->states.Lighting=false;
		m_pMaterial->setWrapModeU(0,video::ENUM_WRAP_MODE_CLAMP_TO_EDGE);
		m_pMaterial->setWrapModeV(0,video::ENUM_WRAP_MODE_CLAMP_TO_EDGE);

		/*
		y 
		¡ü    x
		©¦  ¨J
		©¦¨u
		©¸©¤©¤©¤¡úz
			(++-)        (+++)
		       /6--------/5 
		      /  |      / |
		     /   |  (-++) |
	   (-+-) 1---------2  | 
		     |   7- - -| -4 (+-+)
		     |(+--)    |  /
		     |/        | /
		     0---------3/
		(---)         (--+)
		*/

		video::ITexture* tex = front;
		if (!tex) tex = left;
		if (!tex) tex = back;
		if (!tex) tex = right;
		if (!tex) tex = top;
		if (!tex) tex = bottom;
		//ÎÆÀíÆ«ÒÆÒ»¸öÏñËØ
		const f32 onepixel = tex?(1.0f / (tex->getSize().w * 1.5f)) : 0.0f;
		const f32 t0 = 0.0f + onepixel;
		const f32 t1 = 1.0f - onepixel;

		//front:0123
		m_vertices[ 0]=SVertex(-1,-1,-1,t0,t0,video::COLOR_WHITE);
		m_vertices[ 1]=SVertex(-1,+1,-1,t0,t1,video::COLOR_WHITE);
		m_vertices[ 2]=SVertex(-1,+1,+1,t1,t1,video::COLOR_WHITE);
		m_vertices[ 3]=SVertex(-1,-1,+1,t1,t0,video::COLOR_WHITE);

		//back:4567
		m_vertices[ 4]=SVertex(+1,-1,+1,t0,t0,video::COLOR_WHITE);
		m_vertices[ 5]=SVertex(+1,+1,+1,t0,t1,video::COLOR_WHITE);
		m_vertices[ 6]=SVertex(+1,+1,-1,t1,t1,video::COLOR_WHITE);
		m_vertices[ 7]=SVertex(+1,-1,-1,t1,t0,video::COLOR_WHITE);

		//left:7610
		m_vertices[ 8]=SVertex(+1,-1,-1,t0,t0,video::COLOR_WHITE);
		m_vertices[ 9]=SVertex(+1,+1,-1,t0,t1,video::COLOR_WHITE);
		m_vertices[10]=SVertex(-1,+1,-1,t1,t1,video::COLOR_WHITE);
		m_vertices[11]=SVertex(-1,-1,-1,t1,t0,video::COLOR_WHITE);

		//right:3254
		m_vertices[12]=SVertex(-1,-1,+1,t0,t0,video::COLOR_WHITE);
		m_vertices[13]=SVertex(-1,+1,+1,t0,t1,video::COLOR_WHITE);
		m_vertices[14]=SVertex(+1,+1,+1,t1,t1,video::COLOR_WHITE);
		m_vertices[15]=SVertex(+1,-1,+1,t1,t0,video::COLOR_WHITE);

		//top:2165->6521
		m_vertices[16]=SVertex(+1,+1,-1,t0,t0,video::COLOR_WHITE);
		m_vertices[17]=SVertex(+1,+1,+1,t0,t1,video::COLOR_WHITE);
		m_vertices[18]=SVertex(-1,+1,+1,t1,t1,video::COLOR_WHITE);
		m_vertices[19]=SVertex(-1,+1,-1,t1,t0,video::COLOR_WHITE);

		//bottom:4703
		m_vertices[20]=SVertex(+1,-1,+1,t0,t0,video::COLOR_WHITE);
		m_vertices[21]=SVertex(+1,-1,-1,t0,t1,video::COLOR_WHITE);
		m_vertices[22]=SVertex(-1,-1,-1,t1,t1,video::COLOR_WHITE);
		m_vertices[23]=SVertex(-1,-1,+1,t1,t0,video::COLOR_WHITE);


	}

	CSkyBox::~CSkyBox(){
		m_pMaterial->drop();
	}

	void CSkyBox::render(video::IVideoDriver* driver){
		//camera::ICamera* camera=m_pSceneManager->getActiveCamera();
		camera::ICamera* camera=m_pSceneManager->getLogisticCamera();
		if(camera==NULL)
			camera=m_pSceneManager->getViewingCamera();

		core::matrix4f trans(getAbsoluteTransformation());
		trans.setTranslation(camera->getAbsolutePosition());

		// Draw the sky box between the near and far clip plane
		const f32 viewDistance = (camera->getNear() + camera->getFar()) * 0.5f;
		trans.scale(viewDistance,viewDistance,viewDistance);
		//core::matrix4 scale;
		//scale.setScale(core::vector3df(viewDistance, viewDistance, viewDistance));

		driver->setTransform(video::ENUM_TRANSFORM_WORLD,trans);
		driver->setMaterial(m_pMaterial);
		for (s32 i=0; i<6; ++i)
		{
			m_pMaterial->setTexture(0,m_pTextures[i]);
			driver->drawVertexPrimitiveList(&m_vertices[i<<2],4,m_indices,4,video::ENUM_PRIMITIVE_TYPE_TRIANGLE_STRIP);
		}

	}

	void CSkyBox::onRegisterForRender(){
		if(m_bVisible==false)
			return;

		m_pSceneManager->registerForRender(this,ENUM_SCENE_PASS_SKYBOX);

		IModel::onRegisterForRender();
	}
}
}
}