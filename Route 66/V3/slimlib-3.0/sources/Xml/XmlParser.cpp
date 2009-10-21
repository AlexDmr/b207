#include <Slimlib/Xml/XmlParser.h>
#include <Slimlib/Core/StringClass.h>
#include <stdio.h>
#include <string.h>

#ifdef unix
		#include <errno.h>
#endif

XmlAttribute::XmlAttribute()
{
	name = NULL;
	prefix = NULL;
	value = NULL;
}

XmlAttribute::XmlAttribute(const char *name, const char *value)
{
    this->name = NULL;
	this->value = NULL;

	if(name != NULL)
	{
		this->name = String::duplicate(name);
	}

	if(value != NULL)
	{
		this->value = String::duplicate(value);
	}

   // if(errno != 0) printf("\nERRNO != ZERO\n");
}



XmlAttribute::~XmlAttribute(void)
{
    if(name != NULL) free(name);
    if(value != NULL) free(value);
}



void XmlAttribute::setName(const char *name)
{
	if(name != NULL)
	{
		 if(this->name != NULL) free(this->name);
		 this->name = String::duplicate(name);
	}
}



void XmlAttribute::setValue(const char *value)
{
    if(value != NULL)
	{
		 if(this->value != NULL) free(this->value);
		 this->value = String::duplicate(value);
	}
}



const char *XmlAttribute::getName(void) const
{
    return this->name;
}



const char *XmlAttribute::getLocalName(void) const
{
    return LocalNameOf(name);
}



const char *XmlAttribute::getValue(void) const
{
    return this->value;
}



const char *XmlAttribute::getLocalValue(void) const
{
    return LocalNameOf(value);
}



char *XmlAttribute::serialize(void) const
{
    char *s;
    if((s = (char *) malloc(strlen(this->name) + strlen(this->value) + 4)) == NULL) return NULL;
    sprintf(s, "%s=\"%s\"", this->name, this->value);
    return s;
}



bool XmlAttribute::compareTo(XmlAttribute *attribute)
{
	if(strcmp(attribute->getName(), name) == 0)
	{
		if(strcmp(attribute->getValue(), value) == 0)
		{
			return true;
		}
	}

	return false;
}



XmlAttribute *XmlAttribute::copy()
{
	return new XmlAttribute(name, value);
}



XmlNode::XmlNode()
{
	name = NULL;
	simpleContent = NULL;
	searched_name = NULL;
	parent = NULL;
}


XmlNode::XmlNode(const char* name)
{
	this->name = NULL;
	searched_name = NULL;
	simpleContent = NULL;
	parent = NULL;
	setName(name);
}



XmlNode *XmlNode::getParent()
{
	return parent;
}



void XmlNode::setParent(XmlNode *parent)
{
	this->parent = parent;
}



XmlNode::~XmlNode(void)
{
	if(name != NULL) free(name);
	while(attributes.size() != 0) { XmlAttribute *a = attributes.remove(0); delete(a); }
	if(simpleContent != NULL) free(simpleContent);
	while(complexContent.size() != 0) { XmlNode *n = complexContent.remove(0); delete(n); }
	if(searched_name != NULL) free(searched_name);
}



const char *XmlNode::getName(void) const
{
    return name;
}



const char *XmlNode::getLocalName(void) const
{
    return LocalNameOf(name);
}



bool XmlNode::compareName(const char *string) const
{
	char *radical;

	if((radical = strstr(name, ":")) == NULL)
	{
		return strcmp(name, string) == 0;
	}
	else
	{
		radical += sizeof(char);
		return strcmp(radical, string) == 0;
	}
}


void XmlNode::setName(const char* name)
{
    if(name == NULL)
	{
		name = NULL;
		return;
	}
	if(this->name != NULL) free(this->name);
	this->name = String::duplicate(name);
}




void XmlNode::addAttribute(XmlAttribute *attribute)
{
    attributes.push(attribute);
}



void XmlNode::setSimpleContent(const char* content)
{
    if(content == NULL)
	{
		simpleContent = NULL;
		return;
	}

	simpleContent = String::duplicate(content);
}



