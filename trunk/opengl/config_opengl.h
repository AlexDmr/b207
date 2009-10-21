#ifndef __CONFIG_OPENGL_H__
#define __CONFIG_OPENGL_H__

#include <extgl.h>
/*
extern "C" {
#include <glu.h>
#include <glut.h>
}
*/

//#include "..\physique\alx_geometrie2D.h"
#include "..\physique\alx_point2D.h"
#include "..\physique\alx_vecteur2D.h"
#include "..\physique\alx_repere2D.h"
//#define alx_fichegl_stencil 0x01

// This inits our screen translations and projections
//void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
//void InitializeOpenGL(int width, int height);

// Reféfinition des énumérations OpenGL
//___________________________________________
void Desactiver_rendu_du_buffer_couleur() {glDrawBuffer(GL_NONE);}
void Activer_rendu_du_buffer_couleur()    {glDrawBuffer(GL_BACK);}
//___________________________________________
const GLenum GL_keep()    {return GL_KEEP;}
const GLenum GL_zero()    {return GL_ZERO;}
const GLenum GL_replace() {return GL_REPLACE;}
const GLenum GL_incr()    {return GL_INCR;}
const GLenum GL_decr()    {return GL_DECR;}
const GLenum GL_invert()  {return GL_INVERT;}

//___________________________________________
const GLenum GL_never()    {return GL_NEVER;}
const GLenum GL_less()     {return GL_LESS;}
const GLenum GL_lequal()   {return GL_LEQUAL;}
const GLenum GL_greater()  {return GL_GREATER;}
const GLenum GL_gequal()   {return GL_GEQUAL;}
const GLenum GL_equal()    {return GL_EQUAL;}
const GLenum GL_notequal() {return GL_NOTEQUAL;}
const GLenum GL_always()   {return GL_ALWAYS;}

//___________________________________________
const GLenum GL_points()              {return GL_POINTS;}
const GLenum GL_lines()               {return GL_LINES;}
const GLenum GL_line_strip()          {return GL_LINE_STRIP;}
const GLenum GL_line_loop()           {return GL_LINE_LOOP;}
const GLenum GL_triangles()           {return GL_TRIANGLES;}
const GLenum GL_triangle_strip()      {return GL_TRIANGLE_STRIP;}
const GLenum GL_triangle_fan()        {return GL_TRIANGLE_FAN;}
const GLenum GL_triangle_quads()      {return GL_QUADS;}
const GLenum GL_triangle_quad_strip() {return GL_QUAD_STRIP;}
const GLenum GL_triangle_polygon()    {return GL_POLYGON;}

//___________________________________________
//const GLint GL_replace()         {return GL_REPLACE;}
const GLenum GL_modulate()        {return GL_MODULATE;}
const GLenum GL_add()             {return GL_ADD;}
const GLenum GL_add_signed_ARB()  {return GL_ADD_SIGNED_ARB;}
const GLenum GL_interpolate_ARB() {return 0x8575;}
const GLenum GL_combine()         {return GL_COMBINE;}
const GLenum GL_combine_ARB()     {return GL_COMBINE_ARB;}
const GLenum GL_blend()           {return GL_BLEND;}
const GLenum GL_decal()           {return GL_DECAL;}
const GLenum GL_sub()             {return 0x84E7;}

const GLenum GL_rgb_scale_ARB()   {return 0x8573;}

//___________________________________________
const GLenum GL_texture          () {return GL_TEXTURE;}
const GLenum GL_constant_ARB     () {return GL_CONSTANT_ARB;}
const GLenum GL_primary_color_ARB() {return GL_PRIMARY_COLOR_ARB;}
const GLenum GL_previous_ARB     () {return GL_PREVIOUS_ARB;}

const GLenum GL_src_color        () {return GL_SRC_COLOR;}
const GLenum GL_1_moins_src_color() {return GL_ONE_MINUS_SRC_COLOR;}
const GLenum GL_src_alpha        () {return GL_SRC_ALPHA;}
const GLenum GL_1_moins_src_alpha() {return GL_ONE_MINUS_SRC_ALPHA;}

//___________________________________________
const unsigned int GLenum2UI(const GLenum e) {return (unsigned int)e;}
//___________________________________________

// Gestion du stencil buffer
void Initialiser_tempon();
const unsigned int Tempon();
void Tempon(const unsigned int t);
const unsigned int Incr_tempon();
const unsigned int Decr_tempon();
const unsigned int Nouveau_tempon();
void Op_stencil(GLenum op_non, GLenum op_oui)
  {glStencilOp(op_non, op_oui, op_oui);}
void Fct_stencil(GLenum fct, const unsigned int val, unsigned int masque = ~0)
  {glStencilFunc(fct, val, masque);}
void Activer_stencil(GLenum fct, GLenum op, unsigned int val)
{
 glPushAttrib(GL_STENCIL_BUFFER_BIT);
   glEnable(GL_STENCIL_TEST);
   Op_stencil(GL_KEEP, op);
   Fct_stencil(fct, val, 127);
}
void Depiler_stencil()
{
 glPopAttrib();
}

