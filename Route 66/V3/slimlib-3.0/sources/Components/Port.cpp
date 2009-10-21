#include <Slimlib/Components/Port.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
Port::Port(const char *name)
{
	this->name = NULL;

	if(name != NULL)
	{
		this->name = String::duplicate(name);
	}

	connectorEnd = NULL;

	direction = NULL;
	required = false;
}



Port::~Port()
{
	if(name != NULL) free(name);
	if(connectorEnd != NULL)
	{
		delete(connectorEnd);
	}
	if(direction != NULL) free(direction);
}




const char *Port::getName()
{
	return name;
}



bool Port::registerInterfaceKey(const char *key)
{
	if(key != NULL)
	{
		// Try to add interface and return the result: true = success, false = existing key: add failed
		return interfaceKeys.put(key, NULL);
	}

	// No valid data provided
	return false;
}



bool Port::plugConnectorEnd(ConnectorEnd *connectorEnd)
{
	// If a connector is already plug-in
	if(this->connectorEnd != NULL)
	{
		// Deconnect it
		this->connectorEnd->disconnect();

		//Destroy it
		delete(this->connectorEnd);
	}

	// set up the new connector
	this->connectorEnd = connectorEnd;
	
	if(this->connectorEnd)
	{
		this->connectorEnd->setReceiver(this);
	}

	return true;
}



ConnectorEnd *Port::getConnectorEnd()
{
	return connectorEnd;
}
