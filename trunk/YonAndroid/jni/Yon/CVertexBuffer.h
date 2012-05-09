#ifndef _YON_GUI_MYGUI_CVERTEXBUFFER_H_
#define _YON_GUI_MYGUI_CVERTEXBUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

namespace yon{
namespace gui{
namespace mygui{

	class CVertexBuffer : MyGUI::IVertexBuffer{
	private:
		void* m_pBuffer;
		size_t m_vertexCount;

		void create();
		void destroy();
	public:
		CVertexBuffer();
		~CVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual MyGUI::Vertex* lock();
		virtual void unlock();

	};
}
}
}
#endif