#ifndef __ALX_NOEUD_POLYGONE_SDL_OPENGL_H__
#define __ALX_NOEUD_POLYGONE_SDL_OPENGL_H__

#include "../alx_noeud_scene.h"
#include "../fonctions_serialisations.cpp"
//#include "../../GPC/gpc.h"
#include "../../opengl/config_opengl.h"

#include "../fonctions_serialisations.cpp"

inline const unsigned int POLY_couleur_def()             {return 0x1;}
inline const unsigned int POLY_texture_def()             {return 0x2;}
inline const unsigned int POLY_perimetre_texture()       {return 0x4;}
inline const unsigned int POLY_lisser()                  {return 0x8;}
inline const unsigned int POLY_translucidite()           {return 0x10;}
inline const unsigned int POLY_translucidite_perimetre() {return 0x20;}
inline const unsigned int POLY_polygone_a_afficher()     {return 0x40;}
inline const unsigned int POLY_perimetre_a_afficher()    {return 0x80;}
inline const unsigned int POLY_mode_texture2()           {return 0x100;}
inline const unsigned int POLY_mode_line()               {return 0x200;}

#include "tableau_de_double.h"

// Définit plus bas...
class alx_noeud_polygone_sdl_opengl;
class Interpollateur_couleur_polygone;
typedef void (*Fonction_interpolation_couleur_polygone) ( const alx_noeud_polygone_sdl_opengl &
                                                        , const double x, const double y
                                                        , const double *Tab_col, const double *Tab_tex, const double *Tab_tex2
                                                        , double *Col, double *Tex, double *Tex2
                                                        , void *params);

// L'objet de ce fichier...
class alx_noeud_polygone_sdl_opengl : public alx_noeud_scene
{private:
   Liste_de_rappel L_rap_maj_Info_texture_1, L_rap_maj_Info_texture_2;
     mutable bool texture_1_modif, texture_2_modif;
   CFSD<alx_noeud_polygone_sdl_opengl> classe_seri_poly;
   mutable bool a_ete_modifie, pre_rendu_en_cours, mode_texture_2_en_cours;
   inline const bool A_ete_modifie(gpc_polygon **p) const {*p = poly_ok;
                                                          const bool rep = a_ete_modifie;
                                                          a_ete_modifie = false;
                                                          return rep;}

   void init();
   void init_ptr();
   mutable alx_chaine_char serialisation_type_poly   , serialisation_differences_type_poly
                         , serialisation_contenu_poly, serialisation_differences_contenu_poly
                         , nouv_serialisation_type_poly , seri_diff_type_poly;
   int num_liste_mode_texture2, num_liste, num_liste_perimetre, cote_arrive;
   mutable alx_point2D A_pt_tmp, pt_tmp_consultation;
   static unsigned int *ordre_affichage_poly
                     , nb_elements_ordre_affichage;
   Interpollateur_couleur_polygone *interpollateur
                                 , *interpollateur_perimetre;
   GLenum lignes_GL, combinaison_textures_rgb, combinaison_textures_alpha
                   , combinaison_couleurs_rgb, combinaison_couleurs_alpha
                   , combinaison_textures_source_arg0_Alpha, combinaison_textures_operande0_Alpha
                   , combinaison_couleurs_source_arg0_Alpha, combinaison_couleurs_operande0_Alpha
                   , combinaison_textures_source_arg1_Alpha, combinaison_textures_operande1_Alpha
                   , combinaison_couleurs_source_arg1_Alpha, combinaison_couleurs_operande1_Alpha
                   , combinaison_textures_source_arg2_Alpha, combinaison_textures_operande2_Alpha
                   , combinaison_couleurs_source_arg2_Alpha, combinaison_couleurs_operande2_Alpha
                   , combinaison_textures_source_arg0, combinaison_textures_operande0
                   , combinaison_couleurs_source_arg0, combinaison_couleurs_operande0
                   , combinaison_textures_source_arg1, combinaison_textures_operande1
                   , combinaison_couleurs_source_arg1, combinaison_couleurs_operande1
                   , combinaison_textures_source_arg2, combinaison_textures_operande2
                   , combinaison_couleurs_source_arg2, combinaison_couleurs_operande2
                   , SVG_combinaison_couleurs_rgb, SVG_combinaison_couleurs_alpha, SVG_combinaison_couleurs_source_arg2, SVG_combinaison_couleurs_operande2;
   //bool mode_texture2;
   //bool mode_line;
   float alpha_scale, rgb_scale, constante_combinaison_couleurs, constante_combinaison_textures;
   int ordre_application_textures_rgb  , ordre_application_couleurs_rgb
     , ordre_application_textures_alpha, ordre_application_couleurs_alpha;
   alx_chaine_char cc_Texture_1_desiree, cc_Texture_2_desiree;

