#ifndef __ALX_SIMULATEUR_CLAVIER_H__
#define __ALX_SIMULATEUR_CLAVIER_H__

#include "../physique/alx_classes_bases.h"
#include "alx_simulateur_dispositif_saisie.h"
//#include <list.h>
#include <SDL.h>
//#include "../../physique/alx_geometrie2D.h"  // point, vecteur, droite, segment...

class alx_ds_rapporteur
{
 public:
   int                              id;
   alx_simulateur_dispositif_saisie *sim_ds;

   alx_ds_rapporteur &operator =(const alx_ds_rapporteur &r)
    {
     id     = r.id;
     sim_ds = r.sim_ds;
     return *this;
    }
};

class alx_simulateur_clavier : public alx_classe_base_liant
{
 private:
   int NB_CAR;
   alx_liste<alx_ds_rapporteur> L_rapporteur;
   Uint8 *etat_clavier, *etat_clavier_prcdt;

   void init();
   void distribuer_evennement(alx_evennement_dispositif_saisie &evt);

 public :
   SDL_Event  event;

   alx_simulateur_clavier();

   int  Ajouter_rapporteur(alx_simulateur_dispositif_saisie *r);
   bool Retirer_rapporteur(alx_simulateur_dispositif_saisie *r);

   void simuler();
};

#endif
