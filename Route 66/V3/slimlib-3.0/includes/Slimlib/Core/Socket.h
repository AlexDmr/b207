#ifndef __Class_Socket_
#define __Class_Socket_

#include <Slimlib/Core/Object.h>

#define MAX_SOCKET_BUFFER_SIZE	(32768 - 1)

#if defined(unix) || defined(__APPLE__)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>

	#define socket_t int
	#define SOCKADDR_IN struct sockaddr_in
	#define SOCKADDR_IN_SIZE sizeof(struct sockaddr_in)
	#define SOCKADDR struct sockaddr
	#define SOCKADDR_SIZE sizeof(struct sockaddr)
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET 0
	#define MAX_SOCKET_BUFFER_SIZE	(32768 - 1)
#endif

#if ((defined(WIN32) || defined(_WIN32)) && (!defined(_WIN32_WCE)))
	#include <winsock.h>
#endif

#ifdef _WIN32_WCE
	#include <Afxsock.h>
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
	#define SOCKADDR_SIZE sizeof(struct sockaddr)
	#define SOCKADDR_IN_SIZE sizeof(SOCKADDR_IN)
	#define socket_t SOCKET
	typedef int socklen_t;
        #ifndef _SIZE_T_DEFINED
        #define _SIZE_T_DEFINED
          typedef unsigned int size_t;
        #endif
	typedef int ssize_t;
#endif

/** Socket is a base class to encapsulate network sockets.
 * This class could not be used as is. You should deal with more specialized classes
 * @see TcpServerSocket
 * @see TcpClientSocket
 * @see UdpSocket
 * @see MulticastSocket
 */
class Socket: public Object
{
	protected:
		socket_t socket_handle ;
		struct ip_mreq mreq;            // Used in adding or dropping
		char *buffer;
		size_t buffer_size;
		bool resize_buffer_allowed;

	public:
		Socket(void);
		virtual ~Socket(void);
		bool resizeReceptionBuffer(size_t size);
		size_t getBufferSize();
		void closeSocket(void);
};




/** Socket is a base class to encapsulate TCP network sockets.
 * This class could not be used as is. You should deal with more specialized classes
 * @see TcpServerSocket
 * @see TcpClientSocket
 */
class TcpSocket: public Socket
{
	public:
		TcpSocket(void);
		void setHandle(socket_t handle);
		int sendData(socket_t handle, const char *buffer, size_t nbByteToSend);
		int sendData(const char *buffer, size_t nbByteToSend);
		size_t waitingForData(socket_t handle, char *buffer);
		size_t waitingForData(char *buffer);
		void flush(size_t nbOfBytes);

	private:
		char *data;
		size_t size;
};



/** TcpConnectionHandler is a class to encapsulate the code to execute when a
 * connection occurs on a TcpServerSocket
 * @see TcpServerSocket
 */
class TcpConnectionHandler: public Object
{
	protected:
		TcpSocket socket;

	public:
		TcpConnectionHandler() { }
		virtual ~TcpConnectionHandler() { }

		/** onConnection() contains the code to execute when a connection occurs on a
		 * TcpServerSocket.
		 * @param handle is the handle of the socket returned when the incoming connection was
		 * accepted. With this <i>handle</i> and the protected member <i>socket</i>You
		 * will be able to exchange data with the remote client.
		 * @see setConnectionHandler() in TcpServerSocket
		 */
		virtual void onConnection(socket_t handle);
};



/** TcpServerSocket is a class to encapsulate a TCP server socket
 */
class TcpServerSocket: public TcpSocket
{
	protected:
		bool server_stopped;
		int port;
		TcpConnectionHandler *connection_handler;

	public:
		/** Construct a TcpServerSocket, ready to listen on port <i>port</i>
		 * @param port contains the port to listen
		 */
		TcpServerSocket(int port);
		virtual ~TcpServerSocket(void);

		/** Stop this server
		 */
		void stop();

		/** Set a connection handler. When a incoming connection from a client occurs,
		 * onConnection() method of <i>connection_handler</i> will be called
		 * @param connection_handler is a reference to a instance of TcpConnectionHandler
		 */
		void setConnectionHandler(TcpConnectionHandler *connection_handler);

		/** Server main loop.
		 */
		int run(void);

		/** Return server listening port.
		 * @return (integer) a port number.
		 */
		int getPort();
};



/** TcpClientSocket is a class to encapsulate a TCP client socket
 */
class TcpClientSocket: public TcpSocket
{
	protected:
		SOCKADDR_IN server_sin;

	public:
		TcpClientSocket(void);

		/** Make a connection on server at <i>ip</i>:<i>port</i>.
		 * @param ip contains a string that represent an ip address. Ex: A valid value for
		 * <i>ip</i> could be "129.88.46.229".
		 * @param port contains the targeted port.
		 * @return return 0 if successful, an error code otherwise.
		 */
		int getConnectionTo(const char *ip, int port);