   double *ligne_originale;
   unsigned int taille_relle_ligne_originale, taille_ligne_originale;

 protected:
   double couleur[4], couleur_perimetre[4];
   float const_couleur[4];
   double const_couleur_d[4];
   unsigned int nb_points, attributs_poly;
   gpc_polygon  gpc_poly, gpc_poly_tmp, gpc_poly_add_sub, *poly_ok, *poly_calcul;
   //gpc_tristrip gpc_tri , gpc_tri_tmp;
   unsigned int attribut;
   void Operation(alx_noeud_polygone_sdl_opengl &p, const alx_noeud_scene *n_racine, gpc_op operation);
   void Operation(gpc_polygon *p, gpc_op operation);
   const unsigned int Intersection(const alx_point2D &pt, const gpc_vertex_list *VL, const unsigned int pos2, unsigned int &pos);

   void Init_textures();
   void Stop_textures();
   void Trianguler();
   void Contourner();

   INFOS_TEXTURE *info_texture, *info_texture2;
   double pt_min[2], pt_max[2];
   bool boite_noeud_deja_calculees;

   inline void Marqueur_attributs_poly(const int m, bool b)
    {if(b) attributs_poly |= m;
      else attributs_poly &= ~m;}
   inline void Marqueur_attributs_poly(const int m) {attributs_poly = m;}
   inline const int Marqueur_attributs_poly() const {return attributs_poly;}
   const double *coords_col, *coords_tex, *coords_tex2
              , *coords_col_perimetre, *coords_tex_perimetre;
   const bool get_Mode_line() const {return (POLY_mode_line()&Marqueur_attributs_poly()) != 0;}
   void set_Mode_line(const bool b) {Marqueur_attributs_poly(POLY_mode_line(), b);}

 public :
   alx_noeud_polygone_sdl_opengl();
   ~alx_noeud_polygone_sdl_opengl();

   virtual const char* Real_class_cmd() {return "Void_vers_poly";}
  //____________________________________________________________________________
  //______________________ Manipulation de polygones ___________________________
  // Modes lignes ou polygones
   void Mode_ligne();
   void Mode_polygone();
   void Lignes_GL(const GLenum mode_ligne);
   const GLenum Lignes_GL();

   inline const bool Mode_texture2() const {return (POLY_mode_texture2()&Marqueur_attributs_poly()) != 0;}
          void Mode_texture2(const bool b) ;

   inline void Const_couleur(const float *col) {memcpy(const_couleur, col, sizeof(float)<<2);
                                                for(unsigned int i = 0; i<4; i++) const_couleur_d[i] = const_couleur[i];
                                                A_changer(true);
                                               }
   inline void Const_couleur(const float r, const float v, const float b, const float a) {const_couleur[0]=r; const_couleur[1]=v; const_couleur[2]=b; const_couleur[3]=a;
                                                                                          for(unsigned int i = 0; i<4; i++) const_couleur_d[i] = const_couleur[i];
                                                                                          A_changer(true);
                                                                                         }
   inline const float* Const_couleur()                     const {return const_couleur;}
   inline const float  Const_couleur(const unsigned int i) const {return const_couleur[i];}

   inline void Const_couleur_d(const double r, const double v, const double b, const double a) {Const_couleur((float)r, (float)v, (float)b, (float)a);}
   inline const double* Const_couleur_d()                     const {return const_couleur_d;}

