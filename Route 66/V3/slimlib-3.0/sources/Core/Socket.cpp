#include <Slimlib/Errors.h>
#include <Slimlib/Core/Socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * default buffer size: 20 ko
 */
#define SOCKET_DEFAULT_BUFFER_SIZE  20480

/**
 * default ttl do not pass thru routers
 */
#define SOCKET_DEFAULT_MULTICAST_TTL    1

/**
 * a TCP server socket is able to enqueue 20 pending connections from clients
 */
#define SOCKET_BACKLOG                 20

/**
 * an IP address is xxx.xxx.xxx.xxx\0
 */
#define SOCKET_IP_LENGTH               16

/**
 *
 */
#define SOCKET_LOCAL_NAME_LENGTH      256


#if defined(WIN32) || defined(_WIN32_WCE)
WSADATA WSAData;
bool winsock_initialised = false;
#endif


char *GetHostIp(const char *hostname)
{
	struct hostent *phe;
	char *ip;

	#if defined(WIN32) || defined(_WIN32_WCE)
	if(!winsock_initialised)
	{
			if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
			{
					winsock_initialised = true;
			}
	}
	#endif

	if((ip = (char *) malloc(SOCKET_IP_LENGTH)) == 0) return NULL;

	if((phe = gethostbyname(hostname)) == NULL) return NULL;

	sprintf(ip, "%d.%d.%d.%d", (unsigned char)phe->h_addr[0], (unsigned char)phe->h_addr[1],
		(unsigned char)phe->h_addr[2], (unsigned char)phe->h_addr[3]);

	return ip;
}



char *GetLocalIp(void)
{
	struct hostent *phe;
	char localname[SOCKET_LOCAL_NAME_LENGTH];
	char *ip;

	#if defined(WIN32) || defined(_WIN32_WCE)
	if(!winsock_initialised)
	{
			if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
			{
					winsock_initialised = true;
			}
	}
	#endif

	if((ip = (char *) malloc(SOCKET_IP_LENGTH)) == 0) return NULL;

	#if defined(WIN32) || defined(_WIN32_WCE)
	if(gethostname(localname, sizeof(localname)) == SOCKET_ERROR) return NULL;
	#endif

	#if defined(unix) || defined(__APPLE__)
	if(gethostname(localname, sizeof(localname)) < 0) return NULL;
	#endif

	if((phe = gethostbyname(localname)) == NULL) return NULL;

	sprintf(ip, "%d.%d.%d.%d", (unsigned char)phe->h_addr[0], (unsigned char)phe->h_addr[1],
		(unsigned char)phe->h_addr[2], (unsigned char)phe->h_addr[3]);

	return ip;
}



long GetRawLocalIp(void)
{
	struct hostent *phe;
	char localname[SOCKET_LOCAL_NAME_LENGTH];
	
	#if defined(WIN32) || defined(_WIN32_WCE)
		if(!winsock_initialised)
		{
			if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
			{
				winsock_initialised = true;
			}
		}
	#endif
		
	#if defined(WIN32) || defined(_WIN32_WCE)
	if(gethostname(localname, sizeof(localname)) == SOCKET_ERROR) return 0;
	#endif
		
	#if defined(unix) || defined(__APPLE__)
	if(gethostname(localname, sizeof(localname)) < 0) return 0;
	#endif
		
	if((phe = gethostbyname(localname)) == NULL) return 0;
	
	long ip = (unsigned char) phe->h_addr[0];
	char *o = (char *) &ip;
	o[0] = phe->h_addr[0];
	o[1] = phe->h_addr[1];
	o[2] = phe->h_addr[2];
	o[3] = phe->h_addr[3];
	
	return ip;
}


Socket::Socket(void)
{
	#if defined(WIN32) || defined(_WIN32_WCE)
	if(!winsock_initialised)
	{
			if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
			{
					winsock_initialised = true;
			}
	}
	#endif
	buffer = NULL;
	socket_handle = INVALID_SOCKET;
	resize_buffer_allowed = true;
	resizeReceptionBuffer(SOCKET_DEFAULT_BUFFER_SIZE);
}


Socket::~Socket(void)
{
	closeSocket();
}


bool Socket::resizeReceptionBuffer(size_t size)
{
	if(resize_buffer_allowed)
	{
		if(buffer != NULL)
		{
			free(buffer);
		}

		buffer_size = size;
		if((buffer = (char *) malloc(buffer_size * sizeof(char))) == 0)
		{
			return false;
		}

		return true;
	}

	return false;
}