const char *XmlNode::getSimpleContent(void) const
{
    return simpleContent;
}



void XmlNode::addChildNode(XmlNode *node)
{
    complexContent.push(node);
    node->setParent(this);
}



const char *XmlNode::getAttributeValueByName(const char* name)
{
	if(name != NULL)
	{
		XmlAttribute *a;
		a = attributes.getFirstItem();

		while(a != NULL)
		{
			if(strcmp(name, a->getName()) == 0)
			{
				return a->getValue();
			}

			a = attributes.getNextItem();
		}
	}

    return NULL;
}



XmlAttribute *XmlNode::getAttributeByName(const char* name)
{
	if(name != NULL)
	{
		XmlAttribute *a;
		a = attributes.getFirstItem();

		while(a != NULL)
		{
			if(strcmp(name, a->getName()) == 0)
			{
				return a;
			}

			a = attributes.getNextItem();
		}
	}

    return NULL;
}



void XmlNode::setAttributeValueByName(const char* name, const char* value)
{
    XmlAttribute *a;

    a = attributes.getFirstItem();

    while(a != NULL)
    {
        if(strcmp(name, a->getName()) == 0)
        {
            a->setValue(value);
            return;
        }

		a = attributes.getNextItem();
    }
}



XmlNode *XmlNode::getChildNodeByName(const char* name)
{
    XmlNode *n = complexContent.getFirstItem();
    if(n != NULL)
	{
		if(searched_name != NULL) free(searched_name);
		searched_name = String::duplicate(name);
	}

	while(n != NULL)
    {
		char *radical;

		if(strcmp(searched_name, n->getName()) == 0)
		{
			return n;
		}
		else if((radical = (char *) strstr(n->getName(), ":")) != NULL)
		{
			radical += sizeof(char);
			if(strcmp(searched_name, radical) == 0) return n;
		}

		n = complexContent.getNextItem();
    }

	return NULL;
}



XmlNode *XmlNode::getNextChildNodeByName(void)
{
    XmlNode *n = complexContent.getNextItem();

	while(n != NULL)
    {
		char *radical;

		if((radical = (char *) strstr(n->getName(), ":")) == NULL)
		{
			if(strcmp(searched_name, n->getName()) == 0) return n;
		}
		else
		{
			radical += sizeof(char);
			if(strcmp(searched_name, radical) == 0) return n;
		}

		n = complexContent.getNextItem();
    }

    free(searched_name);
	searched_name = NULL;
    return NULL;
}



XmlNode *XmlNode::getFirstChildNode(void)
{
    return complexContent.getFirstItem();
}



XmlNode *XmlNode::getNextChildNode(void)
{
    return complexContent.getNextItem();
}



void XmlNode::removeChildNodeByName(const char* name)
{
    /**
	 * @todo removeChildNodeByName  tester !!!
	 */
    XmlNode *n;

    n = complexContent.getFirstItem();
    while(n != NULL)
    {
        if(strcmp(name, n->getName()) == 0)
        {
            complexContent.remove();
            return;
        }

		n = complexContent.getNextItem();
    }
}



char *XmlNode::serialize(void)
{
    return serialize(0);
}

char *XmlNode::serialize(int indentationLevel)
{
  const char *content = "<@node_name@attr>@node_scontent@node_ccontent@tab</@node_name>";
	const char *empty = "<@node_name@attr/>";
	String node;
	String indent;

	for(int i = 0; i < indentationLevel+1; i++) indent.concat("\t");

	if((simpleContent == NULL) && (complexContent.size() == 0))
	{
		node = empty;
	}
	else
	{
		node = content;
	}

  node.replace("@node_name", name);

  XmlAttribute *a = attributes.getFirstItem();
	String attr = "";
	while(a != NULL)
  {
		attr.concat(" ");
		char *s = a->serialize();
		attr.concat(s);
		free(s);

		a = attributes.getNextItem();
	}

	node.replace("@attr", attr);

	if(simpleContent != NULL)
	{
			node.replace("@node_scontent", simpleContent);
	}
	else
	{
			node.replace("@node_scontent", "");
	}

	XmlNode *n = complexContent.getFirstItem();
	String ccontent = "\n";
	if(n != NULL)
	{
		while(n != NULL)
		{
			char *s = n->serialize(indentationLevel+1);
			ccontent.concat(indent);
			ccontent.concat(s);
			ccontent.concat("\n");
			ccontent.replace("@tab", indent);
			free(s);
			n = complexContent.getNextItem();
		}

		node.replace("@node_ccontent", ccontent);
	}
	else
	{
		node.replace("@node_ccontent", "");
		node.replace("@tab", "");
	}

	if(indentationLevel == 0) node.replace("@tab", "");

	return String::duplicate(node.get());
}



