#include "../includes/slimsocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) || defined(_WIN32_WCE)
WSADATA WSAData;
bool winsock_initialised = false;
#endif


char *slimGetHostIp(const char *hostname)
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

    ip = new char[SLIMSOCKET_IP_LENGTH];//(char *) malloc(SLIMSOCKET_IP_LENGTH))
    if(ip == NULL) return NULL;

    if((phe = gethostbyname(hostname)) == NULL) return NULL;

    sprintf_s(ip, strlen(ip)+1, "%d.%d.%d.%d", (unsigned char)phe->h_addr[0], (unsigned char)phe->h_addr[1], (unsigned char)phe->h_addr[2], (unsigned char)phe->h_addr[3]);

    return ip;
}



char *slimGetLocalIp(void)
{
    struct hostent *phe;
    char localname[SLIMSOCKET_LOCAL_NAME_LENGTH];
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
    ip = new char[SLIMSOCKET_IP_LENGTH]; //(char *) malloc(SLIMSOCKET_IP_LENGTH)
    if(ip == NULL) return NULL;

    #if defined(WIN32) || defined(_WIN32_WCE)
    if(gethostname(localname, sizeof(localname)) == SOCKET_ERROR) return NULL;
    #endif

    #if defined(unix) || defined(__APPLE__)
    if(gethostname(localname, sizeof(localname)) < 0) return NULL;
    #endif

    if((phe = gethostbyname(localname)) == NULL) return NULL;

    sprintf_s(ip, strlen(ip)+1, "%d.%d.%d.%d", (unsigned char)phe->h_addr[0], (unsigned char)phe->h_addr[1], (unsigned char)phe->h_addr[2], (unsigned char)phe->h_addr[3]);

    return ip;
}


void SlimSocket::init_SlimSocket()
{buffer = NULL;
}

SlimSocket::SlimSocket(void)
{   init_SlimSocket();
    #if defined(WIN32) || defined(_WIN32_WCE)
    if(!winsock_initialised)
    {
        if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
        {
            winsock_initialised = true;
        }
    }
    #endif

    socket_handle = INVALID_SOCKET;
    resize_buffer_allowed = true;
    resizeReceptionBuffer(SLIMSOCKET_DEFAULT_BUFFER_SIZE);
}


SlimSocket::~SlimSocket(void)
{
    closeSocket();
}


bool SlimSocket::resizeReceptionBuffer(int size)
{
    if(resize_buffer_allowed)
    {
        if(buffer != NULL)
		{
		 delete[] buffer;
		}

        buffer_size = size;
        buffer = new char[buffer_size]; //(char *) malloc(buffer_size * sizeof(char))
        if(buffer == NULL) return false;
        return true;
    }
    else return false;
}


void SlimSocket::closeSocket(void)
{
    #if defined(WIN32) || defined(_WIN32_WCE)
    shutdown (socket_handle, 0x00);
    closesocket (socket_handle);
    #endif

    #if defined(unix) || defined(__APPLE__)
    close(socket_handle);
    #endif

}



SlimTcpSocket::SlimTcpSocket(void): SlimSocket()
{

}