size_t Socket::getBufferSize()
{
	return buffer_size;
}

void Socket::closeSocket(void)
{
	if(socket_handle != INVALID_SOCKET)
	{
		#if defined(WIN32) || defined(_WIN32_WCE)
		shutdown (socket_handle, 0x00);
		closesocket (socket_handle);
		#endif

		#if defined(unix) || defined(__APPLE__)
		shutdown (socket_handle, SHUT_RDWR);
		close(socket_handle);
		#endif

		socket_handle = INVALID_SOCKET;
	}
}



TcpSocket::TcpSocket(void): Socket()
{
	size = 0;
	data = buffer;
}



void TcpSocket::flush(size_t nbOfBytes)
{
	if(size > nbOfBytes)
	{
		size -= nbOfBytes;
		data += nbOfBytes;
		return;
	}

	size = 0;
	data = buffer;
}



size_t TcpSocket::waitingForData(socket_t handle, char *buffer)
{
	size_t nbBytesReceived = 0;

	resize_buffer_allowed = false;

	if(!size)
	{
		#if defined(WIN32) || defined(_WIN32_WCE)
		if ((nbBytesReceived = recv(handle, this->buffer, buffer_size, 0)) == SOCKET_ERROR)
		#endif
		#if defined(unix) || defined(__APPLE__)
		if ((nbBytesReceived = recv(handle, this->buffer, buffer_size, 0)) < 0)
		#endif
		{
			resize_buffer_allowed = true;
			return RECEPTION_FAILED;
		}

		size = nbBytesReceived;
	}

	memcpy(buffer, data, size);
	resize_buffer_allowed = true;
	return size;
}



int TcpSocket::sendData(socket_t handle, const char *buffer, size_t nbByteToSend)
{
	#if defined(WIN32) || defined(_WIN32_WCE)
	if (send(handle, buffer, nbByteToSend, 0) == SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (send(handle, buffer, nbByteToSend, 0) < 0)
	#endif
	{
		return SENDDATA_FAILED;
	}

	return NO_ERROR;
}


size_t TcpSocket::waitingForData(char *buffer)
{
	return waitingForData(socket_handle, buffer);
}



int TcpSocket::sendData(const char *buffer, size_t nbByteToSend)
{
	return sendData(socket_handle, buffer, nbByteToSend);
}


void TcpSocket::setHandle(socket_t handle)
{
	socket_handle = handle;
}



void TcpConnectionHandler::onConnection(socket_t handle)
{
	printf("Initial onConnection()\n,");
}




TcpServerSocket::TcpServerSocket(int port)
{
	this->port = port;
	connection_handler = NULL;
	server_stopped = true;
}


void TcpServerSocket::setConnectionHandler(TcpConnectionHandler *connection_handler)
{
	this->connection_handler = connection_handler;
}


int TcpServerSocket::run(void)
{
	int on = 1;

	if ((socket_handle = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		return SOCKET_ALLOCATION_FAILED;
	}

	#if defined(WIN32) || defined(_WIN32_WCE)
	if (setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, (char FAR *)&on, sizeof(int))
		== SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int))
		== SOCKET_ERROR)
	#endif
	{
		return REUSEADDR_FAILED;
	}

	SOCKADDR_IN local_sin;

	local_sin.sin_family = AF_INET;         // host byte order
	local_sin.sin_port = htons(port);     // short, network byte order
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

	if (bind(socket_handle, (SOCKADDR *)&local_sin, SOCKADDR_SIZE) == SOCKET_ERROR)
	{
		return SOCKET_BINDING_FAILED;
	}

	socklen_t sin_size;
	sin_size = SOCKADDR_IN_SIZE;

	if (getsockname(socket_handle, (SOCKADDR*)&local_sin, &sin_size) != SOCKET_ERROR)
	{
		port = ntohs(local_sin.sin_port);
	}

	if (listen(socket_handle, SOCKET_BACKLOG) == SOCKET_ERROR)
	{
		return LISTENING_FAILED;
	}

	server_stopped = false;
	while(!server_stopped)
	{
		SOCKADDR_IN client_sin;
		sin_size = SOCKADDR_IN_SIZE;
		int handle;

		if ((handle = accept(socket_handle, (SOCKADDR*)&client_sin, &sin_size))
			== SOCKET_ERROR)
		{
			return ACCEPT_FAILED;
		}
		if(!server_stopped)
		{
			#ifdef __TRACE_ON
			printf("server: got connection from %s:%d\n", inet_ntoa(client_sin.sin_addr),
				ntohs(client_sin.sin_port));
			#endif

			if(connection_handler != NULL)
			{
				connection_handler->onConnection(handle);
			}
			else
			{
				#ifdef __TRACE_ON
				printf("no connection handler defined");
				#endif
			}
		}
	}

	return NO_ERROR;
}