unsigned int XmlNode::getChildNodesCardinality(void)
{
	return complexContent.size();
}



unsigned int XmlNode::getAttributesCardinality(void)
{
	return attributes.size();
}



XmlAttribute *XmlNode::getFirstAttribute(void)
{
    return attributes.getFirstItem();
}



XmlAttribute *XmlNode::getNextAttribute(void)
{
    return attributes.getNextItem();
}



XmlAttribute *XmlNode::getFirstAttributeWithNameLike(const char *snippet)
{
	XmlAttribute *a = attributes.getFirstItem();

	while((a != NULL) && (strstr(a->getName(), snippet) == NULL))
	{
		a = attributes.getNextItem();
	}

    return a;
}



XmlAttribute *XmlNode::getNextAttributeWithNameLike(const char *snippet)
{
    XmlAttribute *a = attributes.getNextItem();

	while((a != NULL) && (strstr(a->getName(), snippet) == NULL))
	{
		a = attributes.getNextItem();
	}

    return a;
}





bool XmlNode::compareTo(XmlNode *node)
{
	// First, we compare child nodes and attributes cardinality
	if((node->getAttributesCardinality() != getAttributesCardinality()) || (node->getChildNodesCardinality() != getChildNodesCardinality()))
	{
		return false;
	}

	// Then, we compare simplecontent
	if(strcmp(node->getSimpleContent(), simpleContent) != 0)
	{
		return false;
	}

	// Then, we compare if attributes are the same.
	XmlAttribute *a;

	a = node->getFirstAttribute();
	while(a != NULL)
	{
		XmlAttribute *self_a;
		self_a = getFirstAttribute();
		while(self_a != NULL)
		{
			if(!self_a->compareTo(a))
			{
				return false;
			}

			self_a = getNextAttribute();
		}

		a = node->getNextAttribute();
	}

	// Then, we compare if child nodes are the same.
	XmlNode *cn;

	cn = node->getFirstChildNode();
	while(cn != NULL)
	{
		XmlNode *self_cn;
		self_cn = getFirstChildNode();
		while(self_cn != NULL)
		{
			if(!self_cn->compareTo(cn))
			{
				return false;
			}

			self_cn = getNextChildNode();
		}

		cn = node->getNextChildNode();
	}

	return true;
}


XmlNode *XmlNode::copy()
{
	XmlNode *node = new XmlNode(name);
	XmlAttribute *attr = attributes.getFirstItem();
	while(attr != NULL)
	{
		node->addAttribute(attr->copy());
		attr = attributes.getNextItem();
	}

	node->setSimpleContent(simpleContent);

	XmlNode *child = complexContent.getFirstItem();
	while(child != NULL)
	{
		node->addChildNode(child->copy());
		child = complexContent.getNextItem();
	}

	return node;
}





#define PARSER_PI               0
#define PARSER_DOCTYPE          1
#define PARSER_COMMENT          2
#define PARSER_ELEMENT          3
#define PARSER_ELEMENT_END_TAG  4
#define PARSER_PCDATA           5
#define PARSER_END_OF_STREAM    10
#define PARSER_NOT_WELL_FORMED  11

bool XmlParser::isSpace(char c)
{
    if((c == 0x20) || (c == 0x9) || (c == 0xD) || (c == 0xA)) return true;
    return false;
}

