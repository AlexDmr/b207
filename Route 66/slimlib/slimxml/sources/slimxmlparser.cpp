#include <slimxmlparser.h>
#include <slimcore.h>
#include <stdio.h>

#ifdef unix
		#include <errno.h>
#endif

XmlAttribute::XmlAttribute()
{

}

XmlAttribute::XmlAttribute(const char *name, const char *value)
{
    this->name  = new char[strlen(name )+1]; strcpy(this->name , name );
    this->value = new char[strlen(value)+1]; strcpy(this->value, value);
    if(errno != 0) printf("\nERRNO != ZERO\n");
}



XmlAttribute::~XmlAttribute(void)
{
    if(name != NULL)  delete[] name;
    if(value != NULL) delete[] value;
}



void XmlAttribute::setName(const char *name)
{
    if(name != NULL) delete[] this->name;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}



void XmlAttribute::setValue(const char *value)
{
    if(value != NULL) delete[] this->value;
    this->value = new char[strlen(value) + 1];
    strcpy(this->value, value);
}



const char *XmlAttribute::getName(void) const
{
    return this->name;
}



const char *XmlAttribute::getValue(void) const
{
    return this->value;
}



const char *XmlAttribute::serialize(void) const
{
    char *s;
    s = new char[strlen(this->name) + strlen(this->value) + 4];
    if(s == NULL) return NULL;
    sprintf(s, "%s=\"%s\"", this->name, this->value);
    return s;
}



XmlNode::XmlNode()
{

}


XmlNode::XmlNode(const char* name)
{
    this->name = new char[strlen(name)+1]; strcpy(this->name, name);
    simpleContent = NULL;
}



XmlNode::~XmlNode(void)
{
    if(name != NULL) delete[] name;
    attributes.clear();
    if(simpleContent != NULL) delete[] simpleContent;
    complexContent.clear();
}


const char *XmlNode::getName(void) const
{
    return name;
}



void XmlNode::addAttribute(XmlAttribute *attribute)
{
    //printf("add attribute: %s\n", attribute->serialize());
    attributes.push(attribute);
    //printf("attribute added: %s\n", attribute->serialize());
}



void XmlNode::setSimpleContent(const char* content)
{
    simpleContent = new char[strlen(content)+1]; strcpy(simpleContent, content);
}



const char *XmlNode::getSimpleContent(void) const
{
    if(simpleContent == NULL) return NULL;
    return simpleContent;
}



void XmlNode::addChildNode(XmlNode *node)
{
    complexContent.push(node);
}



const char *XmlNode::getAttributeValueByName(const char* name)
{
    XmlAttribute *a;
    attributes.beginSequentialReading();
    /*
    for(int i = 0; i < attributes.size(); i++)
    {
    	XmlAttribute *a = attributes.getPoint
        if(strcmp(name, attributes[i]->getName()) == 0)
        {
        	return attributes[i]->getValue();
        }
     }
    */

    while((a = attributes.readItemPointer()) != NULL)
    {
        if(strcmp(name, a->getName()) == 0)
        {
        	return a->getValue();
        }
    }

    return NULL;
}



void XmlNode::setAttributeValueByName(const char* name, const char* value)
{
    XmlAttribute *a;

    attributes.beginSequentialReading();
    /*
    for(int i = 0; i < attributes.size(); i++)
    {
    	if(strcmp(name, attributes[i]->getName()) == 0)
        {
        	a->setValue(value);
            return;
        }
     }
    */

    while((a = attributes.readItemPointer()) != NULL)
    {
        if(strcmp(name, a->getName()) == 0)
        {
            a->setValue(value);
            return;
        }
    }
}



XmlNode *XmlNode::getChildNodeByName(const char* name)
{   searched_name = new char[strlen(name)+1];
    strcpy(searched_name, name);

    complexContent.beginSequentialReading();
    while(!complexContent.isEndOfVector())
    {
    	XmlNode *n = complexContent.readItemPointer();
        if(strcmp(searched_name, n->getName()) == 0) return n;
    }

    delete[] searched_name;
    return NULL;
}



XmlNode *XmlNode::getNextChildNodeByName(void)
{
    while(!complexContent.isEndOfVector())
    {
    	XmlNode *n = complexContent.readItemPointer();
        if(strcmp(searched_name, n->getName()) == 0) return n;
    }

    delete[] searched_name;
    return NULL;
}



