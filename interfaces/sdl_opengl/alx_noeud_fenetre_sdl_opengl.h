#ifndef __ALX_NOEUD_FENETRE_SDL_OPENGL_H__
#define __ALX_NOEUD_FENETRE_SDL_OPENGL_H__

class alx_fenetre;
class alx_fenetre_opengl;
class alx_interface_sdl_opengl_1;
class alx_simulateur_points_contact;

//#include <winsock.h>
#include "..\alx_noeud_scene.h"
//#include "..\opengl\alx_texture_rendu.h"
#include "alx_noeud_zone_rendu_sdl_opengl.h"
#include "alx_noeud_entete_sdl_opengl.h"
#include "alx_noeud_transformations_sdl_opengl.h"

// Définission des masques bianires pour l'accès aux attributs binaires de la fenêtre
const unsigned int M_fen_gerer_corps          = 1;
const unsigned int M_fen_afficher_corps       = 2;
const unsigned int M_fen_gerer_entete         = 4;
const unsigned int M_fen_afficher_entete      = 8;
const unsigned int M_fen_gerer_bordure        = 16;
const unsigned int M_fen_afficher_bordure     = 32;
const unsigned int M_fen_gerer_zone_rot       = 64;
const unsigned int M_fen_afficher_zone_rot    = 128;
const unsigned int M_fen_fond_touchable       = 256;
const unsigned int M_fen_rendu_ecran_direct   = 1024;
const unsigned int M_fen_bord_translucide     = 2048;
const unsigned int M_fen_corp_translucide     = 4096;
const unsigned int M_fen_lisser               = 8192;
const unsigned int M_fen_navigation_interne   = 16384;
const unsigned int M_fen_voir_si_pt_dans_fils = 32768;
const unsigned int M_fen_tout                 = 0xFFFFFFFF;

class alx_noeud_fenetre_sdl_opengl : public alx_noeud_scene
{private:
    alx_simulateur_points_contact *sim_contact;
    double nb_pixels_par_unite, entete_prcdt[2]
         , nb_pixels_par_unite_entete;
    bool forcer_pre_rendu_entete, fen_contact_avec_corp, fen_texture_tempon_prete;