bool XmlParser::isLetter(char c)
{
    if(((c >= 0x41) && (c <= 0x5A)) || ((c >= 0x61) && (c <= 0x7A))) return true;
    return false;
}

bool XmlParser::isDigit(char c)
{
    if((c >= 0x30) && (c <= 0x39)) return true;
    return false;
}

char *XmlParser::parseName(void)
{
    int i = 0;
    char *name;
    char *firstNameChar = stream;

    while(!isSpace(*stream) && (*stream != '/') && (*stream != '=') && (*stream != '>'))
    {
        stream++;
        i++;
    }

    if((name = (char *) malloc(i+1)) == NULL) return NULL;
    strncpy(name, firstNameChar, i);
    name[i] = 0;
    return name;
}

char *XmlParser::parseAttrValue(char quote)
{
    int i = 0;
    char *value;
    char *firstValueChar = stream;

    while(*stream != quote)
    {
        stream++;
        i++;
    }

    if((value = (char *) malloc(i+1)) == NULL) return NULL;
    strncpy(value, firstValueChar, i);
    value[i] = 0;
    return value;
}

char *XmlParser::parsePcdata(void)
{
    int i = 0;
    char *pcdata;
    char *firstPcdataChar = stream;

    while(*stream != '<')
    {
        stream++;
        i++;
    }

    if((pcdata = (char *) malloc(i+1)) == NULL) return NULL;
    strncpy(pcdata, firstPcdataChar, i);
    pcdata[i] = 0;
    i--;
    while(isSpace(pcdata[i]))
    {
        pcdata[i] = 0;
        i--;
    }

    return pcdata;
}

XmlAttribute *XmlParser::parseAttribute(void)
{
    char *name;
    char *value;
    char quote;

    if((name = parseName()) == NULL) { stream = NULL; return NULL; }
	eatSpaces();
    if(*stream != '=') { stream = NULL; return NULL; }
    stream++;
	eatSpaces();
    quote = *stream;
    if((quote != '\'') && (quote != '"')) { stream = NULL; return NULL; }
    stream++;
    if((value = parseAttrValue(quote)) == NULL) { stream = NULL; return NULL; }

    XmlAttribute *attribute;
    if((attribute = new XmlAttribute(name, value)) == NULL) { stream = NULL; return NULL; }

    stream++;
    //printf("         attr serialized: %s\n", attribute->serialize());
    return attribute;
}

XmlNode *XmlParser::parseElement(void)
{
    char *name;

    if((name = parseName()) == NULL) { stream = NULL; return NULL; }

    XmlNode *element;
    if((element = new XmlNode(name)) == NULL) { stream = NULL; return NULL; }

    eatSpaces();
    while((*stream != '/') && (*stream != '>'))
    {
        XmlAttribute *attribute;
        if((attribute = parseAttribute()) == NULL) { stream = NULL; return NULL; }
        element->addAttribute(attribute);
        eatSpaces();
    }

    return element;
}

bool XmlParser::parseElementContent(XmlNode* node) // return true if all the content is parsed <=> *stream == '/'
{
    char *s;
    char *pcdata;

    eatSpaces();
    if(*stream == '/') return true;

    if(*stream == '>')
    {
        stream++;
        eatSpaces();
    }

    if((pcdata = parsePcdata()) == NULL) { stream = NULL; return false; }

    const char *simpleContent = node->getSimpleContent();

    if(simpleContent == NULL)
    {
        node->setSimpleContent(pcdata);
    }
    else
    {
        if((s = (char*) malloc(strlen(pcdata) + strlen(simpleContent) + 2)) == NULL)  { stream = NULL; return false; }
        sprintf(s,"%s\n%s", simpleContent, pcdata);
        node->setSimpleContent(s);
    }

    if(*(stream+1) == '/') return true;

    return false;
}