XmlNode *XmlNode::getFirstChildNode(void)
{
    complexContent.beginSequentialReading();
    return complexContent.readItemPointer();
}



XmlNode *XmlNode::getNextChildNode(void)
{
    if(complexContent.isEndOfVector())
    {
    	return NULL;
    }
    else
    {
    	return complexContent.readItemPointer();
    }
}



void XmlNode::removeChildNodeByName(const char* name)
{
    //ˆ tester !!!
    XmlNode *n;
    unsigned int i = 0;

    complexContent.beginSequentialReading();
    while((n = complexContent.readItemPointer()) != NULL)
    {
        if(strcmp(name, n->getName()) == 0)
        {
            complexContent.eraseItemAtReadingCursor();
            return;
        }
    }
}



const char *XmlNode::serialize(void)
{
    return serialize(0);
}

const char *XmlNode::serialize(int indentationLevel)
{
    //char *s;
    char *tag;
    char *openingTag;
    char *closingTag;
    char *indentation;

    indentation = new char[indentationLevel + 2];
    if(indentation == NULL) return NULL;
    for(int i = 0; i < indentationLevel+1; i++) indentation[i] = '\t';
    indentation[indentationLevel+1] = 0;

    if((simpleContent == NULL) && (complexContent.size() == 0)) {closingTag = new char[4]; strcpy(closingTag, "/>");}
    else
    {   closingTag = new char[strlen(name) + 5];
        if(closingTag == NULL) return NULL;
        sprintf(closingTag, "</%s>", name);
    }

    openingTag = new char [strlen(name) + 2];
    if(openingTag == NULL) return NULL;
    sprintf(openingTag, "<%s", name);

    attributes.beginSequentialReading();
    while(!attributes.isEndOfVector())
    {
    	XmlAttribute *a = attributes.readItemPointer();
        const char *s = a->serialize();
        tag = new char[strlen(s) + strlen(openingTag) + 2];
        if(tag == NULL) return NULL;
        sprintf(tag, "%s %s", openingTag, s);
        if(openingTag != NULL)
        {
        	delete[] openingTag;
        }
        openingTag = tag;
    }

    if(simpleContent != NULL)
    {   tag = new char[strlen(simpleContent) + strlen(openingTag) + 3];
        if(tag == NULL) return NULL;
        sprintf(tag, "%s>%s", openingTag, simpleContent);
        if(openingTag != NULL)
        {
        	delete[] openingTag;
        }
        openingTag = tag;
    }
    else
    {
        if(complexContent.size() != 0)
        {   tag = new char[strlen(openingTag) + 2];
            if(tag == NULL) return NULL;
            sprintf(tag, "%s>", openingTag);
            if(openingTag != NULL)
			{
				delete[] openingTag;
			}
            openingTag = tag;
        }
    }

    complexContent.beginSequentialReading();
    while(!complexContent.isEndOfVector())
    {
    	XmlNode *n = complexContent.readItemPointer();
        const char *s = n->serialize(indentationLevel+1);
        tag = new char[strlen(s) + strlen(openingTag) + strlen(indentation) + 2];
        if(tag == NULL) return NULL;
        if(openingTag[strlen(openingTag)-1] == '\n') sprintf(tag, "%s%s%s", openingTag, indentation, s);
        else sprintf(tag, "%s\n%s%s", openingTag, indentation, s);
        if(openingTag != NULL)
        {
        	delete[] openingTag;
        }
        openingTag = tag;
    }

    indentation[indentationLevel] = 0;
    if(openingTag[strlen(openingTag)-1] == '>')
    {   tag = new char[strlen(closingTag) + strlen(openingTag) + strlen(indentation) + 2];
        if(tag == NULL) return NULL;
        sprintf(tag, "%s\n%s%s", openingTag, indentation, closingTag);
    }
    else
    {   tag = new char[strlen(closingTag) + strlen(openingTag) + 2];
        if(tag == NULL) return NULL;
        sprintf(tag, "%s%s", openingTag, closingTag);
    }

    return tag;
}

