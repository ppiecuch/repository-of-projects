#ifndef _YON_VIDEO_OGLES1_COGLES1HARDWAREBUFFER_H_
#define _YON_VIDEO_OGLES1_COGLES1HARDWAREBUFFER_H_

#include "IHardwareBuffer.h"
#include "COGLES1Driver.h"
#include "IUnit.h"

namespace yon{
namespace video{
namespace ogles1{
	
	class COGLES1HardwareBuffer : public IHardwareBuffer{
	private:
		void update();
		void updateVertices();
		void updateIndices();

		scene::IUnit* m_pUnit;

		u32 m_uVerticesChangedId;
		u32 m_uIndicesChangedId;

		u32 m_uVertexBufferSize;
		u32 m_uIndexBufferSize;

		GLuint m_vboIds[2];

		u8* m_vertexPosOffset;
		u8* m_vertexColorOffset;
		u8* m_vertexTexcoordOffsets[YON_MATERIAL_MAX_TEXTURES];
		u32 m_uStride;
	public:
		COGLES1HardwareBuffer(scene::IUnit* unit);
		virtual ~COGLES1HardwareBuffer();

		virtual void logon();
		virtual void logoff();

		virtual void draw(ENUM_PRIMITIVE_TYPE pType);
	};
}
	IHardwareBuffer* createHardwareBuffer(scene::IUnit* unit);
}
}
#endif