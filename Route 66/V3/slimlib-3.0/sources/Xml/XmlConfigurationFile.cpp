#include <Slimlib/Errors.h>
#include <Slimlib/Xml/XmlConfigurationFile.h>
#include <Slimlib/Core/File.h>
#include <string.h>

XmlConfigurationFile::XmlConfigurationFile(void)
{
	lastError =	NO_ERROR;
    configuration = NULL;
	internalParser = true;
	xmlParser =	new	XmlParser();
}



XmlConfigurationFile::XmlConfigurationFile(XmlParser *xmlParser)
{
	lastError =	NO_ERROR;
    configuration = NULL;
	internalParser = false;
	this->xmlParser	= xmlParser;
}


XmlConfigurationFile::~XmlConfigurationFile()
{
	delete(configuration);
	if(internalParser) delete(xmlParser);
}



int	XmlConfigurationFile::readFile(const char *filename)
{
	File *file = new File(filename);
	long size;
	char *buffer;

	lastError =	NO_ERROR;
	
	buffer = file->readContent(&size);

	if(file->getLastError()	!= NO_ERROR)
	{
		lastError =	file->getLastError();
		return lastError;
	}	 

	if((configuration =	xmlParser->parse(buffer)) ==	NULL)
	{
		lastError =	NOT_AN_XML_VALID_DOCUMENT;
		return lastError;
	}  
	
	if(strcmp(configuration->getName(),	XMLCONFIGURATIONFILE_TAG) != 0)
	{	
		lastError =	NO_CONFIGURATION_TAG;
	}	 
	
	return lastError;
}

	

XmlNode	*XmlConfigurationFile::getConfigurationData(const char *xmlNodeName)
{
	return configuration->getChildNodeByName(xmlNodeName);
}
   
	
	 
int	XmlConfigurationFile::getLastError(void) const
{
	return lastError;
} 
