#ifndef __ALX_QUEMANDEUR_DE_POINTEUR_H__
#define __ALX_QUEMANDEUR_DE_POINTEUR_H__

#include "../physique/alx_classes_bases.h"
#include "alx_evennement_pointeur.h"

class alx_quemendeur_de_pointeur : public alx_classe_base_liant
{
 public :
   inline void Appel_a_info_sur_pt        (alx_point2D *pt, alx_pointeur_type_evennement type_info)
    {
    // -1 : On veut une info
    //    0 : sur un point
    //      X : le type d'info voulu. voir alx_pointeur_type_evennement
    //        * : adresse du point
    //          X : code � envoyer lors de l'appel � Propager (identifie la r�ponse)
     alx_classe_base_model::nb_infos_comp = 5;
     alx_classe_base_model::infos_comp[0] = -1; // Appel � info
     alx_classe_base_model::infos_comp[1] = 0; //sur un point
     alx_classe_base_model::infos_comp[2] = type_info; // le type s'info voulu
     alx_classe_base_model::infos_comp[3] = (int)(void*)pt;
     alx_classe_base_model::infos_comp[4] = -1; // Le code pour l'appel � la rationnalisation.
     alx_classe_base_model::Oyez_oyez();
    }

   inline void Arreter_appel_a_info_sur_pt(alx_point2D *pt, alx_pointeur_type_evennement type_info)
    {
     alx_classe_base_model::nb_infos_comp = 5;
     alx_classe_base_model::infos_comp[0] = -2; // Appel � arret de l'info
     alx_classe_base_model::infos_comp[1] = 0; //sur un point
     alx_classe_base_model::infos_comp[2] = type_info; // le type s'info qu'on ne veut plus
     alx_classe_base_model::infos_comp[3] = (int)(void*)pt;
     alx_classe_base_model::infos_comp[4] = -1; // Le code pour l'appel � la rationnalisation.
     alx_classe_base_model::Oyez_oyez();
    } 
};

#endif
 