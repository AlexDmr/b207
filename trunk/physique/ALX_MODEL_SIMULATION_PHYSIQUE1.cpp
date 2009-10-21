#include "alx_model_simulation_physique1.h"

alx_model_simulation_physique1::alx_model_simulation_physique1() : alx_classe_base_model()
{masse            = 0.0;
 rotation         = 0.0;
 vitesse_rotation = 0.0;
 nb_primitives_2D = 0;
 mobilite         = Talx_immobile;
 ne_pas_modifier_la_correction = false;

 centre_rot.maj(50.0, 50.0);
}

alx_model_simulation_physique1::alx_model_simulation_physique1(const alx_model_simulation_physique1 &m)
{maj(m);}

alx_model_simulation_physique1::~alx_model_simulation_physique1()
{L_droite2D.Vider();
 L_segment2D.Vider();
 L_cercle2D_physique.Vider();
 L_dep.Vider();}

/******************************************************************************/
/**************************** Les méthodes ************************************/
/******************************************************************************/
void alx_model_simulation_physique1::Informer_Reajustement_translation( alx_vecteur2D *v
                                                                     , alx_point2D *p )
{alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 7;   // Changement de l'objet
 alx_classe_base_model::infos_comp[1] = 1;   // par translation.
 alx_classe_base_model::infos_comp[2] = (int)(void*)v;
 alx_classe_base_model::infos_comp[3] = (int)(void*)p;
 alx_classe_base_model::Oyez_oyez();}

//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Informer_Reajustement_rotation(double *angle, alx_point2D *pt)
{alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 7;   // Changement de l'objet
 alx_classe_base_model::infos_comp[1] = 2;   // par rotation
 alx_classe_base_model::infos_comp[2] = (int)(void*)angle;   // par translation.
 alx_classe_base_model::infos_comp[3] = (int)(void*)pt;   // par translation.
 alx_classe_base_model::Oyez_oyez();}

