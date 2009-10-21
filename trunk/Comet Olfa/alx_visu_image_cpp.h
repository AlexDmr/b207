#ifndef __ALX_VISU_IMAGE_CPP_H__
#define __ALX_VISU_IMAGE_CPP_H__

#include <string>
#include "..\physique\alx_classes_bases.h"
#include "..\Comet Olfa\olfa_comet.h"

class alx_visu_image_cpp : public alx_classe_base_liant, public olfa_comet
{
 private:
   std::string nom_image;
   
 public:
   alx_visu_image_cpp();
   alx_visu_image_cpp( char *nom
                     , Environment *e
                     , alx_interpreteur_tcl  *interp );

   virtual void Charger_image(const std::string &nom) {nom_image = nom;}
   const std::string Nom_image() const {return nom_image;}
};

#endif
 