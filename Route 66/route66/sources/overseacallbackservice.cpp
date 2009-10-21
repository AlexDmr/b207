#include <stdio.h>
#include <stdlib.h>
#include <overseacallbackservice.h>


CallbackParam::CallbackParam(void)
{
	this->xml_node_to_match = NULL;
	this->ip = NULL;
	this->port = 0;
	this->method = NULL;
}



CallbackParam::CallbackParam(XmlNode *xml_node_to_match, const char *ip, unsigned int port, const char *method, const char *answer_to_send = NULL)
{
	this->xml_node_to_match = xml_node_to_match;

	if(answer_to_send == NULL)
	{
		this->answer_to_send = NULL;
	}
	else
	{
		this->answer_to_send = new char[strlen(answer_to_send)+1]; strcpy(this->answer_to_send, answer_to_send);
	}

	this->ip = new char[strlen(ip)+1]; strcpy(this->ip, ip);
	this->port = port;
	this->method = new char[strlen(method)+1]; strcpy(this->method, method);
}



const char *CallbackParam::getXmlNodeToMatch(void)
{
	return xml_node_to_match->getAttributeValueByName("name");
}



const char *CallbackParam::getAnswerToSend(void)
{
	return answer_to_send;
}



const char *CallbackParam::getIp(void)
{
	return ip;
}



unsigned int CallbackParam::getPort(void)
{
	return port;
}



const char *CallbackParam::getMethod(void)
{
	return method;
}



bool CallbackParam::isMatching(XmlNode *xml_node)
{
    #ifdef __TRACE_ON
	printf("#CallbackParam::isMatching.\n");
	#endif

    if(strcmp(xml_node->getName(), xml_node_to_match->getAttributeValueByName("name")) == 0)
    {
        #ifdef __TRACE_ON
        printf("#CallbackParam::isMatching: node names match.\n");
        #endif

        XmlNode *attribute = xml_node_to_match->getChildNodeByName("attribute");
        while(attribute != NULL)
        {
            #ifdef __TRACE_ON
            printf("#CallbackParam::isMatching: there is an attribute to compare.\n");
            #endif

            const char *value = xml_node->getAttributeValueByName(attribute->getAttributeValueByName("name"));

            #ifdef __TRACE_ON
            printf("#CallbackParam::isMatching: attribute value='%s'.\n", value);
            #endif

            if(value == NULL)
            {
                #ifdef __TRACE_ON
                printf("#CallbackParam::isMatching: a required attribute is missing (%s).\n", attribute->getAttributeValueByName("name"));
                #endif

                return false;
            }

            if(strcmp(value, attribute->getAttributeValueByName("value")) != 0)
            {
           		#ifdef __TRACE_ON
           		printf("#CallbackParam::isMatching: attribute values do not match, returning false.\n");
          		#endif

             	return false;
      		}
            attribute = xml_node_to_match->getNextChildNodeByName();
        }

       	#ifdef __TRACE_ON
       	printf("#CallbackParam::isMatching: everything match, returning true.\n");
       	#endif
         return true;
    }

    return false;
}



bool CallbackParam::isEqual(XmlNode *xml_node)
{
    #ifdef __TRACE_ON
	printf("#CallbackParam::isEqual.\n");
	#endif

    if(strcmp(xml_node->getAttributeValueByName("name"), xml_node_to_match->getAttributeValueByName("name")) == 0)
    {
        #ifdef __TRACE_ON
        printf("#CallbackParam::isEqual: node names match.\n");
        #endif

        XmlNode *attribute = xml_node->getChildNodeByName("attribute");
        XmlNode *attribute_to_match = xml_node_to_match->getChildNodeByName("attribute");

        while((attribute != NULL) && (attribute_to_match != NULL))
        {
            #ifdef __TRACE_ON
            printf("#CallbackParam::isEqual: there is an attribute to compare.\n");
            #endif

            if((strcmp(attribute->getAttributeValueByName("value"), attribute_to_match->getAttributeValueByName("value")) != 0) ||
               (strcmp(attribute->getAttributeValueByName("name"), attribute_to_match->getAttributeValueByName("name")) != 0))
            {
           		#ifdef __TRACE_ON
           		printf("#CallbackParam::isEqual: attribute values do not match, returning false.\n");
          		#endif

             	return false;
      		}
            attribute = xml_node->getNextChildNodeByName();
            attribute_to_match = xml_node_to_match->getNextChildNodeByName();
        }

        if((attribute == NULL) && (attribute_to_match == NULL))
        {
       		#ifdef __TRACE_ON
       		printf("#CallbackParam::isEqual: everything match, returning true.\n");
       		#endif
            return true;
        }
        else
        {
            #ifdef __TRACE_ON
            printf("#CallbackParam::isEqual: different attribute count.\n");
            #endif
            return false;
        }
    }

    return false;
}