bool XmlParser::parseElementEndTag(XmlNode* node)
{
    char *name;

    if(*stream == '/') stream++;
    if(*stream == '>')
    {
        stream++;
        return true;
    }


    name = parseName();
    if(strcmp(name, node->getName()) == 0)
    {
        eatSpaces();
        if(*stream == '>')
        {
            stream++;
            return true;
        }
    }
   return false;
}

int XmlParser::identifyNextItemType(void)
{
    if((stream == NULL) || ((stream+1) == NULL) || ((stream+2) == NULL))
    {
        return PARSER_END_OF_STREAM;
    }
    if((*stream == '/') && (*(stream+1) == '>')) return PARSER_ELEMENT_END_TAG;
    if(*stream != '<') return PARSER_PCDATA;
    if(*(stream+1) == '/')
    {
        stream+=2;
        return PARSER_ELEMENT_END_TAG;
    }
    if(*(stream+1) == '?')
    {
        stream++;
        return PARSER_PI;
    }
    if(*(stream+1) == '!')
    {
        stream += 2;
        if(*(stream) == '-') return PARSER_COMMENT;
        if(*(stream) == 'D') return PARSER_DOCTYPE;
        return PARSER_NOT_WELL_FORMED;
    }
    if(isLetter(*(stream+1))) { stream++; return PARSER_ELEMENT; }
    return PARSER_NOT_WELL_FORMED;
}

void XmlParser::eatDoctype(void)
{
    if((stream = strstr(stream, ">")) == NULL) { stream = NULL; return; }
    stream++;
}

void XmlParser::eatProcessingInstruction(void)
{
    if((stream = strstr(stream, "?>")) == NULL) { stream = NULL; return; }
    stream += 2;
}

void XmlParser::eatComments(void)
{
    if((stream = strstr(stream, "-->")) == NULL) { stream = NULL; return; }
    stream += 3;
}

void XmlParser::eatSpaces(void)
{
    while((*stream == 0x20) || (*stream == 0x9) || (*stream == 0xD) || (*stream == 0xA)) stream++;
}

XmlNode *XmlParser::parse(const char *xmlStream)
{
    XmlNode *currentNode = NULL;
    XmlNode *fatherNode = NULL;

    stream = String::duplicate(xmlStream);

    while(stream != NULL)
    {
        eatSpaces();

        switch(identifyNextItemType())
        {
            case PARSER_PI:
                    eatProcessingInstruction();
                    break;

            case PARSER_DOCTYPE:
					eatDoctype();
                    break;

            case PARSER_COMMENT:
					eatComments();
                    break;

            case PARSER_ELEMENT:
					if((currentNode = parseElement()) == NULL) return NULL;
                    if(!parseElementContent(currentNode))
                    {
                        elements.push(currentNode);
                        currentNode = NULL;
                    }
                    break;

            case PARSER_PCDATA:
                    if(elements.size() != 0)
                    {
                        currentNode = elements.getLastItem();
                        parseElementContent(currentNode);
                        currentNode = NULL;
                    }
                    else return NULL;
                    break;

            case PARSER_ELEMENT_END_TAG:
                    if(currentNode == NULL)
                    {
                        if(elements.size() != 0)
                        {
                            currentNode = elements.pop();
                        }
                        else return NULL;
                    }

                    if(parseElementEndTag(currentNode))
                    {
                        if(elements.size() != 0)
                        {
                            fatherNode = elements.getLastItem();
                            fatherNode->addChildNode(currentNode);
                            currentNode = NULL;
                        }
                        else
                        {
                            return currentNode;
                        }
                    }
                    else return NULL;
                    break;
        }
    }
    return NULL;
}

XmlParser::XmlParser()
{
}

XmlParser::~XmlParser()
{
    while(elements.size() != 0) { XmlNode *n = elements.remove(0); delete(n); }
}


char *itoa(long i, char *format)
{
    char tmp[80];
    sprintf(tmp, format, i);
    return String::duplicate(tmp);
}









XmlNamespaceHandler::XmlNamespaceHandler(void)
{
	defaultNamespace = NULL;
	targetNamespace = NULL;
}



