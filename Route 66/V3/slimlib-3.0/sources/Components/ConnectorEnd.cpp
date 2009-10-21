#include <Slimlib/Components/ConnectorEnd.h>

#include <stdlib.h>
#include <Slimlib/Core/StringClass.h>

#define TECHNOLOGY_ID "ConnectorEnd"

ConnectorEnd::ConnectorEnd(const char *technology)
{
	connected = false;
	this->technology = NULL;
	if(technology != NULL)
	{
		this->technology = String::duplicate(technology);
	}

	this->receiver = NULL;
}



ConnectorEnd::~ConnectorEnd()
{
	if(this->technology != NULL) free(this->technology);
}



void ConnectorEnd::setReceiver(Receiver *receiver)
{
    this->receiver = receiver;
}



bool ConnectorEnd::isConnected()
{
	return connected;
}



const char *ConnectorEnd::getTechnology()
{
	return technology;
}


Receiver *ConnectorEnd::getReceiver()
{
	return receiver;
}



const char *ConnectorEnd::getStaticTechnologyId()
{
	const char *id = TECHNOLOGY_ID;
	return id;
}