    void init( alx_fenetre_opengl *f_opengl, const double tx, const double ty
             , alx_interface_sdl_opengl_1 *interface_m
             , alx_prerendeur *prerendeur
             , const double nb_pix_par_unite
             , HDC hdc, HGLRC hglrc
             , ALXGL_TYPE_RENDU type_rendu );
   // Des infos pour la sérialisation/désérialisation
    mutable alx_chaine_char acc_tmp, diff_tmp, cc1, seri_tmp, diff_type_tmp;
    P_void a_ptr_fen[50]; // pointeurs sur les attributs
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation)(unsigned int &pos, void *fptr);
    typedef alx_repere2D* (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_repere) ();
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_chaine_char)   (const alx_chaine_char &);
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_int_4_doubles) (const int n, const double r, const double v, const double b, const double a);
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_4_doubles)     (const double r, const double v, const double b, const double a);
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_2_doubles)     (const double x, const double y);
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_1_double)      (const double x);
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_deserialisation_1_bool)        (const bool b);
      f_fen_deserialisation_repere        frep[9];
      f_fen_deserialisation_chaine_char   fcc[9];
      f_fen_deserialisation_int_4_doubles fd4_i[9];
      f_fen_deserialisation_4_doubles     fd4[9];
      f_fen_deserialisation_2_doubles     fd2[9];
      f_fen_deserialisation_1_double      fd1[9];
      f_fen_deserialisation_1_bool        fb1[9];
    mutable f_fen_deserialisation f_ptr_fen[7];
    P_void f_fen[50]
         , f_fen_diff[50];
    typedef void (alx_noeud_fenetre_sdl_opengl::*f_fen_seri_diff)(unsigned int &pos, void *fptr) const;
    typedef const alx_repere2D*    (alx_noeud_fenetre_sdl_opengl::*f_fen_consultation_repere)     () const;
    typedef const alx_chaine_char& (alx_noeud_fenetre_sdl_opengl::*f_fen_consultation_chaine_char)() const;
    typedef const double*          (alx_noeud_fenetre_sdl_opengl::*f_fen_consultation_4_doubles_i)(const int i) const;
    typedef const double*          (alx_noeud_fenetre_sdl_opengl::*f_fen_consultation_4_doubles)  () const;
    typedef const double           (alx_noeud_fenetre_sdl_opengl::*f_fen_consultation_1_double)   () const;
    typedef const bool             (alx_noeud_fenetre_sdl_opengl::*f_fen_consultation_1_bool)     () const;
    mutable f_fen_seri_diff       f_ptr_seri_diff_fen[7];
      f_fen_consultation_repere      fsd_rep[9];
      f_fen_consultation_chaine_char fsd_cc[9];
      f_fen_consultation_4_doubles_i fsd_d4_i[9];
      f_fen_consultation_4_doubles   fsd_d4[9];
      f_fen_consultation_1_double    fsd_d1[9];
      f_fen_consultation_1_bool      fsd_b1[9];

    void init_ptr();
    void Deserialiser_repere      (unsigned int &pos, void *fptr);
    void Deserialiser_4_doubles_i (unsigned int &pos, void *fptr);
    void Deserialiser_4_doubles   (unsigned int &pos, void *fptr);
    void Deserialiser_2_doubles   (unsigned int &pos, void *fptr);
    void Deserialiser_1_double    (unsigned int &pos, void *fptr);
    void Deserialiser_1_bool      (unsigned int &pos, void *fptr);
    void Deserialiser_chaine_char(unsigned int &pos, void *fptr);
    void Serialiser_diff_repere     (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_4_doubles_i(unsigned int &pos, void *fptr) const;
    void Serialiser_diff_4_doubles  (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_2_doubles  (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_1_double   (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_1_bool     (unsigned int &pos, void *fptr) const;
    void Serialiser_diff_chaine_char(unsigned int &pos, void *fptr) const;

    mutable unsigned int dep_ecriture;
    const   alx_chaine_char *chaine_cour_fen;
    mutable alx_chaine_char *chaine_cour_diff_seri_fen;
    mutable alx_chaine_char *chaine_cour_ecriture_diff_fen
                          , *chaine_cour_ecriture_seri_fen;
    mutable alx_chaine_char serialisation_contenu_fen, serialisation_type_fen
                          , serialisation_differences_type_fen, serialisation_differences_contenu_fen
                          , chaine_tmp;
 protected:
    alx_interface_sdl_opengl_1 *interface_mere;
    alx_point2D   A_pt_tmp, A_pt_tmp2, pt_pour_contient_fen;
    alx_vecteur2D A_v_tmp , A_v_tmp2;
    alx_chaine_char titre;
    alx_noeud_scene repere_des_fils;
    alx_prerendeur *prerendeur_fiche, *prerendeur_entete, *prerendeur_externe;
    alx_noeud_zone_rendu_sdl_opengl zdr_corp, zdr_entete;
    alx_fenetre_opengl *fenetre_opengl;
    alx_fenetre        *fenetre;
    alx_noeud_entete_sdl_opengl noeud_entete;
    alx_noeud_scene noeud_pere_entete;
    alx_vecteur2D dimension_navigation;
    unsigned int M_attrib_fen;
    inline const bool Contact_avec_corp() const {return fen_contact_avec_corp;}
    inline void Contact_avec_corp(bool b)       {fen_contact_avec_corp = b;}
    inline const bool Forcer_pre_rendu_entete() const {return forcer_pre_rendu_entete;}
    inline void Forcer_pre_rendu_entete(bool b)       {forcer_pre_rendu_entete = b;}

 public:
   void init_default();
   alx_noeud_fenetre_sdl_opengl( alx_fenetre_opengl *f
                               , alx_interface_sdl_opengl_1 *interface_m
                               , alx_prerendeur *p
                               , const double nb_pix_par_unite
                               , HDC hdc, HGLRC hglrc
                               , ALXGL_TYPE_RENDU type_rendu);
   alx_noeud_fenetre_sdl_opengl( alx_fenetre_opengl *f
                               , alx_interface_sdl_opengl_1 *interface_m
                               , alx_prerendeur *p
                               , const double nb_pix_par_unite );
   alx_noeud_fenetre_sdl_opengl( const char *titre, const double tx, const double ty
                               , alx_interface_sdl_opengl_1 *interface_m );
   ~alx_noeud_fenetre_sdl_opengl();

   virtual const char* Void_vers_editeur_olfa_comet() {return "Void_vers_fen";}

   inline alx_noeud_scene* Alx_noeud_scene() {return (alx_noeud_scene*)this;}
   inline alx_fenetre_opengl* Fenetre_opengl() {return fenetre_opengl;}
   inline alx_fenetre*        Fenetre()        {return fenetre;}

   void PreRendreEntete();

   inline alx_interface_sdl_opengl_1* Interface_mere() {return interface_mere;}

   inline void abonner_a_dimension    (alx_methode_rappel *m) {fenetre->abonner_a_dimension(m);}
   inline void desabonner_de_dimension(alx_methode_rappel *m) {fenetre->desabonner_de_dimension(m);}

  // Les surcharges pour la sérialisation
   // La différence
   inline virtual const alx_chaine_char& Serialiser_difference_type() const {return alx_noeud_fenetre_sdl_opengl::Serialiser_difference_type( alx_noeud_fenetre_sdl_opengl::Serialiser_difference_contenu() );}
   virtual const alx_chaine_char& Serialiser_difference_type(const alx_chaine_char &diff_contenu) const;
   virtual const alx_chaine_char& Serialiser_difference_contenu() const;
   inline virtual const alx_chaine_char& Serialisation_differences_type()    const {return serialisation_differences_type_fen;}
   inline virtual const alx_chaine_char& Serialisation_differences_contenu() const {return serialisation_differences_contenu_fen;}

   // La sérialisation
   virtual const alx_chaine_char& Serialiser_type(const alx_chaine_char &contenu) const;
   virtual const alx_chaine_char& Serialiser_contenu() const;
   virtual const alx_chaine_char& Serialisation_contenu() const {return serialisation_contenu_fen;}
   inline virtual const alx_chaine_char& Serialiser_type() const {return alx_noeud_fenetre_sdl_opengl::Serialiser_type( alx_noeud_fenetre_sdl_opengl::Serialiser_contenu() );}
   virtual const alx_chaine_char& Serialisation_type   () const {return serialisation_type_fen;}
   // La désérialisation
   inline virtual void Deserialiser_type(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser_type(txt, pos);}
   virtual void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
   inline virtual void Deserialiser(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser(txt, pos);}
   virtual void Deserialiser(const alx_chaine_char &txt, unsigned int &pos);

  // Les surchgarges de la classe alx_arbre
   inline virtual bool A_pour_fils(alx_noeud_scene *e)           {return repere_des_fils.A_pour_fils(e);}
   inline virtual void Ajouter_fils_a_pos(alx_noeud_scene *e, const unsigned int pos)    {repere_des_fils.Ajouter_fils_a_pos(e, pos);}
   inline virtual void Ajouter_fils_au_debut(alx_noeud_scene *e) {repere_des_fils.Ajouter_fils_au_debut(e);}
   inline virtual void Ajouter_fils_a_la_fin(alx_noeud_scene *e) {repere_des_fils.Ajouter_fils_a_la_fin(e);}
   inline virtual void Ajouter_fils_replique(alx_noeud_scene *e) {repere_des_fils.Ajouter_fils_replique(e);}
   inline virtual void Ajouter_fils(alx_noeud_scene *e)          {repere_des_fils.Ajouter_fils(e);}
   inline virtual void Ajouter_fils_avant(alx_noeud_scene *noeud, alx_noeud_scene *noeud_fils) {repere_des_fils.Ajouter_fils_avant(noeud, noeud_fils);}
   inline virtual void Ajouter_fils_it(alx_noeud_scene *e, alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)
                                                         {repere_des_fils.Ajouter_fils_it(e, it);}
   inline virtual void Retirer_fils(alx_noeud_scene *e)          {repere_des_fils.Retirer_fils(e);}
   inline virtual void Retirer_fils_it(alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)
                                                         {repere_des_fils.Retirer_fils_it(it);}
   inline virtual void Vider_fils()                      {repere_des_fils.Vider_fils();}

   //
   inline virtual bool A_pour_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)          {return repere_des_fils.A_pour_L_fils(L);}
   inline virtual void Ajouter_L_fils_au_debut(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L){repere_des_fils.Ajouter_L_fils_au_debut(L);}
   inline virtual void Ajouter_L_fils_a_la_fin(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L){repere_des_fils.Ajouter_L_fils_a_la_fin(L);}
   inline virtual void Ajouter_L_fils_replique(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L){repere_des_fils.Ajouter_L_fils_replique(L);}
   inline virtual void Ajouter_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)         {repere_des_fils.Ajouter_L_fils(L);}
   inline virtual void Ajouter_L_fils( const alx_liste<alx_arbre<alx_noeud_scene*>*> &L
                                   , alx_element_liste<alx_arbre<alx_noeud_scene*>*> &it)     {repere_des_fils.Ajouter_L_fils(L, it);}
   inline virtual void Retirer_L_fils(const alx_liste<alx_arbre<alx_noeud_scene*>*> &L)         {repere_des_fils.Retirer_L_fils(L);}

// Parcours des fils avec des méthodes type machine à état
// Pour un parcours plus facile...
 inline const unsigned int Nb_fils () const {return repere_des_fils.Nb_fils();}

 inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Init_parcours_fils()                                     const {return repere_des_fils.Init_parcours_fils();}
 inline const bool Est_parcours_fils_fini( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it )                  const {return repere_des_fils.Est_parcours_fils_fini    (it);}
 inline const alx_noeud_scene* Courant_dans_parcours_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it )  const {return repere_des_fils.Courant_dans_parcours_fils(it);}
 inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Suivant_dans_parcours_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return repere_des_fils.Suivant_dans_parcours_fils       (it);}
 inline void Terminer_parcours_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*>  *it )                       const {repere_des_fils.Terminer_parcours_fils(it);}

 inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Init_parcours_inverse_fils()                                    const {return repere_des_fils.Init_parcours_inverse_fils();}
 inline const bool Est_parcours_inverse_fils_fini( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it )                 const {return repere_des_fils.Est_parcours_inverse_fils_fini    (it);}
 inline const alx_noeud_scene* Courant_dans_parcours_inverse_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return repere_des_fils.Courant_dans_parcours_inverse_fils(it);}
 inline alx_element_liste<alx_arbre<alx_noeud_scene*>*>* Suivant_dans_parcours_inverse_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it ) const {return repere_des_fils.Suivant_dans_parcours_inverse_fils       (it);}
 inline void Terminer_parcours_inverse_fils( alx_element_liste<alx_arbre<alx_noeud_scene*>*>  *it )                      const {repere_des_fils.Terminer_parcours_inverse_fils(it);}