XmlNamespaceHandler::~XmlNamespaceHandler(void)
{
	if(defaultNamespace != NULL) free(defaultNamespace);
	if(targetNamespace != NULL) free(targetNamespace);
	while(namespaces.size() != 0) { Property *p = namespaces.remove(0); delete(p); }
}



void XmlNamespaceHandler::set_defaultNamespace(const char *uri)
{
	if(uri != NULL)
	{
		if(defaultNamespace != NULL) free(defaultNamespace);
		defaultNamespace = String::duplicate(uri);
	}
}



void XmlNamespaceHandler::set_targetNamespace(const char *uri)
{
	if(uri != NULL)
	{
		if(targetNamespace != NULL) free(targetNamespace);
		targetNamespace = String::duplicate(uri);
	}
}



const char *XmlNamespaceHandler::get_defaultNamespace(void)
{
	return defaultNamespace;
}



const char *XmlNamespaceHandler::get_targetNamespace(void)
{
	return targetNamespace;
}



void XmlNamespaceHandler::addNamespace(const char *key, const char *uri)
{
	if((key != NULL) && (uri != NULL))
	{
		Property *p = new Property();

		p->setKey(key);
		p->setValue(uri);

		namespaces.push(p);
	}
}



void XmlNamespaceHandler::addNamespaces(XmlNode *documentRootNode)
{
	String s;

	if(documentRootNode != NULL)
	{
		XmlAttribute *xmlns = documentRootNode->getFirstAttributeWithNameLike(XML_XMLNS);
		while(xmlns != NULL)
		{
			char *key = (char *) strstr(xmlns->getName(), XML_KEY_SEPARATOR);
			if(key == NULL)
			{
				this->set_defaultNamespace(xmlns->getValue());
			}
			else
			{
				key += sizeof(char);
				s.set(key);
				s.concat(XML_KEY_SEPARATOR);

				Property *p = new Property(s.get(), xmlns->getValue());
				namespaces.push(p);
			}

			xmlns = documentRootNode->getNextAttributeWithNameLike(XML_XMLNS);
		}

		const char *tns = documentRootNode->getAttributeValueByName(XML_TARGETNAMESPACE);
		if(tns != NULL)
		{
			targetNamespace = String::duplicate(tns);
		}
	}
}



const char *XmlNamespaceHandler::getNamespace(const char *namespaceKey)
{
	if(namespaceKey == NULL)
	{
		return NULL;
	}

	Property *p;

	p = namespaces.getFirstItem();


	while(p != NULL)
	{
		if(strstr(namespaceKey, p->getKey()) != NULL)
		{
			return p->getValue();
		}

		p = namespaces.getNextItem();
	}

	return defaultNamespace;
}


const char *XmlNamespaceHandler::getNamespace(XmlNode *node)
{
	if(node != NULL)
	{
		return getNamespace(node->getName());
	}

	return NULL;
}



const char *XmlNamespaceHandler::getNamespace(XmlAttribute *attribute)
{
	if(attribute != NULL)
	{
		return getNamespace(attribute->getName());
	}

	return NULL;
}



const char *XmlNamespaceHandler::getNamespaceKey(const char *uri)
{
	if(uri == NULL)
	{
		return NULL;
	}

	Property *p;

	p = namespaces.getFirstItem();


	while(p != NULL)
	{
		if(strstr(uri, p->getValue()) != NULL)
		{
			return p->getKey();
		}

		p = namespaces.getNextItem();
	}

	return NULL;
}



const char *LocalNameOf(const char *xmlToken)
{
	char *temp;

	temp = (char *) strstr(xmlToken, ":");
	if(temp == NULL) return xmlToken;

	temp += sizeof(char);
	return temp;
}



const char *PrefixOf(const char *xmlToken)
{
	char *temp;

	temp = (char *) strstr(xmlToken, ":");
	if(temp == NULL) return NULL;
/*
	Buffer prefix;
	int prefixSize = abs(xmlToken - temp);
	prefix.allocateBuffer(prefixSize + 1);
	strncpy(prefix.getBuffer(), xmlToken, prefixSize);
	temp += sizeof(char);*/
	return temp;
}

