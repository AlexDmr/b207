#ifndef ALX_simulateur_pointeur_souris_USB_raw
#define ALX_simulateur_pointeur_souris_USB_raw

#include "alx_simulateur_pointeurs_generique.h"
#include "ManyMouse/manymouse.h"

const unsigned int NB_MAX_MOUSE = 128;
const unsigned int T_ETAT_MOUSE = 4;
class alx_simulateur_pointeur_souris_USB_raw : public alx_simulateur_pointeurs_generique
{private:
   alx_evennement_pointeur A_ep_tmp;
   info_comp_evennement    A_ic_tmp;
   alx_point2D pt_dep, pt_arrive, pt_clique, derniere_pos, pt_tmp;
   unsigned int available_mice;
   bool need_scan;
   int tab_etats[NB_MAX_MOUSE][T_ETAT_MOUSE]; // x, y, bt, wheel

 public :
   alx_simulateur_pointeur_souris_USB_raw( int *e_x, int *e_y
                                         , alx_point2D *bg
                                         , alx_vecteur2D *vh, alx_vecteur2D *vv);

   void simuler();

   const bool Need_scan() const {return need_scan;}
   void Scan() {ManyMouse_Quit();
                available_mice = ManyMouse_Init();
                need_scan = false;
               }
   const unsigned int Nb_souris() {return available_mice;}
};

#endif
