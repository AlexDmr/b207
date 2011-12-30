#ifndef __ALX_NOEUD_ZONE_RENDU_SDL_OPENGL_H__
#define __ALX_NOEUD_ZONE_RENDU_SDL_OPENGL_H__

//#include "..\classiques\alx_fiche_opengl.h"
//#include "../../physique/alx_simulateur_points_contact.h"
//#include "../../utilitaires/alx_arbre.h"
#include "..\alx_noeud_scene.h"
#include "..\..\opengl\alx_texture_rendu.h"
#include "..\..\opengl\config_opengl.h"

class alx_noeud_zone_rendu_sdl_opengl : public alx_noeud_scene
{protected:
   alx_noeud_scene *noeud_a_rendre;
   INFOS_TEXTURE *infos_texture_tempon; // Un tableau d'infos, pour les textures zoomées
   bool texture_deja_cree, tempon_deja_cree
      , top_classos, effacer_fond, triangle_control;
   ALXGL_TYPE_RENDU type_rendu;
   GLuint id_text, id_text_tempon;
   HDC hdc; HGLRC hglrc;
   alx_texture_rendu texture_rendu_fiche;
   double nb_pixels_par_unite
        , nb_unite_x, nb_unite_y;
   double angle_camera, couleur[4];
   bool texture_recree;

   int angle;
   unsigned int nb_zooms;
   alx_prerendeur *prerendeur_local
                , *prerendeur_pour_la_zone;

   inline void maj_texture_rendu();
   inline void maj_texture_tempon();
   inline void maj();
   void init();
 public :
   alx_noeud_zone_rendu_sdl_opengl( alx_prerendeur *prerendeur_zone
                                  , double nb_pixels_par_unite
                                  , HDC hdc, HGLRC hglrc
                                  , ALXGL_TYPE_RENDU type_rendu );
  ~alx_noeud_zone_rendu_sdl_opengl();

   virtual const char* Real_class_cmd() {return "Void_vers_zone_rendu";}

   inline const bool Texture_recree() const {return texture_recree;}

  // La gestion de la précision du rendu, en pixel et unités.
   inline const double Nb_pixels_par_unite() const {return nb_pixels_par_unite;}
   inline const double Nb_unite_x()          const {return nb_unite_x;}
   inline const double Nb_unite_y()          const {return nb_unite_y;}
   inline void   Nb_pixels_par_unite(const double n) {nb_pixels_par_unite = n; maj_texture_tempon();}
   inline void   Nb_unite_x         (const double n) {nb_unite_x = n;}
   inline void   Nb_unite_y         (const double n) {nb_unite_y = n;}

  // Le nombre de récursion pour faire des zooms
   inline const unsigned int Nb_zooms() const {return nb_zooms;}
   void Nb_zooms(const unsigned int n);

  // La couleur de fond
   inline const double* Couleur() const {return couleur;}
   inline const double Composante_couleur(const unsigned int i)
    {return couleur[i];}
   inline const void Composante_couleur(const unsigned int i, const double v)
    {couleur[i] = v;}
   inline void Couleur(const double r, const double v, const double b, const double a)
    {couleur[0]=r; couleur[1]=v; couleur[2]=b; couleur[3]=a;}
   inline void Couleur(const double *c)
    {for(unsigned int i=0;i<4;i++) couleur[i] = c[i];}
   inline double * P_couleur(const unsigned int i) {return &(couleur[i]);}

  // Le noeud à rendre
   inline const alx_noeud_scene* Noeud_a_rendre() const {return noeud_a_rendre;}
   inline void Noeud_a_rendre(alx_noeud_scene* n) {noeud_a_rendre = n;}

   inline virtual alx_prerendeur* Prerendeur_local() {return prerendeur_local;}
   inline virtual alx_prerendeur* Prerendeur_pour_la_zone() {return prerendeur_pour_la_zone;}
   inline virtual void Prerendeur_pour_la_zone(alx_prerendeur *p)
    {
     //prerendeur_pour_la_zone->Se_decharger_de( (alx_noeud_scene*)this );
     prerendeur_pour_la_zone = p;
     //p->Prendre_en_charge( (alx_noeud_scene*)this, 1, ACTION_DU_RENDU_rendre_toujours, 'p');
    }
   inline INFOS_TEXTURE* Infos_texture_tempon(const unsigned int i = 0) {return &(infos_texture_tempon[i]);}

   inline void Effacer_fond(bool b) {effacer_fond = b;}
   inline const bool Effacer_fond() const {return effacer_fond;}
   inline void Triangle_control(bool b) {triangle_control = b;}
   inline const bool Triangle_control() const {return triangle_control;}

   virtual void Afficher();
   virtual void PreRendre();

   inline void Top_classos(const bool b) {top_classos = b;}
   inline const bool Top_classos() const {return top_classos;}
};

typedef alx_noeud_zone_rendu_sdl_opengl* P_alx_noeud_zone_rendu_sdl_opengl;
inline alx_noeud_zone_rendu_sdl_opengl* Void_vers_zone_rendu(void *p) {return (alx_noeud_zone_rendu_sdl_opengl*)p;}

#endif
