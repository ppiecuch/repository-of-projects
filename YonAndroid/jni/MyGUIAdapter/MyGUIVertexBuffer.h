#ifndef _MYGUI_MYGUIVERTEXBUFFER_H_
#define _MYGUI_MYGUIVERTEXBUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include "IShap.h"
#include "IGeometryFactory.h"

using namespace yon;

namespace MyGUI{

	class MyGUIVertexBuffer : public MyGUI::IVertexBuffer{
	private:
		const scene::IGeometryFactory* m_pGeometryFty;
		scene::IShap* m_pShap;
		bool m_bHasFillIndices;

	public:
		MyGUIVertexBuffer(const scene::IGeometryFactory* geometryFty);
		~MyGUIVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount(){
			return m_pShap->getVertexCount();
		}

		virtual MyGUI::Vertex* lock(){
			return (MyGUI::Vertex*)m_pShap->getVertices();
		}
		virtual void unlock(){}

		virtual void* getClientArrayPtr() const{
			return m_pShap->getVertices();
		}

		virtual scene::IShap* getShap() const{
			return m_pShap;
		}

		virtual void fillShapIndices();

	};
}
#endif