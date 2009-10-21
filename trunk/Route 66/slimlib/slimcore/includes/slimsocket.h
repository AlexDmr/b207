/*
    Slimsocket for Posix compliant OS
    Cross-Platform Portable Library for socket networking programming
*/

#ifndef _SLIMSOCKET_
#define _SLIMSOCKET_

#if defined(unix) || defined(__APPLE__)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>

	#define SOCKET int
	#define SOCKADDR_IN struct sockaddr_in
	#define INVALID_SOCKET 0
#endif

#if (defined(WIN32) || defined(_WIN32))
  #include <winsock.h>
#endif

#ifdef _WIN32_WCE
	#include <Afxsock.h>
#endif

#ifdef __APPLE__
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
#endif

#if defined(WIN32) || defined(_WIN32_WCE) || defined(__APPLE__)
    typedef int socklen_t;
#endif

#define SLIMSOCKET_DEFAULT_BUFFER_SIZE  65536    //default buffer size: 20 ko
#define SLIMSOCKET_DEFAULT_MULTICAST_TTL    1    //default ttl do not pass through routers
#define SLIMSOCKET_BACKLOG                 20    // a TCP server socket is able to enqueue 20 pending connections from clients
#define SLIMSOCKET_IP_LENGTH               16    // an IP address is xxx.xxx.xxx.xxx\0
#define SLIMSOCKET_LOCAL_NAME_LENGTH      256

#define SLIMSOCKET_NOERROR                  0
#define SLIMSOCKET_NO_PORT_DEFINED          1
#define SLIMSOCKET_NO_GROUP_DEFINED         2
#define SLIMSOCKET_ALLOCATING_FAILED        3
#define SLIMSOCKET_BINDING_FAILED           4
#define SLIMSOCKET_JOINING_FAILED           5
#define SLIMSOCKET_RECEPTION_FAILED         -1
#define SLIMSOCKET_REUSEADDR_FAILED         7
#define SLIMSOCKET_REUSEPORT_FAILED         8
#define SLIMSOCKET_TTL_SETTING_FAILED       9
#define SLIMSOCKET_SENDDATA_FAILED          10
#define SLIMSOCKET_CONNECTION_FAILED        11
#define SLIMSOCKET_ACCEPT_FAILED            12
#define SLIMSOCKET_LISTENING_FAILED         13



/**
 * @brief SlimSocket is a base class to encapsulate network sockets
 */
/**
 * This class could not be used as is. You should deal with more specialized classes
 * @see SlimTcpServerSocket
 * @see SlimTcpClientSocket
 * @see SlimUdpSocket
 * @see SlimMulticastSocket
 */
class SlimSocket
{
    protected:
		SOCKET socket_handle ;

        struct ip_mreq mreq;            // Used in adding or dropping
        char *buffer;
        int buffer_size;
        bool resize_buffer_allowed;

        void init_SlimSocket();

    public:
       SlimSocket(void);
       ~SlimSocket(void);
       bool resizeReceptionBuffer(int size);
       const unsigned int get_resizeReceptionBuffer() const {return buffer_size;}
       void closeSocket(void);
};




/**
 * @brief SlimSocket is a base class to encapsulate TCP network sockets
 */
/**
 * This class could not be used as is. You should deal with more specialized classes
 * @see SlimTcpServerSocket
 * @see SlimTcpClientSocket
 */
class SlimTcpSocket: public SlimSocket
{
    public:
        SlimTcpSocket(void);
        void setHandle(int handle);
        int sendData(int handle, const char *buffer, int nbByteToSend);
        int sendData(const char *buffer, int nbByteToSend);
        int waitingForData(int handle, char *buffer, const unsigned int taille, const unsigned int pos = 0, const bool add_zero = false);
        int waitingForData(char *buffer, const unsigned int taille, const unsigned int pos = 0, const bool add_zero = false);
};