#define PARSER_PI               0
#define PARSER_DOCTYPE          1
#define PARSER_COMMENT          2
#define PARSER_ELEMENT          3
#define PARSER_ELEMENT_END_TAG  4
#define PARSER_PCDATA           5
#define PARSER_END_OF_STREAM    10
#define PARSER_NOT_WELL_FORMED  11

bool SlimXmlParser::isSpace(char c)
{
    if((c == 0x20) || (c == 0x9) || (c == 0xD) || (c == 0xA)) return true;
    return false;
}

bool SlimXmlParser::isLetter(char c)
{
    if(((c >= 0x41) && (c <= 0x5A)) || ((c >= 0x61) && (c <= 0x7A))) return true;
    return false;
}

bool SlimXmlParser::isDigit(char c)
{
    if((c >= 0x30) && (c <= 0x39)) return true;
    return false;
}

char *SlimXmlParser::parseName(void)
{
    int i = 0;
    char *name;
    char *firstNameChar = stream;

    while(!isSpace(*stream) && (*stream != '/') && (*stream != '=') && (*stream != '>'))
    {
        stream++;
        i++;
    }

    name = new char[i+1];
    if(name == NULL) return NULL;
    strncpy(name, firstNameChar, i);
    name[i] = 0;
    return name;
}

char *SlimXmlParser::parseAttrValue(char quote)
{
    int i = 0;
    char *value;
    char *firstValueChar = stream;

    while(*stream != quote)
    {
        stream++;
        i++;
    }

    value = new char[i+1];
    if(value == NULL) return NULL;
    strncpy(value, firstValueChar, i);
    value[i] = 0;
    return value;
}

char *SlimXmlParser::parsePcdata(void)
{
    int i = 0;
    char *pcdata;
    char *firstPcdataChar = stream;

    while(*stream != '<')
    {
        stream++;
        i++;
    }

    pcdata = new char[i+1];
    if(pcdata == NULL) return NULL;
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

XmlAttribute *SlimXmlParser::parseAttribute(void)
{
    char *name;
    char *value;
    char quote;

    if((name = parseName()) == NULL) { stream = NULL; return NULL; }
    if(*stream != '=') { stream = NULL; return NULL; }
    stream++;
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

XmlNode *SlimXmlParser::parseElement(void)
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

bool SlimXmlParser::parseElementContent(XmlNode* node) // return true if all the content is parsed <=> *stream == '/'
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
    {   s = new char[strlen(pcdata) + strlen(simpleContent) + 2];
        if(s == NULL)  { stream = NULL; return false; }
        sprintf(s,"%s\n%s", simpleContent, pcdata);
        node->setSimpleContent(s);
    }

    if(*(stream+1) == '/') return true;

    return false;
}

bool SlimXmlParser::parseElementEndTag(XmlNode* node)
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

int SlimXmlParser::identifyNextItemType(void)
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

void SlimXmlParser::eatDoctype(void)
{
    if((stream = strstr(stream, ">")) == NULL) { stream = NULL; return; }
    stream++;
}

void SlimXmlParser::eatProcessingInstruction(void)
{
    if((stream = strstr(stream, "?>")) == NULL) { stream = NULL; return; }
    stream += 2;
}

void SlimXmlParser::eatComments(void)
{
    if((stream = strstr(stream, "-->")) == NULL) { stream = NULL; return; }
    stream += 3;
}

void SlimXmlParser::eatSpaces(void)
{
    while((*stream == 0x20) || (*stream == 0x9) || (*stream == 0xD) || (*stream == 0xA)) stream++;
}

XmlNode *SlimXmlParser::parse(const char *xmlStream)
{
    XmlNode *debugNode = NULL;
    XmlNode *currentNode = NULL;
    XmlNode *fatherNode = NULL;

    stream = new char[strlen(xmlStream)+1]; strcpy(stream, xmlStream);

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
                        currentNode = elements.getPointerOnLastItem();
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
                            currentNode = elements.pop_reference();
                        }
                        else return NULL;
                    }

                    if(parseElementEndTag(currentNode))
                    {
                        if(elements.size() != 0)
                        {
                            fatherNode = elements.getPointerOnLastItem();
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

SlimXmlParser::SlimXmlParser()
{
}

SlimXmlParser::~SlimXmlParser()
{
    elements.clear();
}


char *itoa(long i, char *format)
{
    char *tmp = new char[80];
    sprintf(tmp, format, i);
    return tmp;
}
