#ifndef __ALX_LIANT_POINTEURS_SIM_CONTACT_H__
#define __ALX_LIANT_POINTEURS_SIM_CONTACT_H__

#include "..\utilitaires\alx_liste.h"

#include "../physique/alx_classes_bases.h"
#include "../physique/alx_model_simulation_physique1.h"
#include "alx_evennement_pointeur.h"
#include "alx_simulateur_pointeurs.h"
#include "../physique/alx_simulateur_points_contact.h"

class alx_liant_ptr_contact : alx_classe_base_liant
{
 private:
   alx_simulateur_pointeurs      *sim_pointeurs;
   alx_simulateur_points_contact *sim_contact;
   alx_liste<alx_ensemble_contact*> L_ens_contact_modif;

 public :
  // Les constructeurs
   alx_liant_ptr_contact( alx_simulateur_pointeurs      *sp
                        , alx_simulateur_points_contact *sc);

  // Les méthodes
   //void simuler();
   void rationnaliser(int num);
};

#endif
