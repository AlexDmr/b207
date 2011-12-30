#ifndef __OLFA_COMET_FENETRE_SDL_OPENGL_H__
#define __OLFA_COMET_FENETRE_SDL_OPENGL_H__

#include "..\interfaces\sdl_opengl\alx_noeud_fenetre_sdl_opengl.h"
#include "..\Comet Olfa\olfa_comet.h"

class olfa_comet_fenetre_sdl_opengl : public alx_noeud_fenetre_sdl_opengl, public olfa_comet
{
 private:
   char c_tmp[1024];
   alx_chaine_char s_tmp;

   void Enregistrer_fenetre_dans_TCL();
   
 public :
   olfa_comet_fenetre_sdl_opengl( char *nom
                                //, cogitant::Environment *e
                                , alx_interpreteur_tcl  *interp
                                , alx_fenetre_opengl *f
                                , alx_interface_sdl_opengl_1 *interface_m //alx_simulateur_points_contact *sim_contact
                                , alx_prerendeur *p
                                , const double nb_pix_par_unite
                                , HDC hdc, HGLRC hglrc
                                , ALXGL_TYPE_RENDU type_rendu);
   olfa_comet_fenetre_sdl_opengl( char *nom
                                //, cogitant::Environment *e
                                , alx_interpreteur_tcl  *interp
                                , alx_fenetre_opengl *f
                                , alx_interface_sdl_opengl_1 *interface_m //alx_simulateur_points_contact *sim_contact
                                , alx_prerendeur *p
                                , const double nb_pix_par_unite );
};

#endif
 