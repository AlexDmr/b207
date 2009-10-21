#include <Slimlib/Core/Property.h>
#include <Slimlib/Core/StringClass.h>
#include <stdlib.h>


Property::Property(void)
{
	this->key = NULL;
	this->value = NULL;
}


Property::Property(const char *key, const char *value)
{
	this->key = NULL; 
	this->value = NULL;
	setKey(key);
	setValue(value);
}


Property::~Property(void)
{
	if(this->key != NULL) free(this->key);
	if(this->value != NULL) free(this->value);
	this->key = NULL;
	this->value = NULL;
}


const char *Property::getKey(void)
{
	return this->key;
}


const char *Property::getValue(void)
{
	return this->value;
}


void Property::setKey(const char *key)
{
	if(this->key != NULL) free(this->key);
	this->key = NULL;
	if(key != NULL) this->key = String::duplicate(key);
}


void Property::setValue(const char *value)
{
	if(this->value != NULL) free(this->value);
	this->value = NULL;
	if(value != NULL) this->value = String::duplicate(value);
}