//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Informer_Reajustement_etirement(alx_vecteur2D *v, alx_point2D *pt)
{
 alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 7;   // Changement de l'objet
 alx_classe_base_model::infos_comp[1] = 3;   // par étirement.
 alx_classe_base_model::infos_comp[2] = (int)(void*)v;
 alx_classe_base_model::infos_comp[3] = (int)(void*)pt;

 alx_classe_base_model::Oyez_oyez();
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_model_simulation_physique1::Informer_au_sujet_de_translation( alx_vecteur2D *v
                                                                     , alx_point2D *p )
{alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 5;   // Changement de l'objet
 alx_classe_base_model::infos_comp[1] = 1;   // par translation.
 alx_classe_base_model::infos_comp[2] = (int)(void*)v;
 alx_classe_base_model::infos_comp[3] = (int)(void*)p;
 alx_classe_base_model::Oyez_oyez();}

//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Informer_au_sujet_de_rotation(double *angle, alx_point2D *pt)
{alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 5;   // Changement de l'objet
 alx_classe_base_model::infos_comp[1] = 2;   // par rotation
 alx_classe_base_model::infos_comp[2] = (int)(void*)angle;   // par translation.
 alx_classe_base_model::infos_comp[3] = (int)(void*)pt;   // par translation.
 alx_classe_base_model::Oyez_oyez();}

//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Informer_au_sujet_de_etirement(alx_vecteur2D *v, alx_point2D *pt)
{alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 5;   // Changement de l'objet
 alx_classe_base_model::infos_comp[1] = 3;   // par étirement.
 alx_classe_base_model::infos_comp[2] = (int)(void*)v;
 alx_classe_base_model::infos_comp[3] = (int)(void*)pt;
 alx_classe_base_model::Oyez_oyez();}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Positionner(double x, double y)
{L_dep.Vider();
 position.maj(x, y);

 alx_point2D pt(x, y);
 L_dep.Ajouter_au_debut( alx_deplacement2D(pt, pt, 0.0, 1.0) );

 // Mise à jour des autres modèles de l'entité:
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 0;   // mise à jour des positions.
 alx_classe_base_model::infos_comp[1] = 0;   // par translation.

 alx_classe_base_model::Oyez_oyez();
}

//______________________________________________________________________________
void alx_model_simulation_physique1::Repositionner( double x, double y
                                                  , double r, bool avertir)
{L_dep.Vider();

 L_dep.Ajouter_au_debut( alx_deplacement2D(position, alx_point2D(x,y), 0.0, 1.0) );
 position.maj(x, y);
 vitesse_rotation = r-rotation;
 rotation = r;

 // Mise à jour des autres modèles de l'entité:
 if(!avertir) return;
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 0;   // mise à jour des positions.
 alx_classe_base_model::infos_comp[1] = 2;   // par translation et rotation.
 alx_classe_base_model::Oyez_oyez();
}

void alx_model_simulation_physique1::Ajouter_droite2D  (const alx_droite2D &d)
{nb_primitives_2D++;
 L_droite2D.Ajouter_a_la_fin( d );

// On informe les intéressés des changements survenus:
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 1;   // mise à jour des composants.
 alx_classe_base_model::infos_comp[1] = 4;   // Un nouveau segment 2D.
 alx_classe_base_model::Oyez_oyez();
}

//______________________________________________________________________________
void alx_model_simulation_physique1::Ajouter_segment2D( const alx_segment2D &seg)
{nb_primitives_2D++;
 L_segment2D.Ajouter_a_la_fin( seg );

// On informe les intéressés des changements survenus:
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 1;   // mise à jour des composants.
 alx_classe_base_model::infos_comp[1] = 1;   // Un nouveau segment 2D.
 alx_classe_base_model::Oyez_oyez();
}

//______________________________________________________________________________
void alx_model_simulation_physique1::Ajouter_segment2D( const alx_point2D &pt_deb
                                                      , const alx_point2D &pt_fin )
{Ajouter_segment2D( alx_segment2D(pt_deb, pt_fin) );}

/******************************************************************************/
void alx_model_simulation_physique1::Ajouter_cercle2D_phys( const alx_cercle2D_physique &cercle2D_phys
                                                          , const double masse)
{// ajout de la primitive dans la liste et incrémentation du nombre total.
 L_cercle2D_physique.Ajouter_a_la_fin(cercle2D_phys);
 nb_primitives_2D++;
 double coef = 1/(masse+this->masse);

 barycentre.dx = (this->masse*barycentre.dx + masse*cercle2D_phys.x)*coef;
 barycentre.dy = (this->masse*barycentre.dy + masse*cercle2D_phys.y)*coef;

 this->masse += masse;

// On informe les intéressés des changements survenus:
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 1;   // mise à jour des composants.
 alx_classe_base_model::infos_comp[1] = 0;   // Un nouveau cercle physique.
 alx_classe_base_model::Oyez_oyez();
}

/******************************************************************************/
void alx_model_simulation_physique1::Ajouter_polygone2D( const alx_polygone2D &pol
                                                       , const double masse)
{L_polygone2D.Ajouter_au_debut(pol);
 nb_primitives_2D++;
/* double coef = 1/(masse+this->masse);

 barycentre.dx = (this->masse*barycentre.dx + masse*cercle2D_phys.x)*coef;
 barycentre.dy = (this->masse*barycentre.dy + masse*cercle2D_phys.y)*coef;
*/
 this->masse += masse;

// On informe les intéressés des changements survenus:
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 1;   // mise à jour des composants.
 alx_classe_base_model::infos_comp[1] = 2;   // Un nouveau polygone.
 alx_classe_base_model::Oyez_oyez();
}

void alx_model_simulation_physique1::Ajouter_ellipse2D ( const alx_ellipse2D &e
                                                       , const double masse )
{L_ellipse2D.Ajouter_au_debut(e);
 nb_primitives_2D++;
 double coef = 1/(masse+this->masse);

 barycentre.dx = (this->masse*barycentre.dx + e.x)*coef;
 barycentre.dy = (this->masse*barycentre.dy + e.y)*coef;

 this->masse += masse;

// On informe les intéressés des changements survenus:
 alx_classe_base_model::nb_infos_comp = 2;
 alx_classe_base_model::infos_comp[0] = 1;   // mise à jour des composants.
 alx_classe_base_model::infos_comp[1] = 3;   // Une nouvelle ellipse.
 alx_classe_base_model::Oyez_oyez();
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
void alx_model_simulation_physique1::Integrer_force_en_vitesse( const alx_vecteur2D &f
                                                              , double dt)
{A_v_tmp.maj(f); //alx_vecteur2D tmp(f);
 /*tmp*/A_v_tmp.Alonger(dt/masse);
 vitesse.Ajouter(A_v_tmp/*tmp*/);}

/******************************************************************************/
/* Permet de mettre à jour le déplacement du modèle par rapport au loi dynamique classique */
/* vitesse  = intégration de l'accélération sur le temps dt*/
/* position = intégration de la vitesse sur le temps dt*/
void alx_model_simulation_physique1::Deplacement_par_loi_dynamique(const double temps_deb, const double dt)
{// Sauvegarde de la dernière position:
/* alx_point2D   *pt_tmp = &(L_dep.back().seg.b);
 alx_vecteur2D position_depart( pt_tmp->x, pt_tmp->y ); */
 A_v_tmp.maj( position ); //alx_vecteur2D position_depart( position );

// On efface la liste des déplacments précédents:
 L_dep.Vider();

 double temps_fin = temps_deb + dt;
 alx_vecteur2D n_position( A_v_tmp ); //position_depart );

// Calcul de la position suivante:
 acceleration.maj(force);
 acceleration.Alonger( 1/masse );
 alx_vecteur2D acc_tmp(acceleration);
 alx_vecteur2D vit_tmp(vitesse+=(acc_tmp*=dt));
 n_position += (vit_tmp*=dt);

 L_dep.Ajouter_au_debut( alx_deplacement2D (A_v_tmp/*position_depart*/, n_position, temps_deb, temps_fin) );
 position.maj( n_position );

// Mise à jour des autres modèles de l'entité:
 alx_classe_base_model::nb_infos_comp = 1;
 alx_classe_base_model::infos_comp[0] = 0;   // mise à jour des positions.
 alx_classe_base_model::Oyez_oyez();
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
bool alx_model_simulation_physique1::Contient(const alx_point2D &p)
{A_pt_tmp.maj(p); // alx_point2D pt(p);

 Appliquer_deplacement_inverse_a( A_pt_tmp ); //pt);

// Voir si on rentre dans les osus espaces définit par les droite (il faut être à gauche de la droite)
 alx_vecteur2D perp, v;
 alx_element_liste<alx_droite2D> *it_d     = L_droite2D.Premier()
                               , *it_fin_d = L_droite2D.Fin();
 for(; it_d!=it_fin_d; it_d=it_d->svt)
  {perp.maj(*((it_d->E()).directeur));
   perp.Perpendiculaire_g();
   v.maj(*((it_d->E()).ref), A_pt_tmp); //pt);
   if(perp.produit_scalaire(v)<0)
     return false;
  }


// Voir au niveau des polygones
 alx_element_liste<alx_polygone2D> *it_p     = L_polygone2D.Premier()
                                 , *it_fin_p = L_polygone2D.Fin();
 for(; it_p!=it_fin_p; it_p=it_p->svt)
   if( (it_p->E()).contient(A_pt_tmp/*pt*/) )
     return true;

// Puis au niveau des cercles physiques
 alx_element_liste<alx_cercle2D_physique> *it_c     = L_cercle2D_physique.Premier()
                                        , *it_fin_c = L_cercle2D_physique.Fin();
 for(; it_c!=it_fin_c; it_c=it_c->svt)
   if( (it_c->E()).contient(A_pt_tmp/*pt*/) )
     return true;

// Puis au niveau des cercles physiques
 alx_element_liste<alx_ellipse2D> *it_e     = L_ellipse2D.Premier()
                                , *it_fin_e = L_ellipse2D.Fin();
 for(; it_e!=it_fin_e; it_e=it_e->svt)
   if( (it_e->E()).contient(A_pt_tmp/*pt*/) )
     return true;

 return false;
}

//------------------------------------------------------------------------------
//  6 : Contact avec un objet positionner par une suite de transformation de repère
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//              * : adresse d'une liste de transformation de repère list<alx_repere2D*>*
void alx_model_simulation_physique1::Point_contact_detecte( alx_point2D *pt, alx_classe_base_liant *liant_demandeur
                                                          , alx_liste<alx_repere2D*> *L_rep)
{// Mise à jour des autres modèles de l'entité:
 alx_classe_base_model::nb_infos_comp = 5;
 alx_classe_base_model::infos_comp[0] = 6;                  // Contact détecté
 alx_classe_base_model::infos_comp[1] = 0;                  // avec un point
 alx_classe_base_model::infos_comp[2] = (int)(void*)pt;     // @ du point
 alx_classe_base_model::infos_comp[3] = (int)(void*)this;   // @ du modèle physique
 alx_classe_base_model::infos_comp[4] = (int)(void*)L_rep;  // @ de la list de repère

 if(liant_demandeur==NULL) alx_classe_base_model::Oyez_oyez();
  else alx_classe_base_model::Colporter(liant_demandeur);
}

//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Point_contact_detecte(alx_point2D *pt, alx_classe_base_liant *liant_demandeur)
{// Mise à jour des autres modèles de l'entité:
 alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 2;                  // Contact détecté
 alx_classe_base_model::infos_comp[1] = 0;                  // avec un point
 alx_classe_base_model::infos_comp[2] = (int)(void*)pt;     // @ du point
 alx_classe_base_model::infos_comp[3] = (int)(void*)this;   // @ du modèle physique

 if(liant_demandeur==NULL) alx_classe_base_model::Oyez_oyez();
  else alx_classe_base_model::Colporter(liant_demandeur);
}

//------------------------------------------------------------------------------
void alx_model_simulation_physique1::Rupture_contact_detecte( alx_point2D *pt, alx_classe_base_liant *liant_demandeur
                                                            , alx_liste<alx_repere2D*> *L_rep )
{// Mise à jour des autres modèles de l'entité:
 alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 3;                  // Rupture de contact
 alx_classe_base_model::infos_comp[1] = 0;                  // c'est un point
 alx_classe_base_model::infos_comp[2] = (int)(void*)pt;     // @ du point
 alx_classe_base_model::infos_comp[3] = (int)(void*)this;   // @ du modèle physique
 alx_classe_base_model::infos_comp[4] = (int)(void*)L_rep;  // @ de la list de repère

 if(liant_demandeur==NULL) alx_classe_base_model::Oyez_oyez();
  else alx_classe_base_model::Colporter(liant_demandeur);
}
/******************************************************************************/
void alx_model_simulation_physique1::Deplacement_contact_detecte( alx_point2D *pt, alx_classe_base_liant *liant_demandeur
                                                                , alx_liste<alx_repere2D*> *L_rep)
{// Mise à jour des autres modèles de l'entité:
 alx_classe_base_model::nb_infos_comp = 4;
 alx_classe_base_model::infos_comp[0] = 4;                  // Contact en mouvement
 alx_classe_base_model::infos_comp[1] = 0;                  // c'est un point
 alx_classe_base_model::infos_comp[2] = (int)(void*)pt;     // @ du point
 alx_classe_base_model::infos_comp[3] = (int)(void*)this;   // @ du modèle physique
 alx_classe_base_model::infos_comp[4] = (int)(void*)L_rep;  // @ de la list de repère

 if(liant_demandeur==NULL) alx_classe_base_model::Oyez_oyez();
  else alx_classe_base_model::Colporter(liant_demandeur);
}

/******************************************************************************/
void alx_model_simulation_physique1::maj(const alx_model_simulation_physique1 &m)
{alx_classe_base_model::maj( m );

 masse    = m.masse;
 rotation = m.rotation;
 mobilite = m.mobilite;

 barycentre.maj(m.barycentre);

// Les caractéristiques cinétiques:
 force.maj(m.force);
 acceleration.maj(m.acceleration);
 vitesse.maj(m.vitesse);
 position.maj(m.position);

 L_dep = m.L_dep;

 vitesse_rotation = m.vitesse_rotation;
 acceleration_rot = m.acceleration_rot;

 nb_primitives_2D = m.nb_primitives_2D;
 L_droite2D.maj(          m.L_droite2D          );
 L_ellipse2D.maj(         m.L_ellipse2D         );
 L_segment2D.maj(         m.L_segment2D         );
 L_cercle2D_physique.maj( m.L_cercle2D_physique );
}

alx_model_simulation_physique1 &alx_model_simulation_physique1::operator =(const alx_model_simulation_physique1 &m)
{maj(m);
 return *this;}



