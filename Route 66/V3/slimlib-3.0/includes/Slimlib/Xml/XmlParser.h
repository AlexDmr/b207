#ifndef __Class_XmlParser_
#define __Class_XmlParser_

#include <Slimlib/Core/VectorClass.h>
#include <Slimlib/Core/Property.h>

#define XML_HEADER              "<?xml version=\"1.0\" ?>"
#define XML_DOCTYPE_FORMAT      "<!DOCTYPE %s PUBLIC \"%s\">"       //1st %s: root element, 2nd %s: reference
#define XML_XMLNS				"xmlns"
#define XML_TARGETNAMESPACE		"targetNamespace"
#define XML_KEY_SEPARATOR		":"


/**
 * @brief XmlAttribute is a class to encapsulate an attribute of an XML tag
 */
class XmlAttribute: public Object
{
    private:
        char *name;
        char *prefix;
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
       virtual ~XmlAttribute(void);



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
		* Retrieve a the attribute name
		* @return a string containing the attribute name
		*/
       const char *getName(void) const;



	   /**
		* Retrieve a the attribute local name
		* Ex: if attribute name is "xsd:type" getLocalName() returns "type".
		* @return a string containing the attribute name's radical
		*/
       const char *getLocalName(void) const;



		/**
		* Retrieve a copy of the attribute value
		* @return a string containing a copy of the attribute value
		*/
       const char *getValue(void) const;


	    /**
		* Retrieve a the attribute local value
		* Ex: if attribute name is "xsd:string" getLocalValue() returns "string".
		* @return a string containing the attribute name's radical
		*/
       const char *getLocalValue(void) const;



       /**
		* Get a textual representation of the attribute (i.e.: something like name="value")
		* @return a string containing a textual representation of the attribute
		*/
       char *serialize(void) const;



	   /**
		* Compare this attribute with <i>attribute</i>.
		* @return true if attribute names and attribute values are equal, false otherwise;
		*/
	   bool compareTo(XmlAttribute *attribute);

	   /**
	    * Make a copy of this attribute
	    * @return a new XmlAttribute object equals to this one
	    */
	   XmlAttribute *copy();
};





/**
 * @brief XmlNode is a class to encapsulate an XML tag and its content
 */
class XmlNode: public Object
{
    private:
        char *name;
        char *prefix;
        char *searched_name;
        XmlNode *parent;

        Vector<XmlAttribute *> attributes;
        char *simpleContent;
        Vector<XmlNode *> complexContent;
        char *serialize(int indentationLevel);

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


			virtual ~XmlNode(void);

			/** Return a pointer onto the parent node, or NULL if this node is a root node.
			 * @return a pointer onto an XmlNode object.
			 */
			XmlNode *getParent();

			/** Set a node as the parent node.
			 * @param parent (XmlNode *) a pointer onto an XmlNode object.
			 */
			void setParent(XmlNode *parent);


        /**
		 * Retrieve the tag name
		 * @return a string that is the tag name.
		 */
        const char *getName(void) const;


		 /**
		  * Retrieve the local tag name, without its namespace's prefix
		  * Ex: if tag name is "xsd:sequence", getLocalName() returns "sequence".
		  * @return a string that is the local tag name
		  */
        const char *getLocalName(void) const;


		/**
		 * Compare <i>string</i> with node's name.
		 * Ex: if node's name is "xsd:schema", compareName("schema").
		 * @param string a string that contains a valid XML local name, e.g. without any namespace's prefix
		 * @return true if <i>string</i> equals node's local name. Otherwise, return false.
		 */
		bool compareName(const char *string) const;



		/**
		 * Set a name to the node. Ex: &lt;node_name&gt;this is node's name is node_name&lt;/node_name&gt;
		 * @param name is a string containing a node name
		 */
        void setName(const char* name);



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
		 * Get the number of child nodes
		 * @return an unsigned int that is the number of child nodes in this node.
		 */
        unsigned int getChildNodesCardinality(void);



		 /**
		 * Get a pointer onto the attribute specified by <i>name</i>
		 * @param name contains the name of the attribute to get
		 * @return a pointer onto an attribute.
		 */
        XmlAttribute *getAttributeByName(const char* name);


        /**
		 * Get a copy of the value of an attribute specified by <i>name</i>
		 * @param name contains the name of the attribute to get the value
		 * @return a copy of the attribute value.
		 */
        const char *getAttributeValueByName(const char* name);



		/**
		 * Get a pointer on the first attribute which name contains <i>snippet</i>
		 * @param snippet contains the snippet that has to be found in attribute's name.
		 * @return a copy of the attribute value.
		 */
        XmlAttribute *getFirstAttributeWithNameLike(const char* snippet);



		/**
		 * Get a pointer on the next attribute which name contains <i>snippet</i>
		 * @param snippet contains the snippet that has to be found in attribute's name.
		 * @return a copy of the attribute value.
		 */
        XmlAttribute *getNextAttributeWithNameLike(const char *snippet);



		/**
		 * Get the number of attributes
		 * @return an unsigned int that is the number of attributes for this node.
		 */
        unsigned int getAttributesCardinality(void);



		/**
		 * Get a pointer on the first attribute, if exists.
		 * @return a pointer on the first attribute, if an attribute exist, NULL otherwise.
		 */
        XmlAttribute *getFirstAttribute(void);


		/**
		 * Get a pointer on the next attribute, if exists.
		 * getFirstAttribute() must be call at least once before
		 * @return a pointer on the next attribute, if an other attribute exist, NULL otherwise.
		 */
        XmlAttribute *getNextAttribute(void);



