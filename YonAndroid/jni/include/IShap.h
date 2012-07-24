#ifndef _YON_SCENE_ISHAP_H_
#define _YON_SCENE_ISHAP_H_

#include "IReferencable.h"
#include "SVertex.h"

namespace yon{
	namespace scene{

		class IShap : public virtual core::IReferencable{
		public:
			virtual const void* getVertices() const = 0;
			virtual void* getVertices() = 0;
			virtual u32 getVertexCount() const = 0;

			virtual const void* getIndices() const = 0;
			virtual void* getIndices() = 0;
			virtual u32 getIndexCount() const = 0;

			virtual void append(const  void* const vertices, u32 numVertices, const void* const indices, u32 numIndices) = 0;
			virtual void append(const IShap* const other) = 0;

			virtual ENUM_VERTEX_TYPE getVertexType() const = 0;
			//获取每个顶点/索引的字节数
			virtual u32 getVertexSize() const = 0;
			virtual u32 getIndexSize() const = 0;

			//当顶点/索引变化时，changedId跟着改变
			virtual u32 getVerticesChangedId() const = 0;
			virtual u32 getIndicesChangedId() const = 0;

			virtual void setVerticesDirty() = 0;
			virtual void setIndicesDirty() = 0;
		};
	} 
}
#endif