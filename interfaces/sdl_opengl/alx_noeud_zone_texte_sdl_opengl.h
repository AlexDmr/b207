#ifndef __ALX_NOEUD_ZONE_TEXTE_SDL_OPENGL_H__
#define __ALX_NOEUD_ZONE_TEXTE_SDL_OPENGL_H__

#include "../../physique/alx_classes_bases.h"
#include "../classiques/alx_zone_texte_opengl.h"
#include "../alx_noeud_scene.h"

#include "../fonctions_serialisations.cpp"

class alx_noeud_zone_texte_sdl_opengl : public alx_noeud_scene
{private:
   alx_zone_texte_opengl *zone_gl;
   CFSD<alx_noeud_zone_texte_sdl_opengl> classe_serialisation;

   void init_ptr();

   mutable alx_chaine_char serialisation_type_zt   , serialisation_differences_type_zt
                         , serialisation_contenu_zt, serialisation_differences_contenu_zt
                         , nouv_serialisation_type , seri_diff_type;
 protected:
   inline alx_chaine_char* Texte() {return zone_gl->Texte();}
   alx_point2D pt_tmp_ztxt;

 public :
   alx_noeud_zone_texte_sdl_opengl( const double tx, const double ty
                                  , const double taille_lettres
                                  , gl_fonte *fonte
                                  , alx_simulateur_dispositif_saisie *sds );
   alx_noeud_zone_texte_sdl_opengl( alx_zone_texte_opengl *zgl );

   virtual const char* Real_class_cmd() {return "Void_vers_zone_texte";}

   inline alx_zone_texte_opengl *Zone_texte_gl()     {return zone_gl;}
   inline alx_classe_base_liant *Liant()             {return (alx_classe_base_liant*)zone_gl;}
   inline const alx_chaine_char& Chaine_char() const {return zone_gl->Chaine_char();}
   inline void Maj_texte(const char *t)              {zone_gl->Maj_texte(t); A_changer(true);Maj_texture(true);}
   inline void Maj_texte(const alx_chaine_char &t)   {zone_gl->Maj_texte(t); A_changer(true);Maj_texture(true);}
   inline void Ecrire(const char *t)                 {zone_gl->Ecrire(t); A_changer(true);Maj_texture(true);}
   inline void Ecrire(const alx_chaine_char &t)      {zone_gl->Ecrire(t); A_changer(true);Maj_texture(true);}
   inline const char* TEXTE() const                  {return zone_gl->Texte()->Texte();}
   inline void TEXTE(const char *T)                  {zone_gl->Maj_texte(T);}

  //____________________________________________________________________________
   const bool Seri_diff_Texte_a_ete_change(const alx_chaine_char **) const;

  //____________________________________________________________________________
   inline virtual void A_changer(const bool b) {zone_gl->A_changer(b); alx_noeud_scene::A_changer(b);}
   inline virtual const bool A_changer() const {alx_noeud_scene::A_changer( zone_gl->A_changer() );
                                                return alx_noeud_scene::A_changer();
                                               }

  //____________________________________________________________________________
   inline virtual void Calculer_boites() {alx_noeud_zone_texte_sdl_opengl::Calculer_boite_fils();
                                          alx_noeud_zone_texte_sdl_opengl::Calculer_boite_noeud();}
   virtual void Calculer_boite_noeud();

  //____________________________________________________________________________
   //virtual void PreRendre();
   virtual void Dessin_noeud ();
  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   virtual info_du_contenant* Noeud_contient(const alx_point2D &point, int action);

  inline alx_model_simulation_physique1* Mp_corp() {return zone_gl->Mp_corp();}

  inline void Optimiser_dimensions()          {zone_gl->Optimiser_dimensions(); A_changer(true); Maj_texture(true);}
  inline void Calculer_dimensions_optimales() {zone_gl->Calculer_dimensions_optimales();}
  inline const double Optimal_tx() const {return zone_gl->Optimal_tx();}
  inline void Optimal_tx(const double v) {zone_gl->Optimal_tx(v);}
  inline const double Optimal_ty() const {return zone_gl->Optimal_ty();}
  inline void Optimal_ty(const double v) {zone_gl->Optimal_ty(v);}

  inline const double Hauteur() const {return zone_gl->Hauteur();}
  inline const double Largeur() const {return zone_gl->Largeur();}
  inline const double Zoom   () const {return zone_gl->Zoom();}
  inline void   Hauteur(const double h) {zone_gl->Hauteur(h); A_changer(true);Maj_texture(true);}
  inline void   Largeur(const double l) {zone_gl->Largeur(l); A_changer(true);Maj_texture(true);}
  inline void   Zoom   (const double z) {zone_gl->Zoom(z);    A_changer(true);Maj_texture(true);}

 // Les méthodes:
  inline void Afficher_fond(const bool b) {zone_gl->Afficher_fond(b);}
  inline const bool Afficher_fond() const {return zone_gl->Afficher_fond();}

