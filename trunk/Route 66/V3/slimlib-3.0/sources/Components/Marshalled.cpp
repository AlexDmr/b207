#include <Slimlib/Components/Marshalled.h>
#include <stdlib.h>

Marshalled::Marshalled(const char *data, size_t size)
{
	this->data = Buffer::createBuffer(data, size);
}


Marshalled::~Marshalled()
{
}


const char *Marshalled::get(size_t *size)
{
	if(size)
	{
		*size = getSize();
	}

	if(data)
	{
		return data->getBuffer();
	}

	return NULL;
}



size_t Marshalled::getSize()
{
	if(data)
	{
		return data->getSize();
	}

	return 0;
}