   inline const float Alpha_scale() const {return alpha_scale;}
   inline void Alpha_scale(const float f) {alpha_scale = f; A_changer(true);}
   inline const float RGB_scale() const {return rgb_scale;}
   inline void RGB_scale(const float f) {rgb_scale = f; A_changer(true);}

   inline const double Alpha_scale_d() const {return alpha_scale;}
   inline void Alpha_scale_d(const double d) {alpha_scale = (float)d; A_changer(true);}
   inline const double RGB_scale_d() const {return rgb_scale;}
   inline void RGB_scale_d(const double d) {rgb_scale = (float)d; A_changer(true);}

   inline const float Constante_combinaison_couleurs() const {return constante_combinaison_couleurs;}
   inline void Constante_combinaison_couleurs(const float f) {constante_combinaison_couleurs = f; A_changer(true);}
   inline const float Constante_combinaison_textures() const {return constante_combinaison_textures;}
   inline void Constante_combinaison_textures(const float f) {constante_combinaison_textures = f; A_changer(true);}

   inline const double Constante_combinaison_couleurs_d() const {return constante_combinaison_couleurs;}
   inline void Constante_combinaison_couleurs_d(const double f) {constante_combinaison_couleurs = (float)f; A_changer(true);}
   inline const double Constante_combinaison_textures_d() const {return constante_combinaison_textures;}
   inline void Constante_combinaison_textures_d(const double f) {constante_combinaison_textures = (float)f; A_changer(true);}

 // Combinaisons des canaux RVB
   inline const int Combinaison_couleurs_source_arg0() const {return combinaison_couleurs_source_arg0;}
   inline void Combinaison_couleurs_source_arg0(const int v) {combinaison_couleurs_source_arg0 = v; A_changer(true);}
   inline const int Combinaison_couleurs_operande0() const   {return combinaison_couleurs_operande0;}
   inline void Combinaison_couleurs_operande0(const int v)   {combinaison_couleurs_operande0 = v; A_changer(true);}
   inline const int Combinaison_textures_source_arg0() const {return combinaison_textures_source_arg0;}
   inline void Combinaison_textures_source_arg0(const int v) {combinaison_textures_source_arg0 = v; A_changer(true);}
   inline const int Combinaison_textures_operande0() const   {return combinaison_textures_operande0;}
   inline void Combinaison_textures_operande0(const int v)   {combinaison_textures_operande0 = v; A_changer(true);}

   inline const int Combinaison_couleurs_source_arg1() const {return combinaison_couleurs_source_arg1;}
   inline void Combinaison_couleurs_source_arg1(const int v) {combinaison_couleurs_source_arg1 = v; A_changer(true);}
   inline const int Combinaison_couleurs_operande1() const   {return combinaison_couleurs_operande1;}
   inline void Combinaison_couleurs_operande1(const int v)   {combinaison_couleurs_operande1 = v; A_changer(true);}
   inline const int Combinaison_textures_source_arg1() const {return combinaison_textures_source_arg1;}
   inline void Combinaison_textures_source_arg1(const int v) {combinaison_textures_source_arg1 = v; A_changer(true);}
   inline const int Combinaison_textures_operande1() const   {return combinaison_textures_operande1;}
   inline void Combinaison_textures_operande1(const int v)   {combinaison_textures_operande1 = v; A_changer(true);}

   inline const int Combinaison_couleurs_source_arg2() const {return combinaison_couleurs_source_arg2;}
   inline void Combinaison_couleurs_source_arg2(const int v) {combinaison_couleurs_source_arg2 = v; A_changer(true);}
   inline const int Combinaison_couleurs_operande2() const   {return combinaison_couleurs_operande2;}
   inline void Combinaison_couleurs_operande2(const int v)   {combinaison_couleurs_operande2 = v; A_changer(true);}
   inline const int Combinaison_textures_source_arg2() const {return combinaison_textures_source_arg2;}
   inline void Combinaison_textures_source_arg2(const int v) {combinaison_textures_source_arg2 = v; A_changer(true);}
   inline const int Combinaison_textures_operande2() const   {return combinaison_textures_operande2;}
   inline void Combinaison_textures_operande2(const int v)   {combinaison_textures_operande2 = v; A_changer(true);}

