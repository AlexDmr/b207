
#ifndef _OVERSEACALLBACKSERVICE_
#define _OVERSEACALLBACKSERVICE_

#include <route66.h>
#include <slimcore.h>
#include <slimxml.h>


#define OCS_SUBSCRIBE "(^<ocs_subscribe>.*</ocs_subscribe>)"
/*
		<ocs_subscribe>
			<address ip="*.*.*.*" port="an integer"/>
			<xml_node_to_match name="the xml node name to match(string)">
				(
					<attribute name="a attribute name" value="a value to match"/>
				)*
   			</xml_node_to_match>
			<method> simplecontent | thewholething | predefinedanswer </method>
			(
				<answer_to_send> an string </answer_to_send>
			)?(requied if method == predefinedanswer, unused else)
		</osc_subscribe>
*/

#define OCS_UNSUBSCRIBE "(^<ocs_unsubscribe>.*</ocs_unsubscribe>)"
/*
		<ocs_unsubscribe>
			<address ip="*.*.*.*" port="an integer"/>
			<xml_node_to_match name="the xml node name to match(string)">
				(
					<attribute name="a attribute name" value="a value to match"/>
				)*
   			</xml_node_to_match>
		</osc_unsubscribe>
*/

#define ANY_XML_NODE "(^<.*>.*)"




class CallbackParam
{
	private:
		XmlNode *xml_node_to_match;
		char *answer_to_send;
		char *ip;
		unsigned int port;
		char *method;
	
	public:
		CallbackParam(void);
	
		CallbackParam(XmlNode *xml_node_to_match, const char *ip, unsigned int port, const char *method, const char *answer_to_send);
		
		const char *getXmlNodeToMatch(void);
		
		const char *getAnswerToSend(void);

		const char *getIp(void);
		
		unsigned int getPort(void);

		const char *getMethod(void);
		
		bool isMatching(XmlNode *xml_node);
		
		bool isEqual(XmlNode *xml_node);
};



class OverseaCallbackService
{
    private:
        Route66 *r66;
        SlimXmlParser xmlParser;
        SlimVector<CallbackParam> callbacks;
        SlimTcpClientSocket socket;
        
    public:
        OverseaCallbackService(Route66 *r66); 
        
        void onPing(unsigned int argc, char **argv);
        
        void onAnyXmlNode(unsigned int argc, char **argv);
   
        void onSubscribe(unsigned int argc, char **argv);
        
        void onUnsubscribe(unsigned int argc, char **argv);
};

#endif
