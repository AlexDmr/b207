#include <slimxmlconfigurationfile.h>



SlimXmlConfigurationFile::SlimXmlConfigurationFile(void)
{
	lastError =	SLIMFILE_NOERROR;
	xmlParser =	new	SlimXmlParser();
}



SlimXmlConfigurationFile::SlimXmlConfigurationFile(SlimXmlParser *xmlParser)
{
	lastError =	SLIMFILE_NOERROR;
	this->xmlParser	= xmlParser;
}




int	SlimXmlConfigurationFile::readFile(const char *filename)
{
	SlimFile *file = new SlimFile(filename); 
	long size;
	char *buffer;

	lastError =	SLIMFILE_NOERROR;
	
	buffer = file->readContent(&size);

	if(file->getLastError()	!= SLIMFILE_NOERROR) 
	{
		lastError =	SLIMCONFIGURATIONFILE_NOT_FOUND;
		return lastError;
	}	 

	if((configuration =	xmlParser->parse(buffer)) ==	NULL)
	{
		lastError =	SLIMCONFIGURATIONFILE_NOT_XML_VALID;
		return lastError;
	}  
	
	if(strcmp(configuration->getName(),	SLIMCONFIGURATIONFILE_TAG) != 0)
	{	
		lastError =	SLIMCONFIGURATIONFILE_NO_CONFIGURATION_TAG;
	}	 
	
	return lastError;
}

	

XmlNode	*SlimXmlConfigurationFile::getConfigurationData(const char *xmlNodeName)
{
	return configuration->getChildNodeByName(xmlNodeName);
}
   
	
	 
int	SlimXmlConfigurationFile::getLastError(void) const
{
	return lastError;
} 
