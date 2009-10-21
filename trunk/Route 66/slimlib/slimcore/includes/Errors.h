#ifndef __Slimlib_Errors_
#define __Slimlib_Errors_

// General error codes
#define _NO_ERROR                        0x0000
#define OUT_OF_MEMORY                   0x0001

// Xml general error codes
#define NOT_AN_XML_VALID_DOCUMENT       0x0101
#define NAMESPACE_NOT_SUPPORTED			0x0102

// File class error codes
#define FILE_NOT_FOUND                  0x1000
#define CANNOT_OPEN_FOR_WRITING    		0x1003
#define ERROR_WHILE_WRITING        		0x1004
#define ERROR_WHILE_READING        		0x1005
#define END_OF_FILE						0x1006
#define FILE_ALREADY_OPEN				0x1007

// Socket classes error codes
#define NO_PORT_DEFINED          		0x2001
#define NO_GROUP_DEFINED         		0x2002
#define SOCKET_ALLOCATION_FAILED        0x2003
#define SOCKET_BINDING_FAILED           0x2004
#define SOCKET_JOINING_FAILED           0x2005
#define RECEPTION_FAILED                0x0000
#define REUSEADDR_FAILED         		0x2007
#define REUSEPORT_FAILED         		0x2008
#define TTL_SETTING_FAILED       		0x2009
#define SENDDATA_FAILED          		0x200A
#define CONNECTION_FAILED        		0x200B
#define ACCEPT_FAILED            		0x200C
#define LISTENING_FAILED         		0x200D


// XmlConfigurationFile class error codes
#define NO_CONFIGURATION_TAG            0x3002

// XmlPattern class error codes
#define NO_PATTERN_TAG					0x4001
#define NO_SCHEMA_REFERENCE				0x4002
#define NO_PATTERN_ID         			0x4003
#define UNEXPECTED_END        			0x4004
#define NO_NAME_ATTRIBUTE				0x4005
#define UNKNOWN_OPERATOR				0x4006
#define NO_OPERATOR_DATA				0x4007

// Vector class error codes
#define VECTOR_IS_EMPTY  				0x5001
#define END_OF_VECTOR	  				0x5002
#define OUT_OF_VECTOR  					0x5003

#endif
