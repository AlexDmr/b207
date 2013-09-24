#ifndef __ALX_IMAGE_H__
#define __ALX_IMAGE_H__

#include "config_opengl.h"
#include <slimthread.h>
#include <stdio.h>

class Mutex;
class alx_image_32
{
 private:
 Mutex *mutex_tempon;
 bool tempon_a_effacer;
 unsigned int ordonnancement_couleurs
            , nb_octets_par_pixel
            , num_il_img;

  // Traitement d'image
  int R_TRANSPARENT, V_TRANSPARENT, B_TRANSPARENT, SEUIL_TRANSPARENT;
  int mode_traitement_image_transparent;
  float R_TRANSPARENT_MTD_2, V_TRANSPARENT_MTD_2, B_TRANSPARENT_MTD_2, SEUIL_TRANSPARENT_MTD_2;
  int   MIN_R, MIN_V, MIN_B;

  // BitmapImage Image_bitmap;
  int l, h;
  int bpp, bpp_r, bpp_v, bpp_b, bpp_a;
  char *tempon;
  unsigned int taille_tempon;
  char nom_image[256];

  void init();
  void prendre_N_dernier_char(unsigned int N, const char *chaine1, char *rep);

  bool inverser_x, inverser_y;
  char *Table_couleurs; unsigned int taille_Table_couleurs, taille_canal_couleurs_pour_Table;

 protected:
   class Thread_maj : public SlimThread {
     protected:
       int tx, ty, source_ordre_couleur, source_nb_octet_par_pix, target_ordre_couleur, target_nb_octet_par_pix;
       const char *buffer;
       bool has_terminated;
       alx_image_32 *img;

      public:
        Thread_maj( alx_image_32 *img, const int tx, const int ty
                  , const int source_ordre_couleur, const int source_nb_octet_par_pix
                  , const int target_ordre_couleur, const int target_nb_octet_par_pix
                  , const char *buffer ) : SlimThread(true) {
           this->img = img;
           this->tx  = tx;
           this->ty  = ty;
           this->source_ordre_couleur    = source_ordre_couleur;
           this->source_nb_octet_par_pix = source_nb_octet_par_pix;
           this->target_ordre_couleur    = target_ordre_couleur;
           this->target_nb_octet_par_pix = target_nb_octet_par_pix;
           this->buffer = buffer;
           has_terminated = false; //printf("Deb(%d)-", img->force_update_after_threaded_maj);
          }
        const bool Has_terminated() const {return has_terminated;}
        virtual void run(void) {
           img->maj_transfo(tx, ty, source_ordre_couleur, source_nb_octet_par_pix, target_ordre_couleur, target_nb_octet_par_pix, buffer, false);
           img->image_processed_by_thread = true; img->thread_is_processing_image = false;
           has_terminated = true;
		   img->force_update_after_threaded_maj = true; //printf("Fin(%d)-", img->force_update_after_threaded_maj);
          }
     };

   Thread_maj *thread_maj;

 public:
  bool image_processed_by_thread, thread_is_processing_image, force_update_after_threaded_maj;

  alx_image_32()
   {init();}

  alx_image_32(const char *n);
 ~alx_image_32()
   {if(tempon) delete[]tempon;}

  // Traitement d'images
  inline const int Mode_traitement_image_transparent() const {return mode_traitement_image_transparent;}
  inline void Mode_traitement_image_transparent(const int v) {mode_traitement_image_transparent = v;}

  inline void Fixer_couleur_transparente()
   {switch( Mode_traitement_image_transparent() )
     {case 1: Fixer_couleur_transparente_mtd_1(R_TRANSPARENT, V_TRANSPARENT, B_TRANSPARENT, SEUIL_TRANSPARENT); break;
      case 2: Fixer_couleur_transparente_mtd_2(R_TRANSPARENT_MTD_2, V_TRANSPARENT_MTD_2, B_TRANSPARENT_MTD_2, SEUIL_TRANSPARENT_MTD_2, MIN_R, MIN_V, MIN_B); break;
      case 3: Fixer_couleur_transparente_mtd_3();
     }
   }
  void Pixels_transparents_mtd_3_V1(float r, float v, float b, float seuil, int min_r, int min_v, int min_b);
  void Fixer_couleur_transparente_mtd_3();

  inline void Pixels_transparents_mtd_1(int r, int v, int b, int seuil) {R_TRANSPARENT = r; V_TRANSPARENT = v; B_TRANSPARENT = b; SEUIL_TRANSPARENT = seuil;}
  void Fixer_couleur_transparente_mtd_1(const int r, const int v, const int b, const int seuil);
  inline void Pixels_transparents_mtd_2(float r, float v, float b, float seuil, int min_r, int min_v, int min_b) {
    R_TRANSPARENT_MTD_2 = r; V_TRANSPARENT_MTD_2 = v; B_TRANSPARENT_MTD_2 = b; SEUIL_TRANSPARENT_MTD_2 = seuil;
    MIN_R = min_r; MIN_V = min_v; MIN_B = min_b;
   }
  void Fixer_couleur_transparente_mtd_2(const float r, const float v, const float b, const float seuil, const int min_r, const int min_v, const int min_b);
  void Merge_Tempon_void(void *T, const unsigned int nb_T_pix);

  void Colorier(const int r, const int v, const int b, const int a);

  // Autres
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
  inline const void* Tempon_void() const {return tempon;}
  inline const unsigned int Taille() const {return Nb_octets_par_pixel()*L()*H();}

  inline const bool Inverser_x() const {return inverser_x;}
  inline const bool Inverser_y() const {return inverser_y;}
  inline void Inverser_x(bool e) {inverser_x = e;}
  inline void Inverser_y(bool e) {inverser_y = e;}

  /*********************************************************************************/
  const bool Sauver_dans_fichier(const char *nf);
  const bool Threaded_maj( const int tx, const int ty
          , const int source_ordre_couleur, const int source_nb_octet_par_pix
          , const int target_ordre_couleur, const int target_nb_octet_par_pix
          , const char *buffer);
  void maj_transfo( const int tx, const int ty
          , const int source_ordre_couleur, const int source_nb_octet_par_pix
          , const int target_ordre_couleur, const int target_nb_octet_par_pix
          , const char *buffer, const bool update_openGL_texture = true);
  void maj(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, const char *buffer);
  void maj(const char *n);
  void maj(const alx_image_32 &img);
  virtual void maj_tempon() {}

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
