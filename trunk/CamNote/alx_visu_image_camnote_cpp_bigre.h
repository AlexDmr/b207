#ifndef __ALX_VISU_IMAGE_CAMNOTE_CPP_BIGRE_H__
#define __ALX_VISU_IMAGE_CAMNOTE_CPP_BIGRE_H__

#include "..\interfaces\interface.h"
#include "alx_visu_image_camnote_cpp.h"
#include "..\Comet Olfa\Creation_objet_dans_tcl.h"

class alx_visu_image_camnote_cpp_bigre : public alx_visu_image_camnote_cpp, public olfa_comet, public alx_noeud_fiche_sdl_opengl //comet_fenetre_rect
{
 private:
   char nom_racine[256]; int numero_courant;
   param_translucidite p_translucidite;
   alx_interface_sdl_opengl_1 *interface_mere;

   alx_noeud_image_sdl_opengl *diapo, *visu_cam;
   alx_noeud_Barriere_booleenne        barriere_avec_pixmiroirs, barriere_sans_pixmiroirs;
   alx_noeud_scene                     etirement_visu_cam;
   alx_noeud_transformations_sdl_opengl<translucidite, depile_bit_de_controle> noeud_translucidite;

   void Rationnaliser_fenetre_diapo(void *param);

 public :
   alx_visu_image_camnote_cpp_bigre( char *nom
                                 //, cogitant::Environment *e
                                 , alx_interpreteur_tcl  *interp
                                 , alx_interface_sdl_opengl_1 *i_mere
                                 , alx_noeud_scene *n_pere
                                 , const INFOS_TEXTURE *text_cam
                                 , const unsigned int nb_octets_par_pix = 4
                                 , const int ordre_couleur = GL_BGRA);

   virtual void Charger_image(const alx_chaine_char &nom);
   virtual void Activation_pix_mirroir(const bool b);
   inline virtual const bool Activation_pix_mirroir() const {return alx_visu_image_camnote_cpp::Activation_pix_mirroir();}
   virtual void Intensite_pixels_miroirs(const float i);
   inline virtual const float Intensite_pixels_miroirs() const {return alx_visu_image_camnote_cpp::Intensite_pixels_miroirs();}

   inline alx_noeud_fiche_sdl_opengl* Noeud_fenetre_sdl_opengl() {return (alx_noeud_fiche_sdl_opengl*)this;}
   inline void Lisser(const bool b) {alx_noeud_fiche_sdl_opengl::Lisser(b); diapo->Lisser(b); visu_cam->Lisser(b);}
   inline const bool Lisser()       {return alx_noeud_fiche_sdl_opengl::Lisser();}
};

#endif
