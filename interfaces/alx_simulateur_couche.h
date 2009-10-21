#ifndef __ALX_SIMULATEUR_COUCHE_H__
#define __ALX_SIMULATEUR_COUCHE_H__

//#include <list.h>
#include "../physique/alx_classes_bases.h"
#include "../physique/alx_model_simulation_physique1.h"
#include "alx_evennement_pointeur.h"
#include "alx_simulateur_pointeurs.h"
#include "../utilitaires/alx_arbre.h"
#include "alx_noeud_scene.h"


class alx_simulateur_couche : public alx_classe_base_liant
{
 private:
   alx_simulateur_pointeurs         *sim_pointeur;
   alx_evennement_pointeur *evt;
   alx_point2D pt_tmp;

   void Delete_L_rappels(info_du_contenant *info);
   void Traiter_evt(alx_evennement_pointeur *evt_ptr, info_du_contenant *info);
   void Elaguer_noeuds_descendants_de(alx_liste<alx_repere2D*> &L_noeuds, alx_noeud_scene *noeud_racine);

 public :
   //alx_noeud_scene *A_racine;

  // Les constructeurs
   alx_simulateur_couche(alx_simulateur_pointeurs *sp);

  // Les méthodes
   bool Zone_pointee(alx_noeud_scene *ns, void *info_sup);
   void simuler();
   void rationnaliser(int num);
   alx_evennement_pointeur* Evt_courant() {return evt;}

   void Traiter_infos(info_du_contenant *info, alx_evennement_pointeur *evt_ptr = (alx_evennement_pointeur*) NULL);
};

#endif