        /**
		 * Set (or change, if already set) the value <i>value</i> of the attribute names <i>name</i>
		 * @param name contains the name of the target attribute
		 * @param value contains the value to set
		 */
        void setAttributeValueByName(const char* name, const char* value);



        /**
		 * Retrieve a reference on a child node which the name is equal to<i>name</i>.
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
        char *serialize(void);


		/**
		* Compare this node with <i>xmlnode</i>.
		* @return true if attribute names, attribute values and node contents are equal, false otherwise;
		*/
	   bool compareTo(XmlNode *node);

	   /**
	    * Make a copy of this node
	    * @return a new XmlNode object equals to this one
	    */
	   XmlNode *copy();

};




/**
 * @brief XmlParser is an xml parser. It takes a string representing an xml document as input and output an XmlNode
 */
class XmlParser: public Object
{
    private:
        char *stream;
        Vector<XmlNode *> elements;

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
        XmlParser();
        virtual ~XmlParser();



        /**
		 * Parse a xml document embedded in <i>xmlStream</i> and return a XmlNode instance
		 * @return an instance of XmlNode, or NULL if parsing failed, due to a non well-formed xml stream
		 */
        XmlNode *parse(const char *xmlStream);
};

/**
 *
 */
char *itoa(long i, char *format);

/**
 * Extract an XML local name from a XML prefixed name. If <i>xmlToken</i> is prefixed by a namespace's prefix,
 * LocalNameOf() will return only the local name part.
 * @param xmlToken an XML node's name.
 * @return return a string that contains the XML local name from <i>xmlToken</i>
 */
const char *LocalNameOf(const char *xmlToken);

/**
 * @brief XmlNamespaceHandler is a class to manipulate namespace definition specified in an XML document's root node.
 * After parsing xmlns attributes of a XML root node (thanks to a call to addNamespaces()), XmlNamespaceHandler can
 * tell you URIs for targetNamespace, defaultNamespace, and Namespaces in use in the XML documents.
 */
class XmlNamespaceHandler: public Object
{
	private:
		Vector<Property *> namespaces;
		char *defaultNamespace;
		char *targetNamespace;

	public:
		/**
		 * Construct an XML namespace handler
		 */
		XmlNamespaceHandler(void);

		/**
		 * Dispose an XML namespace handler
		 */
		virtual ~XmlNamespaceHandler(void);

		/**
		 * Setting manualy the document default namespace.
		 * @param uri a string that contains a namespace uri
		 */
		void set_defaultNamespace(const char *uri);

		/**
		 * Setting manualy the document target namespace.
		 * @param uri a string that contains a namespace uri
		 */
		void set_targetNamespace(const char *uri);

		/**
		 * Setting manualy a namespace in use in a document.
		 * In xmlns:xsd="http://www.w3.org/2001/XMLSchema", 'xsd' is the namespace's key and 'http://www.w3.org/2001/XMLSchema' the namespace's uri.
		 * @param key a string that contains a namespace key
		 * @param uri a string that contains a namespace uri
		 */
		void addNamespace(const char *key, const char *uri);

		/**
		 * addNamespaces is the best way to use XmlNamespaceHandler.
		 * addNamespaces explore <i>documentRootNode<i> and find all xmlns attributes and the targetNamespace attribute.
		 * Then, you can access their value via getNamespace(), getNamespaceKey(), get_defaultNamespace(), and get_targetNamespace();
		 * @param documentRootNode an XmlNode class that contains an XML root node.
		 */
		void addNamespaces(XmlNode *documentRootNode);

		/**
		 * Return the document default namespace, or NULL if not set.
		 */
		const char *get_defaultNamespace(void);

		/**
		 * Return the document target namespace, or NULL if not set.
		 */
		const char *get_targetNamespace(void);

		/**
		 * Return the namespace that corresponds to the key <i>namespaceKey</i>, or NULL if no namespace matches <i>namespaceKey</i>.
		 * @param namespaceKey is a string that contains a namespace key, with or without the final ":"
		 * @return a string that represents the XML namespace that matches <i>namespaceKey</i>
		 */
		const char *getNamespace(const char *namespaceKey);

		/**
		 * Return the namespace that define <i>node</i>.
		 * Ex: if xmlns:xsd="blahblah" is defined and node name is "xsd:sequence", then "blahblah" is returned.
		 * @param node is a childnode of the <i>documentRootNode</i> specified while calling addNamespaces().
		 * @return a string that represents the XML namespace that matches <i>node</i>'s name.
		 */
		const char *getNamespace(XmlNode *node);

		/**
		 * Return the namespace that define <i>attribute</i>.
		 * Ex: if xmlns:xsd="blahblah" is defined and attribute name is "xsd:string", then "blahblah" is returned.
		 * @param attribute is an attribute of a childnode of the <i>documentRootNode</i> specified while calling addNamespaces().
		 * @return a string that represents the XML namespace that matches <i>attribute</i>'s name.
		 */
		const char *getNamespace(XmlAttribute *attribute);

		/**
		 * Return the namespace key that mathes the URI <i>uri</i>.
		 * Ex: if xmlns:xsd="blahblah" is defined and <i>uri</i> is "blahblah", then "xsd:" is returned.
		 * @param uri is a string that contains an URI.
		 * @return a string that represents the namespace key that matches <i>uri</i>.
		 */
		const char *getNamespaceKey(const char *uri);
};

#endif