  // Combinaisons du canal ALPHA
   inline const int Combinaison_couleurs_source_arg0_Alpha() const {return combinaison_couleurs_source_arg0_Alpha;}
   inline void Combinaison_couleurs_source_arg0_Alpha(const int v) {combinaison_couleurs_source_arg0_Alpha = v; A_changer(true);}
   inline const int Combinaison_couleurs_operande0_Alpha() const   {return combinaison_couleurs_operande0_Alpha;}
   inline void Combinaison_couleurs_operande0_Alpha(const int v)   {combinaison_couleurs_operande0_Alpha = v; A_changer(true);}
   inline const int Combinaison_textures_source_arg0_Alpha() const {return combinaison_textures_source_arg0_Alpha;}
   inline void Combinaison_textures_source_arg0_Alpha(const int v) {combinaison_textures_source_arg0_Alpha = v; A_changer(true);}
   inline const int Combinaison_textures_operande0_Alpha() const   {return combinaison_textures_operande0_Alpha;}
   inline void Combinaison_textures_operande0_Alpha(const int v)   {combinaison_textures_operande0_Alpha = v; A_changer(true);}

   inline const int Combinaison_couleurs_source_arg1_Alpha() const {return combinaison_couleurs_source_arg1_Alpha;}
   inline void Combinaison_couleurs_source_arg1_Alpha(const int v) {combinaison_couleurs_source_arg1_Alpha = v; A_changer(true);}
   inline const int Combinaison_couleurs_operande1_Alpha() const   {return combinaison_couleurs_operande1_Alpha;}
   inline void Combinaison_couleurs_operande1_Alpha(const int v)   {combinaison_couleurs_operande1_Alpha = v; A_changer(true);}
   inline const int Combinaison_textures_source_arg1_Alpha() const {return combinaison_textures_source_arg1_Alpha;}
   inline void Combinaison_textures_source_arg1_Alpha(const int v) {combinaison_textures_source_arg1_Alpha = v; A_changer(true);}
   inline const int Combinaison_textures_operande1_Alpha() const   {return combinaison_textures_operande1_Alpha;}
   inline void Combinaison_textures_operande1_Alpha(const int v)   {combinaison_textures_operande1_Alpha = v; A_changer(true);}

   inline const int Combinaison_couleurs_source_arg2_Alpha() const {return combinaison_couleurs_source_arg2_Alpha;}
   inline void Combinaison_couleurs_source_arg2_Alpha(const int v) {combinaison_couleurs_source_arg2_Alpha = v; A_changer(true);}
   inline const int Combinaison_couleurs_operande2_Alpha() const   {return combinaison_couleurs_operande2_Alpha;}
   inline void Combinaison_couleurs_operande2_Alpha(const int v)   {combinaison_couleurs_operande2_Alpha = v; A_changer(true);}
   inline const int Combinaison_textures_source_arg2_Alpha() const {return combinaison_textures_source_arg2_Alpha;}
   inline void Combinaison_textures_source_arg2_Alpha(const int v) {combinaison_textures_source_arg2_Alpha = v; A_changer(true);}
   inline const int Combinaison_textures_operande2_Alpha() const   {return combinaison_textures_operande2_Alpha;}
   inline void Combinaison_textures_operande2_Alpha(const int v)   {combinaison_textures_operande2_Alpha = v; A_changer(true);}

  // Ordre d'application de la texture
   inline void Ordre_application_textures(const int o)     {return Ordre_application_textures(o, o);}
   inline void Ordre_application_textures(const int o_rgb, const int o_alpha) {Ordre_application_textures_rgb(o_rgb); Ordre_application_textures_alpha(o_alpha);}
     inline const int Ordre_application_textures_rgb() const {return ordre_application_textures_rgb;}
     inline void Ordre_application_textures_rgb(const int o) {ordre_application_textures_rgb = o; A_changer(true);}
     inline const int Ordre_application_textures_alpha() const {return ordre_application_textures_alpha;}
     inline void Ordre_application_textures_alpha(const int o) {ordre_application_textures_alpha = o; A_changer(true);}
   inline void Combinaison_textures(const int m) {Combinaison_textures(m, m);}
   inline void Combinaison_textures(const int m_rgb, const int m_alpha) {Combinaison_textures_rgb(m_rgb); Combinaison_textures_alpha(m_alpha); maj();}
     inline void Combinaison_textures_rgb(const int m) {combinaison_textures_rgb = m; A_changer(true);}
     inline const int Combinaison_textures_rgb() const {return combinaison_textures_rgb;}
     inline void Combinaison_textures_alpha(const int m) {combinaison_textures_alpha = m; A_changer(true);}
     inline const int Combinaison_textures_alpha() const {return combinaison_textures_alpha;}