		/* Send data over the socket.<br>
		 * A connection to a TCP server have to be done (via getConnectionTo() ) before
		 * calling sendData().
		 * @param buffer is a reference on a buffer that contains data to send over the
		 *	network.
		 * @param nbyteToSend contains the number of bytes of the buffer to send.
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else.
		 * @see getConnectionTo()
		 */
		//int sendData(const char *buffer, size_t nbByteToSend);

		/* Wait for incoming data from the server.
		 * @param buffer is a reference to a buffer where recieved data could be stored. This
		 *	buffer must be big enough to host incoming data.
		 * @return the number of bytes received and stored in <i>buffer</i>
		 */
		//size_t waitingForData(char *buffer);
};



/** UdpSocket is a class to encapsulate a UDP socket to send and receive datagrams
 */
class UdpSocket: public Socket
{
	protected:
		int port;
		SOCKADDR_IN local_sin;          // Local socket's address
		SOCKADDR_IN dest_sin;           // Recipient socket's address

	public:

		/** Construct a new UdpSocket object.
		 * To be able to send and recieve data, the socket must be opened via open(int port)
		 * @see open(int port)
		 */
		UdpSocket(void);

		/** Construct a new UdpSocket object, pre-configured to work on port <i>port</i>
		 * To be able to send and recieve data, the socket must be opened via open() or
		 * open(int port)
		 */
		UdpSocket(int port);

		/** Initialize and open the socket on a port specified while calling constructor
		 * UdpSocket(int port)
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 * @see UdpSocket(int port)
		 */
		int open(void);

		/** Initialize and open the socket on a port specified by <i>port</i>
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 * @see UdpSocket()
		 * @see UdpSocket(int port)
		 */
		int open(int port);

		/** Set a destination for data send with sendData(char *buffer, int nbByteToSend)
		 * @param ip contains a string that represent an ip address. Ex: A valid value for
		 *	<i>ip</i> could be "129.88.46.229".
		 * @param port contains the targeted port
		 * @see sendData(char *buffer, int nbByteToSend)
		 */
		void setRecipientAddress(const char *ip, int port);

		/** Wait for incoming data
		 * @param buffer is a reference to a buffer where recieved data could be store. It
		 *	must be big enough to host incoming data.
		 * @return the number of bytes received and stored in <i>buffer</i>
		 */
		size_t waitingForData(char *buffer);

		/** Send <i>nbByteToSend</i> of <i>buffer</i> to the location previously defined by a
		 * call to setRecipientAddress() or sendData(char *ip, int port, char *buffer, int
		 * nbByteToSend).
		 * @param buffer is a reference to the buffer where are located data to send
		 * @param nbByteToSend contains the number of bytes of buffer to send
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 */
		int sendData(const char *buffer, size_t nbByteToSend);

		/** Send <i>nbByteToSend</i> of <i>buffer</i> to the location specified with
		 * <i>ip</i> and <i>port</i>
		 * @param ip contains a string that represent an ip address. Ex: A valid value for
		 *	<i>ip</i> could be "129.88.46.229".
		 * @param port contains the targeted port
		 * @param buffer is a reference to the buffer where are located data to send
		 * @param nbByteToSend contains the number of bytes of buffer to send
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 */
		int sendData(const char *ip, int port, const char *buffer, size_t nbByteToSend);
};



/** MulticastSocket is a class to encapsulate a UDP multicast socket to send
 * datagrams to and receive datagrams from an multicast group
 */
class MulticastSocket: public UdpSocket
{
	protected:
			in_addr group_ip;

	public:

		/** Construct a new MulticastSocket object.
		 * To be able to send and recieve data, the socket must be opened via open(char
		 * *group, int port)
		 * @see open(char *group, int port)
		 */
		MulticastSocket(void);

		/** Construct a new MulticastSocket object, pre-configured to work with the
		 * multicast group <i>group</i> on port <i>port</i>. To be able to send and recieve
		 * data, the socket must be opened via open() or open(char *group, int port)
		 * @see open()
		 * @see open(char *group, int port)
		 */
		MulticastSocket(const char *group, int port);

		/** Initialize, open the socket and join the multicast group defined while calling
		 * constructor MulticastSocket(char *group, int port)
		 * @return
		 * @see MulticastSocket(char *group, int port)
		 */
		int open(void);

		/** Initialize, open the socket and join the multicast group specified by <i>group</i>
		 * on port <i>port</i>. To be able to send and recieve data, the socket must be
		 * opened via open() or open(int port)
		 */
		int open(const char *group, int port);
};


char *GetHostIp(const char *hostname);
char *GetLocalIp(void);

long GetRawLocalIp(void);

#endif
