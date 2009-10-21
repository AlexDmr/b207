#include <steadypacketservice.h>


PacketCounter::PacketCounter(void)
{
    this->id = NULL;
    this->count = 0;
}


PacketCounter::PacketCounter(const char *id)
{
    this->id = NULL;
    if(id != NULL)
    {
        this->id = new char[strlen(id)+1]; strcpy(this->id, id);
    }

    this->count = 0;
}



bool PacketCounter::setId(const char *id)
{
    if(id != NULL)
    {   this->id = new char[strlen(id)+1]; strcpy(this->id, id);
        if(this->id == NULL)
        {
            return false;
        }
        return true;
    }
    return false;
}



const char *PacketCounter::getId(void)
{
     return this->id;
}



void PacketCounter::setCount(long count)
{
     this->count = count;
}



long PacketCounter::getCount(void)
{
     return this->count;
}







PacketCounterVector::PacketCounterVector(void)
{
}




int PacketCounterVector::update(const char* id, long count)
{
	PacketCounter *pc;

	packetCounters.beginSequentialReading();
	while((pc = packetCounters.readItemPointer()) != NULL)
	{
		if(strcmp(id, pc->getId()) == 0)
		{
			if(count == pc->getCount())
			{
				return PACKETCOUNTERVECTOR_DBL;
			}
			else
			{
				pc->setCount(count);
				return PACKETCOUNTERVECTOR_ACK;
			}
		}
	}

	pc = new PacketCounter(id);
	pc->setCount(count);
	packetCounters.push(pc);

	return PACKETCOUNTERVECTOR_ACK;
}



void PacketCounterVector::reset(void)
{
	packetCounters.clear();
}






SteadyPacketService::SteadyPacketService(void)
{
	SteadyPacketService("sPs", NULL);
}



SteadyPacketService::SteadyPacketService(const char *id, Route66 *r66)
{
	this->ack_waiter = new Waiter();
	this->packetCount = 0;
	this->r66 = r66;
	this->uid = generate_uid(id);
	this->timeout = STEADYPACKETSERVICE_DEFAULT_TIMEOUT;
	if(this->r66 != NULL)
	{
		char regexp[100];
		sprintf(regexp, "^(<SteadyPacketAck to=\"%s\" .*/>)$", this->uid);
		this->r66->bindMessageCallback(regexp, new Route66MessageCallbackOf<SteadyPacketService>(this, &SteadyPacketService::onAck));
	}
}



void SteadyPacketService::setTimeout(long timeout)
{
	this->timeout = timeout;
}



long SteadyPacketService::getTimeout(void)
{
	return this->timeout;
}



void SteadyPacketService::setRoute66(Route66 *r66)
{
	this->r66 = r66;
}



const char *SteadyPacketService::getId()
{
	return this->uid;
}



char **SteadyPacketService::sendSteadyMessage(XmlNode *message, const char *recipients)
{
	if((r66 == NULL) || (recipients == NULL) || message == NULL)
	{
		return NULL;
	}

	#ifdef __TRACE_ON
	printf("#sps: recipients = '%s'\n", recipients);
	#endif

	char *r, *recipient;

	r = new char[strlen(recipients)+1]; strcpy(r, recipients);

	recipient = strtok(r, ";");

	while(recipient != NULL)
	{
		#ifdef __TRACE_ON
		printf("#sps: recipient = '%s'\n", recipient);
		#endif

		acks.push(recipient);
		recipient = strtok(NULL, ";");
	}

	this->packetCount++;

	XmlNode *spd = new XmlNode("SteadyPacketData");
	spd->addAttribute(new XmlAttribute("sender_id", this->uid));
	spd->addAttribute(new XmlAttribute("packet_nb", itoa(this->packetCount, "%x")));
	spd->addAttribute(new XmlAttribute("recipients", recipients));

	message->addChildNode(spd);

	r66->sendMessage(message->serialize());

	int rc = ack_waiter->timedwait(timeout);

	if(rc != 0)
	{
		char **argv = new char*[acks.size() + 1];
		char *arg;
		char *c;
		int k = 0;
		acks.beginSequentialReading();
		while((c = acks.readItemPointer()) != NULL)
		{
			argv[k] = c;
			#ifdef __TRACE_ON
			printf("#sps: ack %d, %s.\n", k, c);
			#endif
            k++;
		}
		argv[k] = NULL;

		#ifdef __TRACE_ON
		printf("#sps: recipients did not respond.\n", recipient);
		#endif
		acks.clear();
		return argv;
	}
	else
	{
		#ifdef __TRACE_ON
		printf("#sps: sendMessage ok: all acks recieved.\n");
		#endif
		return NULL;
	}
}



int SteadyPacketService::acknowledge(XmlNode *message)
{
	if(r66 == NULL)
	{
		return STEADYPACKETSERVICE_ERROR;
	}

	XmlNode *spd = message->getChildNodeByName("SteadyPacketData");
	if(spd == NULL)
	{
		return STEADYPACKETSERVICE_NO_STEADY_DATA;
	}

	const char *sender_id = spd->getAttributeValueByName("sender_id");
	long packet_nb = strtol(spd->getAttributeValueByName("packet_nb"), NULL, 16);
	const char *recipients = spd->getAttributeValueByName("recipients");

	if(strstr(recipients, this->uid) == NULL)
	{
		return STEADYPACKETSERVICE_NOT_A_RECIPIENT;
	}

	switch(pcv.update(sender_id, packet_nb))
	{
		case PACKETCOUNTERVECTOR_ACK:
			char ack[100];
			sprintf(ack, "<SteadyPacketAck to=\"%s\" from=\"%s\" packet_nb=\"%x\"/>",
							sender_id,
							this->uid,
							packet_nb);
			r66->sendMessage(ack);
			return STEADYPACKETSERVICE_ACK;

		case PACKETCOUNTERVECTOR_DBL:
			return STEADYPACKETSERVICE_DBL;
	}

	return STEADYPACKETSERVICE_ERROR;
}



void SteadyPacketService::onAck(unsigned int argc, char **argv)
{
	XmlNode *ack = xmlParser.parse(argv[0]);

	if(ack == NULL)
	{
		return;
	}

	char *c;
	acks.beginSequentialReading();
	while((c = acks.readItemPointer()) != NULL)
	{
		if(strcmp(c, ack->getAttributeValueByName("from")) == 0)
		{
			if(packetCount == strtol(ack->getAttributeValueByName("packet_nb"), NULL, 16))
			{
				acks.eraseItemAtReadingCursor();
				if(acks.size() == 0)
				{
					ack_waiter->notify();
				}
				return;
			}
		}
	}
}
