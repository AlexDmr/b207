#ifndef __ALX_NOEUD_FICHE_OVALE_ZOOM_SDL_OPENGL_H__
#define __ALX_NOEUD_FICHE_OVALE_ZOOM_SDL_OPENGL_H__

#include "alx_noeud_fiche_ovale_sdl_opengl.h"
#include "alx_noeud_repere_lentille.h"

#define NB_MAX_FILS 32
// const NB_MAX_FILS = 32;
class alx_noeud_fiche_ovale_zoom_sdl_opengl : public alx_noeud_fiche_ovale_sdl_opengl
{private:
   bool rationnalise_foz, afficher_lignes;
   void Rationnaliser_chgmnt_dim(void *param);

 protected:
   const int Trouver_buffer_fils(alx_noeud_scene *n);
   const int Intercaller_noeud_zoom(alx_noeud_scene *e);
   const int Desintercaller_noeud_zoom(alx_noeud_scene *e);
   alx_noeud_scene* buffer_reperes[NB_MAX_FILS][2];
   void Init_buffer_reperes();
   void Maj_buffer_reperes();
   void Rationnaliser_buffer_reperes(void *param);

   alx_noeud_repere_lentille noeud_rep_lentille;
   alx_repere2D_lentille    *rep_lentille;
   bool reallouer_lentille, recalculer_coord_text, pos_tmp;
   double *bordure; unsigned int nb_pt_bord;
   alx_point2D pt_origine, pt_parcours;

 public:
  // Les constructeurs
   alx_noeud_fiche_ovale_zoom_sdl_opengl( const char *titre, const double tx, const double ty
                                        , alx_interface_sdl_opengl_1 *interface_m);
  ~alx_noeud_fiche_ovale_zoom_sdl_opengl();

  virtual const char* Real_class_cmd() {return "Void_vers_fiche_ovale_zoom";}

  // Les méthodes
   virtual void Position_dans_liste_des_fils_changeable(const bool b) {pos_tmp = b;}
   virtual const bool Position_dans_liste_des_fils_changeable() const {return pos_tmp;}
   virtual info_du_contenant* Noeud_contient(const alx_point2D &, int action);
   virtual void Dessin_noeud();
   virtual info_du_contenant* Contenu_dans_fils(alx_point2D &pt, int action);
   virtual void PreRendre();

  // Les méthodes spécifiques
   inline void Afficher_lignes(const bool b) {afficher_lignes = b;}
   inline const bool Afficher_lignes() const {return afficher_lignes;}

   // Les surchgarges de la classe alx_arbre
   virtual bool A_pour_fils(alx_noeud_scene *e);
   virtual void Ajouter_fils_au_debut(alx_noeud_scene *e);
   virtual void Ajouter_fils_a_la_fin(alx_noeud_scene *e);
   virtual void Ajouter_fils_replique(alx_noeud_scene *e);
   virtual void Ajouter_fils(alx_noeud_scene *e);
   virtual void Ajouter_fils_avant(alx_noeud_scene *noeud, alx_noeud_scene *noeud_fils);
   virtual void Ajouter_fils(alx_noeud_scene *e, alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it);
   virtual void Retirer_fils(alx_noeud_scene *e);
   virtual void Retirer_fils(alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it);
   virtual void Vider_fils();

  // Accès à la lentille
   void maj_lentille();
   inline alx_noeud_repere_lentille& Noeud_rep_lentille() {return noeud_rep_lentille;}
   inline alx_repere2D_lentille*     Rep_lentille()       {return rep_lentille;}
};

typedef alx_noeud_fiche_ovale_zoom_sdl_opengl* P_alx_noeud_fiche_ovale_zoom_sdl_opengl;
alx_noeud_fiche_ovale_zoom_sdl_opengl* Void_vers_fiche_ovale_zoom(void *p) {return (alx_noeud_fiche_ovale_zoom_sdl_opengl*)p;}

#endif

