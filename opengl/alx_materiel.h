#ifndef __ALX_MATERIEL_H__
#define __ALX_MATERIEL_H__

//#include "alx_image.h"
#include "config_opengl.h"

typedef enum { alx_type_materiel_couleur
             , alx_type_materiel_texture
             , alx_type_materiel_couleur_et_texture } alx_type_materiel;
class alx_materiel
{
 private:
   alx_type_materiel type_matos;
   //alx_image_32      img_texture;
   INFOS_TEXTURE     info_texture;
   unsigned int      id_texture;
   bool              texture_cree_en_local;
   
 public :
  // Les constructeurs
   alx_materiel(alx_type_materiel type_matos, char *nom_texture);
   alx_materiel(alx_type_materiel type_matos, unsigned int id);
   ~alx_materiel();
   
  // Les méthodes
   void Activer();

   inline unsigned int Id_texture() const {return id_texture;}
   inline void Id_texture(unsigned int i) {id_texture = i;}

   inline const char*  Nom_texture() const {return NULL;}//img_texture.NOM();}

   inline alx_type_materiel Type_materiel()                       {return type_matos;}
   inline void              Type_materiel(alx_type_materiel type) {type_matos = type;}
};

#endif
 