#ifndef __ALX_NOEUD_PLASTIC_CLOCK_H__
#define __ALX_NOEUD_PLASTIC_CLOCK_H__

#include "..\interfaces\sdl_opengl\alx_noeud_fiche_sdl_opengl.h"
#include "..\interfaces\sdl_opengl\alx_noeud_fiche_ovale_sdl_opengl.h"

class alx_plastic_clock : public alx_noeud_scene
{private:
   alx_noeud_fiche_ovale_sdl_opengl *horloge_paris
                                  , *horloge_new_york;
   alx_noeud_fiche_sdl_opengl *fen_plastic_clock;

 public :
  // Les constructeurs
   alx_plastic_clock(const double tx, const double ty, alx_interface_sdl_opengl_1 *interface_m);
  ~alx_plastic_clock();

  // Les méthodes
};

#endif
 