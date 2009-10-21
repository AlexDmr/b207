#ifndef __ALX_IMAGE_H__
#define __ALX_IMAGE_H__

#include "config_opengl.h"

class Mutex;
class alx_image_32
{
 private:
 Mutex *mutex_tempon;
 bool tempon_a_effacer;
 unsigned int ordonnancement_couleurs
            , nb_octets_par_pixel
            , num_il_img;

  //BitmapImage Image_bitmap;
  int l, h;
  int bpp, bpp_r, bpp_v, bpp_b, bpp_a;
  char *tempon;
  unsigned int taille_tempon;
  char nom_image[256];

  void init();
  void prendre_N_dernier_char(unsigned int N, const char *chaine1, char *rep);

  bool inverser_x, inverser_y;

 public:
  alx_image_32()
   {init();}

  alx_image_32(const char *n);
 ~alx_image_32()
   {if(tempon) delete[]tempon;}

  void   Lock_mutex_tempon();
  void UnLock_mutex_tempon();

  inline const int L() const {return l;}
  inline const int H() const {return h;}
  inline void L(const int v) {l = v;}
  inline void H(const int v) {h = v;}

  inline const unsigned int Taille_Tempon () const {if(tempon) {return Nb_octets_par_pixel() * L() * H();} else {return 0;}}
  inline void Nb_octets_par_pixel(const unsigned int n)     {nb_octets_par_pixel = n;}
  inline const unsigned int Nb_octets_par_pixel()     const {return nb_octets_par_pixel;}
  inline const int Ordonnancement_couleurs() const {return ordonnancement_couleurs;}
  inline void Ordonnancement_couleurs(const int o) {ordonnancement_couleurs = o;}
  inline const char* NOM() const {return nom_image;}

  inline char* TEMPON      ()       {return tempon;}
  inline const char* TEMPON_const() const {return tempon;}
  char Rouge(const unsigned int x, const unsigned int y) const;
  char Vert (const unsigned int x, const unsigned int y) const;
  char Bleu (const unsigned int x, const unsigned int y) const;
  char Alpha(const unsigned int x, const unsigned int y) const;
  inline const char* Tempon_const() const {return tempon;}
  inline const unsigned int Taille() const {return Nb_octets_par_pixel()*L()*H();}

  inline const bool Inverser_x() const {return inverser_x;}
  inline const bool Inverser_y() const {return inverser_y;}
  inline void Inverser_x(bool e) {inverser_x = e;}
  inline void Inverser_y(bool e) {inverser_y = e;}

  /*********************************************************************************/
  void maj(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, const char *buffer);
  void maj(const char *n);
  void maj(const alx_image_32 &img);
  bool charger_bmp(const char *n);
  bool Charger_par_sdl(const char *nf);
  /*********************************************************************************/
  // L : luminosité du pixel ((r+v+b)/3)
  // mode 0 : alpha<-L
  // mode 1 : alpha<-255 si L > seuil, L sinon
  // mode 2 : alpha<-255 si L > seuil, 0 sinon
  // mode 3 : alpha<-L   si L > seuil, 0 sinon
  bool fixer_alpha(char *nf, int seuil, int mode);
};

#endif