   inline void Ordre_application_couleurs(const int o)       {return Ordre_application_couleurs(o, o);}
   inline void Ordre_application_couleurs(const int o_rgb, const int o_alpha) {Ordre_application_couleurs_rgb(o_rgb); Ordre_application_couleurs_alpha(o_alpha);}
     inline const int Ordre_application_couleurs_rgb() const {return ordre_application_couleurs_rgb;}
     inline void Ordre_application_couleurs_rgb(const int o) {ordre_application_couleurs_rgb = o; A_changer(true);}
     inline const int Ordre_application_couleurs_alpha() const {return ordre_application_couleurs_alpha;}
     inline void Ordre_application_couleurs_alpha(const int o) {ordre_application_couleurs_alpha = o; A_changer(true);}
   inline void Combinaison_couleurs(const int m) {Combinaison_couleurs(m, m);}
   inline void Combinaison_couleurs(const int m_rgb, const int m_alpha) {Combinaison_couleurs_rgb(m_rgb); Combinaison_couleurs_alpha(m_alpha); maj();}
     inline void Combinaison_couleurs_rgb(const int m) {combinaison_couleurs_rgb = m; A_changer(true);}
     inline const int Combinaison_couleurs_rgb() const {return combinaison_couleurs_rgb;}
     inline void Combinaison_couleurs_alpha(const int m) {combinaison_couleurs_alpha = m; A_changer(true);}
     inline const int Combinaison_couleurs_alpha() const {return combinaison_couleurs_alpha;}

  // Couleur du polygone
   inline void Couleur(const double *col) {memcpy(couleur, col, sizeof(double)<<2); A_changer(true);}
   inline void Couleur(const double r, const double v, const double b, const double a) {couleur[0]=r; couleur[1]=v; couleur[2]=b; couleur[3]=a; A_changer(true);}
   inline const double* Couleur()                     const {return couleur;}
   inline const double  Couleur(const unsigned int i) const {return couleur[i];}
   inline double Couleur(const unsigned int i, const double v) {return (couleur[i] = v);}
  // Couleur du polyèdre
   inline void Couleur_perimetre(const double *col) {memcpy(couleur_perimetre, col, sizeof(double)<<2); A_changer(true);}
   inline void Couleur_perimetre(const double r, const double v, const double b, const double a) {couleur_perimetre[0]=r; couleur_perimetre[1]=v; couleur_perimetre[2]=b; couleur_perimetre[3]=a; A_changer(true);}
   inline const double* Couleur_perimetre()                     const {return couleur_perimetre;}
   inline const double  Couleur_perimetre(const unsigned int i) const {return couleur_perimetre[i];}
   inline void Couleur_perimetre(const unsigned int i, const double v) {couleur_perimetre[i] = v; A_changer(true);}
  // Translucidité de la surface et du périmètre
   inline void Translucidite(const bool b) {Marqueur_attributs_poly(POLY_translucidite(), b); A_changer(true);}
   inline const bool Translucidite() const {return (POLY_translucidite()&Marqueur_attributs_poly()) != 0;}
   inline void Translucidite_perimetre(const bool b) {Marqueur_attributs_poly(POLY_translucidite_perimetre(), b); A_changer(true);}
   inline const bool Translucidite_perimetre() const {return (POLY_translucidite_perimetre()&Marqueur_attributs_poly()) != 0;}

