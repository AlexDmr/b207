#ifndef	__Class_XmlConfigurationFile_
#define	__Class_XmlConfigurationFile_

#include <Slimlib/Xml/XmlParser.h>

#define XMLCONFIGURATIONFILE_TAG					"configuration"


class XmlConfigurationFile: public Object
{
	private:
		int	lastError;
		bool internalParser;
		XmlParser *xmlParser;
		XmlNode *configuration;
		
	public:
		XmlConfigurationFile(void);
		XmlConfigurationFile(XmlParser *xmlParser);
		virtual ~XmlConfigurationFile(void);
		
		int readFile(const	char *filename);
		XmlNode	*getConfigurationData(const	char *xmlNodeName);	  
		
		int	getLastError(void) const;	
};

#endif
