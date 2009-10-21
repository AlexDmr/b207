#ifndef __ALX_CHOSES_COMMUNES_CPP__
#define __ALX_CHOSES_COMMUNES_CPP__

#include "choses_communes.h"

alx_image_opengl *image_bouton_dec  = (alx_image_opengl*)NULL
               , *image_bouton_inc  = (alx_image_opengl*)NULL
               , *image_bouton_rect = (alx_image_opengl*)NULL;
alx_materiel *matos = (alx_materiel*)NULL;

alx_prerendeur* prerendeur_defaut = (alx_prerendeur*)NULL;
alx_prerendeur* Prerendeur_defaut() {return prerendeur_defaut;}

alx_materiel* Matos() {return matos;}
void Initialiser_matos()
{if(!matos)
   matos = new alx_materiel(alx_type_materiel_couleur, (char*)NULL);}

void Initialiser_boutons()
{if(!image_bouton_dec)  image_bouton_dec  = new alx_image_opengl("BDec.bmp");
 if(!image_bouton_inc)  image_bouton_inc  = new alx_image_opengl("BInc.bmp");
 if(!image_bouton_rect) image_bouton_rect = new alx_image_opengl("Brectangle.bmp");}

alx_image_opengl* Image_bouton_dec()  {return image_bouton_dec;}
alx_image_opengl* Image_bouton_inc()  {return image_bouton_inc;}
alx_image_opengl* Image_bouton_rect() {return image_bouton_rect;}


#endif
