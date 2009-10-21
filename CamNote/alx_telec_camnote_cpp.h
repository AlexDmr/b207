#ifndef __ALX_TELEC_CAMNOTE_CPP_H__
#define __ALX_TELEC_CAMNOTE_CPP_H__

#include "alx_telec_cpp.h"
#include "..\physique\math_alex.cpp"

// Evennements
// Changement de numéro :
//   X : adresse d'un tableau de 2 unsigned int [ancien num, nouveau num]
// Changement d'état des pixels mirroirs :
//   X : Une adresse de booléen indicant si les pixels mirroirs sont actifs.
// Changement d'intensite des pixels mirroirs :
//   X : adresse d'un flotant indicant l'intensité courante.
//______________________________________________________________________________
// Rationnalisation :
// 0 : Changement de numero
//   X : ancien numéro
//     X : nouveau numéro
// 1 : Changement portant sur les pixels mirroirs
//   0 : Nouvel état d'activation
//     X : une adresse de booléen indicant si les pixels mirroirs sont actifs.
//   1 : Nouvelle intensité
//     X : un flotant indicant l'intensité courante.
// 1 : Changement de nom racine
//   X : un pointeur sur une std::string
class alx_telec_camnote_cpp : public alx_telec_cpp
{
 private:
   alx_chaine_char nom_de_transparent;
   float intensite_pixels_miroirs;
   bool activation_pix_mirroir;
   Liste_de_rappel L_rappel_activation_pix_mirroir
                 , L_rappel_intensite_pix_mirroir
                 , L_rappel_nom_transparent;
   void init();
   bool changement_activation_pix_mirroirs
      , changement_intensite_pix_mirroirs
      , changement_nom_transparent;
 protected:
   inline void Changement_activation_pix_mirroirs()
    {if(changement_activation_pix_mirroirs) return;
     changement_activation_pix_mirroirs = true;
     L_rappel_activation_pix_mirroir.Rappeler(&activation_pix_mirroir);
     alx_classe_base_model::nb_infos_comp = 3;
     alx_classe_base_model::infos_comp[0] = 1;
     alx_classe_base_model::infos_comp[1] = 0;
     alx_classe_base_model::infos_comp[2] = (int)(void*)(&activation_pix_mirroir);
     alx_classe_base_model::Oyez_oyez();
     changement_activation_pix_mirroirs = false;}
   inline void Changement_intensite_pix_mirroirs()
    {if(changement_intensite_pix_mirroirs) return;
     changement_intensite_pix_mirroirs = true;
     L_rappel_intensite_pix_mirroir.Rappeler(&intensite_pixels_miroirs);
     alx_classe_base_model::nb_infos_comp = 3;
     alx_classe_base_model::infos_comp[0] = 1;
     alx_classe_base_model::infos_comp[1] = 1;
     alx_classe_base_model::infos_comp[2] = (int)(void*)(&intensite_pixels_miroirs);
     alx_classe_base_model::Oyez_oyez();
     changement_intensite_pix_mirroirs = false;}
   inline void Changement_nom_transparent()
    {if(changement_nom_transparent) return;
     changement_nom_transparent = true;
     L_rappel_nom_transparent.Rappeler(&nom_de_transparent);
     alx_classe_base_model::nb_infos_comp = 2;
     alx_classe_base_model::infos_comp[0] = 1;
     alx_classe_base_model::infos_comp[1] = (int)(void*)(&nom_de_transparent);
     alx_classe_base_model::Oyez_oyez();
     changement_nom_transparent = false;}

 public:
   alx_telec_camnote_cpp( char *nom
                        //, cogitant::Environment *e
                        , alx_interpreteur_tcl  *interp ) : alx_telec_cpp(nom, /*e,*/ interp)
                                                          , changement_activation_pix_mirroirs(false)
                                                          , changement_intensite_pix_mirroirs(false)
                                                          , changement_nom_transparent(false) {}

   inline void abonner_a_activation_pix_mirroir(alx_methode_rappel *m) {L_rappel_activation_pix_mirroir.Ajouter_methode_a_la_fin(m);}
   inline virtual void Activation_pix_mirroir(const bool b) {activation_pix_mirroir = b; Changement_activation_pix_mirroirs();}
   inline const bool Activation_pix_mirroir() const {return activation_pix_mirroir;}

   inline void abonner_a_intensite_pix_mirroir(alx_methode_rappel *m) {L_rappel_intensite_pix_mirroir.Ajouter_methode_a_la_fin(m);}
   inline virtual void Intensite_pixels_miroirs(const float i)
    {intensite_pixels_miroirs = Amax( (float)0
                                    , Amin((float)1, i) );
     Changement_intensite_pix_mirroirs();
    }
   inline const float Intensite_pixels_miroirs() const {return intensite_pixels_miroirs;}

   inline void abonner_a_nom_transparent(alx_methode_rappel *m) {L_rappel_nom_transparent.Ajouter_methode_a_la_fin(m);}

   inline void Nom_de_transparent(const alx_chaine_char& n) {nom_de_transparent = n; Changement_nom_transparent();}
   inline const alx_chaine_char& Nom_de_transparent() const {return nom_de_transparent;}
};

#endif
