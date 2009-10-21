#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "route66tcpgateway.h"
#include "route66.h"



int main(int argc, char *argv[])
{
	printf("# Test Route66TcpGateway\n\n");
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

	printf("# Gateway initialisation... ");
	Route66TcpGateway *gateway = new Route66TcpGateway(1600, r66);
	if(gateway->getLastError() != R66_NOERROR)
    {
        printf("Error #%d.\n", gateway->getLastError());
        return 0;
    }
    else
    {
        printf("Ok.\n");
    }

	printf("Gateway started.\n");

	#if defined (unix) || defined (__APPLE__)
	pause();
	#endif

	#ifdef WIN32
	system("pause");
	#endif
	return 0;
}
