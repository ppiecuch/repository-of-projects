#ifndef _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIVERTEXBUFFER_H_
#define _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIVERTEXBUFFER_H_

#include "IMyGUIVertexBuffer.h"
#include "IGeometryFactory.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	class COGLES1MyGUIVertexBuffer : public IMyGUIVertexBuffer{
	private:
		//void* m_pBuffer;
		//size_t m_vertexCount;

		scene::IGeometryFactory* m_pGeometryFty;
		scene::IShap* m_pShap;
		bool m_bHasFillIndices;

		void create();
		void destroy();
	public:
		COGLES1MyGUIVertexBuffer(scene::IGeometryFactory* geometryFty);
		~COGLES1MyGUIVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual MyGUI::Vertex* lock();
		virtual void unlock();

		virtual void* getClientArrayPtr() const{
			//return m_pBuffer;
			return m_pShap->getVertices();
		}

		virtual scene::IShap* getShap() const{
			return m_pShap;
		}

		virtual void fillShapIndices();

	};
}
}
}
}
#endif