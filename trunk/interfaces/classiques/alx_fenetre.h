#ifndef __ALX_FENETRE_H__
#define __ALX_FENETRE_H__

#include "../../physique/alx_classes_bases.h"
class alx_model_simulation_physique1;
class alx_repere2D;
#include "..\..\physique\alx_point2D.h"

#include "..\..\utilitaires\alx_chaine_char.h"

//#include "../../physique/alx_model_simulation_physique1.h"


// Les codes
// 0 : Changement de dimension
//   0 : En longueur
//     *a : ancienne longueur (double)
//     *n : nouvelle longueur (double)
//   1 : En hauteur
//     *a : ancienne hauteur  (double)
//     *n : nouvelle hauteur  (double)
// 1 : Pression
//   0 : un point
//     * : alx_point2D*

class alx_fenetre : public alx_classe_base_liant
{
 protected:
   Liste_de_rappel L_rappel_chgmt_dimension;
   void init_fenetre();
   double h_corp, l_entete, h_entete, lg, rayon_zone_rot, epaisseur_bordure;
   char nom[256];
   bool bloquer_rationnalisation;

   void Cgmnt_lg(const double a, const double n);
   void Cgmnt_ht(const double a, const double n);
 public :
  alx_repere2D *repere;

  // Les constructeurs:
   alx_fenetre();
   alx_fenetre( const char *nom
              , const double longueur, const double hauteur );
   alx_fenetre( const char *nom
              , const double longueur, const double hauteur
              , const double l_entete, const double h_entete
              , const double rayon_zone_rotation );

  // Les méthodes:
   void Contact_avec_point(alx_point2D *pt);

   virtual inline void Appliquer_maj()             {}
   inline unsigned char*  Nom()                    {return (unsigned char*)nom;}
   inline const double Longueur_corp()       const {return lg;}
   inline const double Hauteur_corp()        const {return h_corp;}
   inline const double Longueur_entete()     const {return l_entete;}
   inline const double Hauteur_entete()      const {return h_entete;}
   inline const double Rayon_zone_rotation() const {return rayon_zone_rot;}
   inline const double Epaisseur_bordure()   const {return epaisseur_bordure;}

   inline virtual void Nom(const char *n)                  {strcpy_s(nom, n);/*strcpy(nom, n);*/}
   inline virtual void Longueur_corp(const double v)
    {double tmp = lg;
     lg = v;
     Cgmnt_lg(lg    , v);
     Longueur_entete(v);}
   inline virtual void Hauteur_corp(const double v)
    {double tmp = h_corp;
     h_corp = v;
     Cgmnt_ht(h_corp, v); }
   inline virtual void Longueur_entete(const double v)     {l_entete          = v;}
   inline virtual void Hauteur_entete(const double v)      {h_entete          = v;}
   inline virtual void Rayon_zone_rotation(const double v) {rayon_zone_rot    = v;}
   inline virtual void Epaisseur_bordure(const double v)   {epaisseur_bordure = v;}

   inline virtual alx_model_simulation_physique1* MP_corps() {return NULL;}

   inline void abonner_a_dimension    (alx_methode_rappel *m) {L_rappel_chgmt_dimension.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_dimension(alx_methode_rappel *m) {L_rappel_chgmt_dimension.Retirer_methode(m);}

   inline const alx_model_simulation_physique1* MP_entete()   const {return NULL;}
   inline const alx_model_simulation_physique1* MP_corp()     const {return NULL;}
   inline const alx_model_simulation_physique1* MP_zone_rot() const {return NULL;}

};

#endif
