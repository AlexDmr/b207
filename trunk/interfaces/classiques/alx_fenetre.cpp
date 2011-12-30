#include "alx_fenetre.h"

void alx_fenetre::init_fenetre()
{epaisseur_bordure = 10;
 repere = (alx_repere2D*)NULL;}

alx_fenetre::alx_fenetre()
{init_fenetre();
 Longueur_corp  (-1.0f);
 Longueur_entete(-1.0f);
 Hauteur_corp   (-1.0f);
 Hauteur_entete (-1.0f);
 nom[0] = 0;}

alx_fenetre::alx_fenetre( const char *nom
                        , const double longueur, const double hauteur ) : L_rappel_chgmt_dimension( Ptr_emission_rationnalisation() )
{init_fenetre();
 strcpy_s(this->nom, nom);
 Longueur_corp(longueur);
 Hauteur_corp(hauteur);
 Hauteur_entete(hauteur*0.1);
 Rayon_zone_rotation(longueur*0.05);}

alx_fenetre::alx_fenetre( const char *nom
                        , const double longueur, const double hauteur
                        , const double l_entete, const double h_entete
                        , const double rayon_zone_rotation ) : L_rappel_chgmt_dimension( Ptr_emission_rationnalisation() )
{init_fenetre();
 strcpy_s(this->nom, nom);
 Longueur_corp(longueur);
 Hauteur_corp(hauteur);
 Longueur_entete(l_entete);
 Hauteur_entete(h_entete);
 Rayon_zone_rotation(rayon_zone_rotation);}

//______________________________________________________________________________
// 0 : Changement de dimension
//   0 : En longueur
//     a : ancienne longueur
//     n : nouvelle longueur
//   1 : En hauteur
//     a : ancienne hauteur
//     n : nouvelle hauteur

void alx_fenetre::Cgmnt_lg(const double a, const double n)
{if( Bloquer_rationnalisation() ) return;
 L_rappel_chgmt_dimension.Rappeler(this);
 alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 0;   // Changement de dimension
 alx_classe_base_model::infos_comp[1] = 0;   // En longueur
 alx_classe_base_model::infos_comp[2] = (int)(void*)(&a);
 alx_classe_base_model::infos_comp[3] = (int)(void*)(&n);

 alx_classe_base_model::Oyez_oyez();
}

void alx_fenetre::Cgmnt_ht(const double a, const double n)
{if( Bloquer_rationnalisation() ) return;
 L_rappel_chgmt_dimension.Rappeler(this);
 alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 0;   // Changement de dimension
 alx_classe_base_model::infos_comp[1] = 1;   // En hauteur
 alx_classe_base_model::infos_comp[2] = (int)(void*)(&a);
 alx_classe_base_model::infos_comp[3] = (int)(void*)(&n);

 alx_classe_base_model::Oyez_oyez();
}

void alx_fenetre::Contact_avec_point(alx_point2D *pt)
{alx_classe_base_model::nb_infos_comp = 3;
 alx_classe_base_model::infos_comp[0] = 1;   // Changement de dimension
 alx_classe_base_model::infos_comp[1] = 0;   // En hauteur
 alx_classe_base_model::infos_comp[2] = (int)(void*)(&pt);

 alx_classe_base_model::Oyez_oyez();
}

