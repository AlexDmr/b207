#ifndef __ALX_VISU_IMAGE_CPP_H__
#define __ALX_VISU_IMAGE_CPP_H__

#include "..\physique\alx_classes_bases.h"
#include "..\Comet Olfa\olfa_comet.h"

class alx_visu_image_cpp : public alx_classe_base_liant
{
 private:
   alx_chaine_char nom_image, nom_racine_image;

 public:
   alx_visu_image_cpp( char *nom
                     //, cogitant::Environment *e
                     , alx_interpreteur_tcl  *interp );

   virtual void Charger_image(const alx_chaine_char &nom) {nom_image = nom;}
   const alx_chaine_char Nom_image() const {return nom_image;}

   const alx_chaine_char& Nom_racine_diapo() const {return nom_racine_image;}
   void Nom_racine_diapo(const alx_chaine_char &n) {nom_racine_image = n;}
};

#endif
