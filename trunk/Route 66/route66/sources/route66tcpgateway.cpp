#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
   #include <unistd.h>
#endif
#include <route66tcpgateway.h>


TcpReceiver::TcpReceiver(Route66 *r66): SlimTcpConnectionHandler()
{
    this->r66 = r66;
}


void TcpReceiver::onConnection(int handle)
{
    int len;
    XmlNode *packet;
    XmlNode *message;
    XmlNode *content;
    XmlNode *regexp;
    char *answer;

    socket.setHandle(handle);
    len = socket.waitingForData(buffer, socket.get_resizeReceptionBuffer());
    buffer[len] = 0;

    #ifdef __TRACE_ON
    printf("#onConnection: packet received: %s\n", buffer);
    #endif

    if((packet = xmlParser.parse((const char *)(buffer))) == NULL)
    {
		socket.sendData(ROUTE66TCPGATEWAY_ERROR_PARSING, strlen(ROUTE66TCPGATEWAY_ERROR_PARSING));
		socket.closeSocket();
		return;
    }

    const char *send_method = packet->getName();
    if(strcmp(send_method, ROUTE66TCPGATEWAY_SEND) == 0)
    {

	if((message = packet->getChildNodeByName(ROUTE66TCPGATEWAY_MESSAGE)) == NULL)
	{
	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_MESSAGE_TAG, strlen(ROUTE66TCPGATEWAY_ERROR_MESSAGE_TAG));
	    socket.closeSocket();
	    return;
	}

	if((content = message->getFirstChildNode()) == NULL)
	{
	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_CONTENT_TAG, strlen(ROUTE66TCPGATEWAY_ERROR_CONTENT_TAG));
	    socket.closeSocket();
	    return;
	}

	//DEMO:
	//    printf(">>> WEB SERVER -> CAMELEON RT <<<");

	#ifdef __TRACE_ON
	printf("%s\n", content->serialize());
	#endif

	if(!r66->sendMessage((const char*) content->serialize()))
	{
	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_ROUTE66, strlen(ROUTE66TCPGATEWAY_ERROR_ROUTE66));
	    socket.closeSocket();
	    return;
	}
    }
    else
    if(strcmp(send_method, ROUTE66TCPGATEWAY_SEND_AND_WAIT) == 0)
    {
	if((message = packet->getChildNodeByName(ROUTE66TCPGATEWAY_MESSAGE)) == NULL)
	{
	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_MESSAGE_TAG, strlen(ROUTE66TCPGATEWAY_ERROR_MESSAGE_TAG));
	    socket.closeSocket();
	    return;
	}

	if((regexp = packet->getChildNodeByName(ROUTE66TCPGATEWAY_REGEXP)) == NULL)
	{
	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_REGEXP_TAG, strlen(ROUTE66TCPGATEWAY_ERROR_REGEXP_TAG));
	    socket.closeSocket();
	    return;
	}

	if((content = message->getFirstChildNode()) == NULL)
	{
	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_CONTENT_TAG, strlen(ROUTE66TCPGATEWAY_ERROR_CONTENT_TAG));
	    socket.closeSocket();
	    return;
	}

	//DEMO:
	//    printf(">>> WEB SERVER -> CAMELEON RT <<<");

	char *_regexp = new char[strlen((regexp->getFirstChildNode())->serialize())+2];
	sprintf(_regexp, "(%s)", (regexp->getFirstChildNode())->serialize());

	r66->sendMessageAndWaitAnAnswer((const char*) content->serialize(), _regexp, &answer);

	if(answer != NULL)
	{
	    socket.sendData(answer, strlen(answer));
	}
	else
	{
	    #ifdef __TRACE_ON
	    printf("DEBUG: timedwait timeout\n");
	    #endif

	    socket.sendData(ROUTE66TCPGATEWAY_ERROR_ROUTE66, strlen(ROUTE66TCPGATEWAY_ERROR_ROUTE66));
	}
    }
    else
    {
	socket.sendData(ROUTE66TCPGATEWAY_ERROR_SEND_TAG, strlen(ROUTE66TCPGATEWAY_ERROR_SEND_TAG));
    }

    socket.closeSocket();
}



Route66TcpGateway::Route66TcpGateway(int port, Route66 *r66): SlimThread(true)
{
    lastError = R66TCPGATEWAY_NOERROR;
	tcp = new SlimTcpServerSocket(port);
    tcpReceiver = new TcpReceiver(r66);
    tcp->setConnectionHandler(tcpReceiver);
    resume();
}



Route66TcpGateway::~Route66TcpGateway()
{
}



void Route66TcpGateway::run()
{
    int err;
    int loop;

    loop = 1;
    while(loop == 1)
    {
    	lastError = tcp->start();

    	//#ifdef __TRACE_ON
    	if(lastError != SLIMSOCKET_NOERROR) printf("# Tcp Server Socket Error: %d\n", lastError);
    	//#endif

    	if(lastError == SLIMSOCKET_ACCEPT_FAILED)
    	{
    		printf("# Restarting Gateway... ");
    		tcp->closeSocket();
    		tcp = new SlimTcpServerSocket(port);
    		tcp->setConnectionHandler(tcpReceiver);
    		printf("Ok.\n");
    	}
    	else
    	{
    		loop = 0;
    	}
    }
}



int Route66TcpGateway::getLastError(void) const
{
	return lastError;
}
