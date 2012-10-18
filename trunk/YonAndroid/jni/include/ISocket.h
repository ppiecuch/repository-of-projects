#ifndef _YON_NET_ISOCKET_H_
#define _YON_NET_ISOCKET_H_

#include "IReferencable.h"
#include "yonString.h"

namespace yon{
namespace net{

	enum ENUM_SOCKET_PROTOCOL{
		ENUM_SOCKET_PROTOCOL_TCP = 0
		//TODO
		//ENUM_SOCKET_PROTOCOL_UDP
		//ENUM_SOCKET_PROTOCOL_RAW
	};

	//TODO Data Type

	class ISocket : public virtual core::IReferencable{
	public:

		// Initialize instance of socket.  This method must be called before the object can be used.
		// @return true if properly initialized.
		virtual bool init() = 0;

		virtual bool bindOn(const c8* ip, u16 port) = 0;

		// Prepare listen for connecting socket
		// @param connectionBacklog: specifies connection queue backlog (default 30,000)
		// @return true if a listening socket was created,else the false is returned.  
		virtual bool monitor(s32 connectionBacklog = 30000) = 0;

		// Extracts the first connection request on the queue of pending connections and creates a newly connected socket.  Used with 
		// It is the responsibility of the caller to delete the returned object when finished.
		// @return if successful a pointer to a newly created ISocket object will be returned, else return NULL
		virtual ISocket* acceptSocket() = 0;

		virtual core::stringc getNativeAddress() const = 0;
		virtual core::stringc getPeerAddress() const = 0;
		virtual u16 getNativePort() const = 0;
		virtual u16 getPeerPort() const = 0;

		// Establish a connection to the address specified by address and port.  
		// Connection-based protocol TCP may successfully call connect() only once, 
		// however; connection protocol UDP may use connect() multiple times to change their association.  
		// @param ip: specifies the destination address to connect.
		// @param port: specifies the destination port.
		// @return true if successful connection made, otherwise false.
		virtual bool open(const c8* ip, u16 port) = 0;

		// Attempts to send a block of data on an established connection.
		// @param buf: block of data to be sent.
		// @param bytesToSend: size of data block to be sent.
		// @return number of bytes actually sent.
		//		return 0 means the connection has been shutdown on the other side.
		//		return -1 means that an error has occurred.
		virtual s32 sendData(const c8* buf, s32 bytesToSend) = 0;

		// Attempts to receive a block of data on an established connection.
		// @param pointer: return the reference to the receive buffer pointer if receive data succeessfully, otherwise do no change to it.
		//		note that the user should not free this buffer. pointer can be NULL.
		// @param maxBytes: maximum number of bytes to receive.
		// @return number of bytes actually received.
		//		return 0 means the connection has been shutdown on the other side.
		//		return -1 means that an error has occurred.
		virtual s32 receiveData(c8* &pointer,s32 maxBytes = 1) = 0;

		// Flush the socket descriptor owned by the object.
		// @return true data was successfully sent, else return false;
		virtual bool flush() = 0;

		// Close socket
		// @return true if successfully closed otherwise returns false.
		virtual bool close() = 0;

		// Examine the socket descriptor sets currently owned by the instance of
		// the socket class (the readfds, writefds, and errorfds parameters) to 
		// see whether some of their descriptors are ready for reading, are ready 
		// for writing, or have an exceptional condition pending, respectively.
		// Block until an event happens on the specified file descriptors.
		// @return true if socket has data ready, or false if not ready or timed out.
		bool verify(void) { return verify(0,0); }

		// Examine the socket descriptor sets currently owned by the instance of
		// the socket class (the readfds, writefds, and errorfds parameters) to 
		// see whether some of their descriptors are ready for reading, are ready 
		// for writing, or have an exceptional condition pending, respectively.
		// Block until an event happens on the specified file descriptors.
		// @param timeoutSec: timeout in seconds for select.
		// @param timeoutMicroSec: timeout in micro seconds for select.
		// @return true if socket has data ready, or false if not ready or timed out.
		virtual bool verify(s32 timeoutSec,s32 timeoutMicroSec) = 0;

		// Set the socket as non-blocking or blocking.
		// @param on: enable non-blocking if on value is true, eles disable it.
		// @return true if successful set to non-blocking, else return false;
		virtual bool setNonblocking(bool on=true) = 0;

		// Returns blocking/non-blocking state of socket.
		// @return true if the socket is non-blocking, else return false.
		virtual bool isNonblocking() const= 0;

		//TODO
		//virtual bool setNagleAlgorithm(bool on) = 0;
	};
}
}
#endif