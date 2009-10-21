#ifndef __ALX_NOEUD_EDITEUR_COMET_H__
#define __ALX_NOEUD_EDITEUR_COMET_H__

#include "../interface.h"
#include "..\..\Comet Olfa\olfa_comet.h"
#include "alx_interface_sdl_opengl_1.h"


class alx_noeud_editeur_olfa_comet : public alx_noeud_scene
{
 private:
   olfa_comet *comet;
   alx_interface_sdl_opengl_1                       *interface_mere;
   alx_zone_texte_opengl                            *z_texte;
   alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl *zone_texte;
   alx_noeud_fiche_sdl_opengl                       *fiche;
   void init_interface();

 public :
  // Constructeurs
   alx_noeud_editeur_olfa_comet() {}
   alx_noeud_editeur_olfa_comet(alx_interface_sdl_opengl_1 *i, olfa_comet *c)
    {Initialiser(i, c);}
  ~alx_noeud_editeur_olfa_comet();

  virtual const char* Void_vers_editeur_olfa_comet() {return "Void_vers_bouton";}

  // Méthodes
   inline void Initialiser(alx_interface_sdl_opengl_1 *i, olfa_comet *c)
    {interface_mere = i; Olfa_comet(c); init_interface();}
  // Accès à la fenêtre de l'éditeur
   inline alx_noeud_fiche_sdl_opengl* Fenetre() {return fiche;}
  // Accès à la zone de texte de l'éditeur
   inline alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl* Editeur() {return zone_texte;}

  // Accès à la comet
   inline olfa_comet* Olfa_comet()              {return comet;}
   inline void        Olfa_comet(olfa_comet *c) {comet = c;}

   // Les fonctions de rappels
   void Rationnaliser_chgmnt_dim_fiche(void *param);

  // Les surcharges de la classe alx_noeud_scene
   void PreRendre();
   //void Afficher();
   //info_du_contenant* Contient(alx_point2D &pt, int action);

};

typedef alx_noeud_editeur_olfa_comet* P_alx_noeud_editeur_olfa_comet;
alx_noeud_editeur_olfa_comet* Void_vers_editeur_olfa_comet(void *p) {return (alx_noeud_editeur_olfa_comet*)p;}

#endif