// Gestion de la caméra OpenGL
void camera_pour_ecran_relatif(const double ex, const double angle_degre, const double rapport_hauteur_largeur);
void camera_pour_ecran(const double ex, const double angle, const double rapport_hauteur_largeur);
void camera_pour_ecran( const alx_point2D &coin_bg, const alx_vecteur2D &v_largeur
                      , const double hauteur, const double angle_degre, const double rapport_hauteur_largeur);

void Positionner( const alx_vecteur2D &position
                , const double angle_radian
                , const alx_point2D &centre_rotation );

void Etirer(const alx_vecteur2D &e);

  void Changement_repere(const alx_repere2D &rep);
  void Changement_repere_ordre_inverse(const alx_repere2D &rep);

const int GL_rvb()  {return GL_RGB;}
const int GL_rvba() {return GL_RGBA;}
const int GL_bvr()  {return GL_BGR;}
const int GL_bvra() {return GL_BGRA;}

class Liste_de_rappel;
class alx_methode_rappel;
class INFOS_TEXTURE
{private:
   Liste_de_rappel *L_rappel_changement_dim, *L_rappel_changement_data;
   alx_chaine_char nom, info_data, cc_tmp;
   mutable bool data_modifiees;

 public:
   int start_x, start_y, tx, ty;
   int taille_reelle[2];
   int id_texture;

   INFOS_TEXTURE(const char *n = "");
   void Copier     (const INFOS_TEXTURE &info);
   inline const int  Taille_reelle_x() const {return taille_reelle[0];}
   inline const int  Taille_reelle_y() const {return taille_reelle[1];}
   inline const int  Start_x()         const {return start_x;}
   inline const int  Start_y()         const {return start_y;}
   inline const int  Tx()              const {return tx;}
   inline const int  Ty()              const {return ty;}
   inline const int  Id_texture()      const {return id_texture;}
   inline const bool Data_modifiees()  const {return data_modifiees;}
   inline void Data_modifiees(const bool b) const {data_modifiees = b;}

   inline void Start_x(const int v)         {start_x = v;}
   inline void Start_y(const int v)         {start_y = v;}
   inline void Tx(const int v)              {tx      = v;}
   inline void Ty(const int v)              {ty      = v;}

   inline const alx_chaine_char& Nom() const    {return nom;}
   inline void Nom   (const char *n)            {nom = n;}
   inline void Nom_CC(const alx_chaine_char &n) {nom = n;}
   inline alx_chaine_char* Ptr_Nom() {return &nom;}
   inline const alx_chaine_char* Ptr_Nom_const() const {return &nom;}

   inline const alx_chaine_char& Info_data() const    {return info_data;}
   inline void Info_data   (const char *n)            {info_data = n; Informer_changement_data();}
   inline void Info_data_CC(const alx_chaine_char &n) {info_data = n; Informer_changement_data();}

   void Informer_changement_data      ();
   void abonner_a_changement_data     (alx_methode_rappel *m);
   void desabonner_de_changement_data (alx_methode_rappel *m);

   void Informer_changement_dim      ();
   void abonner_a_changement_dim     (alx_methode_rappel *m);
   void desabonner_de_changement_dim (alx_methode_rappel *m);

   const INFOS_TEXTURE &operator =(const INFOS_TEXTURE &);
};

void CreerTexture(int *id_texture, const int tx, const int ty, const unsigned int nb_octet_par_pixels, const int mode_video, INFOS_TEXTURE *info_texture);
int puissance_de_2_superieur(const int nb, int puissance_min);

template <class T> class Couleur_RVBA
{private:
   T tab_couleur[4];

 public:
  // Constructeurs
   Couleur_RVBA()       {}
   Couleur_RVBA(const T *tab) {maj_couleur(tab);}
   Couleur_RVBA(const T &r, const T &v, const T &b, const T &a) {maj_couleur(r, v, b, a);}

  // Modificateurs
   inline void Rouge(const T v) {tab_couleur[0] = v;}
   inline void Vert (const T v) {tab_couleur[1] = v;}
   inline void Bleu (const T v) {tab_couleur[2] = v;}
   inline void Alpha(const T v) {tab_couleur[3] = v;}
   inline void Indice(const unsigned int i, const T v) {tab_couleur[i] = v;}
   inline void Tab_couleur(const T &r, const T &v, const T &b, const T &a) {Rouge(r); Vert(v); Bleu(b); Alpha(a);}
   inline void Tab_couleur(const T *t) {memcpy(this->tab_couleur, t, 4*sizeof(T));}

   inline void maj_couleur(const T &r, const T &v, const T &b, const T &a) {Rouge(r); Vert(v); Bleu(b); Alpha(a);}
   inline void maj_couleur(const T *t) {memcpy(this->tab_couleur, t, 4*sizeof(T));}

  // Méthodes d'accès
   inline const T Rouge() const {return tab_couleur[0];}
   inline const T Vert () const {return tab_couleur[1];}
   inline const T Bleu () const {return tab_couleur[2];}
   inline const T Alpha() const {return tab_couleur[3];}
   inline const T Indice(const unsigned int i) const {return tab_couleur[i];}
   inline const T* Tab_couleur() const {return tab_couleur;}
};

typedef int ALXGL_TYPE_RENDU;
#define ALXGL_RENDU_DANS_TEXTURE                  1
#define ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE 2
#define ALXGL_RENDU_STENCIL                       4

#endif
