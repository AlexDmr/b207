#include "deformations_dynamiques.h"
//#include <mem.h>

//______________________________________________________________________________
deformation_dynamique::~deformation_dynamique()
{alx_element_liste<alx_methode_rappel*> *it     = L_rappel_pendant.Premier()
                                      , *it_fin = L_rappel_pendant.Fin();
 for(;it!=it_fin;it=it->svt) delete it->E();
 it     = L_rappel_fin.Premier();
 it_fin = L_rappel_fin.Fin();
   for(;it!=it_fin;it=it->svt) delete it->E();
 it     = L_rappel_pendant_tjs.Premier();
 it_fin = L_rappel_pendant_tjs.Fin();
   for(;it!=it_fin;it=it->svt) delete it->E();
 it     = L_rappel_amorce_pendant.Premier();
 it_fin = L_rappel_amorce_pendant.Fin();
   for(;it!=it_fin;it=it->svt) delete it->E();
 it     = L_rappel_amorce_fin.Premier();
 it_fin = L_rappel_amorce_fin.Fin();
   for(;it!=it_fin;it=it->svt) delete it->E();
}

//______________________________________________________________________________
void deformation_dynamique::Simuler(const double t)
{t_courant = t;
 alx_element_liste<alx_methode_rappel*> *it     = L_rappel_pendant_tjs.Premier()
                                      , *it_fin = L_rappel_pendant_tjs.Fin()
                                      , *it_tmp;
 for(;it!=it_fin;it=it->svt)
   it->E()->Rappel();
 it     = L_rappel_pendant.Premier();
 it_fin = L_rappel_pendant.Fin();
 alx_element_liste<double> *it_temps     = L_temps_rappel_pendant.Premier()
                         , *it_temps_tmp;
 while( (it != it_fin)
      &&(it_temps->E() < t) )
  {it_tmp       = it->svt;
   it_temps_tmp = it_temps->svt;
   it->E()->Rappel();
   L_rappel_pendant.Retirer(it);
   L_temps_rappel_pendant.Retirer(it_temps);
   it       = it_tmp;
   it_temps = it_temps_tmp;}

 if(t>=1)
  {it     = L_rappel_fin.Premier();
   it_fin = L_rappel_fin.Fin();
   for(;it!=it_fin;it=it->svt)
     it->E()->Rappel();
  }
}
//______________________________________________________________________________
const bool deformation_dynamique::Simuler_amorce(const int ti)
{if(amorce_traitee)
   return true;
 double t;
 if(delai_amorce==0) {
   t = 1;
  } else {t = (double)(ti-debut_amorce)/(double)(delai_amorce);}
 t_courant = t;
 alx_element_liste<alx_methode_rappel*> *it     = L_rappel_amorce_pendant.Premier()
                                      , *it_fin = L_rappel_amorce_pendant.Fin()
                                      , *it_tmp;
 alx_element_liste<double> *it_temps     = L_temps_rappel_amorce_pendant.Premier()
                         , *it_temps_tmp;

 while( (it != it_fin)
      &&(it_temps->E() < t) )
  {it_tmp       = it->svt;
   it_temps_tmp = it_temps->svt;
   it->E()->Rappel();
   L_rappel_amorce_pendant.Retirer(it);
   L_temps_rappel_amorce_pendant.Retirer(it_temps);
   it       = it_tmp;
   it_temps = it_temps_tmp;}

 if(t>=1)
  {it     = L_rappel_amorce_fin.Premier();
   it_fin = L_rappel_amorce_fin.Fin();
   for(;it!=it_fin;it=it->svt)
     it->E()->Rappel();
   amorce_traitee = true;
   Temps_debut(ti);
   if(duree==0) {duree=1; debut_amorce=--temps_debut;}
   temps_fin=temps_debut+duree;
   return true;
  }
 return false;
}


