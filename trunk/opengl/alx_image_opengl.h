#ifndef __ALX_IMAGE_OPENGL_H__
#define __ALX_IMAGE_OPENGL_H__

#include "alx_image.h"
#include "alx_sommet_opengl.h"
#include "config_opengl.h"

//#include <SDL_image.h>

class alx_image_opengl : public alx_image_32
{private:
   bool image_creer_en_interne, forcer_destruction_texture;
   GLuint id_text;
   double couleur[4];
   double etirement_x, etirement_y;
   INFOS_TEXTURE info_texture;
   bool texture_cree, image_creer;
   bool couleur_def, lisser, mipmap;
   bool translucidite;
   int  ordre_couleur_texture, nb_octets_par_pixels_texture;

   void init();
 public :
   //alx_sommet_opengl corp[4];

   alx_image_opengl(const bool mipmap = false);
   alx_image_opengl(const INFOS_TEXTURE info_text, const double taille_x, const double taille_y);
   alx_image_opengl(const char *n, const bool mipmap = false);
   alx_image_opengl(const alx_image_opengl &img);
   ~alx_image_opengl() {}

   inline const int Nb_octets_par_pixels_texture() const {return nb_octets_par_pixels_texture;}
   inline const int Ordre_couleur_texture       () const {return ordre_couleur_texture;}
   inline void Nb_octets_par_pixels_texture(const int v) {
     if(v != nb_octets_par_pixels_texture) forcer_destruction_texture = true;
     nb_octets_par_pixels_texture = v;
     if(forcer_destruction_texture) {maj_tempon();
                                     forcer_destruction_texture = false;}
    }
   inline void Ordre_couleur_texture       (const int v) {
     if(v != ordre_couleur_texture) forcer_destruction_texture = true;
     ordre_couleur_texture = v;
     if(forcer_destruction_texture) {maj_tempon();
                                     forcer_destruction_texture = false;}
    }

   inline const unsigned int Taille_Tempon () const {return alx_image_32::Taille_Tempon();}

   inline void Lisser(const bool b) {lisser = b;}
   inline const bool Lisser() const {return lisser;}
   inline void Image_creer_en_interne(const bool b) {image_creer_en_interne = b;}
   inline const bool Image_creer_en_interne() const {return image_creer_en_interne;}
   inline void MipMap(const bool b) {mipmap = b;}
   inline const bool MipMap() const {return mipmap;}

   inline const bool Couleur_def() const {return couleur_def;}
   inline void Couleur_def(bool e) {couleur_def = e;}
   inline const double* Couleur() const {return couleur;}
   inline const double Composante_couleur(const unsigned int i) {return couleur[i];}
   inline void Composante_couleur(const unsigned int i, const double v)
    {couleur[i] = v;}
    inline const double Couleur(const unsigned int i) {return Composante_couleur(i);}
    inline void Couleur(const unsigned int i, const double v)
     {Composante_couleur(i, v);}
   inline void Couleur(const double r, const double v, const double b, const double a)
    {couleur[0]=r; couleur[1]=v; couleur[2]=b; couleur[3]=a;}
   inline void Couleur(const double *c)
    {for(unsigned int i=0;i<4;i++) couleur[i] = c[i];}
   inline double* P_couleur(const unsigned int i) {return &(couleur[i]);}

   inline const double Etirement_x() const {return etirement_x;}
   inline const double Etirement_y() const {return etirement_y;}
   inline void Etirement_x(const double e) {etirement_x = e;}
   inline void Etirement_y(const double e) {etirement_y = e;}
   inline void Etirement(const double ex, const double ey) {Etirement_x(ex); Etirement_y(ey);}

   inline const bool Inverser_x() const {return alx_image_32::Inverser_x();}
   inline const bool Inverser_y() const {return alx_image_32::Inverser_y();}
   inline void Inverser_x(bool e) {alx_image_32::Inverser_x(e);}
   inline void Inverser_y(bool e) {alx_image_32::Inverser_y(e);}

   inline const double Lg() const {return L();}
   inline const double Ht() const {return H();}
   inline void Lg(const double v) {L(v);}
   inline void Ht(const double v) {H(v);}

   inline void Translucidite(const bool b) {translucidite = b;}
   inline const bool Translucidite() const {return translucidite;}

   inline const unsigned int Id_text()  const {return id_text;}
   inline       INFOS_TEXTURE* Ptr_Info_texture  ()       {return &info_texture;}
   inline       INFOS_TEXTURE& Info_texture      ()       {return info_texture;}
   inline const INFOS_TEXTURE& Info_texture_const() const {return info_texture;}
   void maj(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, const char *buffer);
   void maj(const char *n);
   void maj(const INFOS_TEXTURE &info);
   void maj(const alx_image_opengl &img);
   void maj(const alx_image_32 *img_32);
   void maj_tempon(const alx_image_32 *img_32);
   void maj_tempon();
   void afficher();
};

#endif
