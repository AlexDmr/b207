#include "alx_image.h"
#include "../physique/math_alex.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <extgl.h>
#include <SDL.h>
#include <string.h>
//#include <SDL_image.h>
#include <il.h>

#include <slimthread.h>

//#include <boost/filesystem/operations.hpp> // includes boost/filesystem/path.hpp
//#include <boost/filesystem/fstream.hpp>

  Uint32 rmask, gmask, bmask, amask
       , rdec , gdec , bdec , adec; // Les d�calage (en bit) � faire.

//______________________________________________________________________________
void alx_image_32::init()
   {num_il_img = 0;
    mode_traitement_image_transparent = 0;
    R_TRANSPARENT = V_TRANSPARENT = B_TRANSPARENT = 0; SEUIL_TRANSPARENT = 64;
    Pixels_transparents_mtd_2(1, 1, 1, 0.2, 128, 128, 128);
    Inverser_x( false );
    Inverser_y( false );
    tempon_a_effacer = false;
    nom_image[0] = 0;
    tempon = NULL;
    taille_tempon = 0;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000; rdec = 24;
      gmask = 0x00ff0000; gdec = 16;
      bmask = 0x0000ff00; bdec = 8;
      amask = 0x000000ff; adec = 0;
     #else
      rmask = 0x000000ff; rdec = 0;
      gmask = 0x0000ff00; gdec = 8;
      bmask = 0x00ff0000; bdec = 16;
      amask = 0xff000000; adec = 24;
    #endif
    //ilGenImages(1, &ImageName);
    mutex_tempon = new Mutex();
   }

//______________________________________________________________________________
void alx_image_32::Lock_mutex_tempon  () {mutex_tempon->lock();}

//______________________________________________________________________________
void alx_image_32::UnLock_mutex_tempon() {mutex_tempon->unlock();}

//______________________________________________________________________________
void alx_image_32::Fixer_couleur_transparente_mtd_2(const float r, const float v, const float b, const float seuil, const int min_r, const int min_v, const int min_b)
{ unsigned int pos = 0;
  unsigned char *T = (unsigned char *)tempon
              , m_r = (unsigned char)min_r
              , m_v = (unsigned char)min_v
              , m_b = (unsigned char)min_b;
  float rap_rv = (1+r) / (1+v)
      , rap_rb = (1+r) / (1+b)
      , rap_vb = (1+v) / (1+b)
      , rb, rv, vb;

  for(int y=0; y < H(); y++)
   {for(int x=0; x < L(); x++)
     {rv = (1+(float)T[pos+0]) / (1+(float)T[pos+1]);
      rb = (1+(float)T[pos+0]) / (1+(float)T[pos+2]);
      vb = (1+(float)T[pos+1]) / (1+(float)T[pos+2]);
      if( rv < rap_rv) {rv = rv / rap_rv;} else {rv = rap_rv / rv;}
      if( rb < rap_rb) {rb = rb / rap_rb;} else {rb = rap_rb / rb;}
      if( vb < rap_vb) {vb = vb / rap_vb;} else {vb = rap_vb / vb;}
      if( rv > seuil
        &&rb > seuil
        &&vb > seuil
        &&(unsigned char)T[pos+0] > m_r
        &&(unsigned char)T[pos+1] > m_v
        &&(unsigned char)T[pos+2] > m_b
        ) {T[pos + 3] = 0;} else {T[pos + 3] = 0xFF;}

      pos += 4;
     }
   }
}

//______________________________________________________________________________
void alx_image_32::Fixer_couleur_transparente_mtd_1(const int r, const int v, const int b, const int seuil)
{ unsigned int pos = 0;
  unsigned char *T = (unsigned char *)tempon;

  for(int y=0; y < H(); y++)
   {for(int x=0; x < L(); x++)
     {if( (int)T[pos+0] > (r - seuil) && (int)T[pos+0] < (r + seuil)
        &&(int)T[pos+1] > (v - seuil) && (int)T[pos+1] < (v + seuil)
        &&(int)T[pos+2] > (b - seuil) && (int)T[pos+2] < (b + seuil)
        ) {T[pos + 3] = 0;} else {T[pos + 3] = 0xFF;}

      pos += 4;
     }
   }
}

//______________________________________________________________________________
void alx_image_32::Merge_Tempon_void_4(void *T_void)
{ unsigned int pos = 0;
  char *T = (char*) T_void;

  for(int y=0; y < H(); y++)
   {for(int x=0; x < L(); x++)
     {if( T[pos+3] ) {tempon[pos]   = T[pos];
                      tempon[pos+1] = T[pos+1];
                      tempon[pos+2] = T[pos+2];
                     }
      pos += 4;
     }
   }
}

//______________________________________________________________________________
void alx_image_32::Colorier(const int r, const int v, const int b, const int a)
{unsigned char R = r, V = v, B = b, A = a, *T = (unsigned char*) tempon;
 unsigned int pos = 0;

 for(int y=0; y < H(); y++)
  {for(int x=0; x < L(); x++)
    {T[pos+0] = R;
     T[pos+1] = V;
     T[pos+2] = B;
     T[pos+3] = A;
     pos += 4;
    }
  }
}