//@}

  // Gestion des couleurs liées à la fenêtre
   const double* Couleur_fond() const                                                {return zdr_corp.Couleur();}
   const double Composante_couleur_fond(const unsigned int i)                        {return zdr_corp.Composante_couleur(i);}
   const void Composante_couleur_fond(const unsigned int i, const double v)          {zdr_corp.Composante_couleur(i, v);}
   void Couleur_fond(const double r, const double v, const double b, const double a) {zdr_corp.Couleur(r, v, b, a);}
   void Couleur_fond(const double *c)                                                {zdr_corp.Couleur(c);}
   double* P_couleur_fond(const unsigned int i)                                      {return zdr_corp.P_couleur(i);}

  // Gestion de l'affichage des boites englobantes
   inline virtual void Afficher_boites(const bool b)      {alx_noeud_scene::Afficher_boites(b);}
   inline virtual void Afficher_boite_noeud(const bool b) {alx_noeud_scene::Afficher_boite_noeud(b);}
   inline virtual const bool Afficher_boite_noeud() const {return alx_noeud_scene::Afficher_boite_noeud();}
   inline virtual void Afficher_boites_fils(const bool b) {repere_des_fils.Afficher_boites_fils(b);}
   inline virtual const bool Afficher_boites_fils() const {return repere_des_fils.Afficher_boites_fils();}

  // Par rapport à la zone de rendu :
   inline virtual void Repere_des_fils(const double x, const double y, const double r=0, const double ex=1, const double ey=1)
    {repere_des_fils.alx_repere2D::maj(x, y, r, ex, ey);}
   inline const alx_repere2D* Repere_des_fils() const {return (alx_repere2D*)(&repere_des_fils);}
   inline alx_repere2D* Repere_des_fils_pour_modif() {return (alx_repere2D*)(&repere_des_fils);}
   inline alx_noeud_scene* Noeud_repere_fils() {return &repere_des_fils;}
   inline const alx_noeud_scene* Noeud_a_rendre() const {return zdr_corp.Noeud_a_rendre();}
   inline void Noeud_a_rendre(alx_noeud_scene* n) {zdr_corp.Noeud_a_rendre(n);}
   inline alx_repere2D* Repere_du_contenu() {return &repere_des_fils;}
   inline virtual void Etirement_du_contenu(const double e)                   {repere_des_fils.Etirement(e,e);}
   inline virtual void Etirement_du_contenu(const double ex, const double ey) {repere_des_fils.Etirement(ex,ey);}
   inline virtual void Etirement_du_contenu(const alx_vecteur2D &ve)          {repere_des_fils.Etirement(ve);}
   inline virtual const alx_vecteur2D& Etirement_du_contenu() const {return repere_des_fils.Etirement();}
   inline virtual const alx_point2D& Origine_du_contenu() const           {return repere_des_fils.Origine();}
   inline virtual void Origine_du_contenu(const alx_point2D &pt)          {repere_des_fils.Origine(pt.x, pt.y);}
   inline virtual void Origine_du_contenu(const double x, const double y) {repere_des_fils.Origine(-x, -y);}
   inline virtual void Translation_du_contenu(const double tx, const double ty) {repere_des_fils.alx_repere2D::Translation(tx, ty);}
   inline virtual void Translation_du_contenu(const alx_vecteur2D &v)           {repere_des_fils.alx_repere2D::Translation(-v.Dx(), -v.Dy());}
   inline virtual void Translation_du_contenu(const alx_point2D &pt)            {repere_des_fils.alx_repere2D::Translation(-pt.X(), -pt.Y());}
   inline virtual const double Rotation_du_contenu() const {return repere_des_fils.Rotation();}
   inline virtual void Rotation_du_contenu(const double r) {repere_des_fils.Rotation(r);}
   inline virtual void Pivoter_le_contenu(const double r, const double x=0, const double y=0) {repere_des_fils.alx_repere2D::Pivoter(r,x,y);}

   inline void Position_des_fils_du_contenu_changeable(const bool b) {repere_des_fils.Position_des_fils_changeable(b);}
   inline const bool Position_des_fils_du_contenu_changeable() const {return repere_des_fils.Position_des_fils_changeable();}
  // Le titre:
   inline virtual void Titre(const alx_chaine_char &t) {titre = t;}
   inline virtual void Titre(const char *t)            {titre = t;}
   inline virtual const alx_chaine_char& Titre() const {return titre;}

  //____________________________________________________________________________
   inline virtual const alx_point2D& Position_bg() const {return Origine();}
   inline virtual void Position_bg(const alx_point2D &p)
    {Position_bg(p.x, p.y);}
   inline virtual void Position_bg(const double x, const double y)
    {alx_noeud_scene::Origine(x, y);}


  // Les surcharges de la classe alx_noeud_scene :
   virtual void PreRendre();
   virtual void Afficher();
   virtual info_du_contenant* Contient(const alx_point2D &pt, int action);

  // Les méthodes spécifiques :
   inline void Epaisseur_bordure(const double v) {fenetre->Epaisseur_bordure(v);}
   inline void Hauteur_entete(const double v)    {fenetre->Hauteur_entete(v);}
   inline void Hauteur_corp(const double v)      {fenetre->Hauteur_corp(v);}
   inline void Longueur_entete(const double v)   {fenetre->Longueur_entete(v);}
   inline void Longueur_corp(const double v)     {fenetre->Longueur_corp(v);}
   inline void Dimension_corp(const alx_point2D &p)           {Dimension_corp(p.x, p.y);}
   inline void Dimension_corp(const alx_vecteur2D &v)         {Dimension_corp(v.dx, v.dy);}
   inline void Dimension_corp(const double x, const double y) {fenetre->Hauteur_corp(y); fenetre->Longueur_corp(x);}

   inline void Dimension_navigation(const double dx, const double dy) {dimension_navigation.maj(dx, dy);}
   inline const alx_vecteur2D& Dimension_navigation() const {return dimension_navigation;}
   inline const double Dimension_navigation_x() const {return dimension_navigation.dx;}
   inline const double Dimension_navigation_y() const {return dimension_navigation.dy;}

  //__________________________________________________________________
   inline alx_noeud_zone_rendu_sdl_opengl& Zdr_corp()   {return zdr_corp;}
   inline alx_noeud_zone_rendu_sdl_opengl& Zdr_entete() {return zdr_entete;}
  //__________________________________________________________________
   inline void Prerendeur_entete(alx_prerendeur *p) {prerendeur_entete = p;}
   inline alx_prerendeur* Prerendeur_entete()       {return prerendeur_entete;}
  //__________________________________________________________________
   inline void Prerendeur_externe(alx_prerendeur *p) {prerendeur_externe = p;}
   inline alx_prerendeur* Prerendeur_externe()       {return prerendeur_externe;}
  //__________________________________________________________________
   inline void Prerendeur_fiche(alx_prerendeur *p) {prerendeur_fiche = p;}
   inline alx_prerendeur* Prerendeur_fiche()       {return prerendeur_fiche;}

