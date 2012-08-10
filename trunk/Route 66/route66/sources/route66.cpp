#include <route66.h>

void Route66::init(void)
{lastError = mcast_socket->open();
 if(lastError != R66_NOERROR) return;
 waiter_answer = new Waiter();
// XXX
 const unsigned int nb_max_args    = 64
                 , taille_max_arg = 65536;
 tab_argv = new char *[nb_max_args];
 for(unsigned int i=0; i<nb_max_args; i++)
   tab_argv[i] = new char[taille_max_arg];

 // Les buffers de réception
 derniere_position = 0;
 nb_msg            = 0;
 indexs_msg = new unsigned int[nb_max_msgs];
 tempon_msg = new char[taille_tempon_msg];
 this->answer_regexp = NULL;
// XXX
 resume();}

Route66::Route66(const char *roadFilename) : SlimThread(true)
                                           , taille_tempon_msg(1048576)
                                           , nb_max_msgs(256)
{
    lastError = SLIMSOCKET_NOERROR;

    lastError = getRoad(roadFilename);
    if(lastError != R66_NOERROR) return;

   	init();
}



Route66::Route66(const char *ip, const int port) : SlimThread(true)
                                                 , taille_tempon_msg(1048576)
                                                 , nb_max_msgs(256)
{
 char tmp [256];

 lastError = SLIMSOCKET_NOERROR;

    mcast_socket = new SlimMulticastSocket(ip, port);
    mcast_socket->setRecipientAddress(ip, port);
    strcpy_s(ip_txt, ip);
    _itoa_s(port, tmp, 10); strcpy_s(port_txt, tmp);

    init();
}



int Route66::getRoad(const char *roadFilename)
{
        XmlNode *road;

        SlimFile *file = new SlimFile(roadFilename);
        long size;
        char *buffer;

        buffer = file->readContent(&size);

        if(file->getLastError() != SLIMFILE_NOERROR) return R66_ROAD_DEFINITON_FILE_NOT_FOUND;

        if((road = xmlParser.parse(buffer)) == NULL) return R66_ROAD_NO_DEFINITON;

	const char *ip   = road->getAttributeValueByName(R66_MULTICASTGROUP);
	const char *port = road->getAttributeValueByName(R66_PORT);

	strcpy_s(ip_txt, ip);
    strcpy_s(port_txt, port);

        if(ip   == NULL) return R66_ROAD_BAD_DEFINITON;
	if(port == NULL) return R66_ROAD_BAD_DEFINITON;

	mcast_socket = new SlimMulticastSocket(ip, atoi(port));
	mcast_socket->setRecipientAddress(ip, atoi(port));

	return R66_NOERROR;
}

//#define __TRACE_ON 1
void Route66::Manage_callbacks()
{unsigned int len, k, nb_argc;
	#ifdef __TRACE_ON
         printf("\nR66::Manage_callbacks()\n");
    #endif
 mutex_message_a_traiter.lock();
	#ifdef __TRACE_ON
         printf(" lock nb_msg=%d", nb_msg);
    #endif
 for(unsigned int index = 0; index<nb_msg; index++)
  {char *c_tmp = &(tempon_msg[indexs_msg[index]]);
   #ifdef __TRACE_ON 
		if(this->answer_regexp) {printf(" regexp");} else {printf(" no regexp");}
   #endif
   if((this->answer_regexp != NULL) && regexec(this->answer_regexp, c_tmp))
    {answer_regexp = NULL;
	 #ifdef __TRACE_ON
         printf(" got answer");
       #endif
     answer = new char[strlen(c_tmp)+1]; strcpy_s (answer, strlen(c_tmp)+1, c_tmp);
	 #ifdef __TRACE_ON
         printf("\nR66::waiter_answer->notify();\n");
       #endif
     waiter_answer->notify();
     // DEBUG 2008 mutex_message_a_traiter.unlock();
     continue;
    }

	#ifdef __TRACE_ON
         printf(" m_callbacks.beginSequentialReading");
    #endif
   Message_callback_struct *cb;
   m_callbacks.beginSequentialReading();
	#ifdef __TRACE_ON
         printf(" while");
    #endif
   while(((cb = m_callbacks.readItemPointer()) != NULL))
    {
     if(regexec(cb->regexp, c_tmp))
      {// regexec found a match
       // first, count the nb of arguments
       nb_argc = 0;
       while(nb_argc < NSUBEXP)
        {if(cb->regexp->startp[nb_argc] == NULL) break;
         nb_argc++;
        }

       // At this point, argc contains the real number of arguments plus one.
       #ifdef __TRACE_ON
         printf("Match found: argc =%d\n", nb_argc);
       #endif

       if(nb_argc == 1)
        {cb->callback->OnMessage(0, NULL);}
        else
         {
           // allocation of argv
           //printf("R66 : Preparing to callback...\n");
           //tab_argv = 0;
           //XXXtab_argv = (char**)malloc(nb_argc * sizeof(char*));
           //XXXmemset(tab_argv, 0, nb_argc * sizeof(char*));
           //now, allocation of each parameter string
           //printf("  Allocating and copying the arguments\n");
            for(k = 1; k < nb_argc; k++)
           {  len = ((unsigned int)(cb->regexp->endp[k]) - (unsigned int)(cb->regexp->startp[k]));
              //printf("    Allocating byte %d for argument %d\n", len, k);
              //XXX tab_argv[k-1] = (char *) malloc(len+1);
              memcpy(tab_argv[k-1], cb->regexp->startp[k], len);
              tab_argv[k-1][len] = 0;
           }

           // callback call
           #ifdef __TRACE_ON
             printf("Calling callback\n");
           #endif
           cb->callback->OnMessage(nb_argc-1, tab_argv);
           #ifdef __TRACE_ON
             printf("End of Calling callback\n");
           #endif
         }
      }
    }
  }
 message_a_traiter = false;
   nb_msg            = 0;
   derniere_position = 0;
 mutex_message_a_traiter.unlock();
}

