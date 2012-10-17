#ifndef _YON_NET_ISOCKET_H_
#define _YON_NET_ISOCKET_H_

#include "IReferencable.h"

namespace yon{
namespace net{

	enum ENUM_SOCKET_PROTOCOL{
		ENUM_SOCKET_PROTOCOL_TCP = 0,
		ENUM_SOCKET_PROTOCOL_UDP
		//TODO
		//ENUM_SOCKET_PROTOCOL_RAW
	};

	//TODO Data Type

	class ISocket : public virtual core::IReferencable{
	public:

		// Initialize instance of socket.  This method must be called before the object can be used.
		// @return true if properly initialized.
		virtual bool init() = 0;

		// Establish a connection to the address specified by address and port.  
		// Connection-based protocol TCP may successfully call connect() only once, 
		// however; connection protocol UDP may use connect() multiple times to change their association.  
		// @param ip: specifies the destination address to connect.
		// @param port: specifies the destination port.
		// @return true if successful connection made, otherwise false.
		virtual bool connect(const c8* ip, u16 port) = 0;

		// Attempts to send a block of data on an established connection.
		// @param buf: block of data to be sent.
		// @param bytesToSend: size of data block to be sent.
		// @return number of bytes actually sent.
		// @return of zero means the connection has been shutdown on the other side.
		// @return of -1 means that an error has occurred.
		virtual s32 sendData(const c8* buf, size_t bytesToSend) = 0;

		// Flush the socket descriptor owned by the object.
		// @return true data was successfully sent, else return false;
		virtual bool flush() = 0;

		// Close socket
		// @return true if successfully closed otherwise returns false.
		virtual bool close() = 0;

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