#ifndef _STEADYPACKETSERVICE_
#define _STEADYPACKETSERVICE_

#include <slimcore.h>
#include <slimxml.h>
#include <route66.h>

class PacketCounter
{
      private:
              char *id;
              long count;
      
      public:
             PacketCounter(void);
             PacketCounter(const char *id);
             bool setId(const char *id);
             const char *getId(void);
             void setCount(long count);
             long getCount(void);       
};




#define PACKETCOUNTERVECTOR_ACK         		0
#define PACKETCOUNTERVECTOR_DBL         		1

class PacketCounterVector
{
      private:
              SlimVector<PacketCounter> packetCounters;
      
      public:
             PacketCounterVector(void);
             int update(const char* id, long count);
             void reset(void);
};


#define STEADYPACKETSERVICE_DEFAULT_TIMEOUT         	1000 /* 1 sec */
#define STEADYPACKETSERVICE_ACK         		0
#define STEADYPACKETSERVICE_DBL         		1
#define STEADYPACKETSERVICE_NOT_A_RECIPIENT     2
#define STEADYPACKETSERVICE_NO_STEADY_DATA		3
#define STEADYPACKETSERVICE_ERROR				4

class SteadyPacketService
{
      private:
              Route66 *r66;
              char *uid;
              PacketCounterVector pcv;
              long packetCount;   
              SlimVector<char> acks;
              Waiter *ack_waiter; 
              long timeout;
              SlimXmlParser xmlParser;
              
              void onAck(unsigned int argc, char** argv);

      public:
           SteadyPacketService(void);
           SteadyPacketService(const char *id, Route66 *r66);
           void setTimeout(long timeout);
           long getTimeout(void);
           void setRoute66(Route66 *r66);
           const char *getId();
           char **sendSteadyMessage(XmlNode *message, const char *recipients);
           int acknowledge(XmlNode *message);
};

#endif