//______________________________________________________________________________
void deformation_dynamique::abonner_a_rappel_fin   (alx_methode_rappel *mr)
{L_rappel_fin.Ajouter_a_la_fin(mr);}
void deformation_dynamique::desabonner_a_rappel_fin(alx_methode_rappel *mr)
{L_rappel_fin.Retirer(mr);}
void deformation_dynamique::abonner_a_rappel_fin_amorce   (alx_methode_rappel *mr)
{L_rappel_amorce_fin.Ajouter_a_la_fin(mr);}
void deformation_dynamique::desabonner_a_rappel_fin_amorce(alx_methode_rappel *mr)
{L_rappel_amorce_fin.Retirer(mr);}

//______________________________________________________________________________
void deformation_dynamique::abonner_a_rappel_pendant_amorce   (alx_methode_rappel *mr, const double t_ref)
{alx_element_liste<alx_methode_rappel*> *it     = L_rappel_amorce_pendant.Premier()
                                      , *it_fin = L_rappel_amorce_pendant.Fin();
 alx_element_liste<double> *it_temps = L_temps_rappel_amorce_pendant.Premier();

 while( (it!=it_fin)
      &&(it_temps->E() < t_ref) )
  {it       = it->svt;
   it_temps = it_temps->svt;}

 L_rappel_amorce_pendant.Ajouter_avant(mr, it);
 L_temps_rappel_amorce_pendant.Ajouter_avant(t_ref, it_temps);}
