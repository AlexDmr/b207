#ifndef __ALX_NOEUD_ZONE_TEXTE_SDL_OPENGL_H__
#define __ALX_NOEUD_ZONE_TEXTE_SDL_OPENGL_H__

#include "alx_noeud_zone_texte_sdl_opengl.h"

      
class alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl : public alx_noeud_scene
{
 private:
   alx_zone_texte_opengl *zone_gl;
   
 public :
   alx_noeud_zone_texte_sdl_opengl( alx_zone_texte_opengl *zgl );

   void Afficher ();
  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   info_du_contenant* Contient(alx_point2D &point, int action);
};

#endif

 