  // Interpollation pour le corp du polygone
   inline void Coords_couleurs(const double *v) {coords_col = v;}
   inline const double* Coords_couleurs() const {return coords_col;}
   inline void Coords_textures(const double *v) {coords_tex = v;}
   inline const double* Coords_textures() const {return coords_tex;}
   inline void Coords_textures2(const double *v) {coords_tex2 = v;}
   inline const double* Coords_textures2() const {return coords_tex2;}
   Fonction_interpolation_couleur_polygone Fct_interpollation() const;
   void Fct_interpollation(Fonction_interpolation_couleur_polygone f);
  // Interpollation pour le perimètre du polygone
   inline void Perimetre_texture(const bool b) {Marqueur_attributs_poly(POLY_perimetre_texture(), b); A_changer(true);}
   inline const bool Perimetre_texture() const {return (POLY_perimetre_texture()&Marqueur_attributs_poly()) != 0;}
   inline void Coords_couleurs_perimetre(const double *v) {coords_col_perimetre = v;}
   inline const double* Coords_couleurs_perimetre() const {return coords_col_perimetre;}
   inline void Coords_textures_perimetre(const double *v) {coords_tex_perimetre = v;}
   inline const double* Coords_textures_perimetre() const {return coords_tex_perimetre;}
   Fonction_interpolation_couleur_polygone Fct_interpollation_perimetre() const;
   void Fct_interpollation_perimetre(Fonction_interpolation_couleur_polygone f);

   inline void maj() {bool b = Mode_texture2(); Mode_texture2(false);
                        maj_tess();
                        A_changer(true);
                        Pre_rendu_exeptionnel(true);
                      Mode_texture2(b);
                     }
   inline void maj_tess() {Trianguler(); Contourner();}
  // Les maj surcharge de la mise à jour du repère
   inline void maj( const double x, const double y
                  , const double r
                  , const double ex  , const double ey
                  , const double gx=0, const double gy=0)
    {alx_noeud_scene::maj(x, y, r, ex, ey, gx, gy);}
   inline void maj(const alx_repere2D &r)
    {alx_noeud_scene::maj(r.Origine(), r.Rotation(), r.Etirement(), r.Glissement());
     Ordre_rendu(r.Ordre_rendu());}
   inline void maj(const alx_point2D &o, const double r, const alx_vecteur2D &e)
    {Origine(o); Rotation(r); Etirement(e);}
   inline void maj(const alx_point2D &o, const double r, const alx_vecteur2D &e, const alx_vecteur2D &g)
    {Origine(o); Rotation(r); Etirement(e); Glissement(g);}

