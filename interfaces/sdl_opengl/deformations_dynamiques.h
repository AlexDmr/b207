#ifndef __ALX_DEFORMATION_DYNAMIQUE_H__
#define __ALX_DEFORMATION_DYNAMIQUE_H__

#include "..\interface.h"
#include "..\..\physique\math_alex.cpp"

typedef double (*Fonction_interpolation) (const double valeur_depart, const double valeur_arrive, const double t);

double Fonction_interpolation_lineaire(const double valeur_depart, const double valeur_arrive, const double t)
{return valeur_depart + (valeur_arrive-valeur_depart)*t;}

double Fonction_interpolation_cosinusoidale(const double valeur_depart, const double valeur_arrive, const double t)
{return valeur_depart + (valeur_arrive-valeur_depart)*(cos(PI*(1-t))+1)/2;}

class deformation_dynamique
{private:
   int temps_debut, temps_fin, duree, delai_amorce, debut_amorce;
   double t_courant;
   bool amorce_traitee;
   alx_liste<alx_methode_rappel*> L_rappel_pendant
                                , L_rappel_pendant_tjs
                                , L_rappel_fin
                                , L_rappel_amorce_pendant
                                , L_rappel_amorce_fin;
   alx_liste<double> L_temps_rappel_pendant, L_temps_rappel_amorce_pendant;
 public:
   Fonction_interpolation fonction_interpolation;

   deformation_dynamique(const int duree)
    {this->duree = duree; fonction_interpolation = Fonction_interpolation_cosinusoidale;
     this->delai_amorce = t_courant = 0;
     amorce_traitee = false;}
   deformation_dynamique(const int amorce, const int duree)
    {this->duree = duree; fonction_interpolation = Fonction_interpolation_cosinusoidale;
     this->delai_amorce = amorce; t_courant = 0;
     amorce_traitee = false;}
  ~deformation_dynamique();

  //____________________________________________________________________________
   inline const double T_courant() const {return t_courant;}
   inline const double V_courant() const {return fonction_interpolation(0,1,t_courant);}
  //____________________________________________________________________________
   inline virtual void Delai_amorce(const int t) {this->delai_amorce = t;}
   inline virtual const int Delai_amorce() const {return delai_amorce;}
  //____________________________________________________________________________
   virtual void Demarrer() {debut_amorce=temps_debut=(int)SDL_GetTicks();
                            if(duree==0) {duree=1; debut_amorce=--temps_debut;}
                            temps_fin=temps_debut+duree;};
  //____________________________________________________________________________
   const bool Simuler_amorce(const int ti);
   virtual void Simuler(const double t);
   virtual void Simuler() {Simuler(Position_temporelle_relative());}
  //____________________________________________________________________________
   inline const double Position_temporelle_relative(const int t) {return ((double)(t-Temps_debut()))
                                                                        / (double)(duree);}
   inline const double Position_temporelle_relative() {return ((double)(SDL_GetTicks()-Temps_debut()))
                                                            / (double)(Temps_fin()-Temps_debut());}
  //____________________________________________________________________________
   inline const int Temps_debut() const {return temps_debut;}
   inline void Temps_debut(const int t) {temps_debut = t;}
  //____________________________________________________________________________
   inline const int Temps_fin() const {return temps_fin;}
   inline void Temps_fin(const int t) {temps_fin = t;}

  //____________________________________________________________________________
   inline void F_interpollation(Fonction_interpolation &f) {fonction_interpolation = f;}
   inline Fonction_interpolation& F_interpollation()       {return fonction_interpolation;}

  //____________________________________________________________________________
  // Les méthodes liées aux fonctions de rappel
   void abonner_a_rappel_pendant   (alx_methode_rappel *mr);
   void abonner_a_rappel_pendant   (alx_methode_rappel *mr, const double t_ref);
   void desabonner_a_rappel_pendant(alx_methode_rappel *mr);
   void abonner_a_rappel_fin   (alx_methode_rappel *mr);
   void desabonner_a_rappel_fin(alx_methode_rappel *mr);
   void abonner_a_rappel_pendant_amorce   (alx_methode_rappel *mr, const double t_ref);
   void desabonner_a_rappel_pendant_amorce(alx_methode_rappel *mr);
   void abonner_a_rappel_fin_amorce   (alx_methode_rappel *mr);
   void desabonner_a_rappel_fin_amorce(alx_methode_rappel *mr);
};
deformation_dynamique* Void_vers_deformation_dynamique(void *p) {return (deformation_dynamique*)p;}

