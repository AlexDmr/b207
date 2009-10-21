#include "affichage_pointeurs.h"


affichage_pointeurs::affichage_pointeurs(alx_simulateur_pointeurs *s)
 {sim_pointeurs = s;}

bool affichage_pointeurs::Associer_fonction_affichage_a_pointeur(f_affichage f, void *param, const int n)
{
 // On vérifie que le pointeur existe bien :mai
  if( sim_pointeurs->Pointeur(n)==NULL )
    return false;

 // Si oui, a-t-il déja une fonction d'affichage affectée?
  alx_element_liste<association_affichage_num_pointeur> *it     = L_fonctions_affichage.Premier()
                                                      , *it_fin = L_fonctions_affichage.Fin();
  while((it!=it_fin) && ((it->E()).num!=n))
    it = it->svt;

  if(it==it_fin)
    L_fonctions_affichage.Ajouter_au_debut( association_affichage_num_pointeur(f, param, n) );
   else (it->E()).f = f;
  return true;
 }

void affichage_pointeurs::Afficher()
 {alx_element_liste<association_affichage_num_pointeur> *it     = L_fonctions_affichage.Premier()
                                                      , *it_fin = L_fonctions_affichage.Fin();
  alx_point2D *pt;
  for(;it!=it_fin;it = it->svt)
   {pt = &( (sim_pointeurs->Pointeur( (it->E()).num ))->pt );
    (it->E()).f(pt->x, pt->y, (it->E()).param);
   }
 }
