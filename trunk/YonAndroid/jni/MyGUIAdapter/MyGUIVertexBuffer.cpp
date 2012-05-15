#include "MyGUIVertexBuffer.h"

#include "ILogger.h"

namespace MyGUI{
	MyGUIVertexBuffer::MyGUIVertexBuffer(const scene::IGeometryFactory* geometryFty)
		:m_pGeometryFty(geometryFty),
		m_pShap(NULL),
		m_bHasFillIndices(false)
	{}

	MyGUIVertexBuffer::~MyGUIVertexBuffer(){
		if(m_pShap)
			m_pShap->drop();
	}

	void MyGUIVertexBuffer::setVertexCount(size_t _count){
		if(m_pShap==NULL||m_pShap->getVertexCount()!=_count){
			if(m_pShap)
				m_pShap->drop();
			m_pShap=m_pGeometryFty->createShap(scene::ENUM_VERTEX_TYPE_3V1T1C,_count,0);
			m_bHasFillIndices=false;
		}

	}

	void MyGUIVertexBuffer::fillShapIndices(){
		if(m_bHasFillIndices==false){
			m_pGeometryFty->fillShapIndices(m_pShap);
			m_bHasFillIndices=true;

			Logger->debug("fillShapIndices:%08X(v:%d,i:%d)\n",m_pShap,m_pShap->getVertexCount(),m_pShap->getIndexCount());
		}
	}

}