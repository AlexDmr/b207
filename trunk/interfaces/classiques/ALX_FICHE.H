#ifndef __ALX_FICHE_H__
#define __ALX_FICHE_H__

#include "alx_fenetre.h"
#include "../../physique/alx_model_simulation_physique1.h"

class alx_fiche : public alx_fenetre
{
 private:
   void init();
   void maj_mp_zrec();
   void maj_mp_redim();
   double h_base;
   alx_vecteur2D vect_info[8], depassement[8];
   bool zone_redim_active[8];

 public :
   alx_point2D centre_entete
             , centre_corp
             , centre_zone_rot
             , info_point;
   alx_vecteur2D vect_dec;
   alx_model_simulation_physique1 mp_entete
                                , mp_corp
                                , mp_zone_rot
                                , mp_zone_redim[8]; // 654
                                                    // 7 3
                                                    // 012

  // Les constructeurs:
   alx_fiche();
   alx_fiche( const char *n, const double hauteur, const double largeur);
   alx_fiche( const char *n, const double hauteur, const double largeur
            , const double hauteur_entete, const double r_zone_rot);

  // Les méthodes:
   inline virtual void Appliquer_maj() {maj_mp_zrec(); maj_mp_redim();}
   void rationnaliser(int num);
   inline const double H_base() const {return h_base;}

   inline virtual alx_model_simulation_physique1* MP_corps() {return &mp_corp;}
   inline const bool Zone_redim_active(const unsigned int i) {return zone_redim_active[i];}

   inline const alx_model_simulation_physique1* MP_entete()   const {return &mp_entete;}
   inline const alx_model_simulation_physique1* MP_corp()     const {return &mp_corp;}
   inline const alx_model_simulation_physique1* MP_zone_rot() const {return &mp_zone_rot;}
};

#endif
