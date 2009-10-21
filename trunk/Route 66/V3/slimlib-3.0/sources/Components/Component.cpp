#include <Slimlib/Components/Component.h>
#include <string.h>

#define COMPONENT_NAME "Component"

Component::Component(const char *name)
{
	this->name = NULL;

	if(name != NULL)
	{
		this->name = String::duplicate(name);
	}

	port = NULL;
}



Component::~Component()
{
	if(name != NULL) free(name);
}



const char *Component::getName()
{
	return name;
}



void Component::addPort(Port *port)
{
	ports.put(port->getName(), port);
}



Port *Component::getPort(const char *portName)
{
	return ports.get(portName);
}



bool Component::plugConnectorEndInPort(const char *portName, ConnectorEnd *connectorEnd)
{
	Port *port = getPort(portName);

	if(port != NULL)
	{
		return port->plugConnectorEnd(connectorEnd);
	}

	return false;
}


const char *Component::getStaticName()
{
	const char *name = COMPONENT_NAME;
	return name;
}
