#include "olfa_comet_fenetre_sdl_opengl.h"

void olfa_comet_fenetre_sdl_opengl::Enregistrer_fenetre_dans_TCL()
{
 s_tmp  = "set ";
 s_tmp += olfa_comet::Nom_usage();
 s_tmp += " ";
 s_tmp += itoa((int)(void*)this, c_tmp, 10);
 strcpy(c_tmp, s_tmp.Texte());
 olfa_comet::Interpreteur()->Evaluer(c_tmp);
}

olfa_comet_fenetre_sdl_opengl::olfa_comet_fenetre_sdl_opengl( char *nom
                                //, cogitant::Environment *e
                                , alx_interpreteur_tcl  *interp
                                , alx_fenetre_opengl *f
                                , alx_interface_sdl_opengl_1 *interface_m //alx_simulateur_points_contact *sim_contact
                                , alx_prerendeur *p
                                , const double nb_pix_par_unite
                                , HDC hdc, HGLRC hglrc
                                , ALXGL_TYPE_RENDU type_rendu) : alx_noeud_fenetre_sdl_opengl(f, interface_m, p, nb_pix_par_unite, hdc, hglrc, type_rendu)
                                                               , olfa_comet(nom, /*e,*/ interp)
{
 Enregistrer_fenetre_dans_TCL();
}

olfa_comet_fenetre_sdl_opengl::olfa_comet_fenetre_sdl_opengl( char *nom
                                //, cogitant::Environment *e
                                , alx_interpreteur_tcl  *interp
                                , alx_fenetre_opengl *f
                                , alx_interface_sdl_opengl_1 *interface_m //alx_simulateur_points_contact *sim_contact
                                , alx_prerendeur *p
                                , const double nb_pix_par_unite ) : alx_noeud_fenetre_sdl_opengl(f, interface_m, p, nb_pix_par_unite)
                                                                  , olfa_comet(nom, /*e,*/ interp)
{
 Enregistrer_fenetre_dans_TCL();
}

//#include "..\SWIG_TCL\alx_noeud_fenetre_sdl_opengl_wrap.cxx"
 