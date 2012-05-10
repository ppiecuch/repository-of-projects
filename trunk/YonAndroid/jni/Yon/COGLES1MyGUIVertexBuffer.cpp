#include "COGLES1MyGUIVertexBuffer.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	COGLES1MyGUIVertexBuffer::COGLES1MyGUIVertexBuffer()
		:m_vertexCount(0),
		m_pBuffer(NULL)
	{}

	COGLES1MyGUIVertexBuffer::~COGLES1MyGUIVertexBuffer(){
		destroy();
	}

	void COGLES1MyGUIVertexBuffer::setVertexCount(size_t _count){
		if(m_vertexCount!=_count){
			m_vertexCount=_count;
			destroy();
			create();
		}
	}
	size_t COGLES1MyGUIVertexBuffer::getVertexCount(){
		return m_vertexCount;
	}


	MyGUI::Vertex* COGLES1MyGUIVertexBuffer::lock(){
		return (MyGUI::Vertex*)m_pBuffer;
	}
	void COGLES1MyGUIVertexBuffer::unlock(){}

	void COGLES1MyGUIVertexBuffer::create(){
		size_t size=m_vertexCount*sizeof(MyGUI::Vertex);
		m_pBuffer=new char[size];
	}
	void COGLES1MyGUIVertexBuffer::destroy(){
		if (m_pBuffer)
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}
	}
}
}
}
}