template<class T> class deformation_dynamique_scalaire : public deformation_dynamique
{
 private:
   T *scalaire, scal_initial, *scal_dest;
   T tmp_deb, tmp_fin;
 public :
   deformation_dynamique_scalaire(const int duree, T *scal_deb, T *scal_fin) : deformation_dynamique(duree)
    {scalaire = scal_deb; scal_dest = scal_fin;}
   deformation_dynamique_scalaire(const int duree, T *scal_deb, const T fin) : deformation_dynamique(duree)
    {scalaire  = scal_deb;
     tmp_fin   = fin;
     scal_dest = &tmp_fin;}
   deformation_dynamique_scalaire(const int duree, const T deb, const T fin) : deformation_dynamique(duree)
    {tmp_deb = deb;
     tmp_fin = fin;
     scalaire = &tmp_deb; scal_dest = &tmp_fin;}

   virtual void Demarrer()
    {deformation_dynamique::Demarrer();
     scal_initial = *scalaire;}
   virtual void Simuler(const double t)
    {*scalaire = fonction_interpolation(scal_initial, *scal_dest, t);}
   inline virtual void Simuler()
    {Simuler( Position_temporelle_relative() ); }
};

class deformation_dynamique_repere : public deformation_dynamique
{
 protected:
   alx_repere2D repere_initiale, *rep_depart, *rep_destination;

 public :
   deformation_dynamique_repere( const int duree
                               , alx_repere2D *rep_depart, alx_repere2D *rep_destination );
   virtual void Demarrer();
   virtual void Simuler(const double t);
   inline virtual void Simuler()
    {Simuler( Position_temporelle_relative() ); }
   inline void Rep_dep(alx_repere2D *r) {rep_depart      = r;}
   inline void Rep_fin(alx_repere2D *r) {rep_destination = r;}
};
deformation_dynamique* Void_vers_deformation_dynamique_repere(void *p) {return (deformation_dynamique_repere*)p;}
//______________________________________________________________________________
class deformation_dynamique_repere_et_dim : public deformation_dynamique_repere
{
 protected:
   double ex_cour, ey_cour;

 public :
   double ex_deb, ex_fin
        , ey_deb, ey_fin;

   deformation_dynamique_repere_et_dim( const int duree
                                          , alx_repere2D *rep_depart
                                          , alx_repere2D *rep_destination);
   virtual void Demarrer() {Demarrer(ex_deb, ey_deb);}
   virtual void Demarrer(const double x_deb, const double y_deb);
   inline virtual void Simuler(const double t) {Simuler(t, ex_fin, ey_fin);}
   virtual void Simuler(const double t, const double x_fin, const double y_fin);
   inline virtual void Simuler() {Simuler(ex_fin, ey_fin);}
   inline virtual void Simuler(const double x_fin, const double y_fin)
    {Simuler( Position_temporelle_relative(), x_fin, y_fin); }

   inline const double Etirement_x_courant() const {return ex_cour;}
   inline const double Etirement_y_courant() const {return ey_cour;}

   inline deformation_dynamique_repere* Deformation_dynamique_repere() {return (deformation_dynamique_repere*)this;}
};
deformation_dynamique* Void_vers_deformation_dynamique_repere_et_dim(void *p) {return (deformation_dynamique_repere_et_dim*)p;}
//______________________________________________________________________________
class deformation_dynamique_etirement_fenetre : public deformation_dynamique_repere_et_dim
{
 protected:
   alx_noeud_fenetre_sdl_opengl *fen_depart, *fen_destination;
   bool bord_translucide, corp_translucide;
   double translucidite_depart, translucidite_arrive
        , l, h;

 public :
   Fonction_interpolation fonction_interpolation_opacite;

   deformation_dynamique_etirement_fenetre( const int duree
                                , alx_noeud_fenetre_sdl_opengl *fen_depart      , const double translucidite_depart
                                , alx_noeud_fenetre_sdl_opengl *fen_destination , const double translucidite_arrive);
   deformation_dynamique_etirement_fenetre( const int duree
                                , alx_noeud_fenetre_sdl_opengl *fen_depart, const double translucidite_depart
                                , alx_repere2D *rep_destination           , const double translucidite_arrive
                                , const double lg_lie_a_repere, const double ht_lie_a_repere);
   virtual void Demarrer();
   virtual void Simuler(const double t);
   inline virtual void Simuler()
    {Simuler( Position_temporelle_relative() ); }

  // Méthodes pour notre ami SWIG qui tue
   inline deformation_dynamique_repere* Deformation_dynamique_repere() {return (deformation_dynamique_repere*)this;}
};
deformation_dynamique* Void_vers_deformation_dynamique_etirement_fenetre(void *p) {return (deformation_dynamique_etirement_fenetre*)p;}
//______________________________________________________________________________
class deformation_dynamique_fenetre : public deformation_dynamique_repere
{
 private:
   alx_noeud_fenetre_sdl_opengl *fen_depart, *fen_destination;
   double hauteur_corp_depart, longueur_corp_depart;

 public :
   deformation_dynamique_fenetre( const int duree
                                , alx_noeud_fenetre_sdl_opengl *fen_depart
                                , alx_noeud_fenetre_sdl_opengl *fen_destination );
   virtual void Demarrer();
   virtual void Simuler(const double t);
   inline virtual void Simuler()
    {Simuler( Position_temporelle_relative() ); }
};
deformation_dynamique* Void_vers_deformation_dynamique_fenetre(void *p) {return (deformation_dynamique_fenetre*)p;}

#endif
