#ifndef	_SLIMXMLCONFIGURATIONFILE_
#define	_SLIMXMLCONFIGURATIONFILE_

#include <slimcore.h>
#include <slimxmlparser.h>

#define SLIMCONFIGURATIONFILE_TAG					"configuration"
#define SLIMCONFIGURATIONFILE_NOT_FOUND				5
#define SLIMCONFIGURATIONFILE_NOT_XML_VALID			6
#define SLIMCONFIGURATIONFILE_NO_CONFIGURATION_TAG	7

class SlimXmlConfigurationFile
{
	private:
		int	lastError;
		SlimXmlParser *xmlParser;
		XmlNode *configuration;
		
	public:
		SlimXmlConfigurationFile(void);
		SlimXmlConfigurationFile(SlimXmlParser *xmlParser);
		~SlimXmlConfigurationFile(void);
		
		int readFile(const	char *filename);
		XmlNode	*getConfigurationData(const	char *xmlNodeName);	  
		
		int	getLastError(void) const;	
};

#endif
