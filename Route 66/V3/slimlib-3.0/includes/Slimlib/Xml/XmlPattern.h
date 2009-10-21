#ifndef __Class_XmlPattern_
#define __Class_XmlPattern_

#include <Slimlib/Core/VectorClass.h>
#include <Slimlib/Xml/XmlParser.h>

#define SXP_XMLNS                   "xmlns:sxp"
#define SXP_XMLNS_CONTENT           "http://iihm.imag.fr/balme/sxp/01"

//sxp language keyword
#define SXP_PATTERN                 "sxp:pattern"
#define SXP_AND                     "sxp:and"
#define SXP_OR                      "sxp:or"
#define SXP_ELEMENT                 "sxp:element"
#define SXP_SIMPLECONTENT_EQUAL     "sxp:simplecontent_equal"
#define SXP_SIMPLECONTENT_MATCH     "sxp:simplecontent_match"
#define SXP_SIMPLECONTENT_CONTAIN   "sxp:simplecontent_contain"
#define SXP_SIMPLECONTENT_EXCLUDE   "sxp:simplecontent_exclude"
#define SXP_COMPLEXCONTENT_CONTAIN  "sxp:complexcontent_contain"
#define SXP_COMPLEXCONTENT_EXCLUDE  "sxp:complexcontent_exclude"
#define SXP_ATTRIBUTE_EXIST			"sxp:attribute_exist"
#define SXP_ATTRIBUTE_EQUAL			"sxp:attribute_equal"
#define SXP_ATTRIBUTE_ID            "sxp:id"
#define SXP_ATTRIBUTE_NAME          "sxp:name"
#define SXP_RESULT_ELEMENT_NAME		"sxp:pattern_match"

class PatternItem;

class SxpElement: public Object
{
	private:
		Vector<XmlNode *> matches;
		int match;
		char *element_id;
		char *element_name;
		PatternItem *element_pattern;
		
	public:
		SxpElement(const char *element_name, const char *element_id);
		virtual ~SxpElement() {};
		void setElementPattern(PatternItem *element_pattern);
		const char *getId() const;
		long getNbOfMatches(void) const;
		XmlNode *getMatchAt(int position);
		int isMatch(XmlNode *source, bool toplevel);
		int evaluate(void) const;
		void print(void) const;
};




class PatternItem: public Object
{
	private:
		int op;
		Vector<PatternItem *> operands;
		SxpElement *element;
		char *data;
		char *data_ext;
		
	public:
		PatternItem(int op);
		PatternItem(SxpElement *element);
		virtual ~PatternItem() {};
		int addOperand(PatternItem *item);
		void setData(const char *data);
		void setDataExt(const char *data);
		int isMatch(XmlNode *source);
		int evaluate();
};




class Pattern: public Object
{
	private:
		char *id;
		PatternItem *topLevelItem;
		Vector<SxpElement *> topLevelElements;
	
	public:
		Pattern(const char *pattern_id);
		virtual ~Pattern() {};
		void setTopLevelItem(PatternItem *patternItem);
		PatternItem *getTopLevelItem(void) const;
		const char *getId(void) const;
		void addTopLevelElement(SxpElement *element);
		long getNbOfTopLevelElement(void) const;
		SxpElement *getTopLevelElementAt(int position);
		void print(void);
};


/**
 * @brief XmlPattern is a class that encapsulate pattern matching mechanisms
 * on xml document. 
 */
 /**
 * XmlPattern understand pattern expressed in the xml-language defined by http://iihm.imag.fr/balme/sxp/01 namespace.
 * To be usable, an xml-expressed pattern have to be compile to a Pattern object. 
 * A match result is a XML structure that comply http://iihm.imag.fr/balme/sxp/01
 */ 
class XmlPattern: public Object
{
    private:
        XmlParser *xmlParser;
        Pattern *pattern;
        
        int lastError;
        void init(void);
        
        PatternItem *compile_toplevel_operator(XmlNode *node, int op);
        PatternItem *compile_element(XmlNode *node, bool toplevel);
        PatternItem *compile_element_operator(XmlNode *node, int op);
        void analyse_element(XmlNode *element, Pattern *pattern);
            
    public:
    	/**
		* Construct an XmlPattern object ready to run
		*/
        XmlPattern(void);
        
        
        
        /**
		* Construct an XmlPattern object ready to run that share an XmlParser with someone else.
		* @param xmlParser is a pointer to a valid XmlParser object
		*/
        XmlPattern(XmlParser *xmlParser);
        
        
        
         /**
		 * Destruct a Pattern object
		 */
        virtual ~XmlPattern() {};
        
        
        
        /**
		* Compile a xml-formated pattern. This xml-expressed pattern must comply with http://iihm.imag.fr/balme/sxp/01 namespace.
		* @param pattern is a pointer to a null-terminated string that contains an xml-formated pattern. 
		* @return a reference to a Pattern object, or NULL if an error occurs while compiling.
		*/
        Pattern *compile_pattern(const char *pattern);
        
        
        
        /**
		* Compile a pattern stored in an XML structure. This XML structure must be obtained by parsing an xml-formatted pattern compliant with http://iihm.imag.fr/balme/sxp/01 namespace
		* @param pattern is a pointer to a null-terminated string that contains an xml-formated pattern. 
		* @return a reference to a Pattern object, or NULL if an error occurs while compiling.
		* @error an error code
		*/
        Pattern *compile_pattern(XmlNode *pattern);
        
        
        
        /**
        * Test if the xml-formated string contained in <i>source</i> match the pattern <i>pattern</i>. 
        * If one or more match are found in <i>source</i>, <i>getMatch()</i> will return an XmlNode object containing the result.
        * @param source contains the xml-formated string to analyse.
        * @param pattern is a valid compiled pattern object.
        * @return an XML structure containing a result if getMatch() found a match, NULL else.
        */
        XmlNode *getMatch(const char *source, Pattern *pattern);
        
        
        
        /**
        * Test if the XML structure contained in <i>source</i> match the pattern <i>pattern</i>. 
        * If one or more match are found in <i>source</i>, <i>getMatch()</i> will return an XmlNode object containing the result.
        * @param source contains an XML structure to analyse.
        * @param pattern is a valid compiled pattern object.
        * @return an XML structure containing a result if getMatch() found a match, NULL else.
        */
        XmlNode *getMatch(XmlNode *source, Pattern *pattern);
        
        
        /**
        * @return the last error code
        */
        int getLastError(void) const;
};

#endif
