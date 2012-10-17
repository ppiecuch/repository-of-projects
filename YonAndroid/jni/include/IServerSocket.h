#ifndef _YON_NET_ISERVERSOCKET_H_
#define _YON_NET_ISERVERSOCKET_H_

#include "ISocket.h"

namespace yon{
namespace net{

	class IServerSocket : public virtual core::IReferencable{
	public:
		// Extracts the first connection request on the queue of pending connections and creates a newly connected socket.  Used with 
		// It is the responsibility of the caller to delete the returned object when finished.
		// @return if successful a pointer to a newly created ISocket object will be returned, else return NULL
		virtual ISocket *accept();
	};
}
}
#endif