#ifndef __Class_Route66_
#define __Class_Route66_

#include <Slimlib/Core.h>
#include <Slimlib/Xml.h>
#include <stdio.h>


/** The predifined default timeout for sendMessageAndWaitForAnAnswer() calls
 */
#define R66_WAIT_AN_ANSWER_TIMEOUT          5000 /* 5 sec */

/** The predifined default Route66 road ip address
 */
#define Route66_DEFAULT_IP "224.0.0.66"

/** The predifined default Route66 road port
 */
#define Route66_DEFAULT_PORT 6666


/** A class to encapsulate a Route66 compliant callback.
 * This <b>abstract</b> class must be used via the template class
 * Route66MessageCallbackOf.
 */
class Route66MessageCallback
{
	private:
		/** The callback id, needed to remove this callback from the vector
		 */
		int id;

		/** The regular expression that message have to match to trigger this callback
		 */
		Regexp *regexp;

	public:
		/** Perform the call of callback method. Implemented in the callback template class.
		 * @param an Array of objects of class String
		 */
		virtual void OnMessage (Array<String *> *argv) = 0;

		/** Destruct an object of class Route66MessageCallback and it's regexp member
		 */
		virtual ~Route66MessageCallback();

		/** Set the callback id
		 * @param id the callback id
		 */
		void setId(int id);

		/** Set the regular expression that message have to match to trigger this callback
		 * @param regexp a pointer onto an object of class Regexp
		 */
		void setRegexp(Regexp *regexp);

		/** Retrieve the callback id
		 * @return return the callback id
		 */
		int getId();

		/** Retrieve the callback's regular expression
		 */
		Regexp *getRegexp();
};


/** A template class to use Route66MessageCallback.
 */
template <class T> class Route66MessageCallbackOf : public Route66MessageCallback
{
	protected:
		/** The object that host the method to use as a callback
		 */
		T* Object;

		/** Describe arguments that a callback MUST have to be compliant.
		 * Callbacks have to declare one argument: a pointer onto an object of class
		 * Array<String *>
		 */
		typedef	void ( T::*Callback_function )(Array<String *> *argv);

		/** The callback function
		 */
		Callback_function MessageCb;

	public:
		/** Construct a Route66MessageCallbackOf object, that make the method <i>m_cb</i> of
		 * an object <i>o</i> of class T
		 * @param o a pointer onto an object of class T
		 * @param m_cb a pointer onto the adress of <i>o</i>'s method to use as a callback.
		 */
		Route66MessageCallbackOf ( T* o, Callback_function m_cb )
			: Object (o), MessageCb( m_cb )
		{
		}

		/** Destruct a Route66MessageCallbackOf object. This method do NOT destroy the object
		 * that host the callback.
		 */
		~Route66MessageCallbackOf ()
		{
		}

		/** Perform the call of the callback method.
		 * @see Route66MessageCallback::OnMessage()
		 */
		void OnMessage (Array<String *> *argv)
		{
			(Object->*MessageCb) (argv);
		}
};


/** Route66 is a class that deals with Route66 software bus.
*/
class Route66 : public Thread
{
	private:
		/** Next callback id to use. Incremented after each use.
		 */
		int callbackIds;

		/** Multicast socket to connect the road
		 */
		MulticastSocket *mcast_socket;

		/** Reception buffer
		 */
		Buffer buffer;

		/** Vector of binded callbacks
		 */
		Vector<Route66MessageCallback *> m_callbacks;

		/** Contains the last error code
		 */
		int lastError;

		/** A mutex to make getLastError() thread safe
		 */
		Mutex mutex_getLastError;

		/** A mutex to make bindMessageCallback() thread safe
		 */
		Mutex mutex_bindMessageCallback;

		/** A mutex to make unbindMessageCallback() thread safe
		 */
		Mutex mutex_unbindMessageCallback;

		/** A mutex to make sendMessage() thread safe
		 */
		Mutex mutex_sendMessage;

		/** A mutex to make sendMessageAndWaitAnAnswer() thread safe
		 */
		Mutex mutex_sendMessageAndWaitAnAnswer;