/**
 * @brief SlimTcpConnectionHandler is a class to encapsulate the code to execute when a connection occurs on a SlimTcpServerSocket
 */
/**
 * @see SlimTcpServerSocket
 */
class SlimTcpConnectionHandler
{
    protected:
        SlimTcpSocket socket;

    public:
        SlimTcpConnectionHandler() { }
        virtual ~SlimTcpConnectionHandler() { }



        /**
		 * onConnection() contains the code to execute when a connection occurs on a SlimTcpServerSocket.
		 * @param handle is the handle of the socket returned when the incoming connection was accepted.
		 * With this <i>handle</i> and the protected member <i>socket</i>You will be able to exchange data with the remote client
		 * @see setConnectionHandler() in SlimTcpServerSocket
		 */
        virtual void onConnection(int handle);

        void closeSocket () {socket.closeSocket();}
};



/**
 * @brief SlimTcpServerSocket is a class to encapsulate a TCP server socket
 */
class SlimTcpServerSocket: public SlimTcpSocket
{
        protected:
            int server_stopped;

			#if defined (unix) || defined (__APPLE__)
            struct sockaddr_in local_sin;
			#endif

			#ifdef WIN32
			SOCKADDR_IN local_sin;
			#endif

            int port;
            SlimTcpConnectionHandler *connection_handler;

        public:

          /**
		   * Construct a SlimTcpServerSocket, ready to listen on port <i>port</i>
		   * @param port contains the port to listen
		   */
           SlimTcpServerSocket(int port, SlimTcpConnectionHandler *c = NULL);
           ~SlimTcpServerSocket(void) { }



          /**
		   * Set a connection handler. When a incoming connection from a client occurs, onConnection() method of <i>connection_handler</i> will be called
		   * @param connection_handler is a reference to a instance of SlimTcpConnectionHandler
		   */
           void setConnectionHandler(SlimTcpConnectionHandler *connection_handler);



          /**
	       * Run the Tcp server.
	       * @return 0 (SLIMSOCKET_NOERROR) if exit normaly, the appropriate error code else.
	       */
           int start(void);
};





/**
 * @brief SlimTcpClientSocket is a class to encapsulate a TCP client socket
 */
class SlimTcpClientSocket: public SlimTcpSocket
{
    protected:
		#if defined (unix) || defined (__APPLE__)
        struct sockaddr_in server_sin;
		#endif

		#ifdef WIN32
		struct sockaddr_in server_sin; //SOCKADDR_IN server_sin;
		#endif

    public:
        SlimTcpClientSocket(void);

        /**
		 * Make a connection on server at <i>ip</i>:<i>port</i>.
		 * @param ip contains a string that represent an ip address. Ex: A valid value for <i>ip</i> could be "129.88.46.229".
		 * @param port contains the targeted port.
		 */
        int getConnectionTo(const char *ip, int port);



       /**
		 * Send data over the socket.<br>
		 * A connection to a TCP server have to be done (via getConnectionTo() ) before calling sendData()
		 * @param buffer is a reference on a buffer that contains data to send over the network.
		 * @param nbyteToSend contains the number of bytes of the buffer to send.
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else.
		 * @see getConnectionTo()
		 * @see #defines in slimsocket.h for error codes description
		 */
        int sendData(const char *buffer, int nbByteToSend);



        /**
		 * Wait for incoming data from the server.
		 * @param buffer is a reference to a buffer where recieved data could be stored. This buffer must be big enough to host incoming data.
		 * @return the number of bytes received and stored in <i>buffer</i>
		 */
        int waitingForData(char *buffer, const unsigned int taille, const unsigned int pos = 0, const bool add_zero = false);

};



/**
 * @brief SlimUdpSocket is a class to encapsulate a UDP socket to send and receive datagrams
 */
class SlimUdpSocket: public SlimSocket
{
    protected:
        int port;
		SOCKADDR_IN local_sin;          // Local socket's address
        SOCKADDR_IN dest_sin;           // Recipient socket's address