//______________________________________________________________________________
void deformation_dynamique::desabonner_a_rappel_pendant_amorce(alx_methode_rappel *mr)
{alx_element_liste<alx_methode_rappel*> *it     = L_rappel_amorce_pendant.Premier()
                                      , *it_fin = L_rappel_amorce_pendant.Fin();
 alx_element_liste<double> *it_temps = L_temps_rappel_amorce_pendant.Premier();

 for(; it!=it_fin; it=it->svt)
  {if(it->E()==mr)
    {L_rappel_amorce_pendant.Retirer(it);
     L_temps_rappel_amorce_pendant.Retirer(it_temps);}
   it_temps = it_temps->svt;
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
void deformation_dynamique::abonner_a_rappel_pendant   (alx_methode_rappel *mr)
{L_rappel_pendant_tjs.Ajouter_a_la_fin( mr );
}
//______________________________________________________________________________
//______________________________________________________________________________
void deformation_dynamique::abonner_a_rappel_pendant   (alx_methode_rappel *mr, const double t_ref)
{alx_element_liste<alx_methode_rappel*> *it     = L_rappel_pendant.Premier()
                                      , *it_fin = L_rappel_pendant.Fin();
 alx_element_liste<double> *it_temps = L_temps_rappel_pendant.Premier();

 while( (it!=it_fin)
      &&(it_temps->E() < t_ref) )
  {it       = it->svt;
   it_temps = it_temps->svt;}

 L_rappel_pendant.Ajouter_avant(mr, it);
 L_temps_rappel_pendant.Ajouter_avant(t_ref, it_temps);}
//______________________________________________________________________________
void deformation_dynamique::desabonner_a_rappel_pendant(alx_methode_rappel *mr)
{alx_element_liste<alx_methode_rappel*> *it     = L_rappel_pendant.Premier()
                                      , *it_fin = L_rappel_pendant.Fin();
 alx_element_liste<double> *it_temps = L_temps_rappel_pendant.Premier();

 for(; it!=it_fin; it=it->svt)
  {if(it->E()==mr)
    {L_rappel_pendant.Retirer(it);
     L_temps_rappel_pendant.Retirer(it_temps);}
   it_temps = it_temps->svt;
  }

 // On retire aussi dans la liste L_rappel_pendant_tjs
 it     = L_rappel_pendant_tjs.Premier();
 it_fin = L_rappel_pendant_tjs.Fin();
 for(; it!=it_fin; it=it->svt)
  {if(it->E()==mr)
     L_rappel_pendant_tjs.Retirer(it);
  }
}

//______________________________________________________________________________
void Arranger_valeurs(double &val1, const double val2, const double intervalle)
{double dist_dep = absd(val1-val2);
 if(absd(val1+intervalle-val2) < dist_dep) val1 += intervalle;
  else if(absd(val1-intervalle-val2) < dist_dep) val1 -= intervalle;
}

deformation_dynamique_repere::deformation_dynamique_repere( const int duree
                               , alx_repere2D *rep_depart, alx_repere2D *rep_destination ) : deformation_dynamique(duree)
{this->rep_depart      = rep_depart;
 this->rep_destination = rep_destination;
}

void deformation_dynamique_repere::Demarrer()
{deformation_dynamique::Demarrer();
 repere_initiale.maj( *rep_depart );
 repere_initiale.rotation  = Modulo_double(repere_initiale.rotation,  2*PI);
 rep_destination->rotation = Modulo_double(rep_destination->rotation, 2*PI);
 Arranger_valeurs(repere_initiale.rotation, rep_destination->rotation, 2*PI);
}

void deformation_dynamique_repere::Simuler(const double t)
{rep_depart->rotation = fonction_interpolation( repere_initiale.rotation
                                              , rep_destination->rotation
                                              , t);
 rep_depart->origine.x = fonction_interpolation( repere_initiale.origine.x
                                               , rep_destination->origine.x
                                               , t);
 rep_depart->origine.y = fonction_interpolation( repere_initiale.origine.y
                                               , rep_destination->origine.y
                                               , t);
 rep_depart->etirement.dx = fonction_interpolation( repere_initiale.etirement.dx
                                                  , rep_destination->etirement.dx
                                                  , t);
 rep_depart->etirement.dy = fonction_interpolation( repere_initiale.etirement.dy
                                                  , rep_destination->etirement.dy
                                                  , t);
 deformation_dynamique::Simuler(t);
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
deformation_dynamique_repere_et_dim::deformation_dynamique_repere_et_dim( const int duree
                                                                        , alx_repere2D *rep_depart
                                                                        , alx_repere2D *rep_destination) : deformation_dynamique_repere(duree, rep_depart, rep_destination)
{ex_deb = ex_fin = ey_deb = ey_fin = 0;}

void deformation_dynamique_repere_et_dim::Demarrer(const double x_deb, const double y_deb)
{deformation_dynamique_repere::Demarrer();
 ex_deb = x_deb;
 ey_deb = y_deb;}

void deformation_dynamique_repere_et_dim::Simuler(const double t, const double x_fin, const double y_fin)
{deformation_dynamique_repere::Simuler(t);
 ex_fin  = x_fin;
 ey_fin  = y_fin;
 ex_cour = fonction_interpolation(ex_deb, x_fin, t);
 ey_cour = fonction_interpolation(ey_deb, y_fin, t);
 rep_depart->etirement.dx = repere_initiale.etirement.dx*ex_cour;
 rep_depart->etirement.dy = repere_initiale.etirement.dy*ey_cour;
 }


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
deformation_dynamique_etirement_fenetre::deformation_dynamique_etirement_fenetre( const int duree
                                                            , alx_noeud_fenetre_sdl_opengl *fen_depart
                                                            , const double translucidite_depart
                                                            , alx_noeud_fenetre_sdl_opengl *fen_destination
                                                            , const double translucidite_arrive ) : deformation_dynamique_repere_et_dim( duree
                                                                                                                                       , (alx_repere2D*)(fen_depart)
                                                                                                                                       , (alx_repere2D*)(fen_destination) )
{this->fen_depart      = fen_depart;
 this->fen_destination = fen_destination;
 this->translucidite_depart = translucidite_depart;
 this->translucidite_arrive = translucidite_arrive;
 this->bord_translucide = fen_depart->Corp_translucide();
 this->corp_translucide = fen_depart->Bord_translucide();
 fonction_interpolation = Fonction_interpolation_cosinusoidale;
 fonction_interpolation_opacite = Fonction_interpolation_lineaire;
 }

deformation_dynamique_etirement_fenetre::deformation_dynamique_etirement_fenetre( const int duree
                                                                                , alx_noeud_fenetre_sdl_opengl *fen_depart
                                                                                , const double translucidite_depart
                                                                                , alx_repere2D *rep_destination
                                                                                , const double translucidite_arrive
                                                                                , const double lg_lie_a_repere
                                                                                , const double ht_lie_a_repere ) : deformation_dynamique_repere_et_dim( duree
                                                                                                                                                      , (alx_repere2D*)(fen_depart)
                                                                                                                                                      , rep_destination )
{this->fen_depart      = fen_depart;
 this->fen_destination = (alx_noeud_fenetre_sdl_opengl*)NULL;
 this->translucidite_depart = translucidite_depart;
 this->translucidite_arrive = translucidite_arrive;
 this->bord_translucide = fen_depart->Corp_translucide();
 this->corp_translucide = fen_depart->Bord_translucide();

 fonction_interpolation         = Fonction_interpolation_cosinusoidale;
 fonction_interpolation_opacite = Fonction_interpolation_lineaire;
 l = lg_lie_a_repere;
 h = ht_lie_a_repere;
 }

void deformation_dynamique_etirement_fenetre::Demarrer()
{deformation_dynamique_repere_et_dim::Demarrer(1, 1);
 fen_depart->Gerer_contacts(false);
 if( (translucidite_depart<1)||(translucidite_arrive<1) )
   fen_depart->Fenetre_translucide(true);
 }

void deformation_dynamique_etirement_fenetre::Simuler(const double t)
{// Pour le cas où les dimensions de la fenêtre changent en cours de transition, on recalcule les facteurs d'étirement
 if(fen_destination)
  {deformation_dynamique_repere_et_dim::ex_fin = (fen_destination->Longueur_corp() * fen_destination->Etirement().dx)
                                               / (fen_depart->Longueur_corp()      * repere_initiale.etirement.dx);
   deformation_dynamique_repere_et_dim::ey_fin = (fen_destination->Hauteur_corp()  * fen_destination->Etirement().dy)
                                               / (fen_depart->Hauteur_corp()       * repere_initiale.etirement.dy);
  }
  else
   {deformation_dynamique_repere_et_dim::ex_fin = (rep_destination->etirement.dx)
                                                / (repere_initiale.etirement.dx);
    deformation_dynamique_repere_et_dim::ey_fin = (rep_destination->etirement.dy)
                                                / (repere_initiale.etirement.dy);
   }

// On simule au niveau deformation_dynamique_repere_et_dim
 deformation_dynamique_repere_et_dim::Simuler(t);

// On simule au niveau de notre classe
 if(t>=1)
   translucidite_depart = translucidite_depart;
 double trans = fonction_interpolation_opacite( translucidite_depart
                                              , translucidite_arrive
                                              , t);
 fen_depart->Translucidite_globale( Amax( (double)0
                                        , Amin( (double)1
                                              , trans)) );
 trans = fen_depart->Translucidite_corp();
 fen_depart->Mettre_a_jour();

 if(t>=1)
  {if(trans > 0)
     fen_depart->Gerer_contacts(true);
   if(trans >= 1)
    {fen_depart->Bord_translucide(this->bord_translucide);
     fen_depart->Corp_translucide(this->corp_translucide);}
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//   alx_noeud_fenetre_sdl_opengl *fen_depart, *fen_destination;
//   double hauteur_corp_depart, longueur_corp_depart;
deformation_dynamique_fenetre::deformation_dynamique_fenetre( const int duree
                               , alx_noeud_fenetre_sdl_opengl *fen_depart
                               , alx_noeud_fenetre_sdl_opengl *fen_destination ) : deformation_dynamique_repere( duree
                                                                                                               , (alx_repere2D*)(fen_depart)
                                                                                                               , (alx_repere2D*)(fen_destination) )
{
 this->fen_depart      = fen_depart;
 this->fen_destination = fen_destination;
}

void deformation_dynamique_fenetre::Demarrer()
{
 deformation_dynamique_repere::Demarrer();
 hauteur_corp_depart  = fen_depart->Hauteur_corp();
 longueur_corp_depart = fen_depart->Longueur_corp();
}


void deformation_dynamique_fenetre::Simuler(const double t)
{
 deformation_dynamique_repere::Simuler(t);
 fen_depart->Hauteur_corp( fonction_interpolation( hauteur_corp_depart
                                                 , fen_destination->Hauteur_corp()
                                                 , t) );
 fen_depart->Longueur_corp( fonction_interpolation( longueur_corp_depart
                                                  , fen_destination->Longueur_corp()
                                                  , t) );
 fen_depart->Mettre_a_jour_bordure();
}