OverseaCallbackService::OverseaCallbackService(Route66 *r66)
{
	this->r66 = r66;

	r66->bindMessageCallback("<ping/>", new Route66MessageCallbackOf<OverseaCallbackService>(this, &OverseaCallbackService::onPing));
	r66->bindMessageCallback(OCS_SUBSCRIBE, new Route66MessageCallbackOf<OverseaCallbackService>(this, &OverseaCallbackService::onSubscribe));
    r66->bindMessageCallback(OCS_UNSUBSCRIBE, new Route66MessageCallbackOf<OverseaCallbackService>(this, &OverseaCallbackService::onUnsubscribe));
    r66->bindMessageCallback(ANY_XML_NODE, new Route66MessageCallbackOf<OverseaCallbackService>(this, &OverseaCallbackService::onAnyXmlNode));

}



void OverseaCallbackService::onPing(unsigned int argc, char **argv)
{
	#ifdef __TRACE_ON
	printf("#oversea_callback_service pong.\n");
	#endif
}


void OverseaCallbackService::onAnyXmlNode(unsigned int argc, char **argv)
{
	XmlNode *xml_node;
	if((xml_node = xmlParser.parse((const char *)(argv[0]))) == NULL)
	{
		return;
	}

	if((strcmp(xml_node->getName(), "ocs_subscribe") == 0) || (strcmp(xml_node->getName(), "ocs_unsubscribe") == 0))
	{
		#ifdef __TRACE_ON
		printf("#onAnyXmlNode return\n");
		#endif

		return;
	}

	CallbackParam *cp;
	//debug

	#ifdef __TRACE_ON
	printf("#OverseaCallbackService::onAnyXmlNode: callbacks vector scanning\n");
	#endif

	callbacks.beginSequentialReading();
	while((cp = callbacks.readItemPointer() ) != NULL)
	{
		#ifdef __TRACE_ON
		printf("#OverseaCallbackService::onAnyXmlNode: scanning: begin of while\n");
		#endif

  		//if(strcmp(cp->getXmlNodeToMatch(), xml_node->getName()) == 0)
  		if(cp->isMatching(xml_node))
		{
			#ifdef __TRACE_ON
			printf("-> match found: %s, ip: %s, port:%d, method:%s, answer:%s.\n",
				cp->getXmlNodeToMatch(),
				cp->getIp(),
				cp->getPort(),
				cp->getMethod(),
				cp->getAnswerToSend());
			#endif

			if(socket.getConnectionTo(cp->getIp(), cp->getPort()) == 0)
			{
				const char *data_to_send;

				if(strcmp(cp->getMethod(), "predefinedanswer") == 0)
				{
					data_to_send = cp->getAnswerToSend();
				}
				else if(strcmp(cp->getMethod(), "simplecontent") == 0)
				{
					data_to_send = xml_node->getSimpleContent();
				}

				else if(strcmp(cp->getMethod(), "thewholething") == 0)
				{
					data_to_send = xml_node->serialize();
				}

				if((data_to_send != NULL) && (strlen(data_to_send) != 0))
				{
					socket.sendData(data_to_send, strlen(data_to_send));
				}

				socket.closeSocket();
			}
			else
			{
				#ifdef __TRACE_ON
				printf("# ERROR: Connection to host failed.\n");
				#endif
			}
		}

		#ifdef __TRACE_ON
		printf("#OverseaCallbackService::onAnyXmlNode: scanning: end of while\n");
		#endif
	}

	#ifdef __TRACE_ON
	printf("#OverseaCallbackService::onAnyXmlNode: scanning: end of fonction\n");
	#endif
}



