#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <slimxml.h>
#include <route66.h>
#include <steadypacketservice.h>



class Receiver
{
    private:
        Route66 *r66;
        
    public:
        Receiver(Route66 *r66) { this->r66 = r66; }
        
        void onAnyMessage(unsigned int argc, char **argv)
        {
                int i;
                for(i = 0; i < argc; i++)
                {
                   printf("#r66>'%s'\n", argv[i]);
                }
                
        }
};



int main(int argc, char *argv[])
{
    printf("# Test SteadyPacketService\n\n");
	printf("# Route66 initialisation... ");
   
	Route66 *r66 = new Route66("224.0.0.80", 1500);
	
	if(r66->getLastError() != R66_NOERROR)
    {
        printf("Error #%d.\n", r66->getLastError());
        return 0;
    }
    else
    {
        printf("Ok.\n");
    }
    
    Receiver *rec = new Receiver(r66);

    r66->bindMessageCallback("(.*)", new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onAnyMessage));
	if(r66->getLastError() != R66_NOERROR)
    {
        printf("# Route66 error #%d while binding callback.\n", r66->getLastError());
        return 0;
    }
    
    SteadyPacketService *sps = new SteadyPacketService("sps1", r66);
    sps->setTimeout(20000);
    printf("sps id: %s\n", sps->getId());
    XmlNode *node = new XmlNode("message");
    node->setSimpleContent("steady message 1");
    
    char **acks = sps->sendSteadyMessage(node, "lionel;alex;");
    if(acks != NULL)
    {
    	int k = 0;
    	while(acks[k] != NULL)
    	{
    		printf("%s\n", acks[k]);
    		k++;
    	}
    }
    else
    {
    	printf("ack ok.\n");
    }
    
    node = new XmlNode("message");
    node->setSimpleContent("steady message 2");
    
    acks = sps->sendSteadyMessage(node, "elodie;olivier;");
	if(acks != NULL)
    {
    	int k = 0;
    	while(acks[k] != NULL)
    	{
    		printf("%s\n", acks[k]);
    		k++;
    	}
    }
    else
    {
    	printf("ack ok.\n");
    }
	
    #if defined (unix) || defined (__APPLE__)
    pause();
    #endif

    #ifdef WIN32
    system("pause");
    #endif
    return 0;
}
