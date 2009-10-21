#ifndef _SLIMXMLPARSER_
#define _SLIMXMLPARSER_

#include <slimcore.h>

#define XML_HEADER              "<?xml version=\"1.0\" ?>"
#define XML_DOCTYPE_FORMAT      "<!DOCTYPE %s PUBLIC \"%s\">"       //1st %s: root element, 2nd %s: reference


/**
 * @brief XmlAttribute is a class to encapsulate an attribute of an XML tag
 */ 
class XmlAttribute
{
    private:
        char *name;
        char *value;
    
    public:
    
    	/**
		* Construct an empty attribute (no name, no value)
		*/
       XmlAttribute();
       
       
       
       /**
		* Construct an attribute called <i>name</i> with <i>value</i> as value.
		* @param name a string containing a well-formatted xml attribute name
		* @param value a string containing a xml-valid attribute value
		*/
       XmlAttribute(const char *name, const char *value);
       ~XmlAttribute(void);



		/**
		* Set (or change if already defined) the attribute name
		* @param name is an attibute name
		*/
       void setName(const char *name);



		/**
		* Set (or change if already defined) the attribute value
		* @param value is an attibute value
		*/
       void setValue(const char *value); 



		/**
		* Retrieve a copy of the attribute name
		* @return a string containing a copy of the attribute name
		*/
       const char *getName(void) const;



		/**
		* Retrieve a copy of the attribute value
		* @return a string containing a copy of the attribute value
		*/
       const char *getValue(void) const;
       
       
       
       /**
		* Get a textual representation of the attribute (i.e.: something like name="value")
		* @return a string containing a textual representation of the attribute
		*/
       const char *serialize(void) const;
};





/**
 * @brief XmlNode is a class to encapsulate an XML tag and its content
 */
class XmlNode
{
    private:
        char *name;
        char *searched_name;
        
        SlimVector<XmlAttribute> attributes;
        char *simpleContent;
        SlimVector<XmlNode> complexContent;
        const char *serialize(int indentationLevel);
        
    public:
    
    	/**
		 * Construct an empty node
		 */
        XmlNode();
        
       
        
        /**
		 * Construct an empty node named <i>name</i>
		 * @param name contains a valid tag name
		 */
        XmlNode(const char* name);
        ~XmlNode(void);
        
        
        
        /**
		 * Retrieve a copy of the tag name
		 * @return a copy of the tag name. 
		 */
        const char *getName(void) const;
        
        
        
        /**
		 * Add an attribute to the node
		 * @param attribute is a pointer on an instance of XmlAttribute
		 */
        void addAttribute(XmlAttribute *attribute);
        
        
        
        /**
		 * Set a simple (e.g. a textual) content to the node. Ex: &lt;tag&gt;this is a simple content&lt;/tag&gt;
		 * @param content is a string containing a textual content
		 */
        void setSimpleContent(const char* content);
        
        
        
        /**
		 * Get a copy of the simple (e.g. the textual) content of the node. 
		 * Ex: 
		 *      if the node is &lt;node&gt;this is &lt;child_node&gt;something&lt;/child_node&gt; a simple content&lt;/node&gt;, 
		 *      getSimpleContent() will return "this is a simple content"
		 *  @return a copy of simple content of the node, or NULL if node do not have any simple content
		 */
        const char *getSimpleContent(void) const;
        
        
        
        /**
		 * Add a child node to the node
		 * @param node is a pointer to an instance of XmlNode
		 */
        void addChildNode(XmlNode *node);
        
        
        
        /**
		 * Get a copy of the value of an attribute specified by <i>name</i>
		 * @param name contains the name of the attribute to get the value
		 * @return a copy of the attribute value.
		 */
        const char *getAttributeValueByName(const char* name);
        
        
        
        /**
		 * Set (or change, if already set) the value <i>value</i> of the attribute names <i>name</i>
		 * @param name contains the name of the target attribute
		 * @param value contains the value to set
		 */
        void setAttributeValueByName(const char* name, const char* value);
       
       
        /**
		 * Retrieve a reference on a child node whitch the name is equal to<i>name</i>.
		 * If several child nodes have the same name, a pointer on the first one in list is retrieved.
		 * If no child node named <i>name</i> exist, NULL is returned. 
		 * @param name contains the name of the child node to get. 
		 * @return a pointer on an instance of XmlNode, or NULL if no child node match <i>name</i>
		 * @see getNextChildNodeByName()
		 */
        XmlNode *getChildNodeByName(const char* name);
       
       
       
       
        /**
		 * Retrieved a reference on the next child node named <i>name</i>. 
		 * getChildNodeByName() must be call with <i>name</i> as parameter at least once before.
		 * @return  a pointer on an instance of XmlNode, or NULL if no more child node match <i>name</i>
		 * @see getNextChildNodeByName()
		 */
        XmlNode *getNextChildNodeByName(void);
        
        
        
        /**
		 * Retrieve a reference on the first child node in list (whatever its name)
		 * If node does not contain any child node, NULL is returned.
		 * @return a pointer on an instance of XmlNode, or NULL if node contains no child node
		 * @see getNextChildNode()
		 */
        XmlNode *getFirstChildNode(void);
        
        
        
        /**
		 * Retrieve a reference on the next child node is list
		 * getFirstChildNode() must be call at least once before
		 * @return a pointer on an instance of XmlNode, or NULL if node contains no more child node
		 * @see getFirstChildNode()
		 */
        XmlNode *getNextChildNode(void);
        
        
        
        /**
		 * Remove and destroy a the first child node in list named <i>name</i>`
		 * @param name contains the name of the child node to remove and destroy
		 */
		void removeChildNodeByName(const char* name);
        
        
        
        /**
		 * Get a textual representation of the node. Ex: &lt;node param1="value1"&gt;blah blah&lt;child_node&gt;blah&lt;/child_node&gt;&lt;/node&gt;
		 * @return a textual representation of the node
		 */
        const char *serialize(void);
};




/**
 * @brief SlimXmlParser is an xml parser. It takes a string representing an xml document as input and output an XmlNode
 */ 
class SlimXmlParser
{
    private:
        char *stream;
        SlimVector<XmlNode> elements;
    
        bool isSpace(char c);
        bool isLetter(char c);
        bool isDigit(char c);
        char *parseName(void);
        char *parseAttrValue(char quote);
        char *parsePcdata(void);
        XmlAttribute *parseAttribute(void);
        XmlNode *parseElement(void);
        bool parseElementContent(XmlNode* node);
        bool parseElementEndTag(XmlNode* node);
        int identifyNextItemType(void);
        void eatDoctype(void);
        void eatProcessingInstruction(void);
        void eatComments(void);
        void eatSpaces(void);
        
    public:
        SlimXmlParser();
        ~SlimXmlParser();
        
        
        
        /**
		 * Parse a xml document embedded in <i>xmlStream</i> and return a XmlNode instance
		 * @return an instance of XmlNode, or NULL if parsing failed, due to a non well-formed xml stream
		 */
        XmlNode *parse(const char *xmlStream);
};

char *itoa(long i, char *format);

#endif
