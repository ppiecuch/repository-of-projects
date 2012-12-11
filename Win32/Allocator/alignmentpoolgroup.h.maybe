#ifndef _YON_CORE_ALIGNMENTPOOLGROUP_H_
#define _YON_CORE_ALIGNMENTPOOLGROUP_H_

#include "alignmentpool.h"

namespace yon{
namespace core{

	class alignmentpoolgroup{
		alignmentpool<256,4,1> m_pool1;
		alignmentpool<256,4,2> m_pool2;
		alignmentpool<256,4,4> m_pool4;
		alignmentpool<256,4,8> m_pool8;
		alignmentpool<256,4,16> m_pool16;
		alignmentpool<256,4,32> m_pool32;
		alignmentpool<256,4,64> m_pool64;
		alignmentpool<256,4,128> m_pool128;

		alignmentpoolgroup();
		alignmentpoolgroup(const alignmentpoolgroup&);
		alignmentpoolgroup& operator = (const alignmentpoolgroup&);
	public:
		static alignmentpoolgroup* GetInstance(){
			static alignmentpoolgroup instance;
			return &instance;
		}

		void* allocate(u32 align,u32 size,u32 count){
			switch(align)
			{
			case 1:
				return m_pool1.allocate(size,count);
			case 2:
				return m_pool2.allocate(size,count);
			case 4:
				return m_pool4.allocate(size,count);
			case 8:
				return m_pool8.allocate(size,count);
			case 16:
				return m_pool16.allocate(size,count);
			case 32:
				return m_pool32.allocate(size,count);
			case 64:
				return m_pool64.allocate(size,count);
			case 128:
				return m_pool128.allocate(size,count);
			}
			YON_DEBUG_BREAK_IF(true);
		}
		void deallocate(u32 align,void* p){
			switch(align)
			{
			case 1:
				m_pool1.deallocate(p);
				return;
			case 2:
				m_pool2.deallocate(p);
				return;
			case 4:
				m_pool4.deallocate(p);
				return;
			case 8:
				m_pool8.deallocate(p);
				return;
			case 16:
				m_pool16.deallocate(p);
				return;
			case 32:
				m_pool32.deallocate(p);
				return;
			case 64:
				m_pool64.deallocate(p);
				return;
			case 128:
				m_pool128.deallocate(p);
				return;
			}
			YON_DEBUG_BREAK_IF(true);
		}
	};
}
}
#endif