int TcpServerSocket::getPort()
{
	return port;
}


void TcpServerSocket::stop()
{
	closeSocket();
	server_stopped = true;
}


TcpServerSocket::~TcpServerSocket()
{
	stop();
}


TcpClientSocket::TcpClientSocket(void): TcpSocket()
{
}



int TcpClientSocket::getConnectionTo(const char *ip, int port)
{
	if(port == 0) return NO_PORT_DEFINED;

	// Create a datagram socket, Sock.
	if ((socket_handle = socket (AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		return SOCKET_ALLOCATION_FAILED;
	}

	// Fill out the local socket's address information.
	server_sin.sin_family = AF_INET;
	server_sin.sin_port = htons (port);
	server_sin.sin_addr.s_addr = inet_addr(ip);

	if (connect(socket_handle, (SOCKADDR*) &server_sin, SOCKADDR_IN_SIZE) == SOCKET_ERROR)
	{
		return CONNECTION_FAILED;
	}

	return NO_ERROR;
}



// size_t TcpClientSocket::waitingForData(char *buffer)
// {
// 	return TcpSocket::waitingForData(socket_handle, buffer);
// }
//
//
//
// int TcpClientSocket::sendData(const char *buffer, size_t nbByteToSend)
// {
// 	return TcpSocket::sendData(socket_handle, buffer, nbByteToSend);
// }



UdpSocket::UdpSocket(void): Socket()
{
	port = 0;
}



UdpSocket::UdpSocket(int port): Socket()
{
	this->port = port;
}



int UdpSocket::open(void)
{
	int on = 1;

	if(port == 0) return NO_PORT_DEFINED;

	// Create a datagram socket, Sock.
	if ((socket_handle = socket (AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		//printf ("Allocating socket failed! Error: %d\n", WSAGetLastError ());
		return SOCKET_ALLOCATION_FAILED;
	}

	// we need to used multiple "server" on the same host
	#if defined(WIN32) || defined(_WIN32_WCE)
	if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEADDR, (char FAR *)&on,
		sizeof (on)) == SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEADDR, (char *)&on,
		sizeof (on)) == SOCKET_ERROR)
	#endif
	{
		return REUSEADDR_FAILED;
	};

	#ifdef SO_REUSEPORT
	#if defined(WIN32) || defined(_WIN32_WCE)
	if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEPORT, (char FAR *)&on,
		sizeof (on)) == SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEPORT, (char *)&on,
		sizeof (on)) < 0)
	#endif
	{
		return REUSEPORT_FAILED;
	}
	#endif

	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (port);
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

	// Associate the local address with socket_handle.
	#if defined(WIN32) || defined(_WIN32_WCE)
	if (bind (socket_handle, (struct sockaddr FAR *) &local_sin, sizeof (local_sin))
		== SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (bind (socket_handle, (struct sockaddr *) &local_sin, sizeof (local_sin)) < 0)
	#endif
	{
		closeSocket();
		return SOCKET_BINDING_FAILED;
	}

	return NO_ERROR;
}



int UdpSocket::open(int port)
{
	this->port = port;
	return open();
}



void UdpSocket::setRecipientAddress(const char *ip, int port)
{
	// Fill out the desination socket's address information.
	dest_sin.sin_family = AF_INET;
	dest_sin.sin_port = htons (port);
	dest_sin.sin_addr.s_addr = inet_addr (ip);
}



