#include <cstdlib>
#include <iostream>

using namespace std;

#include <Route66/Route66.h>
#include <Slimlib/Errors.h>
#include <Slimlib/Core.h>
#include <Slimlib/Xml.h>

#define ROUTE66_IP "ip"
#define ROUTE66_PORT "port"
#define DUMPER66_CONFIGURATIONFILENAME "dumperconf.xml"

class Dumper66
{
	private:
		Route66 *route66;
		char *ip;
		int port;
		
		
		
		//____________________________________________________________________________________
		void readConfigurationFile(const char *filename = NULL)
		{
			XmlConfigurationFile *conf = new XmlConfigurationFile();

			// Declare expected configuration data node
			XmlNode *ip = NULL;
			XmlNode *port = NULL;

			// if helloconf.xml is succefully read
			if((filename && 
				 (conf->readFile(filename) == NO_ERROR)) ||
				 (conf->readFile(DUMPER66_CONFIGURATIONFILENAME) == NO_ERROR))
			{
				// Retrieve node from file
				ip = conf->getConfigurationData(ROUTE66_IP);
				port = conf->getConfigurationData(ROUTE66_PORT);
			}

			// Set user-defines values or default values
			if(ip != NULL) this->ip = strdup(ip->getSimpleContent());
			if(port != NULL) this->port = strtol(port->getSimpleContent(), NULL, 10);
		}
		
		
		
		//____________________________________________________________________________________
		void run()
		{
			cout<<"Dumper66 is listening "<<((ip != NULL)?ip:Route66_DEFAULT_IP)<<":"
				<<((port !=	0)?port:Route66_DEFAULT_PORT)<< endl;
			route66 = new Route66(ip, port);
			route66->bindMessageCallback(
				"(.*)", new Route66MessageCallbackOf<Dumper66>(this, &Dumper66::onAnyMessage));
			route66->resume();
		}
		
	public:
		//____________________________________________________________________________________
		Dumper66()
		{
			ip = NULL;
			port = 0;
			readConfigurationFile();
			run();
		}
		
		

		//____________________________________________________________________________________
		Dumper66(const char *filename)
		{
			ip = NULL;
			port = 0;
			readConfigurationFile(filename);
			run();
		}
		
		//____________________________________________________________________________________
		void onAnyMessage(Array<String *> *argv)
		{
			cout<<argv->get(0)->get()<<"\n";
		}
};



//________________________________________________________________________________________
int main(int argc, char *argv[])
{
	Dumper66 *d66;
	if(argc == 0)
	{
		d66 = new Dumper66();		
	}
	else
	{
		cout << argv[1] << endl;
		d66 = new Dumper66(argv[1]);
	}
	
	pause();
	delete d66;
	return EXIT_SUCCESS;
}
