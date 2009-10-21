#ifndef __ALX_NOEUD_LIGNE_SDL_OPENGL_H__
#define __ALX_NOEUD_LIGNE_SDL_OPENGL_H__

#include "../alx_noeud_scene.h"
#include "../fonctions_serialisations.cpp"
#include "../../GPC/gpc.h"
#include "../../opengl/config_opengl.h"

#include "../fonctions_serialisations.cpp"

#include "tableau_de_double.h"
/*
class alx_noeud_ligne_sdl_opengl;
class alx_noeud_ligne_sdl_opengl : public alx_noeud_scene
{private:
   CFSD<alx_noeud_ligne_sdl_opengl> classe_seri_poly;

   void init();
   void init_ptr();
   mutable alx_chaine_char serialisation_type_ligne   , serialisation_differences_type_ligne
                         , serialisation_contenu_ligne, serialisation_differences_contenu_ligne
                         , nouv_serialisation_type_ligne , seri_diff_type_ligne;
   int num_liste;
   alx_point2D A_pt_tmp;
   static unsigned int *ordre_affichage_poly
                     , nb_elements_ordre_affichage;
   Interpollateur_couleur_polygone *interpollateur
                                 , *interpollateur_perimetre;

 protected:
   double couleur[4];
   unsigned int nb_points, attributs_ligne;

   bool boite_noeud_deja_calculees;

   inline void Marqueur_attributs_ligne(const int m, bool b)
    {if(b) attributs_ligne |= m;
      else attributs_ligne &= ~m;}
   inline void Marqueur_attributs_ligne(const int m) {attributs_ligne = m;}
   inline const int Marqueur_attributs_ligne() const {return attributs_ligne;}
   const double *coords_col;

 public :
  // Les constructeurs__________________________________________________________
   alx_noeud_ligne_sdl_opengl();
  ~alx_noeud_ligne_sdl_opengl();

  // Les méthodes d'accès_______________________________________________________

  // Les méthodes métiers_______________________________________________________

};
*/
#endif
