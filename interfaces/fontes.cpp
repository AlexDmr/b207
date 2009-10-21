#include "fontes.h"

gl_fonte *fonteArial = (gl_fonte*)NULL;

void Initialiser_fontes()
{
 if(!fonteArial)
   fonteArial = new gl_fonte("Arial", 0);
}

gl_fonte* fonte_Arial() {return fonteArial;}

