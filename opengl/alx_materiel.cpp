#include "alx_materiel.h"
#include <glew.h>

//------------------------------------------------------------------------------
//---------------------------- Les constructeurs -------------------------------
//------------------------------------------------------------------------------
alx_materiel::alx_materiel(alx_type_materiel type_matos, char *nom_texture)
{
 /*this->type_matos = type_matos;
 if(nom_texture)
  {
   texture_cree_en_local = true;
   glGenTextures(1, &id_texture);
   img_texture.maj(nom_texture);

   glBindTexture(GL_TEXTURE_2D, id_texture);
   info_texture.id_texture = id_texture;
   info_texture.tx = puissance_de_2_superieur(img_texture.L(), 64);
   info_texture.ty = puissance_de_2_superieur(img_texture.H(), 64);
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA
               , info_texture.tx, info_texture.ty, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_texture.TEMPON());
  }
  else
   {
    texture_cree_en_local = false;
   }*/
 texture_cree_en_local = false;  
}
//------------------------------------------------------------------------------
alx_materiel::alx_materiel(alx_type_materiel type_matos, unsigned int id)
{texture_cree_en_local = false;
 id_texture = id;}
//------------------------------------------------------------------------------
alx_materiel::~alx_materiel()
{if(texture_cree_en_local) glDeleteTextures(1, &id_texture);}

//------------------------------------------------------------------------------
//----------------------------- Les méthodes -----------------------------------
//------------------------------------------------------------------------------
void alx_materiel::Activer()
{
 switch(type_matos)
  {case alx_type_materiel_couleur:
     glDisable(GL_TEXTURE_2D);
   break;
   default:
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, id_texture);
  }
}


