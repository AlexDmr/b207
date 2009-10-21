#ifndef _SLIMXMLPATTERN_
#define _SLIMXMLPATTERN_

#include <slimcore.h>
#include <slimxmlparser.h>

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
#define SXP_ATTRIBUTE_ID            "sxp:id"
#define SXP_ATTRIBUTE_NAME          "sxp:name"
#define SXP_RESULT_ELEMENT_NAME		"sxp:pattern_match"

#define SXP_NO_ERROR                    0
#define SXP_FALSE						0
#define SXP_TRUE						1
#define SXP_NO_PATTERN_TAG				2
#define SXP_NO_SCHEMA_REFERENCE			3
#define SXP_NAMESPACE_NOT_SUPPORTED		4
#define SXP_ERROR_NO_PATTERN_ID         5
#define SXP_ERROR_UNEXPECTED_END        6
#define SXP_NO_NAME_ATTRIBUTE			7
#define SXP_UNKNOWN_OPERATOR			8
#define SXP_NO_OPERATOR_DATA			9

#define SXPPI_AND						0
#define SXPPI_OR						1
#define SXPPI_ELEMENT					10
#define SXPPI_COMPLEXCONTENT_CONTAIN	20
#define SXPPI_COMPLEXCONTENT_EXCLUDE	21
#define SXPPI_SIMPLECONTENT_CONTAIN		22
#define SXPPI_SIMPLECONTENT_EQUAL		23
#define SXPPI_SIMPLECONTENT_MATCH		24
#define SXPPI_SIMPLECONTENT_EXCLUDE		26

#define SXP_TOPLEVEL_ELEMENT			true
#define SXP_SUB_ELEMENT					false

class PatternItem;

class SxpElement
{
	private:
		SlimVector<XmlNode> matches;
		int match;
		char *element_id;
		char *element_name;
		PatternItem *element_pattern;
		
	public:
		SxpElement(const char *element_name, const char *element_id);
		void setElementPattern(PatternItem *element_pattern);
		const char *getId() const;
		long getNbOfMatches(void) const;
		XmlNode *getMatchAt(int position);
		int isMatch(XmlNode *source, bool toplevel);
		int evaluate(void) const;
		void print(void) const;
};




class PatternItem
{
	private:
		int op;
		SlimVector<PatternItem> operands;
		SxpElement *element;
		char *data;
		
	public:
		PatternItem(int op);
		PatternItem(SxpElement *element);
		int addOperand(PatternItem *item);
		void setData(const char *data);
		int isMatch(XmlNode *source);
		int evaluate();
};




class Pattern
{
	private:
		char *id;
		PatternItem *topLevelItem;
		SlimVector<SxpElement> topLevelElements;
	
	public:
		Pattern(const char *pattern_id);
		void setTopLevelItem(PatternItem *patternItem);
		PatternItem *getTopLevelItem(void) const;
		const char *getId(void) const;
		void addTopLevelElement(SxpElement *element);
		long getNbOfTopLevelElement(void) const;
		SxpElement *getTopLevelElementAt(int position);
		void print(void) const;
};


/**
 * @brief SlimXmlPattern is a class that encapsulate pattern matching mechanisms
 * on xml document. 
 */
 /**
 * SlimXmlPattern understand pattern expressed in the xml-language defined by http://iihm.imag.fr/balme/sxp/01 namespace.
 * To be usable, an xml-expressed pattern have to be compile to a Pattern object. 
 * A match result is a XML structure that comply http://iihm.imag.fr/balme/sxp/01
 */ 
class SlimXmlPattern
{
    private:
        SlimXmlParser *xmlParser;
        Pattern *pattern;
        
        int lastError;
        void init(void);
        
        PatternItem *compile_toplevel_operator(XmlNode *node, int op);
        PatternItem *compile_element(XmlNode *node, bool toplevel);
        PatternItem *compile_element_operator(XmlNode *node, int op);
        void analyse_element(XmlNode *element, Pattern *pattern);
            
    public:
    	/**
		* Construct an SlimXmlPattern object ready to run
		*/
        SlimXmlPattern(void);
        
        
        
        /**
		* Construct an SlimXmlPattern object ready to run that share an SlimXmlParser with someone else.
		* @param xmlParser is a pointer to a valid SlimXmlParser object
		*/
        SlimXmlPattern(SlimXmlParser *xmlParser);
        
        
        
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
