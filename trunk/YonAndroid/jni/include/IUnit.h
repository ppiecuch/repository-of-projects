#ifndef _YON_SCENE_IUNIT_H_
#define _YON_SCENE_IUNIT_H_

#include "IReferencable.h"
#include "yonTypes.h"
#include "SVertex.h"
#include "IMaterial.h"

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

			virtual const SVertex* getVertices() const = 0;
			virtual SVertex* getVertices() = 0;
			virtual u32 getVertexCount() const = 0;

			virtual const u16* getIndices() const = 0;
			virtual u16* getIndices() = 0;
			virtual u32 getIndexCount() const = 0;

			virtual void append(const  SVertex* const vertices, u32 numVertices, const u16* const indices, u32 numIndices) = 0;

			virtual void append(const IUnit* const other) = 0;
		};
	}//scene
}//yon
#endif