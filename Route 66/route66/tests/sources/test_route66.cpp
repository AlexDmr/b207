#include <slimcore.h>
#include <route66.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



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
                   printf("%s\n", argv[i]);
                }
                
        }
};



int main(int argc, char *argv[])
{
    printf("# Test Route66\n");
    //Route66 *r66 = new Route66("data/road.xml");
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
    r66->sendMessage("- Route66 Test Message -");
    if(r66->getLastError() != R66_NOERROR)
    {
        printf("# Route66 error #%d while sending a message.\n", r66->getLastError());
        return 0;
    }
    
    printf("uid: %s\n", generate_uid(NULL)); 

	
    #if defined (unix) || defined (__APPLE__)
    pause();
    #endif

    #ifdef WIN32
    system("pause");
    #endif
    return 0;
}