int SlimTcpSocket::waitingForData(char *tempon, const unsigned int taille, const unsigned int pos, const bool add_zero)
{
    unsigned int nbBytesReceived, flags;
    flags = 0;
    resize_buffer_allowed = false;

    #if defined(WIN32) || defined(_WIN32_WCE)
    if ((nbBytesReceived = recv(socket_handle, /*this->buffer*/tempon + pos, /*buffer_size*/taille-pos, flags)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if ((nbBytesReceived = recv(socket_handle, /*this->buffer*/tempon + pos, /*buffer_size*/taille-pos, flags)) < 0)
    #endif
    {
        resize_buffer_allowed = true;
        return SLIMSOCKET_RECEPTION_FAILED;
    }
    else
    {
        //memcpy(buffer + pos, this->buffer, nbBytesReceived);
        if(add_zero) tempon[nbBytesReceived] = 0;
        resize_buffer_allowed = true;
        return (int) nbBytesReceived;
    }
}



int SlimTcpSocket::sendData(const char *tempon, int nbByteToSend)
{
    #if defined(WIN32) || defined(_WIN32_WCE)
    if (send(socket_handle, tempon, nbByteToSend, 0) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (send(socket_handle, tempon, nbByteToSend, 0) < 0)
    #endif
    {
        return SLIMSOCKET_SENDDATA_FAILED;
    }

    return SLIMSOCKET_NOERROR;
}


int SlimTcpSocket::waitingForData(int handle, char *tempon, const unsigned int taille, const unsigned int pos, const bool add_zero)
{
    unsigned int nbBytesReceived
               , flags;
   flags = 0;

    resize_buffer_allowed = false;

    #if defined(WIN32) || defined(_WIN32_WCE)
    if ((nbBytesReceived = recv(handle, tempon + pos, taille-pos, flags)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if ((nbBytesReceived = recv(handle, tempon + pos, taille-pos, flags)) < 0)
    #endif
    {
        resize_buffer_allowed = true;
        return SLIMSOCKET_RECEPTION_FAILED;
    }
    else
    {
        //memcpy(buffer, this->buffer, nbBytesReceived);
        if(add_zero) tempon[nbBytesReceived] = 0;
        resize_buffer_allowed = true;
        return (int) nbBytesReceived;
    }
}



int SlimTcpSocket::sendData(int handle, const char *tempon, int nbByteToSend)
{
    #if defined(WIN32) || defined(_WIN32_WCE)
    if (send(handle, tempon, nbByteToSend, 0) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (send(handle, tempon, nbByteToSend, 0) < 0)
    #endif
    {
        return SLIMSOCKET_SENDDATA_FAILED;
    }

    return SLIMSOCKET_NOERROR;
}


void SlimTcpSocket::setHandle(int handle)
{
    socket_handle = (SOCKET) handle;
}



void SlimTcpConnectionHandler::onConnection(int handle)
{
    printf("Initial onConnection()\n,");
}



SlimTcpServerSocket::SlimTcpServerSocket(int port, SlimTcpConnectionHandler *c)
{
    this->port               = port;
    this->connection_handler = c;
}


void SlimTcpServerSocket::setConnectionHandler(SlimTcpConnectionHandler *connection_handler)
{
    this->connection_handler = connection_handler;
}


int SlimTcpServerSocket::start(void)
{
    struct sockaddr_in client_sin;
    socklen_t sin_size;
    int on = 1;
    int handle;

    server_stopped = false;

    #if defined(WIN32) || defined(_WIN32_WCE)
    if ((socket_handle = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if ((socket_handle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    #endif
    {
        return SLIMSOCKET_ALLOCATING_FAILED;
    }

    #if defined(WIN32) || defined(_WIN32_WCE)
    if (setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, (char FAR *)&on, sizeof(int)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
    #endif
    {
        return SLIMSOCKET_REUSEADDR_FAILED;
    }

    local_sin.sin_family = AF_INET;         // host byte order
    local_sin.sin_port = htons(port);     // short, network byte order
    local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

    #if defined(WIN32) || defined(_WIN32_WCE)
    if (bind(socket_handle, (SOCKADDR*)&local_sin, sizeof(struct sockaddr)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (bind(socket_handle, (struct sockaddr *)&local_sin, sizeof(struct sockaddr)) < 0)
    #endif
    {
        return SLIMSOCKET_BINDING_FAILED;
    }

    #if defined(WIN32) || defined(_WIN32_WCE)
    if (listen(socket_handle, SLIMSOCKET_BACKLOG) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (listen(socket_handle, SLIMSOCKET_BACKLOG) < 0)
    #endif
    {
        return SLIMSOCKET_LISTENING_FAILED;
    }

    while(!server_stopped)
    {
        sin_size = sizeof(struct sockaddr_in);

        #if defined(WIN32) || defined(_WIN32_WCE)
        if ((handle = accept(socket_handle, (SOCKADDR*)&client_sin, &sin_size)) == SOCKET_ERROR)
        #endif
        #if defined(unix) || defined(__APPLE__)
        if ((handle = accept(socket_handle, (struct sockaddr *)&client_sin, &sin_size)) < 0)
        #endif
        {
            return SLIMSOCKET_ACCEPT_FAILED;
        }

        #ifdef __TRACE_ON
        printf("server: got connection from %s\n", inet_ntoa(client_sin.sin_addr));
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

    return SLIMSOCKET_NOERROR;
}



SlimTcpClientSocket::SlimTcpClientSocket(void): SlimTcpSocket()
{

}



int SlimTcpClientSocket::getConnectionTo(const char *ip, int port)
{
    if(port == 0) return SLIMSOCKET_NO_PORT_DEFINED;

    // Create a datagram socket, Sock.
    #if defined(WIN32) || defined(_WIN32_WCE)
    if ((socket_handle = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET )
    #endif
    #if defined(unix) || defined(__APPLE__)
    if ((socket_handle = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
    #endif
    {
        return SLIMSOCKET_ALLOCATING_FAILED;
    }

    // Fill out the local socket's address information.
    server_sin.sin_family = AF_INET;
    server_sin.sin_port = htons (port);
    server_sin.sin_addr.s_addr = inet_addr(ip);

    #if defined(WIN32) || defined(_WIN32_WCE)
    if (connect(socket_handle, (SOCKADDR*) &server_sin, sizeof(server_sin/*struct sockaddr*/)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (connect(socket_handle, (struct sockaddr *) &server_sin, sizeof(struct sockaddr)) < 0)
    #endif
    {
        return SLIMSOCKET_CONNECTION_FAILED;
    }

    return SLIMSOCKET_NOERROR;
}



int SlimTcpClientSocket::waitingForData(char *tempon, const unsigned int taille, const unsigned int pos, const bool add_zero)
{
    return SlimTcpSocket::waitingForData(socket_handle, tempon, taille, pos, add_zero);
}



int SlimTcpClientSocket::sendData(const char *tempon, int nbByteToSend)
{
    return SlimTcpSocket::sendData(socket_handle, tempon, nbByteToSend);
}



SlimUdpSocket::SlimUdpSocket(void): SlimSocket()
{
    port = 0;
}



SlimUdpSocket::SlimUdpSocket(int port): SlimSocket()
{
    this->port = port;
}



int SlimUdpSocket::open(void)
{
    int on = 1;

    if(port == 0) return SLIMSOCKET_NO_PORT_DEFINED;

    // Create a datagram socket, Sock.
    #if defined(WIN32) || defined(_WIN32_WCE)
    if ((socket_handle = socket (AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if ((socket_handle = socket (AF_INET, SOCK_DGRAM, 0)) < 0 )
    #endif
    {
        //printf ("Allocating socket failed! Error: %d\n", WSAGetLastError ());
        return SLIMSOCKET_ALLOCATING_FAILED;
    }

    // we need to used multiple "server" on the same host
    #if defined(WIN32) || defined(_WIN32_WCE)
    if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEADDR, (char FAR *)&on, sizeof (on)) == INVALID_SOCKET)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof (on)) < 0)
    #endif
    {
        return SLIMSOCKET_REUSEADDR_FAILED;
    };

    #ifdef SO_REUSEPORT
    #if defined(WIN32) || defined(_WIN32_WCE)
    if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEPORT, (char FAR *)&on, sizeof (on)) == INVALID_SOCKET)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (setsockopt (socket_handle, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof (on)) < 0)
    #endif
    {
        return SLIMSOCKET_REUSEPORT_FAILED;
    }
    #endif

    // Fill out the local socket's address information.
    local_sin.sin_family = AF_INET;
    local_sin.sin_port = htons (port);
    local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

    // Associate the local address with socket_handle.
    #if defined(WIN32) || defined(_WIN32_WCE)
    if (bind (socket_handle, (struct sockaddr FAR *) &local_sin, sizeof (local_sin)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (bind (socket_handle, (struct sockaddr *) &local_sin, sizeof (local_sin)) < 0)
    #endif
    {
        //printf ("Binding socket failed! Error: %d\n", WSAGetLastError ());
        #if defined(WIN32) || defined(_WIN32_WCE)
        closesocket (socket_handle);
        #endif
        #if defined(unix) || defined(__APPLE__)
        close(socket_handle);
        #endif
        return SLIMSOCKET_BINDING_FAILED;
    }

    return SLIMSOCKET_NOERROR;
}



int SlimUdpSocket::open(int port)
{
    this->port = port;
    return open();
}



void SlimUdpSocket::setRecipientAddress(const char *ip, int port)
{
  // Fill out the desination socket's address information.
  dest_sin.sin_family = AF_INET;
  dest_sin.sin_port = htons (port);
  dest_sin.sin_addr.s_addr = inet_addr (ip);
}



int SlimUdpSocket::sendData(const char *tempon, int nbByteToSend)
{
    #if defined(WIN32) || defined(_WIN32_WCE)
    if(sendto(socket_handle,
              tempon,
              nbByteToSend,
              0,
              (struct sockaddr FAR *) &dest_sin,
              sizeof (dest_sin)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if(sendto(socket_handle,
              tempon,
              nbByteToSend,
              0,
              (struct sockaddr *) &dest_sin,
              sizeof (dest_sin)) < 0)
    #endif
    {
        //printf ("sendto failed! Error: %d\n", WSAGetLastError ());
        return SLIMSOCKET_SENDDATA_FAILED;
    }

    return SLIMSOCKET_NOERROR;
}



int SlimUdpSocket::sendData(const char *ip, int port, const char *tempon, int nbByteToSend)
{
    setRecipientAddress(ip, port);
    return sendData(tempon, nbByteToSend);
}



int SlimUdpSocket::waitingForData(char *tempon)
{
    SOCKADDR_IN recv_sin;                // Holds the source address on
                                         // recvfrom function returns
    socklen_t iRecvLen;
    int nbBytesReceived;

   iRecvLen = sizeof (recv_sin);    // Length of recv_sin

    resize_buffer_allowed = false;
    #if defined(WIN32) || defined(_WIN32_WCE)
    nbBytesReceived = recvfrom (socket_handle,
					this->buffer,
					buffer_size,
					0,
					(struct sockaddr FAR *) &recv_sin,
					(int FAR *) &iRecvLen);
    if (nbBytesReceived == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    nbBytesReceived = recvfrom (socket_handle,
					this->buffer,
					buffer_size,
					0,
					(struct sockaddr *) &recv_sin,
					(socklen_t *) &iRecvLen);
    if (nbBytesReceived < 0)
    #endif
    {
        //printf ("recvfrom failed! Error: %d\n", WSAGetLastError ());
        #if defined(WIN32) || defined(_WIN32_WCE)
        closesocket (socket_handle);
        #endif
        #if defined(unix) || defined(__APPLE__)
        close(socket_handle);
        #endif
        resize_buffer_allowed = true;
        return SLIMSOCKET_RECEPTION_FAILED;
    }
    else
    {
        memcpy(tempon, this->buffer, nbBytesReceived);
        resize_buffer_allowed = true;
        return nbBytesReceived;
    }
}



SlimMulticastSocket::SlimMulticastSocket(void): SlimUdpSocket()
{
    #if defined(WIN32) || defined(_WIN32_WCE)
    group_ip.S_un.S_addr = 0;
    #endif
    #if defined(unix) || defined(__APPLE__)
    group_ip.s_addr = 0;
    #endif
}

SlimMulticastSocket::SlimMulticastSocket(const char *group, int port): SlimUdpSocket(port)
{
    #if defined(WIN32) || defined(_WIN32_WCE)
    group_ip.S_un.S_addr = inet_addr(group);
    #endif
    #if defined(unix) || defined(__APPLE__)
    group_ip.s_addr = inet_addr(group);
    #endif
}


int SlimMulticastSocket::open(const char *group, int port)
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

int SlimMulticastSocket::open(void)
{

    unsigned int ttl = SLIMSOCKET_DEFAULT_MULTICAST_TTL;

    #if defined(WIN32) || defined(_WIN32_WCE)
    if(group_ip.S_un.S_addr == 0) return SLIMSOCKET_NO_GROUP_DEFINED;
    #endif
    #if defined(unix) || defined(__APPLE__)
    if(group_ip.s_addr == 0) return SLIMSOCKET_NO_GROUP_DEFINED;
    #endif

    SlimUdpSocket::open();

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
                    sizeof (mreq)) < 0)
    #endif
    {
        printf ("setsockopt failed! Error: %d\n", WSAGetLastError ());
        #if defined(WIN32) || defined(_WIN32_WCE)
        closesocket (socket_handle);
        #endif
        #if defined(unix) || defined(__APPLE__)
        close(socket_handle);
        #endif
        return SLIMSOCKET_JOINING_FAILED;
    }

    // Set the default Time-To-Live of the multicast
    #if defined(WIN32) || defined(_WIN32_WCE)
    if (setsockopt (socket_handle, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *)&ttl, sizeof(ttl)) == SOCKET_ERROR)
    #endif
    #if defined(unix) || defined(__APPLE__)
    if (setsockopt (socket_handle, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl)) < 0)
    #endif
    {
        #if defined(WIN32) || defined(_WIN32_WCE)
        closesocket (socket_handle);
        #endif
        #if defined(unix) || defined(__APPLE__)
        close(socket_handle);
        #endif
        return SLIMSOCKET_TTL_SETTING_FAILED;
    }

    return SLIMSOCKET_NOERROR;
}