//   inline void Appliquer_maj()                   {fenetre->Appliquer_maj();}
   inline const double Epaisseur_bordure() const {return fenetre->Epaisseur_bordure();}
   inline const double Hauteur_entete()    const {return fenetre->Hauteur_entete();}
   inline const double Hauteur_corp()      const {return fenetre->Hauteur_corp();}
   inline const double Longueur_entete()   const {return fenetre->Longueur_entete();}
   inline const double Longueur_corp()     const {return fenetre->Longueur_corp();}
   virtual inline const double Hauteur_totale() const
    {return Hauteur_corp() + Hauteur_entete() + 2*Epaisseur_bordure();}
   virtual inline const double Longueur_totale() const
    {return Longueur_corp() + 2*Epaisseur_bordure();}
  //__________________________________________________________________
   inline void Nb_pixels_par_unite(const double n) {nb_pixels_par_unite = n;}
   inline const double Nb_pixels_par_unite() const {return nb_pixels_par_unite;}
   inline void Nb_pixels_par_unite_entete(const double n) {nb_pixels_par_unite_entete = n;}
   inline const double Nb_pixels_par_unite_entete() const {return nb_pixels_par_unite_entete;}

  //__________________________________________________________________
   inline alx_simulateur_points_contact* Sim_contact() {return sim_contact;}

  // Les attributs binaires
  // Par rapport à la zone de rendu
   inline const bool Triangle_control() const {return zdr_corp.Triangle_control();}
   inline void Triangle_control(bool b)       {zdr_corp.Triangle_control(b);}
   inline const bool Rendre_fond()      const {return zdr_corp.Effacer_fond();}
   inline void Rendre_fond(bool b)            {zdr_corp.Effacer_fond(b);}
   inline void Top_classos(const bool b)      {zdr_corp.Top_classos(b);}
   inline const bool Top_classos()      const {return zdr_corp.Top_classos();}

  // Ceux propres à la fenêtre :
   inline void Marqueur_attributs_fen(const unsigned int m, bool b)
    {if(b) M_attrib_fen |= m;
      else M_attrib_fen &= ~m;}
   inline void Marqueur_attributs_fen(const unsigned int m) {M_attrib_fen = m;}
   inline const unsigned int Marqueur_attributs_fen() const {return M_attrib_fen;}

  //__________________________________________________________________
   inline const bool Voir_si_pt_dans_fils() const {return M_fen_voir_si_pt_dans_fils&Marqueur_attributs_fen();}
   inline void Voir_si_pt_dans_fils(bool b)       {Marqueur_attributs_fen(M_fen_voir_si_pt_dans_fils, b);}
  //__________________________________________________________________
   inline const bool Fond_touchable() const {return M_fen_fond_touchable&Marqueur_attributs_fen();}
   inline void Fond_touchable(bool b)       {Marqueur_attributs_fen(M_fen_fond_touchable, b);}
  //__________________________________________________________________
   inline const bool Rendu_ecran_direct() const {return M_fen_rendu_ecran_direct&Marqueur_attributs_fen();}
   inline void Rendu_ecran_direct(bool b)       {Marqueur_attributs_fen(M_fen_rendu_ecran_direct, b);}
  //__________________________________________________________________
   inline const bool Navigation_interne() const {return M_fen_navigation_interne&Marqueur_attributs_fen();}
   inline void Navigation_interne(const bool b) {Marqueur_attributs_fen(M_fen_navigation_interne, b);}
  //__________________________________________________________________
   inline void Lisser(const bool b) {Marqueur_attributs_fen(M_fen_lisser, b);}
   inline const bool Lisser()       {return M_fen_lisser&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Texture_tempon_prete(const bool b) {fen_texture_tempon_prete = b;}
   inline const bool Texture_tempon_prete() const {return fen_texture_tempon_prete;}
  //__________________________________________________________________
   inline void Gerer_corps(const bool b) {Marqueur_attributs_fen(M_fen_gerer_corps, b);}
   inline const bool Gerer_corps() const {return M_fen_gerer_corps&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Gerer_entete(const bool b) {Marqueur_attributs_fen(M_fen_gerer_entete, b);}
   inline const bool Gerer_entete() const {return M_fen_gerer_entete&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Gerer_bordure(const bool b) {Marqueur_attributs_fen(M_fen_gerer_bordure, b);}
   inline const bool Gerer_bordure() const {return M_fen_gerer_bordure&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Gerer_zone_rot(const bool b) {Marqueur_attributs_fen(M_fen_gerer_zone_rot, b);}
   inline const bool Gerer_zone_rot() const {return M_fen_gerer_zone_rot&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Afficher_corps(const bool b) {Marqueur_attributs_fen(M_fen_afficher_corps, b);}
   inline const bool Afficher_corps() const {return M_fen_afficher_corps&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Afficher_entete(const bool b) {Marqueur_attributs_fen(M_fen_afficher_entete, b);}
   inline const bool Afficher_entete() const {return M_fen_afficher_entete&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Afficher_bordure(const bool b) {Marqueur_attributs_fen(M_fen_afficher_bordure, b);}
   inline const bool Afficher_bordure() const {return M_fen_afficher_bordure&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Afficher_zone_rot(const bool b) {Marqueur_attributs_fen(M_fen_afficher_zone_rot, b);}
   inline const bool Afficher_zone_rot() const {return M_fen_afficher_zone_rot&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Bord_translucide(const bool b) {Marqueur_attributs_fen(M_fen_bord_translucide, b);}
   inline const bool Bord_translucide() const {return M_fen_bord_translucide&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Corp_translucide(const bool b) {Marqueur_attributs_fen(M_fen_corp_translucide, b);}
   inline const bool Corp_translucide() const {return M_fen_corp_translucide&Marqueur_attributs_fen();}
  //__________________________________________________________________
   inline void Fenetre_translucide(const bool b)
    {Corp_translucide(b);
     Bord_translucide(b);}
  //__________________________________________________________________
   inline virtual void Mettre_a_jour_bordure()
    {fenetre->Appliquer_maj();
     fenetre_opengl->Mettre_a_jour();}
  //__________________________________________________________________
   inline virtual void Mettre_a_jour()
    {Forcer_pre_rendu_entete(true);
     Pre_rendu_exeptionnel(true);
     Mettre_a_jour_bordure();}
  //__________________________________________________________________
   inline void Translucidite_corp(const double a)
    {const double *tab = fenetre_opengl->Couleur_masque_corp();
     fenetre_opengl->Couleur_masque_corp(tab[0], tab[1], tab[2], a);}
   inline const double Translucidite_corp() const {return fenetre_opengl->Couleur_masque_corp()[3];}
  //__________________________________________________________________
   inline void Translucidite_bord(const double a)
    {const double *tab = fenetre_opengl->Couleur_masque_bord();
     fenetre_opengl->Couleur_masque_bord(tab[0], tab[1], tab[2], a);}
   inline const double Translucidite_bord() const {return fenetre_opengl->Couleur_masque_bord()[3];}
  //__________________________________________________________________
   inline void Translucidite_globale(const double a)
    {const double *tab = fenetre_opengl->Couleur_masque_bord();
     fenetre_opengl->Couleur_masque_bord(tab[0], tab[1], tab[2], a);
     tab = fenetre_opengl->Couleur_masque_corp();
     fenetre_opengl->Couleur_masque_corp(tab[0], tab[1], tab[2], a);
     //fenetre_opengl->Mettre_a_jour();
    }

  //__________________________________________________________________
   inline void Couleur_masque_corp(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_masque_corp(r, v, b, a);}
   inline const double* Couleur_masque_corp() const {return fenetre_opengl->Couleur_masque_corp();}
  //__________________________________________________________________
   inline void Couleur_masque_bord(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_masque_bord(r, v, b, a);}
   inline const double* Couleur_masque_bord() const {return fenetre_opengl->Couleur_masque_bord();}
  //__________________________________________________________________
   inline void Couleur_masque_globale(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_masque_bord(r, v, b, a);
     fenetre_opengl->Couleur_masque_corp(r, v, b, a);}

   //___________________________________________________________________________
   inline void Couleur_bordure_ext_angle(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_bordure_ext_angle(r,v,b,a);}
   //___________________________________________________________________________
   inline void Couleur_bordure_ext(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_bordure_ext(r,v,b,a);}
   //___________________________________________________________________________
   inline void Couleur_bordure_int_angle(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_bordure_int_angle(r,v,b,a);}
   //___________________________________________________________________________
   inline void Couleur_bordure_int(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_bordure_int(r,v,b,a);}
   //___________________________________________________________________________
   inline void Couleur_entete(const int num_coin, const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_entete(num_coin,r,v,b,a);}
   //___________________________________________________________________________
   inline void Couleur_corps(const double r, const double v, const double b, const double a)
    {fenetre_opengl->Couleur_corps(r,v,b,a);}

   //___________________________________________________________________________
   inline const double* Couleur_corps()             const {return fenetre_opengl->Couleur_corps();}
   inline const double* Couleur_bordure_ext_angle() const {return fenetre_opengl->Couleur_bordure_ext_angle();}
   inline const double* Couleur_bordure_ext()       const {return fenetre_opengl->Couleur_bordure_ext();}
   inline const double* Couleur_bordure_int_angle() const {return fenetre_opengl->Couleur_bordure_int_angle();}
   inline const double* Couleur_bordure_int()       const {return fenetre_opengl->Couleur_bordure_int();}
   inline const double* Couleur_entete(const int n) const {return fenetre_opengl->Couleur_entete(n);}

   //___________________________________________________________________________
   inline double* P_couleur_corps(const unsigned int i)                        {return fenetre_opengl->P_couleur_corps(i);}
   inline double* P_couleur_bordure_ext_angle(const unsigned int i)            {return fenetre_opengl->P_couleur_bordure_ext_angle(i);}
   inline double* P_couleur_bordure_ext(const unsigned int i)                  {return fenetre_opengl->P_couleur_bordure_ext(i);}
   inline double* P_couleur_bordure_int_angle(const unsigned int i)            {return fenetre_opengl->P_couleur_bordure_int_angle(i);}
   inline double* P_couleur_bordure_int(const unsigned int i)                  {return fenetre_opengl->P_couleur_bordure_int(i);}
   inline double* P_couleur_entete(const unsigned int n, const unsigned int i) {return fenetre_opengl->P_couleur_entete(n, i);}
   inline double* P_couleur_masque_corp(const unsigned int i)                  {return fenetre_opengl->P_couleur_masque_corp(i);}
   inline double* P_couleur_masque_bord(const unsigned int i)                  {return fenetre_opengl->P_couleur_masque_bord(i);}
};

typedef alx_noeud_fenetre_sdl_opengl* P_alx_noeud_fenetre_sdl_opengl;
alx_noeud_fenetre_sdl_opengl* Void_vers_fen(void *ptr) {return (alx_noeud_fenetre_sdl_opengl*)ptr;}

#endif