int UdpSocket::sendData(const char *buffer, size_t nbByteToSend)
{
	#if defined(WIN32) || defined(_WIN32_WCE)
	if(sendto(socket_handle,
						buffer,
						nbByteToSend,
						0,
						(struct sockaddr FAR *) &dest_sin,
						sizeof (dest_sin)) == SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if(sendto(socket_handle,
						buffer,
						nbByteToSend,
						0,
						(struct sockaddr *) &dest_sin,
						sizeof (dest_sin)) < 0)
	#endif
	{
		//printf ("sendto failed! Error: %d\n", WSAGetLastError ());
		return SENDDATA_FAILED;
	}

	return NO_ERROR;
}



int UdpSocket::sendData(const char *ip, int port, const char *buffer, size_t nbByteToSend)
{
	setRecipientAddress(ip, port);
	return sendData(buffer, nbByteToSend);
}



size_t UdpSocket::waitingForData(char *buffer)
{
	SOCKADDR_IN recv_sin;                // Holds the source address on
																				// recvfrom function returns
	socklen_t iRecvLen;
	ssize_t nbBytesReceived;

	iRecvLen = sizeof (recv_sin);    // Length of recv_sin

	resize_buffer_allowed = false;

	#if defined(WIN32) || defined(_WIN32_WCE)
	nbBytesReceived =
		recvfrom (socket_handle,
			this->buffer,
			buffer_size,
			0,
			(struct sockaddr FAR *) &recv_sin,
			(int FAR *) &iRecvLen);
	#endif
	#if defined(unix) || defined(__APPLE__)
	nbBytesReceived =
		recvfrom (socket_handle,
			this->buffer,
			buffer_size,
			0,
			(struct sockaddr *) &recv_sin,
			(socklen_t *) &iRecvLen);
	#endif

	if (nbBytesReceived == SOCKET_ERROR)
	{
		closeSocket();
		resize_buffer_allowed = true;
		return RECEPTION_FAILED;
	}
	else
	{
		memcpy(buffer, this->buffer, nbBytesReceived);
		resize_buffer_allowed = true;
		return (size_t) nbBytesReceived;
	}
}



MulticastSocket::MulticastSocket(void): UdpSocket()
{
	#if defined(WIN32) || defined(_WIN32_WCE)
	group_ip.S_un.S_addr = 0;
	#endif
	#if defined(unix) || defined(__APPLE__)
	group_ip.s_addr = 0;
	#endif
}

MulticastSocket::MulticastSocket(const char *group, int port): UdpSocket(port)
{
	#if defined(WIN32) || defined(_WIN32_WCE)
	group_ip.S_un.S_addr = inet_addr(group);
	#endif
	#if defined(unix) || defined(__APPLE__)
	group_ip.s_addr = inet_addr(group);
	#endif
}


int MulticastSocket::open(const char *group, int port)
{
	this->port = port;
	#if defined(WIN32) || defined(_WIN32_WCE)
	group_ip.S_un.S_addr = inet_addr(group);
	#endif
	#if defined(unix) || defined(__APPLE__)
	group_ip.s_addr = inet_addr(group);
	#endif
	return open();
}

int MulticastSocket::open(void)
{

	unsigned int ttl = SOCKET_DEFAULT_MULTICAST_TTL;

	#if defined(WIN32) || defined(_WIN32_WCE)
	if(group_ip.S_un.S_addr == 0) return NO_GROUP_DEFINED;
	#endif
	#if defined(unix) || defined(__APPLE__)
	if(group_ip.s_addr == 0) return NO_GROUP_DEFINED;
	#endif

	UdpSocket::open();

	// Join the multicast group from which to receive datagrams.
	#if defined(WIN32) || defined(_WIN32_WCE)
	mreq.imr_multiaddr.s_addr = group_ip.S_un.S_addr;
	#endif
	#if defined(unix) || defined(__APPLE__)
	mreq.imr_multiaddr.s_addr = group_ip.s_addr;
	#endif
	mreq.imr_interface.s_addr = INADDR_ANY;

	#if defined(WIN32) || defined(_WIN32_WCE)
	if (setsockopt (socket_handle,
									IPPROTO_IP,
									IP_ADD_MEMBERSHIP,
									(char FAR *)&mreq,
									sizeof (mreq)) == SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (setsockopt (socket_handle,
									IPPROTO_IP,
									IP_ADD_MEMBERSHIP,
									(char *)&mreq,
									sizeof (mreq)) == SOCKET_ERROR)
	#endif
	{
		closeSocket();
		return SOCKET_JOINING_FAILED;
	}

	// Set the default Time-To-Live of the multicast
	#if defined(WIN32) || defined(_WIN32_WCE)
	if (setsockopt (socket_handle, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *)&ttl,
		sizeof(ttl)) == SOCKET_ERROR)
	#endif
	#if defined(unix) || defined(__APPLE__)
	if (setsockopt (socket_handle, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl,
		sizeof(ttl)) == SOCKET_ERROR)
	#endif
	{
		closeSocket();
		return TTL_SETTING_FAILED;
	}

	return NO_ERROR;
}
