#include "CVertexBuffer.h"

namespace yon{
namespace gui{
namespace mygui{

	CVertexBuffer::CVertexBuffer()
		:m_vertexCount(0),
		m_pBuffer(NULL)
	{}

	CVertexBuffer::~CVertexBuffer(){
		destroy();
	}

	void CVertexBuffer::setVertexCount(size_t _count){
		if(m_vertexCount!=_count){
			m_vertexCount=count;
			destroy();
			create();
		}
	}
	size_t CVertexBuffer::getVertexCount(){
		return m_vertexCount;
	}


	MyGUI::Vertex* CVertexBuffer::lock(){
		return (MyGUI::Vertex*)m_pBuffer;
	}
	void CVertexBuffer::unlock(){}

	void CVertexBuffer::create(){
		size_t size=m_vertexCount*sizeof(MyGUI::Vertex);
		m_pBuffer=new char[size];
	}
	void CVertexBuffer::destroy(){
		if (m_pBuffer)
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}
	}
}
}
}