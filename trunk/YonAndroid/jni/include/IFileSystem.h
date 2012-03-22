#ifndef _YON_IO_IFILESYSTEM_H_
#define _YON_IO_IFILESYSTEM_H_

#include "IReferencable.h"

namespace yon{
	namespace io{

		class IFileSystem : public virtual core::IReferencable{
		public:
			virtual ~IFileSystem(){}
		};
	}//io
}//yon
#endif