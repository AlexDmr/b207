#ifndef __ALX_NOEUD_FICHE_ZOOM_SDL_OPENGL_H__
#define __ALX_NOEUD_FICHE_ZOOM_SDL_OPENGL_H__

#include "alx_noeud_fiche_sdl_opengl.h"
#include "alx_noeud_repere_lentille.h"

class alx_noeud_fiche_zoom_sdl_opengl : public alx_noeud_fiche_sdl_opengl
{private:
   void Rationnaliser_chgmnt_dim(void *param);
   bool rationnalise_fz;

 protected:
   alx_noeud_repere_lentille noeud_rep_lentille;
   alx_repere2D_lentille    *rep_lentille;
   bool reallouer_lentille, recalculer_coord_text, pos_tmp;
   void maj_lentille();

 public:
  // Les constructeurs
   alx_noeud_fiche_zoom_sdl_opengl( const char *titre, const double tx, const double ty
                                  , alx_interface_sdl_opengl_1 *interface_m);
  ~alx_noeud_fiche_zoom_sdl_opengl();

  virtual const char* Real_class_cmd() {return "Void_vers_fiche_zoom";}

  // Les méthodes
   virtual void Position_dans_liste_des_fils_changeable(const bool b) {pos_tmp = b;}
   virtual const bool Position_dans_liste_des_fils_changeable() const {return pos_tmp;}
   virtual info_du_contenant* Contient(alx_point2D &, int action);
   virtual void Dessin_noeud();
   virtual void PreRendre();
};

typedef alx_noeud_fiche_zoom_sdl_opengl* P_alx_noeud_fiche_zoom_sdl_opengl;
inline alx_noeud_fiche_zoom_sdl_opengl* Void_vers_fiche_zoom(void *p) {return (alx_noeud_fiche_zoom_sdl_opengl*)p;}

#endif
