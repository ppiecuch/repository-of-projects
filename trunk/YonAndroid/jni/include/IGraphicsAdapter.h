#ifndef _YON_SCENE_IGRAPHICSADAPTER_H_
#define _YON_SCENE_IGRAPHICSADAPTER_H_

#include "IReferencable.h"
#include "SColor.h"
#include "SVertex.h"
#include "rect.h"
#include "yonArray.h"

namespace yon{
	namespace video{
		struct SMaterial;
		class ITexture;
	}
namespace scene{

	/*
	------------->x
	|
	|
	|
	|
	y

	vertex:
	0---------3
	|         |
	|         |
	1---------2
	uv:

	topLeft-----
	|           |
	--------bottomRight

	u0,v1------u1,v1
	 |          |
	u0,v0------u0,v1
	*/

	//ê����뷽ʽ�����룬��javax.microedition.lcdui.Graphics��������һ�£�
	//����ˮƽ��λ���ĸ���ֱ��λ������BASELINE�������ֶ�λ������û�ã���������
	//��ָ������ʱ����λ���ǳɶ�ʹ�õģ����Ա���ѡ��һ��ˮƽ��һ����ֱ�㡣
	enum MASK_ACTHOR{
		MASK_ACTHOR_HCENTER = 1,
		MASK_ACTHOR_VCENTER = 2,
		MASK_ACTHOR_LEFT    = 4,
		MASK_ACTHOR_RIGHT   = 8,
		MASK_ACTHOR_TOP     = 16,
		MASK_ACTHOR_BOTTOM  = 32,
		MASK_ACTHOR_BASELINE= 64
	};

	//�任��ʽ����javax.microedition.lcdui.game.Sprite��������һ�£�
	enum ENUM_TRANS{
		ENUM_TRANS_MIRROR			= 2,
		ENUM_TRANS_MIRROR_ROT180	= 1,
		ENUM_TRANS_MIRROR_ROT270	= 4,
		ENUM_TRANS_MIRROR_ROT90		= 7,
		ENUM_TRANS_NONE				= 0,
		ENUM_TRANS_ROT180			= 3,
		ENUM_TRANS_ROT270			= 6,
		ENUM_TRANS_ROT90			= 5
	};

	class IGraphicsAdapter : public virtual core::IReferencable{
	public:
		virtual ~IGraphicsAdapter(){}

		//ע�⣺������ƹ��̲��������sceneMgr->render����ô�ǵû���֮ǰ�ȵ���camera->render
		//virtual void beginBatch(s32 layerId) = 0;
		//virtual void endBatch() = 0;

		//���Ƴɹ�����true�����򷵻�false
		virtual bool drawImage(const c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY,bool useAlpha=false, u32 color=0xFFFFFFFF) = 0;

		//Draws the specified image by using the anchor point.
		//anchorĬ��ΪTOP|LEFT
		//���Ƴɹ�����true�����򷵻�false
		virtual bool drawImage(video::ITexture* texture, s32 x, s32 y, bool useAlpha=false,MASK_ACTHOR anchor=(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP)) = 0;

		//Copies a region of the specified source image to a location within the destination, possibly transforming (rotating and reflecting) the image data using the chosen transform function.
		//transformĬ��ΪENUM_TRANS_NONE��anchorĬ��ΪTOP|LEFT
		//���Ƴɹ�����true�����򷵻�false
		//virtual bool drawRegion(const c8* imageName, s32 x_src, s32 y_src, s32 width, s32 height, ENUM_TRANS transform=ENUM_TRANS_NONE, s32 x_dest=0, s32 y_dest=0, MASK_ACTHOR anchor=(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP)) = 0;


		virtual void clearZ(s32 z) = 0;
		//virtual bool drawRegion(const c8* imageName, const core::rectf& uv, s32 x_dest, s32 y_dest, s32 destW, s32 destH, ENUM_TRANS transform=ENUM_TRANS_NONE, MASK_ACTHOR anchor=(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP),bool useAlpha=false, u32 color=0xFFFFFFFF) = 0;
		//virtual bool drawRegion(const c8* imageName, const core::dimension2di& dim, const core::rectf& uv, s32 x_dest, s32 y_dest, s32 destW, s32 destH, ENUM_TRANS transform=ENUM_TRANS_NONE, MASK_ACTHOR anchor=(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP),bool useAlpha=false, u32 color=0xFFFFFFFF) = 0;
		virtual bool drawRegion(video::ITexture* texture, const core::rectf& uv, const core::position2di poss[4], ENUM_TRANS transform=ENUM_TRANS_NONE, bool useAlpha=false, u32 color=0xFFFFFFFF) = 0;
		virtual bool drawFill(video::ITexture* texture, const core::rectf& uv, const core::position2di poss[4], ENUM_TRANS transform=ENUM_TRANS_NONE, u32 color=0xFFFFFFFF) = 0;

		//virtual bool drawRegion(video::ITexture* texture, const core::rectf& uv, s32 x_dest, s32 y_dest, s32 destW, s32 destH, ENUM_TRANS transform=ENUM_TRANS_NONE, MASK_ACTHOR anchor=(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP),bool useAlpha=false, const video::SColor& color=video::COLOR_WHITE) = 0;

		//virtual void beginResident(s32 id) = 0;
		//virtual bool eraseResident(s32 id) = 0;
		//virtual void endResident() = 0;
		//ʹ�÷�����
		//��Ӷ���ʱ��
		//beginLayer(XXX);
		//beginElement(YYY);
		//drawRegion(...);
		//drawRegion(...);
		//......
		//endElement();
		//endLayer();
		//
		//���ƻ���ʱ��
		//beginLayer(XXX);
		//setDrawElements(...);
		//endLayer();
		virtual void beginLayer(s32 id) = 0;
		virtual bool eraseLayer(s32 id) = 0;
		virtual void endLayer() = 0;
		virtual void beginElement(s32 id) = 0;
		virtual bool eraseElement(s32 id) = 0;
		virtual void setDrawElements(const core::array<s32>& ids) = 0;
		virtual void endElement() = 0;
		virtual void renderLayer(s32 id) = 0;
		//��Чר�ýӿ�
		//virtual void drawVertexPrimitiveList(video::IMaterial* material,const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,scene::ENUM_VERTEX_TYPE vType=scene::ENUM_VERTEX_TYPE_3V1T1C) =0;
		virtual void drawVertexPrimitiveList(const video::SMaterial& material,const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,scene::ENUM_VERTEX_TYPE vType=scene::ENUM_VERTEX_TYPE_3V1T1C) = 0;
		virtual void render() = 0;
	};
}
}
#endif