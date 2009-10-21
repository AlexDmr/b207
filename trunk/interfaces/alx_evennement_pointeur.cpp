#include "alx_evennement_pointeur.h"
#include "alx_simulateur_pointeurs.h"

alx_evennement_pointeur::alx_evennement_pointeur( alx_evennement_pointeur *evt )
{maj(*evt);}

alx_evennement_pointeur::alx_evennement_pointeur( const int id
                                                , const alx_pointeur_type_evennement t_evt
                                                , const info_comp_evennement        &comp
                                                , alx_noeud_scene *r)
{maj(id, t_evt, comp, r);}

void alx_evennement_pointeur::maj( const int id
                                 , const alx_pointeur_type_evennement t_evt
                                 , const info_comp_evennement        &comp
                                 , alx_noeud_scene *r )
{identifiant = id;
 type_evt    = t_evt;
 info_comp   = comp;
 racine      = r;}

alx_noeud_scene* alx_evennement_pointeur::Noeud_sous_pointeur()                   {return ptr->Noeud_sous_pointeur();}
void             alx_evennement_pointeur::Noeud_sous_pointeur(alx_noeud_scene *n) {ptr->Noeud_sous_pointeur(n);}
