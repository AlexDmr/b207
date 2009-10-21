#ifndef _ROUTE66TCPGATEWAY_
#define _ROUTE66TCPGATEWAY_

#include <route66.h>

#define R66TCPGATEWAY_NOERROR		0

//Tag definitions
#define ROUTE66TCPGATEWAY_SEND "send"
#define ROUTE66TCPGATEWAY_SEND_AND_WAIT "send_and_wait"
#define ROUTE66TCPGATEWAY_MESSAGE "message"
#define ROUTE66TCPGATEWAY_REGEXP "answer_regexp"

// Error messages
#define ROUTE66TCPGATEWAY_ERROR_PARSING "Route66TcpGateway_ERROR: WHILE PARSING RECEIVED DATA."
#define ROUTE66TCPGATEWAY_ERROR_SEND_TAG "Route66TcpGateway: ERROR: SEND METHOD NOT RECOGNIZED."
#define ROUTE66TCPGATEWAY_ERROR_MESSAGE_TAG "Route66TcpGateway_ERROR: MESSAGE TAG NOT FOUND."
#define ROUTE66TCPGATEWAY_ERROR_REGEXP_TAG "Route66TcpGateway_ERROR: REGEXP TAG NOT FOUND."
#define ROUTE66TCPGATEWAY_ERROR_CONTENT_TAG "Route66TcpGateway_ERROR: CONTENT TAG NOT FOUND."
#define ROUTE66TCPGATEWAY_ERROR_ROUTE66 "Route66TcpGateway_ERROR: ROUTE66 IS NOT REACHEABLE."

class TcpReceiver: public SlimTcpConnectionHandler
{
    private:
        Route66 *r66;
		SlimXmlParser xmlParser;
        char buffer[SLIMSOCKET_DEFAULT_BUFFER_SIZE];
        
    public:
		TcpReceiver(Route66 *r66);
		void onConnection(int handle);
};


class Route66TcpGateway: public SlimThread
{
    private:
        SlimTcpServerSocket *tcp;
        int port;
        TcpReceiver *tcpReceiver;

		int lastError;
        
    public:
        Route66TcpGateway(int port, Route66 *r66);
        virtual ~Route66TcpGateway();
        virtual void run();

		int getLastError(void) const;
};

#endif