//______________________________________________________________________________
bool alx_image_32::fixer_alpha(char *nf, int seuil, int mode)
   {
    if( mode<0 || mode > 3) return false;

    FILE *f;
    char p, g;
    int L;
    unsigned char r, v, b;
    fpos_t p_l = 0x12
         , p_h = 0x16
         , p_t = 54;

    f = fopen(nf, "rb");
    if(f==NULL) return false;

     // c'est partie!
     fsetpos(f, &p_l); fread(&p, 1, 1, f); fread(&g, 1, 1, f); if(l != 256*g + p) return false;
     fsetpos(f, &p_h); fread(&p, 1, 1, f); fread(&g, 1, 1, f); if(h != 256*g + p) return false;
     fsetpos(f, &p_t);
     int i, t=l*h;

     for(i=0;i<t;i++)
      {
       // Lecture pixel par pixel.
       fread(&b, 1, 1, f); //bleu
       fread(&v, 1, 1, f); //vert
       fread(&r, 1, 1, f); //rouge
       L = (r+v+b)/3;
       ((unsigned int*)tempon)[i] &= ~amask;
       switch(mode)
        {
         case 0: ((unsigned int*)tempon)[i] = L; break;
         case 1: if(L > seuil) ((unsigned int*)tempon)[i] |= 0xFF << adec; else ((unsigned int*)tempon)[i] |= L << adec; break;
         case 2: if(L > seuil) ((unsigned int*)tempon)[i] |= 0xFF << adec; else ((unsigned int*)tempon)[i] |= 0 << adec; break;
         case 3: if(L > seuil) ((unsigned int*)tempon)[i] |= L << adec;    else ((unsigned int*)tempon)[i] |= 0 << adec; break;
        }
      }
    fclose(f);
    return true;
   }

//______________________________________________________________________________
bool alx_image_32::Charger_par_sdl(const char *nf)
{mutex_tempon->lock();
   if(num_il_img > 0)
    {ilDeleteImages(1, &num_il_img);}
   ilGenImages(1, &num_il_img);
   ilBindImage(num_il_img);
   alx_chaine_char f_name( nf );
     f_name.Remplacer_char_par('/', '\\');
   if (!ilLoadImage( f_name.Texte() ))
    {printf("ERREUR DE CHARGEMENT DE L'IMAGE %s", f_name.Texte());
     tempon=NULL;
     l = h = nb_octets_par_pixel = taille_tempon = 0;
     mutex_tempon->unlock();
     return false;
    }
   nb_octets_par_pixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
   switch(ilGetInteger(IL_IMAGE_FORMAT))
    {case IL_RGB       : ordonnancement_couleurs = GL_RGB;       break;
     case IL_BGR       : ordonnancement_couleurs = GL_BGR;       break;
     case IL_RGBA      : ordonnancement_couleurs = GL_RGBA;      break;
     case IL_BGRA      : ordonnancement_couleurs = GL_BGRA;      break;
     case IL_LUMINANCE : ordonnancement_couleurs = GL_LUMINANCE; break;
     case IL_LUMINANCE_ALPHA : ordonnancement_couleurs = GL_LUMINANCE_ALPHA; break;
     case IL_COLOR_INDEX: ordonnancement_couleurs = GL_COLOR_INDEX; break;
//     case IL_COLOUR_INDEX: ordonnancement_couleurs = GL_COLOUR_INDEX; break;
     default:
       printf("Au secours !!! Pb de chargement de %s", nf);
     break;
    }

   l = ilGetInteger(IL_IMAGE_WIDTH);
   h = ilGetInteger(IL_IMAGE_HEIGHT);
   unsigned int t = nb_octets_par_pixel * l * h;
   if (t>taille_tempon) {
     if(tempon) delete[] tempon;
     taille_tempon = nb_octets_par_pixel * l * h;
     tempon = new char[taille_tempon];
    }
   memcpy(tempon, ilGetData(), t);

 if( Mode_traitement_image_transparent() ) {Fixer_couleur_transparente();}
 mutex_tempon->unlock();
 return true;
}

