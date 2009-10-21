#include "alx_simulateur_pointeurs_generique.h"

void alx_simulateur_pointeurs_generique::init()
{cour = 0;
// noeud_pere = NULL;
}

/*******************************************************************************/
void alx_simulateur_pointeurs_generique::distribuer_evennement(alx_evennement_pointeur &evt)
{alx_element_liste<alx_pointeur_rapporteur> *it
                                          , *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin); it=it->svt)
  {
   evt.Identifiant( (it->E()).id + evt.Identifiant() );
   (it->E()).sim_ptr->L_evt.Ajouter_a_la_fin( evt );
  }
}

/*******************************************************************************/
alx_simulateur_pointeurs_generique::alx_simulateur_pointeurs_generique( int *e_x, int *e_y
                                                                      , alx_point2D *bg
                                                                      , alx_vecteur2D *vh
                                                                      , alx_vecteur2D *vv)
{init();
 ecran_x = e_x;
 ecran_y = e_y;
 coin_bg = bg;
 vect_h  = vh;
 vect_v  = vv;}

/*******************************************************************************/
int alx_simulateur_pointeurs_generique::Ajouter_rapporteur(alx_simulateur_pointeurs *r, alx_noeud_scene *pere, unsigned int nb)
{alx_element_liste<alx_pointeur_rapporteur> *it
                                          , *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin)&&((it->E()).sim_ptr!=r); it=it->svt) ;

 if(it!=it_fin) return false; // r est déja rapporteur

 alx_pointeur_rapporteur rapporteur;
  rapporteur.sim_ptr = r;
  rapporteur.id = r->Ajouter_pointeur("", pere, nb);

 L_rapporteur.Ajouter_a_la_fin( rapporteur );
 return rapporteur.id;
}

/*******************************************************************************/
bool alx_simulateur_pointeurs_generique::Retirer_rapporteur(alx_simulateur_pointeurs *r)
{alx_element_liste<alx_pointeur_rapporteur> *it
                                          , *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin)&&((it->E()).sim_ptr!=r); it=it->svt) ;

 if(it==it_fin) return false; // r n'est pas un rapporteur
 L_rapporteur.Retirer(it);
 return true;
}


/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
void alx_simulateur_pointeurs_generique::convertir_coordonnees( const double x, const double y
                                                           , alx_point2D &res)
{res.maj(*coin_bg);

 A_v_tmp.maj(*vect_h); // dep.maj(*vect_h);
 /*dep*/A_v_tmp.Alonger( x/ *ecran_x );
 res.Translation(A_v_tmp);//dep);

 /*dep*/A_v_tmp.maj(*vect_v);
 /*dep*/A_v_tmp.Alonger( (*ecran_y - y - 1.0)/ *ecran_y );
 res.Translation(A_v_tmp);//dep);
}

/*******************************************************************************/
void alx_simulateur_pointeurs_generique::simuler()
{}


