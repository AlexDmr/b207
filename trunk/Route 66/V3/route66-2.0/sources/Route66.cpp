#include <Slimlib/Errors.h>
#include <Route66/Route66.h>
#include <string.h>



//________________________________________________________________________________________
void Route66MessageCallback::setId(int id)
{
	this->id = id;
}



//________________________________________________________________________________________
void Route66MessageCallback::setRegexp(Regexp *regexp)
{
	this->regexp = regexp;
}



//________________________________________________________________________________________
int Route66MessageCallback::getId()
{
	return id;
}



//________________________________________________________________________________________
Regexp *Route66MessageCallback::getRegexp()
{
	return regexp;
}



//________________________________________________________________________________________
Route66MessageCallback::~Route66MessageCallback()
{
	delete regexp;
}



//________________________________________________________________________________________
void Route66::init(void)
{
	callbackIds = 0;
	lastError = mcast_socket->open();
	if(lastError != NO_ERROR) return;
	waiter_answer = new Waiter();
	buffer.allocateBuffer(mcast_socket->getBufferSize());
}



//________________________________________________________________________________________
Route66::~Route66(void)
{
	mcast_socket->closeSocket();
	delete mcast_socket;
	delete waiter_answer;
}




//________________________________________________________________________________________
Route66::Route66(const char *ip, const int port) : Thread(THREAD_SUSPENDED)
{
	lastError = NO_ERROR;
	mcast_socket = NULL;
	waiter_answer = NULL;
	answer = NULL;

	int _port = port;
	if(_port == 0)
	{
		_port = Route66_DEFAULT_PORT;
	}

	if(ip != NULL)
	{
		mcast_socket = new MulticastSocket(ip, _port);
		mcast_socket->setRecipientAddress(ip, _port);
	}
	else
	{
		mcast_socket = new MulticastSocket(Route66_DEFAULT_IP, _port);
		mcast_socket->setRecipientAddress(Route66_DEFAULT_IP, _port);
	}

	init();
}




//________________________________________________________________________________________
void Route66::run(void)
{
	while(terminated == THREAD_RUNNING)
	{
		lastError = mcast_socket->waitingForData(buffer.getBuffer());
		if (lastError == RECEPTION_FAILED)
		{
			return;
		}
		else
		{
			Route66MessageCallback *cb = m_callbacks.getFirstItem();
			while(cb != NULL)
			{
				if(cb->getRegexp()->exec(buffer.getBuffer()))
				{
					cb->OnMessage(cb->getRegexp()->getSubArray());
				}

				cb = m_callbacks.getNextItem();
			}
		}
	}

	terminated = THREAD_TERMINATED;
}



//________________________________________________________________________________________
void Route66::stop()
{
	if(terminated == THREAD_RUNNING)
	{
		terminated = THREAD_STOPPING;
		while(terminated != THREAD_TERMINATED) 
		{
			sendMessage("R66EOT");
			Thread::sleep(10);
		}
	}
}



//________________________________________________________________________________________
int Route66::getLastError(void) const
{
	return lastError;
}



//________________________________________________________________________________________
int Route66::bindMessageCallback(const char *regexp_str, Route66MessageCallback *callback)
{
	mutex_bindMessageCallback.lock();
	Regexp *regexp = new Regexp(regexp_str);
	if(regexp != NULL)
	{
		callbackIds++;
		callback->setId(callbackIds);
		callback->setRegexp(regexp);

		m_callbacks.push(callback);
		if(m_callbacks.getLastError() != NO_ERROR)
		{
			return 0;
		}

		int id = callback->getId();
		mutex_bindMessageCallback.unlock();
		return id;
	}

	mutex_bindMessageCallback.unlock();
	return 0;
}




//________________________________________________________________________________________
void Route66::unbindMessageCallback(int id)
{
	mutex_unbindMessageCallback.lock();
	Route66MessageCallback *callback = m_callbacks.getFirstItem();

	while(callback != NULL)
	{
		if(callback->getId() == id)
		{
			m_callbacks.remove();
			delete callback;
			return;
		}

		callback = m_callbacks.getNextItem();
	}

	mutex_unbindMessageCallback.unlock();
}




//________________________________________________________________________________________
int Route66::sendMessage(const char *message)
{
	mutex_sendMessage.lock();
	lastError = mcast_socket->sendData((char *)message, strlen (message) + 1);
	mutex_sendMessage.unlock();
	return lastError;
}



//________________________________________________________________________________________
Array<String *> *Route66::sendMessageAndWaitForAnAnswer(const char *message, const char *answer_regexp)
{
	int rc;
	Array<String *> *answer;

	mutex_sendMessageAndWaitAnAnswer.lock();

	int callbackId = bindMessageCallback(answer_regexp,
		new Route66MessageCallbackOf<Route66>(this, &Route66::onAnswer));
	this->answer = NULL;

	sendMessage(message);

	rc = waiter_answer->timedwait(R66_WAIT_AN_ANSWER_TIMEOUT);

	unbindMessageCallback(callbackId);

	answer = this->answer;
	mutex_sendMessageAndWaitAnAnswer.unlock();

	return answer;
}



//________________________________________________________________________________________
void Route66::onAnswer(Array<String *> *argv)
{
	answer = argv;
	waiter_answer->notify();
}
