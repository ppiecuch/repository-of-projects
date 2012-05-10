#ifndef _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIVERTEXBUFFER_H_
#define _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIVERTEXBUFFER_H_

#include "IMyGUIVertexBuffer.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	class COGLES1MyGUIVertexBuffer : public IMyGUIVertexBuffer{
	private:
		void* m_pBuffer;
		size_t m_vertexCount;

		void create();
		void destroy();
	public:
		COGLES1MyGUIVertexBuffer();
		~COGLES1MyGUIVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual MyGUI::Vertex* lock();
		virtual void unlock();

		virtual void* getClientArrayPtr() const{
			return m_pBuffer;
		}

	};
}
}
}
}
#endif