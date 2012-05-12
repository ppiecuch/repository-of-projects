#include "COGLES1MyGUIVertexBuffer.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	COGLES1MyGUIVertexBuffer::COGLES1MyGUIVertexBuffer(scene::IGeometryFactory* geometryFty)
		//:m_vertexCount(0),
		//m_pBuffer(NULL)
		:m_pGeometryFty(geometryFty),
		m_pShap(NULL),
		m_bHasFillIndices(false)
	{}

	COGLES1MyGUIVertexBuffer::~COGLES1MyGUIVertexBuffer(){
		//destroy();
		if(m_pShap)
			m_pShap->drop();
	}

	void COGLES1MyGUIVertexBuffer::setVertexCount(size_t _count){
		//if(m_vertexCount!=_count){
		//	m_vertexCount=_count;
		//	destroy();
		//	create();
		//}
		//m_shap.m_vertices.set_used(_count);
		if(m_pShap==NULL||m_pShap->getVertexCount()!=_count){
			if(m_pShap)
				m_pShap->drop();
			m_pShap=m_pGeometryFty->createShap(scene::ENUM_VERTEX_TYPE_3V1T1C,_count,0);
			m_bHasFillIndices=false;
		}
		
	}
	size_t COGLES1MyGUIVertexBuffer::getVertexCount(){
		//return m_vertexCount;
		return m_pShap->getVertexCount();
	}


	MyGUI::Vertex* COGLES1MyGUIVertexBuffer::lock(){
		//return (MyGUI::Vertex*)m_pBuffer;
		return (MyGUI::Vertex*)m_pShap->getVertices();
	}
	void COGLES1MyGUIVertexBuffer::unlock(){}

	void COGLES1MyGUIVertexBuffer::create(){
		//size_t size=m_vertexCount*sizeof(MyGUI::Vertex);
		//m_pBuffer=new char[size];
	}
	void COGLES1MyGUIVertexBuffer::destroy(){
		//if (m_pBuffer)
		//{
		//	delete [] m_pBuffer;
		//	m_pBuffer = NULL;
		//}
	}

	void COGLES1MyGUIVertexBuffer::fillShapIndices(){
		if(m_bHasFillIndices==false){
			m_pGeometryFty->fillShapIndices(m_pShap);
			m_bHasFillIndices=true;
		}
	}
}
}
}
}