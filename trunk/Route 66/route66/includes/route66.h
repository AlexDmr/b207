#ifndef _ROUTE66_
#define _ROUTE66_

#include <slimcore.h>
#include <slimxml.h>
#include <stdio.h>

#define R66_MAX_REGEXP_LEN                  300
#define R66_WAIT_AN_ANSWER_TIMEOUT          5000 /* 5 sec */

#define R66_MULTICASTGROUP                  "multicast_group"
#define R66_PORT                            "port"

#define R66_NOERROR                          0
#define R66_OUT_OF_MEMORY                   11
#define R66_ROAD_BAD_REGEXP                 12
#define R66_ROAD_DEFINITON_FILE_NOT_FOUND   13
#define R66_ROAD_BAD_DEFINITON              14
#define R66_ROAD_NO_DEFINITON               15



class Route66MessageCallback
{
    public:
	virtual void OnMessage (unsigned int argc, char **argv) = 0;
};



template <class T> class Route66MessageCallbackOf : public Route66MessageCallback
{
    protected:
	T*      Object;
	typedef	void ( T::*Callback_function )(unsigned int argc, char **argv);
	Callback_function MessageCb;

    public:
	Route66MessageCallbackOf ( T* o, Callback_function m_cb ) : Object (o), MessageCb( m_cb )
	{
	}

	~Route66MessageCallbackOf ()
	{
	}

	void OnMessage (unsigned int argc, char **argv)
	{
		(Object->*MessageCb) (argc, argv);
	}
};



// For internal use of Route66 class only
typedef struct Message_callback_struct
{
    char regexp_str[R66_MAX_REGEXP_LEN];
    Regexp *regexp;
    Route66MessageCallback *callback;
} Message_callback_struct;



/**
* Route66 is a class that deals with Route66 software bus.
* @author Lionel Balme
* @version 2.0
*/

class Route66 : public SlimThread
{
    private:
        char **tab_argv;
        const unsigned int taille_tempon_msg
                         , nb_max_msgs;
        char *tempon_msg;
        unsigned int *indexs_msg, nb_msg, derniere_position;
        bool message_a_traiter;

	SlimMulticastSocket *mcast_socket;
	char buffer[SLIMSOCKET_DEFAULT_BUFFER_SIZE], ip_txt[16], port_txt[16];

	SlimVector<Message_callback_struct> m_callbacks;

	SlimXmlParser xmlParser;

	int lastError; // error code eventually raised while socket init

	Mutex mutex_getLastError;
	Mutex mutex_bindMessageCallback;
	Mutex mutex_sendMessage;
	Mutex mutex_sendMessageAndWaitAnAnswer;
        Mutex mutex_message_a_traiter;

	// send Message and waiting for answer mechanism
	Waiter *waiter_answer;
	char *answer;
	Regexp *answer_regexp;

    protected:
    void init(void);
	void run(void);
	int getRoad(const char *roadFilename);

    public:
	/**
	* Create a new Route66 object
	* @param roadFilename is a filename of a road description file. A road description file is an XML document  containing a root tag like this following one: <road multicast_group="224.0.0.80" port="1500"/>.
	*/
	Route66(const char *roadFilename);

	/**
	* Create a new Route66 object
	* @param ip is the ip address of the multicast group to use. Ex: "224.0.0.80" is a valid value for ip.
	* @param port is the port og the group. Ex: 1500 is a valid value for port.
	*/
	Route66(const char *ip, const int port);

	virtual ~Route66() { };

	int getLastError(void) const;
        const char* PORT() const {return port_txt;}
        const char* IP()   const {return ip_txt;}

	/**
	* Bind a method of an object to a regular expression. When a message matching this regular expression will run in the bus, the binded method will be call
	*<P>
	* To be a valid callback, a method have to accept two parameters: unsigned int argc and char **argv. argc will store the number of arguments and argv is an array that will contains all substrings defined as argument in the regular expression.<br><br>
	* Ex:
	* <br>
	*   - Receiver *rec = new Receiver();<br>
	*   bindMessageCallback("Ping", new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onPing));<br>
	*   =>If "Ping" pass on the bus, rec->onPing(unsigned int argc, char **argv) will be call with argc == 0 and argv == null.<br>
	*<br>
	*   - Receiver *rec = new Receiver();<br>
	*   bindMessageCallback("(Ping)", new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onPing));<br>
	*   =>If "Ping" pass on the bus, rec->onPing(unsigned int argc, char **argv) will be call with argv[0] == "Ping".<br>
	*<br>
	*   - Receiver *rec = new Receiver();<br>
	*   bindMessageCallback("Ping=([0-9]*).*Pong=([A-Z]*)", new Route66MessageCallbackOf<Receiver>(rec, &Receiver::onPing));<br>
	*   =>If "Ping=45 and Pong=QWERTY" pass on the bus, rec->onPing(unsigned int argc, char **argv) will be call with argv[0] == "45" and argv[1] == "QWERTY".<br>
	* @param regexp_str the regular expression to match.
	* @param callback an instance of  Route66MessageCallback containing the address of the object and the method to call.
	*/
	void bindMessageCallback(const char *regexp_str, Route66MessageCallback *callback);
        void Manage_callbacks();
	/**
	* Send a message over the bus
	* <p>
	* Ex: sendMessage("This is a valid message for Route66");
	* @param message is the string to send.
	* @return 0 (NO_ERROR) if succed, an error code else.
	*/
	int sendMessage(const char *message);

	/**
	* Send a message over the bus and block until an answer that match a specified regular expression is get.
	* <p>
	* Ex:<br>
	*   char *answer;
	*   sendMessageAndWaitAnAnswer("Ping", ".* Pong.*", &answer) will send "Ping" over route66 and wait for a message that contains the string " Pong".<br>
	*   If a message like "This is a Pong." is sent over route66, sendMessageAndWaitAnAnswer will unblock, store "This is a Pong." in answer and return.<br>
	* @param message is the string to send.
	* @param answer_regexp is the regular expression the answer have to match.
	* @param answer is the answer expected.
	*/
	void sendMessageAndWaitAnAnswer(const char *message, const char *answer_regexp, char **answer);
};

#endif