//______________________________________________________________________________
bool alx_image_32::charger_bmp(const char *n)
   {mutex_tempon->lock();
    FILE *f;
    unsigned char p, g;
    fpos_t p_l = 0x12
         , p_h = 0x16
         , p_t = 54;
    //Uint32 r, v, b, a;
    unsigned char intensite;

    f = fopen(n, "rb");
    if(f==NULL) return false;
    strcpy(nom_image, n);
    Inverser_y(false);

     // c'est partie!
     fsetpos(f, &p_l); fread(&p, 1, 1, f); fread(&g, 1, 1, f); l = 256*g + p;
     fsetpos(f, &p_h); fread(&p, 1, 1, f); fread(&g, 1, 1, f); h = 256*g + p;
     fsetpos(f, &p_t);
     int i, t=l*h;

     tempon = new char[4*t];
     taille_tempon = sizeof(unsigned int)*t;
     //memset(tempon, 0xFF, 4*t);

     ordonnancement_couleurs = GL_BGR;
     nb_octets_par_pixel = 3;

     /*a = 0xFF << adec;
     int dec;
     if(adec == 24) dec = 0;
      else dec = 8;*/
     fread(tempon, 3*t, 1, f);
     //for(i=0;i<t;i++)
      {
       //fread(&(((unsigned int*)tempon)[i]), 3, 1, f);
       //((unsigned int*)tempon)[i] = ((unsigned int*)tempon)[i]<<dec | a;
      /*
       // Lecture pixel par pixel.
       fread(&intensite, 1, 1, f); //bleu
       b = intensite; b = b << bdec;
       fread(&intensite, 1, 1, f); //vert
       v = intensite; v = v << gdec;
       fread(&intensite, 1, 1, f); //rouge
       r = intensite; r = r << rdec;
       a = 0xFF << adec;
       ((unsigned int*)tempon)[i] = r //& rmask
                                  | v //& gmask
                                  | b //& bmask
                                  | a ;//& amask; */
      }
    fclose(f);

    if( Mode_traitement_image_transparent() ) {Fixer_couleur_transparente();}
    mutex_tempon->unlock();
    return true;
   }

//______________________________________________________________________________
void alx_image_32::prendre_N_dernier_char(unsigned int N, const char *chaine1, char *rep)
{unsigned int taille = 0, deb, zero = 0;
 while(chaine1[taille]!=0) taille++;
 deb = Amax(zero, taille-N);
 for(unsigned int i=deb;i<taille;i++)
  rep[i-deb] = chaine1[i];
}

//______________________________________________________________________________
void alx_image_32::maj(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, const char *buffer)
{unsigned int t = tx*ty*nb_octet_par_pix;
 mutex_tempon->lock();
 if (t>taille_tempon) {
   if(tempon) delete[] tempon;
   taille_tempon = nb_octet_par_pix * tx * ty;
   tempon = new char[taille_tempon];
  }
 if(buffer && tempon)
   memcpy(tempon, buffer, t);
 Ordonnancement_couleurs (ordre_couleur);
 Nb_octets_par_pixel     (nb_octet_par_pix);
 l = tx;
 h = ty;

 if( Mode_traitement_image_transparent() ) {Fixer_couleur_transparente();}
 mutex_tempon->unlock();
}

//______________________________________________________________________________
void alx_image_32::maj(const alx_image_32 &img)
{mutex_tempon->lock();
 ordonnancement_couleurs = img.Ordonnancement_couleurs();
 nb_octets_par_pixel     = img.Nb_octets_par_pixel();
 l = img.L();
 h = img.H();
 //bpp, bpp_r, bpp_v, bpp_b, bpp_a;
 unsigned int t = img.Taille();
 if (t>taille_tempon) {
   if(tempon) delete[] tempon;
   taille_tempon = nb_octets_par_pixel * l * h;
   tempon = new char[taille_tempon];
  }
 memcpy(tempon, img.Tempon_const(), t);
 memcpy(nom_image, img.NOM(), 256);

 Inverser_y( img.Inverser_y() );
 Inverser_x( img.Inverser_x() );

 if( Mode_traitement_image_transparent() ) {Fixer_couleur_transparente();}
 mutex_tempon->unlock();
}

//______________________________________________________________________________
void alx_image_32::maj(const char *n)
{char tmp[5];
 tmp[4] = 0;
 prendre_N_dernier_char(4, n, tmp);
   Charger_par_sdl(n);
}

//______________________________________________________________________________
alx_image_32::alx_image_32(const char *n)
{init();
 maj(n);}

//______________________________________________________________________________
char alx_image_32::Rouge(const unsigned int x, const unsigned int y) const
{if( (ordonnancement_couleurs==GL_RGBA)
   ||(ordonnancement_couleurs==GL_RGB) ) return tempon[nb_octets_par_pixel*(x+l*y)];
 // sinon, BGR ou BGRA
 return tempon[nb_octets_par_pixel*(x+l*y)+2];
}

char alx_image_32::Vert (const unsigned int x, const unsigned int y) const
{return tempon[nb_octets_par_pixel*(x+l*y)+1];}

char alx_image_32::Bleu (const unsigned int x, const unsigned int y) const
{if( (ordonnancement_couleurs==GL_RGBA)
   ||(ordonnancement_couleurs==GL_RGB) ) return tempon[nb_octets_par_pixel*(x+l*y)+2];
 // sinon, BGR ou BGRA
 return tempon[nb_octets_par_pixel*(x+l*y)];
}

char alx_image_32::Alpha(const unsigned int x, const unsigned int y) const
{if(nb_octets_par_pixel<4) return 0xFF;
 return tempon[nb_octets_par_pixel*(x+l*y)+3];
}

