#ifndef __SWIG_WRAPPER_ALX_FICHE_SDL_OPENGL_H__
#define __SWIG_WRAPPER_ALX_FICHE_SDL_OPENGL_H__

#include <winsock.h>
#include "olfa_comet.h"
#include <tcl.h>

class CamNote;
class alx_visu_image_camnote_cpp_bigre;
class alx_telec_camnote_cpp_bigre;
class alx_interface_sdl_opengl_1;
class alx_noeud_fenetre_sdl_opengl;
class alx_noeud_image_sdl_opengl;
class alx_noeud_visualisation_camera;
class alx_noeud_zone_texte_sdl_opengl;

class resultat_creation
{
 public :
   void *ptr;
   const char *reponse_texte;

   resultat_creation(void *p, const char *t) {ptr = p; reponse_texte = t;}
};

//______________________________________________________________________________
//______________________________________________________________________________
void Enregistrer_interface(olfa_comet *comet, alx_interface_sdl_opengl_1 *i);

void Enregistrer_alx_interpreteur_tcl(alx_interpreteur_tcl *interp_pere, alx_interpreteur_tcl *interp, const alx_chaine_char &nom);
void Enregistrer_noeud_bigre(alx_interpreteur_tcl *interp, const alx_noeud_scene *noeud, const alx_chaine_char &nom);
void Enregistrer_fenetre_bigre(alx_interpreteur_tcl *interp, const alx_noeud_fenetre_sdl_opengl *fen, const alx_chaine_char &nom);
void Enregistrer_image_bigre(alx_interpreteur_tcl *interp, const alx_noeud_image_sdl_opengl *img, const alx_chaine_char &nom);
void Enregistrer_image_opengl(alx_interpreteur_tcl *interp, const alx_image_opengl *img, const alx_chaine_char &nom);
void Enregistrer_zone_texte_bigre(alx_interpreteur_tcl *interp, const alx_noeud_zone_texte_sdl_opengl *zt, const alx_chaine_char &nom);
void Enregistrer_visualisation_camera(alx_interpreteur_tcl *interp, const alx_noeud_visualisation_camera *cam, const alx_chaine_char &nom);
//______________________________________________________________________________
void Initialisation_commandes_tcl_interfaces(alx_interpreteur_tcl *interp);
void Enregistrer(alx_interpreteur_tcl *interp, const char *nom_classe, const char *nom_var, const char *var_interne, const char *methode_acces);
inline void Enregistrer(alx_interpreteur_tcl *interp, olfa_comet &c, const char *methode_acces)
 {Enregistrer(interp, c.Classe_comet().Texte(), c.Nom_usage().Texte(), c.Nom_interne().Texte(), methode_acces);}

void Enregistrer_accesseur( alx_interpreteur_tcl *interp
                          , const char *nom_classe
                          , const char *nom_var
                          , const char *nom_object_accede
                          , const char *nom_methode_acces);
void Enregistrer_CamNote     (olfa_comet *comet, CamNote *camnote);
void Enregistrer_CamNote_Visu(olfa_comet *comet, alx_visu_image_camnote_cpp_bigre *visu);
void Enregistrer_CamNote_Tele(olfa_comet *comet, alx_telec_camnote_cpp_bigre *tele);

class comet_noeud_scene : public olfa_comet, public alx_noeud_scene
{
 protected:

 public :
   comet_noeud_scene( const char *nom_usage);
                    //, cogitant::Environment *e);

   inline alx_noeud_scene* Alx_noeud_scene() {return (alx_noeud_scene*)this;}
   inline olfa_comet*      Olfa_comet()      {return (olfa_comet*)this;}
   virtual void Afficher()
     {/*if( Interpreteur() )
       {Proposer_evolutions   ();
        Analyser_propositions ();
        Effectuer_propositions(); }*/
      alx_noeud_scene::Afficher();}
};

#endif
