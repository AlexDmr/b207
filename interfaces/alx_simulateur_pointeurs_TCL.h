#ifndef __ALX_SIMULATEUR_POINTEURS_TCL_H__
#define __ALX_SIMULATEUR_POINTEURS_TCL_H__

#include "alx_simulateur_pointeurs_generique.h"
#include "../physique/alx_methode_rappel.h"
//#include <SDL.h>

class alx_simulateur_pointeurs_TCL : public alx_simulateur_pointeurs_generique
{private:
  Liste_de_rappel L_rap_simulation;

 public :
   alx_simulateur_pointeurs_TCL( int *e_x, int *e_y
                               , alx_point2D *bg
                               , alx_vecteur2D *vh, alx_vecteur2D *vv );
   void simuler();
   void abonner_a_simulation    (alx_methode_rappel *m);
   const bool desabonner_de_simulation(alx_methode_rappel *m);
};

#endif

