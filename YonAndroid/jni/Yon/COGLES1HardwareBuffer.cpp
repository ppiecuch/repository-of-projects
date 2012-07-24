#include "COGLES1HardwareBuffer.h"
#include "COGLES1Driver.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{
	COGLES1HardwareBuffer::COGLES1HardwareBuffer(scene::IUnit* unit)
		:m_pUnit(unit),m_uVertexBufferSize(0),m_uIndexBufferSize(0),m_uVerticesChangedId(0),m_uIndicesChangedId(0)
		//m_uVerticesChangedId(unit->getShap()->getVerticesChangedId()),m_uIndicesChangedId(unit->getShap()->getIndicesChangedId())
	{
			m_pUnit->grab();

			//u32 vertexBufferSize=m_pUnit->getShap()->getVertexSize()*m_pUnit->getShap()->getVertexCount();
			//u32 indexBufferSize=m_pUnit->getShap()->getIndexSize()*m_pUnit->getShap()->getIndexCount();
			//GLenum usage=m_pUnit->getHardwareBufferUsageType()==ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC?GL_STATIC_DRAW:GL_DYNAMIC_DRAW;

			m_uVertexBufferSize=m_pUnit->getShap()->getVertexSize()*m_pUnit->getShap()->getVertexCount();
			m_uIndexBufferSize=m_pUnit->getShap()->getIndexSize()*m_pUnit->getShap()->getIndexCount();
			
			//GLenum vertexUsage=m_pUnit->getVertexHardwareBufferUsageType()==ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC?GL_STATIC_DRAW:GL_DYNAMIC_DRAW;
			//GLenum indexUsage=m_pUnit->getIndexHardwareBufferUsageType()==ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC?GL_STATIC_DRAW:GL_DYNAMIC_DRAW;

			glGenBuffers(2,m_vboIds);

			/*glBindBuffer(GL_ARRAY_BUFFER,m_vboIds[0]);
			glBufferData(GL_ARRAY_BUFFER,vertexBufferSize,m_pUnit->getShap()->getVertices(),vertexUsage);
			glBindBuffer(GL_ARRAY_BUFFER,0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vboIds[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexBufferSize,m_pUnit->getShap()->getIndices(),indexUsage);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);*/

			update();

			//¼ÆËãÆ«ÒÆÁ¿
			m_vertexPosOffset=getVertexPosOffset(m_pUnit->getVertexType());
			m_vertexColorOffset=getVertexColorOffset(m_pUnit->getVertexType());
			for(u32 i=0;i<YON_MATERIAL_MAX_TEXTURES;++i)
				m_vertexTexcoordOffsets[i]=getVertexTexcoordOffset(m_pUnit->getVertexType(),i);
			m_uStride=m_pUnit->getShap()->getVertexSize();

			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("Instance COGLES1HardwareBuffer,offset:{%d,%d,%d,%d},changedId:{%d,%d},stride:%d",m_vertexPosOffset,m_vertexColorOffset,m_vertexTexcoordOffsets[0],m_vertexTexcoordOffsets[1],m_uVerticesChangedId,m_uIndicesChangedId,m_uStride).c_str());
	}
	COGLES1HardwareBuffer::~COGLES1HardwareBuffer(){
		glDeleteBuffers(2,m_vboIds);
		m_pUnit->drop();

		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release COGLES1HardwareBuffer");
	}

	void COGLES1HardwareBuffer::updateVertices(){
		u32 vertexBufferSize=m_pUnit->getShap()->getVertexSize()*m_pUnit->getShap()->getVertexCount();
		glBindBuffer(GL_ARRAY_BUFFER,m_vboIds[0]);
		if(m_uVertexBufferSize<vertexBufferSize||m_uVerticesChangedId==0)
		{
			GLenum vertexUsage=m_pUnit->getVertexHardwareBufferUsageType()==ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC?GL_STATIC_DRAW:GL_DYNAMIC_DRAW;
			glBufferData(GL_ARRAY_BUFFER,vertexBufferSize,m_pUnit->getShap()->getVertices(),vertexUsage);
			m_uVertexBufferSize=vertexBufferSize;

			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"rebuild COGLES1HardwareBuffer.vertices");
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER,0,vertexBufferSize,m_pUnit->getShap()->getVertices());

			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"update COGLES1HardwareBuffer.vertices");
		}
		//COGLES1Driver::checkError(__FILE__,__LINE__);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	
		m_uVerticesChangedId=m_pUnit->getShap()->getVerticesChangedId();
	}
	void COGLES1HardwareBuffer::updateIndices(){
		u32 indexBufferSize=m_pUnit->getShap()->getIndexSize()*m_pUnit->getShap()->getIndexCount();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vboIds[1]);
		if(m_uIndexBufferSize<indexBufferSize||m_uIndicesChangedId==0)
		{
			GLenum indexUsage=m_pUnit->getIndexHardwareBufferUsageType()==ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC?GL_STATIC_DRAW:GL_DYNAMIC_DRAW;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexBufferSize,m_pUnit->getShap()->getIndices(),indexUsage);
			m_uIndexBufferSize=indexBufferSize;

			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"rebuild COGLES1HardwareBuffer.indices");
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,indexBufferSize,m_pUnit->getShap()->getIndices());

			//Logger->debug(YON_LOG_SUCCEED_FORMAT,"update COGLES1HardwareBuffer.indices");
		}
		//COGLES1Driver::checkError(__FILE__,__LINE__);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		m_uIndicesChangedId=m_pUnit->getShap()->getIndicesChangedId();
	}
	void COGLES1HardwareBuffer::update(){
		if(m_uVerticesChangedId!=m_pUnit->getShap()->getVerticesChangedId())
			updateVertices();
		if(m_uIndicesChangedId!=m_pUnit->getShap()->getIndicesChangedId())
			updateIndices();
	}
	void COGLES1HardwareBuffer::draw(ENUM_PRIMITIVE_TYPE pType){
		update();

		glBindBuffer(GL_ARRAY_BUFFER,m_vboIds[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vboIds[1]);

		switch(m_pUnit->getVertexType()){
		case scene::ENUM_VERTEX_TYPE_2V1T1C:
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glVertexPointer(2, GL_FLOAT, m_uStride,m_vertexPosOffset);
				glColorPointer(4,GL_UNSIGNED_BYTE, m_uStride,m_vertexColorOffset);
				glTexCoordPointer(2, GL_FLOAT, m_uStride,m_vertexTexcoordOffsets[0]);
				glDrawElements(pType, m_pUnit->getShap()->getIndexCount(), GL_UNSIGNED_SHORT,0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
		case scene::ENUM_VERTEX_TYPE_3V1T1C:
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glVertexPointer(3, GL_FLOAT, m_uStride,m_vertexPosOffset);
				glColorPointer(4,GL_UNSIGNED_BYTE, m_uStride,m_vertexColorOffset);
				glTexCoordPointer(2, GL_FLOAT, m_uStride,m_vertexTexcoordOffsets[0]);
				//Logger->debug("%08x,%d,%d\n",m_pUnit->getShap(),m_pUnit->getShap()->getVertexCount(),m_pUnit->getShap()->getIndexCount());
				glDrawElements(pType, m_pUnit->getShap()->getIndexCount(), GL_UNSIGNED_SHORT,0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
		case scene::ENUM_VERTEX_TYPE_3V2T1C:
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);

				glVertexPointer(3, GL_FLOAT, m_uStride,m_vertexPosOffset);
				glColorPointer(4,GL_UNSIGNED_BYTE, m_uStride,m_vertexColorOffset);

				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, m_uStride,m_vertexTexcoordOffsets[0]);

				glClientActiveTexture(GL_TEXTURE1);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, m_uStride,m_vertexTexcoordOffsets[1]);

				glDrawElements(pType, m_pUnit->getShap()->getIndexCount(), GL_UNSIGNED_SHORT,0);

				glClientActiveTexture(GL_TEXTURE1);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glClientActiveTexture(GL_TEXTURE0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
		}
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}
}
	IHardwareBuffer* createHardwareBuffer(scene::IUnit* unit){
		return new ogles1::COGLES1HardwareBuffer(unit);
	}
}
}