void OverseaCallbackService::onSubscribe(unsigned int argc, char **argv)
{
	#ifdef __TRACE_ON
 	printf("#OverseaCallbackService::onSubscribe+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	#endif

 	XmlNode *subscribe;
	if((subscribe = xmlParser.parse((const char *)(argv[0]))) == NULL)
	{
		#ifdef __TRACE_ON
		printf("#OverseaCallbackService::onSubscribe: parsing failed\n");
		#endif
  		return;
	}

	XmlNode *address, *xml_node_to_match, *method, *answer_to_send;
	if(	((address = subscribe->getChildNodeByName("address")) == NULL) ||
		((xml_node_to_match = subscribe->getChildNodeByName("xml_node_to_match")) == NULL) ||
		((method = subscribe->getChildNodeByName("method")) == NULL))
	{
		#ifdef __TRACE_ON
		printf("#OverseaCallbackService::onSubscribe: a required child node is missing\n");
		#endif
  		return;
	}

	char *answer_data = NULL;
	if(strcmp(method->getSimpleContent(), "predefinedanswer") == 0)
	{
		if((answer_to_send = subscribe->getChildNodeByName("answer_to_send")) == NULL)
		{
			#ifdef __TRACE_ON
			printf("#OverseaCallbackService::onSubscribe: no answer to send\n");
			#endif
   			return;
		}

		answer_data = new char[strlen(answer_to_send->getSimpleContent())+1]; strcpy(answer_data, answer_to_send->getSimpleContent());
	}

	#ifdef __TRACE_ON
	printf("#OverseaCallbackService::onSubscribe: added node: %s\n", xml_node_to_match->serialize());
	#endif

 	CallbackParam *cp = new CallbackParam(xml_node_to_match,
											address->getAttributeValueByName("ip"),
											(unsigned int) strtol(address->getAttributeValueByName("port"), NULL, 10),
											method->getSimpleContent(),
											answer_data);

	callbacks.push(cp);
	if(answer_data) delete[] answer_data;

	//debug
	#ifdef __TRACE_ON
	printf("#debug: dump callbacks vector\n");

	callbacks.beginSequentialReading();
	while((cp = callbacks.readItem() ) != NULL)
	{
		printf("node: %s, ip: %s, port:%d, method:%s, answer:%s.\n",
				cp->getXmlNodeToMatch(),
				cp->getIp(),
				cp->getPort(),
				cp->getMethod(),
				cp->getAnswerToSend());
	}
	#endif
}



void OverseaCallbackService::onUnsubscribe(unsigned int argc, char **argv)
{
	#ifdef __TRACE_ON
 	printf("#OverseaCallbackService::onUnsubscribe----------------------------------------------------------------\n");
	#endif

 	XmlNode *unsubscribe;
	if((unsubscribe = xmlParser.parse((const char *)(argv[0]))) == NULL)
	{
		#ifdef __TRACE_ON
		printf("#OverseaCallbackService::onUnsubscribe: parsing failed\n");
		#endif
  		return;
	}

	XmlNode *address, *xml_node_to_match;
	if(	((address = unsubscribe->getChildNodeByName("address")) == NULL) ||
		((xml_node_to_match = unsubscribe->getChildNodeByName("xml_node_to_match")) == NULL))
	{
		#ifdef __TRACE_ON
		printf("#OverseaCallbackService::onUnsubscribe: a required childnode is missing\n");
		#endif
  		return;
	}

	CallbackParam *cp;

	//debug
	#ifdef __TRACE_ON
	printf("#unsubscribe\n");
	printf("#debug: dump callbacks vector\n");
	#endif

	callbacks.beginSequentialReading();
	while((cp = callbacks.readItemPointer() ) != NULL)
	{
		#ifdef __TRACE_ON
		printf("node: %s, ip: %s, port:%d, method:%s, answer:%s.\n",
				cp->getXmlNodeToMatch(),
				cp->getIp(),
				cp->getPort(),
				cp->getMethod(),
				cp->getAnswerToSend());
		#endif

		if( cp->isEqual(xml_node_to_match) &&
			(strcmp(cp->getIp(), address->getAttributeValueByName("ip")) == 0) &&
			(cp->getPort() == (unsigned int) strtol(address->getAttributeValueByName("port"), NULL, 10)))
		{
			#ifdef __TRACE_ON
			printf("## erasing item\n");
			#endif

			callbacks.eraseItemAtReadingCursor();
		}
	}
}
