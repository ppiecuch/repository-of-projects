#ifndef _YON_SCENE_IUNIT_H_
#define _YON_SCENE_IUNIT_H_

#include "IReferencable.h"
#include "yonTypes.h"
#include "SVertex.h"
#include "IMaterial.h"
#include "IShap.h"

namespace yon{
	namespace scene{
		
		//�൱��irrlicht��IMeshBuffer

		//�Բ���Ϊ��λ,�������㼯�ϼ���������
		//���ֲַ������ڶԸ��ӵ�ģ�ͽ����߼�����,�������;
		//����,һЩ�Կ�����ĳЩԭ��,�Զ��㼯�ϵ�������һЩ����,
		//����������������ЩҪ��,1000-10000��������һ�����͵�����.
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