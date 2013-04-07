#ifndef _YON_NET_ISESSION_H_
#define _YON_NET_ISESSION_H_

#include "IReferencable.h"

namespace yon{
namespace net{

	class ISession : public virtual core::IReferencable{
	public:
		virtual bool lock() = 0;
		virtual bool unlock() = 0;
	};
}
}
#endif