    public:

    	/**
		 * Construct a new SlimUdpSocket object.
		 * To be able to send and recieve data, the socket must be opened via open(int port)
		 * @see open(int port)
		 */
        SlimUdpSocket(void);



        /**
		 * Construct a new SlimUdpSocket object, pre-configured to work on port <i>port</i>
		 * To be able to send and recieve data, the socket must be opened via open() or open(int port)
		 */
        SlimUdpSocket(int port);


       	/**
		 * Initialize and open the socket on a port specified while calling constructor SlimUdpSocket(int port)
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 * @see SlimUdpSocket(int port)
		 */
        int open(void);



        /**
		 * Initialize and open the socket on a port specified by <i>port</i>
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 * @see SlimUdpSocket()
		 * @see SlimUdpSocket(int port)
		 */
        int open(int port);



        /**
		 * Set a destination for data send with sendData(char *buffer, int nbByteToSend)
		 * @param ip contains a string that represent an ip address. Ex: A valid value for <i>ip</i> could be "129.88.46.229".
		 * @param port contains the targeted port
		 * @see sendData(char *buffer, int nbByteToSend)
		 */
        void setRecipientAddress(const char *ip, int port);



        /**
		 * Wait for incoming data
		 * @param buffer is a reference to a buffer where recieved data could be store. It must be big enough to host incoming data.
		 * @return the number of bytes received and stored in <i>buffer</i>
		 */
        int waitingForData(char *buffer);



        /**
		 * Send <i>nbByteToSend</i> of <i>buffer</i> to the location previously defined by a call to setRecipientAddress() or sendData(char *ip, int port, char *buffer, int nbByteToSend).
		 * @param buffer is a reference to the buffer where are located data to send
		 * @param nbByteToSend contains the number of bytes of buffer to send
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 */
        int sendData(const char *buffer, int nbByteToSend);



        /**
		 * Send <i>nbByteToSend</i> of <i>buffer</i> to the location specified with <i>ip</i> and <i>port</i>
		 * @param ip contains a string that represent an ip address. Ex: A valid value for <i>ip</i> could be "129.88.46.229".
		 * @param port contains the targeted port
		 * @param buffer is a reference to the buffer where are located data to send
		 * @param nbByteToSend contains the number of bytes of buffer to send
		 * @return 0 (SLIMSOCKET_NOERROR) if no error occurs, an error code else
		 */
        int sendData(const char *ip, int port, const char *buffer, int nbByteToSend);
};



/**
 * @brief SlimMulticastSocket is a class to encapsulate a UDP multicast socket to send datagrams to and receive datagrams from an multicast group
 */
class SlimMulticastSocket: public SlimUdpSocket
{
    protected:
        in_addr group_ip;

    public:

        /**
		 * Construct a new SlimMulticastSocket object.
		 * To be able to send and recieve data, the socket must be opened via open(char *group, int port)
		 * @see open(char *group, int port)
		 */
        SlimMulticastSocket(void);



        /**
		 * Construct a new SlimMulticastSocket object, pre-configured to work with the multicast group <i>group</i> on port <i>port</i>
		 * To be able to send and recieve data, the socket must be opened via open() or open(char *group, int port)
		 * @see open()
		 * @see open(char *group, int port)
		 */
        SlimMulticastSocket(const char *group, int port);



        /**
		 * Initialize, open the socket and join the multicast group defined while calling constructor SlimMulticastSocket(char *group, int port)
		 * @return
		 * @see SlimMulticastSocket(char *group, int port)
		 */
        int open(void);



        /**
		 * Initialize, open the socket and join the multicast group specified by <i>group</i> on port <i>port</i>
		 * To be able to send and recieve data, the socket must be opened via open() or open(int port)
		 */
        int open(const char *group, int port);
};


char *slimGetHostIp(const char *hostname);
char *slimGetLocalIp(void);

#endif
