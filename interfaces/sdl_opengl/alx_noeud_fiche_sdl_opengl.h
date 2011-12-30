#ifndef __ALX_NOEUD_FICHE_SDL_OPENGL_H__
#define __ALX_NOEUD_FICHE_SDL_OPENGL_H__

class alx_fiche;
class alx_fiche_opengl;
#include "alx_noeud_fenetre_sdl_opengl.h"
#include "../../physique/alx_simulateur_points_contact.h"

/*
//#include "../../utilitaires/alx_arbre.h"
#include "../alx_noeud_scene.h"
#include "../opengl/alx_texture_rendu.h"
#include "alx_noeud_zone_rendu_sdl_opengl.h"
*/


class alx_noeud_fiche_sdl_opengl : public alx_noeud_fenetre_sdl_opengl
{private :
   mutable alx_chaine_char serialisation_type_fr, serialisation_differences_type_fr;

 protected:
   alx_methode_rappel *nf_rappel_chgmnt_dim;
   alx_arbre<alx_model_simulation_physique1*> A_corp
                                            , A_entete
                                            , A_zone_rot
                                            , A_zone_redim[8];
   P_alx_ensemble_contact ens_contact_fiche1
                        , ens_contact_fiche1_redim[8];
   alx_modele_contact *mod_contact_entete, *mod_contact_zone_rot
                    , *mod_contact_corp;

   alx_fiche         *fiche;
   alx_fiche_opengl  *fiche_gl;

   inline void init();
   void Rationnaliser_chgmnt_dim(void *param);

 public :
  // Les constructeurs :
   inline alx_noeud_scene* Noeud_scene() {return (alx_noeud_scene*)(alx_noeud_fenetre_sdl_opengl*)this;}
   alx_noeud_fiche_sdl_opengl( const char *titre, const double tx, const double ty
                             , alx_interface_sdl_opengl_1 *interface_m);
   alx_noeud_fiche_sdl_opengl( alx_fiche_opengl *f_opengl
                             , alx_interface_sdl_opengl_1 *interface_m
                             , alx_prerendeur *prerendeur_de_la_fiche
                             , double nb_pixels_par_unite = 1
                             , HDC hdc = NULL, HGLRC hglrc = NULL
                             , ALXGL_TYPE_RENDU type_rendu = ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE);

   virtual const char* Real_class_cmd() {return "Void_vers_fiche";}

  // Les méthodes surchargées de la classe alx_noeud_scene.
   virtual void PreRendre();
   virtual void Dessin_noeud();
   virtual info_du_contenant* Noeud_contient(const alx_point2D &pt, int action); // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.

   virtual void Titre(const char *t);
   virtual void Titre(const alx_chaine_char &t);
   inline virtual const alx_chaine_char& Titre() const {return titre;}

  // Les méthodes spécifiques
   inline alx_prerendeur* Prerendeur() {return zdr_corp.Prerendeur_local();}

   inline alx_fiche_opengl* Fiche_opengl() {return fiche_gl;}
   inline alx_fiche*        Fiche()        {return fiche;}

  // Gestion des boîtes englobantes
   inline virtual void Calculer_boites() {alx_noeud_fiche_sdl_opengl::Calculer_boite_fils(); alx_noeud_fiche_sdl_opengl::Calculer_boite_noeud();}
   virtual void Calculer_boite_noeud();
   virtual void Calculer_boite_fils();

  // Gestion des modèles de contacts
   inline alx_modele_contact* Mod_contact_corp()     {return mod_contact_corp;}
   inline alx_modele_contact* Mod_contact_entete()   {return mod_contact_entete;}
   inline alx_modele_contact* Mod_contact_zone_rot() {return mod_contact_zone_rot;}

   void abonner_fiche_a( alx_classe_base_liant *liant
                       , int n);

  // Les surcharges pour la sérialisation
   // La différence
   inline virtual const alx_chaine_char& Serialiser_difference_type() const {return Serialiser_difference_type( Serialiser_difference_contenu() );}
   virtual const alx_chaine_char& Serialiser_difference_type(const alx_chaine_char &diff_contenu) const;
   virtual const alx_chaine_char& Serialiser_difference_contenu() const;
   inline virtual const alx_chaine_char& Serialisation_differences_type()    const {return serialisation_differences_type_fr;}
   inline virtual const alx_chaine_char& Serialisation_differences_contenu() const {return Serialisation_differences_contenu();}

   // La sérialisation
   virtual const alx_chaine_char& Serialiser_type(const alx_chaine_char &contenu) const;
   virtual const alx_chaine_char& Serialiser_contenu() const;
   virtual const alx_chaine_char& Serialisation_contenu() const {return alx_noeud_fenetre_sdl_opengl::Serialisation_type();}
   inline virtual const alx_chaine_char& Serialiser_type() const {return Serialiser_type( Serialiser_contenu() );}
   virtual const alx_chaine_char& Serialisation_type   () const {return serialisation_type_fr;}
   // La désérialisation
   inline virtual void Deserialiser_type(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser_type(txt, pos);}
   virtual void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
   inline virtual void Deserialiser(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser(txt, pos);}
   virtual void Deserialiser(const alx_chaine_char &txt, unsigned int &pos);

};

typedef alx_noeud_fiche_sdl_opengl* P_alx_noeud_fiche_sdl_opengl;
inline alx_noeud_fiche_sdl_opengl* Void_vers_fiche(void *p) {return (alx_noeud_fiche_sdl_opengl*)p;}

#endif
