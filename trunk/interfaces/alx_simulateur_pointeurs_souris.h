#ifndef __ALX_SIMULATEUR_POINTEURS_SOURIS_H__
#define __ALX_SIMULATEUR_POINTEURS_SOURIS_H__

#include "alx_simulateur_pointeurs_generique.h"
#include <SDL.h>

class alx_simulateur_pointeurs_souris : public alx_simulateur_pointeurs_generique
{private:
   alx_evennement_pointeur A_ep_tmp;
   info_comp_evennement    A_ic_tmp;
   alx_point2D pt_dep, pt_arrive, pt_clique, derniere_pos, pt_tmp;

 public :
   alx_simulateur_pointeurs_souris( int *e_x, int *e_y
                                  , alx_point2D *bg
                                  , alx_vecteur2D *vh, alx_vecteur2D *vv);

   void simuler();
};

#endif
 