  //____________________________________________________________________________
   inline const INFOS_TEXTURE* Info_texture() const {return info_texture;}
   void Info_texture(INFOS_TEXTURE *i) {info_texture = i; texture_1_modif = true; Trianguler();}
   const alx_chaine_char& Texture_1_desiree() const {return cc_Texture_1_desiree;}
   inline void     abonner_a_maj_Info_texture_1(alx_methode_rappel *m) {L_rap_maj_Info_texture_1.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_maj_Info_texture_1(alx_methode_rappel *m) {L_rap_maj_Info_texture_1.Retirer_methode         (m);}

  //____________________________________________________________________________
   inline const INFOS_TEXTURE* Info_texture2() const {return info_texture2;}
   void Info_texture2(INFOS_TEXTURE *i) {info_texture2 = i; texture_2_modif = true; Trianguler(); A_changer(true);}
   const alx_chaine_char& Texture_2_desiree() const {return cc_Texture_2_desiree;}
   inline void     abonner_a_maj_Info_texture_2(alx_methode_rappel *m) {L_rap_maj_Info_texture_2.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_maj_Info_texture_2(alx_methode_rappel *m) {L_rap_maj_Info_texture_2.Retirer_methode         (m);}

   inline void Lisser(const bool b) {Marqueur_attributs_poly(POLY_lisser(), b); A_changer(true);}
   inline const bool Lisser() const {return (POLY_lisser()&Marqueur_attributs_poly()) != 0;}

          //__________________________________________________________//
   const double* Tab_pt(const unsigned int num_contour)                    const;
   const double* Tab_pt(const unsigned int num_contour, const unsigned num_pt) const;
   const alx_point2D& Pt(const unsigned int num_contour, const unsigned num_pt) const;
   const unsigned int Nb_pt(const unsigned int num_contour)                const;
   const unsigned int Est_contour_plein(const unsigned int num_contour)    const;
   const unsigned int Nb_contours()                                        const;

          //__________________________________________________________//
   void Ajouter_contour(const unsigned int nb_points, double *Tspt);
   inline void Ajouter_contour(Tab_double &tab) {Ajouter_contour((const unsigned int )tab.Taille()/2, (double*)tab.Tab() ); }
   void Retirer_contour(const unsigned int nb_points, double *Tspt);
   inline void Retirer_contour(Tab_double &tab) {Retirer_contour((const unsigned int )tab.Taille()/2, (double*)tab.Tab() ); }
   void Vider();
   inline void Difference   (alx_noeud_polygone_sdl_opengl &p, const alx_noeud_scene *n_racine = NULL) {Operation(p, n_racine, GPC_DIFF); }
   inline void Intersection (alx_noeud_polygone_sdl_opengl &p, const alx_noeud_scene *n_racine = NULL) {Operation(p, n_racine, GPC_INT);  }
   inline void Union        (alx_noeud_polygone_sdl_opengl &p, const alx_noeud_scene *n_racine = NULL) {Operation(p, n_racine, GPC_UNION);}
   inline void XOR          (alx_noeud_polygone_sdl_opengl &p, const alx_noeud_scene *n_racine = NULL) {Operation(p, n_racine, GPC_XOR);  }

   inline void Difference   (gpc_polygon *p) {Operation(p, GPC_DIFF);}
   inline void Intersection (gpc_polygon *p) {Operation(p, GPC_INT);}
   inline void Union        (gpc_polygon *p) {Operation(p, GPC_UNION);}
   inline void XOR          (gpc_polygon *p) {Operation(p, GPC_XOR);}

   inline void Maj_poly_gpc (gpc_polygon *p) {Vider(); Union(p);}
          void Maj_poly     (alx_noeud_polygone_sdl_opengl &p);
  //____________________________________________________________________________
   inline virtual void Calculer_boites() {alx_noeud_scene::Calculer_boite_fils(); alx_noeud_polygone_sdl_opengl::Calculer_boite_noeud();}
   virtual void Calculer_boite_noeud();

  //____________________________________________________________________________
   virtual void PreRendre();
   virtual void Dessin_texture_noeud();
   virtual void Dessin_noeud();
   void Afficher_polygone();
   void Afficher_perimetre();
   void Polygone_a_afficher(const bool b) {Marqueur_attributs_poly(POLY_polygone_a_afficher(), b);}
   const bool Polygone_a_afficher() const {return (POLY_polygone_a_afficher()&Marqueur_attributs_poly()) != 0;}
   void Perimetre_a_afficher(const bool b) {Marqueur_attributs_poly(POLY_perimetre_a_afficher(), b);}
   const bool Perimetre_a_afficher() const {return (POLY_perimetre_a_afficher()&Marqueur_attributs_poly()) != 0;}

   virtual info_du_contenant* Noeud_contient(const alx_point2D &pt, int action); // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.

 // Les surcharges pour la sérialisation
  // La différence _____________________________________________________________
  inline virtual const alx_chaine_char& Serialiser_difference_type() const {return Serialiser_difference_type( Serialiser_difference_contenu() );}
  virtual const alx_chaine_char& Serialiser_difference_type(const alx_chaine_char &diff_contenu) const;
  virtual const alx_chaine_char& Serialiser_difference_contenu() const;
  inline virtual const alx_chaine_char& Serialisation_differences_type()    const {return serialisation_differences_type_poly;}
  inline virtual const alx_chaine_char& Serialisation_differences_contenu() const {return serialisation_differences_contenu_poly;}
  // La sérialisation __________________________________________________________
  virtual const alx_chaine_char& Serialiser_type(const alx_chaine_char &contenu) const;
  virtual const alx_chaine_char& Serialiser_contenu() const;
  virtual const alx_chaine_char& Serialisation_contenu () const {return serialisation_contenu_poly;}
  inline virtual const alx_chaine_char& Serialiser_type() const {return Serialiser_type( Serialiser_contenu() );}
  virtual const alx_chaine_char& Serialisation_type    () const {return serialisation_type_poly;}
  // La désérialisation ________________________________________________________
  inline virtual void Deserialiser_type(const alx_chaine_char &txt) {unsigned int pos=0; this->Deserialiser_type(txt, pos);}
  virtual void Deserialiser_type(const alx_chaine_char &txt, unsigned int &pos);
  inline virtual void Deserialiser(const alx_chaine_char &txt) {unsigned int pos=0; Deserialiser(txt, pos);}
  virtual void Deserialiser(const alx_chaine_char &txt, unsigned int &pos);

  void Deseri_info_texture_1(const alx_chaine_char &t);
  const bool Seri_diff_info_texture_1_a_ete_change(const alx_chaine_char **) const;
  void Deseri_info_texture_2(const alx_chaine_char &t);
  const bool Seri_diff_info_texture_2_a_ete_change(const alx_chaine_char **) const;

};

typedef alx_noeud_polygone_sdl_opengl* P_alx_noeud_polygone_sdl_opengl;
inline alx_noeud_polygone_sdl_opengl* Void_vers_poly (void *p) {return (alx_noeud_polygone_sdl_opengl*)p;}

//_______________________________________________________________________________________________________________
//__________________ Définition des procédures d'interpolations de couleur ou texture ___________________________
//_______________________________________________________________________________________________________________
  void Fonction_interpolation_poly_boite_englobante( const alx_noeud_polygone_sdl_opengl &
                                                   , const double x, const double y
                                                   , const double *Tab_col, const double *Tab_tex, const double *Tab_tex2
                                                   , double *Col, double *Tex, double *Tex2
                                                   , void *params);
  inline Fonction_interpolation_couleur_polygone get_Fonction_interpolation_poly_boite_englobante() {return Fonction_interpolation_poly_boite_englobante;}

class Interpollateur_couleur_polygone
{private:
   double x, y, *Col, *Tex, *Tex2;
   void *params;
   Fonction_interpolation_couleur_polygone fct;
   void init();
   Liste_de_rappel L_rappel_avant_interpollation;

 public :
  // Les constructeurs________________
   Interpollateur_couleur_polygone()                                          {init();}
   Interpollateur_couleur_polygone(Fonction_interpolation_couleur_polygone f) {init(); Fct(f);}
   ~Interpollateur_couleur_polygone()                                         {}

  // Les méthodes d'accès____________________
   inline const double X() const {return x;}
   inline void X(const double v) {x = v;}
   inline const double Y() const {return y;}
   inline void Y(const double v) {y = v;}
   inline void Coords(const double vx, const double vy) {X(vx); Y(vy);}

   inline double* Couleur()       {return Col;}
   inline void Couleur(double *v) {Col = v;}
   inline double* Texture()       {return Tex;}
   inline void Texture(double *v) {Tex = v;}
   inline double* Texture2()       {return Tex2;}
   inline void Texture2(double *v) {Tex2 = v;}

   inline void* Params()       {return params;}
   inline void Params(void *v) {params = v;}

   inline void CTP(double *C, double *T1, double *T2)            {Couleur(C); Texture2(T2); Texture(T1);}
   inline void CTP(double *C, double *T1, double *T2, double *P) {Couleur(C); Texture2(T2); Texture(T1); Params(P);}

   inline Fonction_interpolation_couleur_polygone Fct()       {return fct;}
   inline void Fct(Fonction_interpolation_couleur_polygone f) {fct = f;}

  // Les méthodes métiers
   void Interpoller( const alx_noeud_polygone_sdl_opengl &
                   , const double *Tab_col, const double *Tab_tex, const double *Tab_tex2);
   inline void Interpoller( const alx_noeud_polygone_sdl_opengl &poly
                          , const double *Tab_col, const double *Tab_tex, const double *Tab_tex2
                          , const double vx, const double vy) {Coords(vx, vy);
                                                               Interpoller(poly, Tab_col, Tab_tex, Tab_tex2);}
};

#endif

