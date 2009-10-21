#ifndef __ALX_NOEUD_FICHE_OVALE_SDL_OPENGL_H__
#define __ALX_NOEUD_FICHE_OVALE_SDL_OPENGL_H__

#include "alx_noeud_fenetre_sdl_opengl.h"
#include "../../physique/alx_simulateur_points_contact.h"

class alx_fiche_ovale;
class alx_fiche_ovale_opengl;
class alx_interface_sdl_opengl_1;

/*
//#include "../../utilitaires/alx_arbre.h"
#include "../alx_noeud_scene.h"
#include "../opengl/alx_texture_rendu.h"
#include "alx_noeud_zone_rendu_sdl_opengl.h"
*/


class alx_noeud_fiche_ovale_sdl_opengl : public alx_noeud_fenetre_sdl_opengl, public alx_classe_base_liant
{private:
   bool rationnalise_fo;
   alx_point2D pt_pour_origine_interne, pt_tmp_boite;

 protected:
   alx_methode_rappel *nfo_rappel_chgmnt_dim;
   double coord_x_prcdt, coord_y_prcdt;
   bool svg_ne_pas_pre_rendre;
   mutable alx_point2D origine_logique;
   alx_point2D origine_interne_logique;
   double lg_deb, ht_deb;
   alx_arbre<alx_model_simulation_physique1*> A_corp
                                            , A_entete
                                            , A_zone_rot
                                            , A_zone_redim;
   P_alx_ensemble_contact ens_contact_fiche1
                        , ens_contact_fiche1_redim;
   alx_modele_contact *mod_contact_entete, *mod_contact_zone_rot, *mod_contact_zone_redim
                    , *mod_contact_corp;

   alx_fiche_ovale         *fiche;
   alx_fiche_ovale_opengl  *fiche_gl;

   inline void init();
   void Rationnaliser_chgmnt_dim(void *param);

 public :
   inline alx_noeud_scene* Noeud_scene() {return (alx_noeud_scene*)(alx_noeud_fenetre_sdl_opengl*)this;}
   alx_noeud_fiche_ovale_sdl_opengl( const char *nom, const double tx, const double ty
                                   , alx_interface_sdl_opengl_1 *interface_m);
   alx_noeud_fiche_ovale_sdl_opengl( alx_fiche_ovale_opengl *f_opengl
                             , alx_interface_sdl_opengl_1 *interface_m
                             , alx_prerendeur *prerendeur_de_la_fiche
                             , double nb_pixels_par_unite = 1
                             , HDC hdc = NULL, HGLRC hglrc = NULL
                             , ALXGL_TYPE_RENDU type_rendu = ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE);

   virtual const char* Real_class_cmd() {return "Void_vers_fiche_ovale";}

  // Gestion des boîtes englobantes
   inline virtual void Calculer_boites() {alx_noeud_fiche_ovale_sdl_opengl::Calculer_boite_fils(); alx_noeud_fiche_ovale_sdl_opengl::Calculer_boite_noeud();}
   virtual void Calculer_boite_noeud();
   virtual void Calculer_boite_fils();

   virtual void PreRendre();
   virtual void Dessin_noeud();
  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   virtual info_du_contenant* Noeud_contient(const alx_point2D &, int action);

   inline alx_prerendeur* Prerendeur() {return zdr_corp.Prerendeur_local();} //Prerendeur_fiche();}

   inline alx_modele_contact* Mod_contact_corp()   {return mod_contact_corp;}
   inline alx_modele_contact* Mod_contact_entete()   {return mod_contact_entete;}
   inline alx_modele_contact* Mod_contact_zone_rot() {return mod_contact_zone_rot;}

   inline bool Rendre_fond() const     {return zdr_corp.Effacer_fond();}
   inline void Rendre_fond(bool b)     {zdr_corp.Effacer_fond(b);}

   inline alx_fiche_ovale_opengl* Fiche_opengl() {return fiche_gl;}
   inline alx_fiche_ovale*        Fiche()        {return fiche;}

   inline void Top_classos(bool b) {zdr_corp.Top_classos(b);}

   void abonner_fiche_a( alx_classe_base_liant *liant, int n);

   void rationnaliser(int num);

   virtual void Titre(const char *t);
   virtual void Titre(const alx_chaine_char &t);
   inline virtual const alx_chaine_char& Titre() const {return titre;}

  // La position du coin en bas à gauche (/ au repère père)
   virtual const alx_point2D& Position_bg() const;
   inline virtual void Position_bg(const alx_point2D &p);
   inline virtual void Position_bg(const double x, const double y);
  // L'origine (/ au repère père)
   inline virtual const alx_point2D& Origine() const;
   inline virtual void Origine(const alx_point2D &pt);
   inline virtual void Origine(const double x, const double y);
  // L'origine interne (/ au repère de la fenètre)
   inline virtual const alx_point2D& Origine_du_contenu() const {return origine_interne_logique;}
   virtual void Origine_du_contenu(const alx_point2D &pt);
   inline virtual void Origine_du_contenu(const double x, const double y) {pt_pour_origine_interne.maj(x, y); Origine_du_contenu(pt_pour_origine_interne);}

  // Les surcharges de la classe alx_noeud_fenetre
   virtual const double Longueur_totale() const;
};

typedef alx_noeud_fiche_ovale_sdl_opengl* P_alx_noeud_fiche_ovale_sdl_opengl;
alx_noeud_fiche_ovale_sdl_opengl* Void_vers_fiche_ovale(void *p) {return (alx_noeud_fiche_ovale_sdl_opengl*)p;}

#endif