//#define DEBUG_R66 0
void Route66::run(void)
{
 while(1) //while(!Terminated)
  {lastError = mcast_socket->waitingForData(buffer);
   #ifdef DEBUG_R66
     printf("Route66->Msg\n");
   #endif
   if (lastError == SLIMSOCKET_RECEPTION_FAILED)
    {break;}
    else {
          #ifdef DEBUG_R66
            printf("Route66->Lock\n");
          #endif
          mutex_message_a_traiter.lock();
            message_a_traiter = true;
            indexs_msg[nb_msg] = derniere_position;
            nb_msg++;
            unsigned int taille_str_buffer = strlen(buffer);
            memcpy(&(tempon_msg[derniere_position]), buffer, taille_str_buffer+1);
            derniere_position += taille_str_buffer+1;
          mutex_message_a_traiter.unlock();
          #ifdef DEBUG_R66
            printf("Route66->UnLock\n");
          #endif
         }
  }
  printf("\nEnd of the SOCKET R66\n");
  mcast_socket->closeSocket();
}



int Route66::getLastError(void) const
{
    return lastError;
}



void Route66::bindMessageCallback(const char *regexp_str, Route66MessageCallback *callback)
{
    Message_callback_struct cbs;

    mutex_bindMessageCallback.lock();
    memset(&cbs, 0, sizeof(cbs));
    memcpy(cbs.regexp_str, regexp_str, strlen(regexp_str));
    cbs.callback = callback;
    cbs.regexp = regcomp(cbs.regexp_str);

    #ifdef __TRACE_ON
    if(cbs.regexp == NULL) printf("regcomp failed.\n"); // else regdump(cbs.regexp);
    #endif

    m_callbacks.push(cbs);      // Maybe an error might be catch here
                                //(out of mem could occure while push)
    #ifdef __TRACE_ON
    printf ("a message callback was succefully binded to route66.\n");
    #endif
    mutex_bindMessageCallback.unlock();
}



int Route66::sendMessage(const char *message)
{
    //mutex_sendMessage.lock();
    lastError = mcast_socket->sendData(message, strlen (message) + 1);
    //mutex_sendMessage.unlock();
    return lastError;
}



void Route66::sendMessageAndWaitAnAnswer(const char *message, const char *answer_regexp, char **answer)
{
    int rc;

    mutex_sendMessageAndWaitAnAnswer.lock();

    #ifdef __TRACE_ON
    printf("answer regexp: !%s!\n", answer_regexp);
    #endif

    this->answer = NULL;
    this->answer_regexp = regcomp(answer_regexp);

    sendMessage(message);
    rc = waiter_answer->timedwait(R66_WAIT_AN_ANSWER_TIMEOUT);
    if(rc != 0)
    {
	   *answer = NULL;
    }
    else
    {
	   *answer = new char[strlen(this->answer)+1]; strcpy_s(*answer, strlen(this->answer)+1, this->answer);
    }

    mutex_sendMessageAndWaitAnAnswer.unlock();
}