		/** A waiter to make sendMessageAndWaitForAnAnswer() waiting for an answer
		 */
		Waiter *waiter_answer;

		/** Contains an answer for the waiting sendMessageAndWaitForAnAnswer() call
		 */
		Array<String *> *answer;

	protected:
		/** Perform common initialization tasks
		 */
		void init(void);

		/** Implement the Route66 main loop. NEVER CALL THAT METHOD MANUALLY. Use
		 * Route66::resume().
		 */
		void run(void);

	public:
		/** Create a new Route66 object
		 * @param ip is the ip address of the multicast group to use. Ex: "224.0.0.80" is a
		 * valid value for ip.
		 * If <i>ip</i> is NULL and/or <i>port</i> is 0, Route66 will use predifined standard
		 * values.
		 * @param port is the port og the group. Ex: 1500 is a valid value for port.
		 */
		Route66(const char *ip = NULL, const int port = 0);

		/** Destruct a Route66 object
		 */
		virtual ~Route66(void);

		/** Gracefully stop a route66 object
		 * @todo find a better way to deal with the blocking waitingForData() problem
		 */
		virtual void stop();

		/** Retrieve the last error code
		 */
		int getLastError(void) const;

		/** Bind a method of an object to a regular expression. When a message matching this
		 * regular expression will run in the bus, the binded method will be call
		 *<P>
		 * To be a valid callback, a method have to accept as parameter: a pointer onto an
		 * object of class Array<String *><br><br>
		 * Ex:
		 * <br>
		 *	- Receiver *rec = new Receiver();<br>
		 *		bindMessageCallback("Ping",
		 *			new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onPing));<br>
		 *	=> If "Ping" pass on the bus, rec->onPing(Array<String *> *argv) will be call
		 *		with argv == NULL.<br>
		 *<br>
		 *	- Receiver *rec = new Receiver();<br>
		 *		bindMessageCallback("(Ping)",
		 *			new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onPing));<br>
		 *	=> If "Ping" pass on the bus, rec->onPing(Array<String *> *argv) will be call
		 *		with argv->get(0)->get() == "Ping".<br>
		 *<br>
		 *	- Receiver *rec = new Receiver();<br>
		 *		bindMessageCallback("Ping=([0-9]*).*Pong=([A-Z]*)",
		 *			new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onPing));<br>
		 *	=> If "Ping=45 and Pong=QWERTY" pass on the bus,
		 *		rec->onPing(Array<String *> *argv) will be call with argv->get(0)->get() == "45"
		 *		and argv->get(1)->get() == "QWERTY".<br>
		 * @param regexp_str the regular expression to match.
		 * @param callback an instance of  Route66MessageCallback containing the address of
		 *	the object and the method to call.
		 * @return the callback id.
		 */
		int bindMessageCallback(const char *regexp_str, Route66MessageCallback *callback);

		/** Unbind a method. Method will not be called any more.
		 */
		void unbindMessageCallback(int callbackId);

		/** Send a message over the bus
		* <p>
		* Ex: sendMessage("This is a valid message for Route66");
		* @param message is the string to send.
		* @return 0 (NO_ERROR) if succed, an error code else.
		*/
		int sendMessage(const char *message);

		/** Send a message over the bus and block until an answer that match a specified
		 *regular expression is get.
		 * <p>
		 * Ex:<br>
		 *	Array<String *> *answer = sendMessageAndWaitForAnAnswer("Ping", ".* Pong.*") will
		 *	send "Ping" over route66 and wait for a message that contains the string "
		 *	Pong".<br>
		 *	If a message like "This is a Pong." is sent over route66,
		 *	sendMessageAndWaitAnAnswer will unblock, store "This is a Pong." in answer and
		 *	return.<br>
		 * @param message is the string to send.
		 * @param answer_regexp is the regular expression the answer have to match.
		 * @return a pointer onto an object of class Array<String *>, or NULL if the timeout
		 *	is reached.
		 */
		Array<String *> *sendMessageAndWaitForAnAnswer(const char *message,
			const char *answer_regexp);

		/** For internal use of Route66 only: related to sendMessageAndWaitAnAnswer()
		 */
		void onAnswer(Array<String *> *argv);
};

#endif
