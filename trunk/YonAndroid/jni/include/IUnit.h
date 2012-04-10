#ifndef _YON_SCENE_IUNIT_H_
#define _YON_SCENE_IUNIT_H_

#include "IReferencable.h"
#include "yonTypes.h"
#include "SVertex.h"
#include "IMaterial.h"
#include "IShap.h"

namespace yon{
	namespace scene{
		
		//相当于irrlicht的IMeshBuffer

		//以材质为单位,包含顶点集合及索引集合
		//这种分层有助于对复杂的模型进行逻辑分组,方便管理;
		//再者,一些显卡基于某些原因,对顶点集合的数量有一些限制,
		//分组有利于满足这些要求,1000-10000个顶点是一个典型的配置.
		class IUnit : public virtual core::IReferencable{
		public:
			virtual ~IUnit(){}

			virtual video::IMaterial* getMaterial() = 0;
			virtual const video::IMaterial* getMaterial() const = 0;

			/*virtual const void* getVertices() const = 0;
			virtual void* getVertices() = 0;
			virtual u32 getVertexCount() const = 0;

			virtual const void* getIndices() const = 0;
			virtual void* getIndices() = 0;
			virtual u32 getIndexCount() const = 0;

			virtual void append(const  void* const vertices, u32 numVertices, const void* const indices, u32 numIndices) = 0;

			virtual void append(const IUnit* const other) = 0;*/

			virtual IShap* getShap() const = 0;
			virtual void setShap(IShap* shap) = 0;

			//virtual ENUM_DIMEN_MODE getDimenMode() const = 0;
			virtual ENUM_VERTEX_TYPE getVertexType() const = 0;
		};
	}//scene
}//yon
#endif