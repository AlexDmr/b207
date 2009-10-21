#include "alx_telec_cpp.h"

void alx_telec_cpp::init()
{Numero_transparent(1);}

alx_telec_cpp::alx_telec_cpp( char *nom
                            //, cogitant::Environment *e
                            , alx_interpreteur_tcl  *interp )
{init();
 this->nom = nom;}

const unsigned int alx_telec_cpp::Numero_transparent() const {return num_trans;}
void alx_telec_cpp::Numero_transparent(const unsigned int num)
  {int n = num_trans; num_trans = num; Changement_numero(n, num_trans);}

