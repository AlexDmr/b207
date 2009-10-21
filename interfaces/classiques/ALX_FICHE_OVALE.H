#ifndef __ALX_FICHE_OVALE_H__
#define __ALX_FICHE_OVALE_H__

#include "alx_fenetre.h"
#include "../../physique/alx_model_simulation_physique1.h"

class alx_fiche_ovale : public alx_fenetre
{
 private:
   void init();
   void maj_mp_zrec();
   void maj_mp_redim();
   double h_base;
   // Pour la redimension de la zone de redimension justement ^_^
     double l_au_contact, h_au_contact, eloignement_bordure_pt;
     alx_point2D   pt_contact_depart, info_point;
     bool redim_verticale, redim_horizontale;
     alx_vecteur2D position_initiale
                 , vect_dec, vect_dec_x, vect_dec_y, vect_dep;

     unsigned int num_zone_redim; //321
                                  //4 0
                                  //567
     unsigned int Calculer_num_zone_redim(const alx_vecteur2D &pt);

 public :
   alx_point2D centre_entete
             , centre_corp
             , centre_zone_rot;
   alx_model_simulation_physique1 mp_entete
                                , mp_corp
                                , mp_zone_rot
                                , mp_zone_redim;

  // Les constructeurs:
   alx_fiche_ovale();
   alx_fiche_ovale( const char *n, const double hauteur, const double largeur);
   alx_fiche_ovale( const char *n, const double hauteur, const double largeur
                  , const double hauteur_entete, const double r_zone_rot);

  // Les méthodes:
   inline virtual void Appliquer_maj() {maj_mp_zrec(); maj_mp_redim();}
   void rationnaliser(int num);
   inline double H_base() {return h_base;}

   inline virtual alx_model_simulation_physique1* MP_corps() {return &mp_corp;}

  // Surcharges de la fenetre
   void Longueur_corp(const double v);
   inline const double Longueur_corp() const {return alx_fenetre::Longueur_corp();}
   void Hauteur_corp(const double v);
   inline const double Hauteur_corp() const {return alx_fenetre::Hauteur_corp();}

   inline const alx_model_simulation_physique1* MP_entete()   const {return &mp_entete;}
   inline const alx_model_simulation_physique1* MP_corp()     const {return &mp_corp;}
   inline const alx_model_simulation_physique1* MP_zone_rot() const {return &mp_zone_rot;}
};

#endif
