#include <Slimlib/Core/Exception.h>
#include <Slimlib/Core/StringClass.h>

Exception::Exception(void)
{
	name = NULL;
	code = 0;
}

Exception::Exception(const char *name, int code)
{
	this->name = String::duplicate(name);
	this->code = code;
}
//bof bof
Exception::~Exception(void)
{
	//if(name != NULL) free(name);
}

const char *Exception::getName(void)
{
	return name;
}

int Exception::getCode(void)
{
	return code;
}








//OutOfMemoryException::OutOfMemoryException(void): Exception("Out Of Memory", 1) {}