  inline void Editable(const bool e) {zone_gl->Editable(e);A_changer(true);Maj_texture(true);}
  inline const bool Editable() const {return zone_gl->Editable();}
  inline void Couleur_texte_V(const double *c) {zone_gl->Couleur_texte(c);A_changer(true);Maj_texture(true);}
  inline void Couleur_texte(const double r, const double v, const double b, const double a) {zone_gl->Couleur_texte(r,v,b,a);A_changer(true);Maj_texture(true);}
  inline void Couleur_texte(const unsigned int canal, const double v) {zone_gl->Couleur_texte(canal, v);A_changer(true);Maj_texture(true);}
  inline const double Couleur_texte(const unsigned int canal) const {return zone_gl->Couleur_texte(canal);}
  inline const double* Couleur_texte() const {return zone_gl->Couleur_texte();}

  inline void Couleur_curseur(const double r, const double v, const double b, const double a) {zone_gl->Couleur_curseur(r,v,b,a);A_changer(true);Maj_texture(true);}
  inline void Couleur_curseur(const double *c) {zone_gl->Couleur_curseur(c);A_changer(true);Maj_texture(true);}
  inline const double* Couleur_curseur() const {return zone_gl->Couleur_curseur();}
  void Couleur_fond(const double r, const double v, const double b, const double a);
  void Couleur_fond(const double *c);
  inline const double* Couleur_fond() const {return zone_gl->Couleur_fond();}

  inline const bool Lisser() const {return zone_gl->Lisser();}
  inline void Lisser(const bool b) {zone_gl->Lisser(b); A_changer(true); Maj_texture(true);}
  inline const bool Translucide() const {return translucide;}
  inline void Translucide(const bool b) {translucide = b; A_changer(true); Maj_texture(true);}

  inline void Ligne_de_depart(const int l) {zone_gl->Ligne_de_depart(l); A_changer(true);Maj_texture(true);}
  inline const int Ligne_de_depart() const {return zone_gl->Ligne_de_depart();}

  inline void Position_curseur(const int p) {zone_gl->Position_curseur(p); A_changer(true);Maj_texture(true);}
  inline const int Position_curseur() const {return zone_gl->Position_curseur();}
  inline char* Derniere_phrase() {return zone_gl->Derniere_phrase();}
  inline void Positionner_curseur_a_la_fin() {zone_gl->Positionner_curseur_a_la_fin(); A_changer(true);Maj_texture(true);}

  inline void abonner_a_caractere_tape(alx_methode_rappel *m)     {zone_gl->abonner_a_caractere_tape(m);}
  inline void Informer_que_caractere_tape(int c)                  {zone_gl->Informer_que_caractere_tape(c); A_changer(true);Maj_texture(true);}

  inline void abonner_a_chgmnt_pos_curseur(alx_methode_rappel *m) {zone_gl->abonner_a_chgmnt_pos_curseur(m);}
  inline void Informer_que_pos_curseur_change(int pos)            {zone_gl->Informer_que_pos_curseur_change(pos);}

 // Les surcharges pour la sérialisation
  // La différence _____________________________________________________________
  inline virtual const alx_chaine_char& Serialiser_difference_type() const {return Serialiser_difference_type( Serialiser_difference_contenu() );}
  virtual const alx_chaine_char& Serialiser_difference_type(const alx_chaine_char &diff_contenu) const;
  virtual const alx_chaine_char& Serialiser_difference_contenu() const;
  inline virtual const alx_chaine_char& Serialisation_differences_type()    const {return serialisation_differences_type_zt;}
  inline virtual const alx_chaine_char& Serialisation_differences_contenu() const {return serialisation_differences_contenu_zt;}
  // La sérialisation __________________________________________________________
  virtual const alx_chaine_char& Serialiser_type(const alx_chaine_char &contenu) const;
  virtual const alx_chaine_char& Serialiser_contenu() const;
  virtual const alx_chaine_char& Serialisation_contenu() const {return serialisation_contenu_zt;}
  inline virtual const alx_chaine_char& Serialiser_type() const {return Serialiser_type( Serialiser_contenu() );}
  virtual const alx_chaine_char& Serialisation_type   () const {return serialisation_type_zt;}
  // La désérialisation ________________________________________________________
  inline virtual void Deserialiser_type(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser_type(txt, pos);}
  virtual void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
  inline virtual void Deserialiser(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser(txt, pos);}
  virtual void Deserialiser(const alx_chaine_char &txt, unsigned int &pos);

};

typedef alx_noeud_zone_texte_sdl_opengl* P_alx_noeud_zone_texte_sdl_opengl;
alx_noeud_zone_texte_sdl_opengl* Void_vers_zone_texte(void *p) {return (alx_noeud_zone_texte_sdl_opengl*)p;}

#endif

