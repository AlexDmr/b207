#include <stdio.h>
#include <stdlib.h>
#include <route66.h>
#include <overseacallbackservice.h>


int main(int argc, char *argv[])
{
    printf("# Test OverseaCallbackService\n\n");
   
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
    
    OverseaCallbackService *ocs = new OverseaCallbackService(r66);

	r66->sendMessage("<ping/>");

    #if defined (unix) || defined (__APPLE__)
    pause();
    #endif

    #ifdef WIN32
    system("pause");
    #endif
    return 0;
}
