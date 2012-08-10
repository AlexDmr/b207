#include "alx_image.h"
#include "../physique/math_alex.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <SDL.h>
#include <string.h>
//#include <SDL_image.h>
#include <il.h>

#include <slimthread.h>

//#include <boost/filesystem/operations.hpp> // includes boost/filesystem/path.hpp
//#include <boost/filesystem/fstream.hpp>

  Uint32 rmask, gmask, bmask, amask
       , rdec , gdec , bdec , adec; // Les décalage (en bit) à faire.

//______________________________________________________________________________
void alx_image_32::init()
   {num_il_img = 0;
    mode_traitement_image_transparent = 0;
    R_TRANSPARENT = V_TRANSPARENT = B_TRANSPARENT = 0; SEUIL_TRANSPARENT = 64;
    Pixels_transparents_mtd_2(1, 1, 1, 0.2, 128, 128, 128);
	l = h = bpp = bpp_r = bpp_v = bpp_b = bpp_a = 0;
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

    image_processed_by_thread = thread_is_processing_image = false;
    thread_maj = NULL;
	force_update_after_threaded_maj = false;

    taille_canal_couleurs_pour_Table = 5;
    taille_Table_couleurs = 1<<(3*taille_canal_couleurs_pour_Table);
    Table_couleurs = new char[taille_Table_couleurs];
   }

//______________________________________________________________________________
void alx_image_32::Lock_mutex_tempon  () {mutex_tempon->lock();}

//______________________________________________________________________________
void alx_image_32::UnLock_mutex_tempon() {mutex_tempon->unlock();}

//______________________________________________________________________________
void alx_image_32::Pixels_transparents_mtd_3_V1(float r, float v, float b, float seuil, int min_r, int min_v, int min_b) {
  unsigned int dec = 8 - taille_canal_couleurs_pour_Table;
  unsigned char m_r = (unsigned char)min_r >> dec
              , m_v = (unsigned char)min_v >> dec
              , m_b = (unsigned char)min_b >> dec;
  int Table_R, Table_V, Table_B;
  float rap_rv = (1+r) / (1+v)
      , rap_rb = (1+r) / (1+b)
      , rap_vb = (1+v) / (1+b)
      , rb, rv, vb;
  unsigned int pos;
  for(pos = 0; pos < taille_Table_couleurs; pos++) {
     Table_R = pos                                             & ((1<<taille_canal_couleurs_pour_Table) - 1);
     Table_V = (pos >> taille_canal_couleurs_pour_Table)       & ((1<<taille_canal_couleurs_pour_Table) - 1);
     Table_B = (pos >> (taille_canal_couleurs_pour_Table<<1) ) & ((1<<taille_canal_couleurs_pour_Table) - 1);

     rv = (1+(float)Table_R) / (1+(float)Table_V);
     rb = (1+(float)Table_R) / (1+(float)Table_B);
     vb = (1+(float)Table_V) / (1+(float)Table_B);
     if( rv < rap_rv) {rv = rv / rap_rv;} else {rv = rap_rv / rv;}
     if( rb < rap_rb) {rb = rb / rap_rb;} else {rb = rap_rb / rb;}
     if( vb < rap_vb) {vb = vb / rap_vb;} else {vb = rap_vb / vb;}

     if( rv > seuil
       &&rb > seuil
       &&vb > seuil
       &&(unsigned char)Table_R > m_r
       &&(unsigned char)Table_V > m_v
       &&(unsigned char)Table_B > m_b
       ) {Table_couleurs[pos] = 0;
         } else {Table_couleurs[pos] = 0xFF;}

    }
}

//______________________________________________________________________________
void alx_image_32::Fixer_couleur_transparente_mtd_3() {
  if( nb_octets_par_pixel < 4 ) return;
  unsigned char *T = (unsigned char *)tempon;
  unsigned int r, v, b; unsigned int dec = 8 - taille_canal_couleurs_pour_Table;

  unsigned int pos = 0, size_img = 4*L()*H();
  while(pos < size_img) {
     r = T[pos++]>>dec;
     v = (T[pos++]>>dec)<<taille_canal_couleurs_pour_Table;
     b = (T[pos++]>>dec)<<(taille_canal_couleurs_pour_Table<<1);
     T[pos++] = Table_couleurs[ r | v | b ];
    }
}

//______________________________________________________________________________
void alx_image_32::Fixer_couleur_transparente_mtd_2(const float r, const float v, const float b, const float seuil, const int min_r, const int min_v, const int min_b)
{ if( nb_octets_par_pixel < 4 ) return;

  unsigned int pos = 0;
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

      if(rv > seuil
        &&rb > seuil
        &&vb > seuil
        &&(unsigned char)T[pos+0] > m_r
        &&(unsigned char)T[pos+1] > m_v
        &&(unsigned char)T[pos+2] > m_b
        ) {T[pos + 3] = 0;} else {T[pos + 3] = 0xFF;}

      pos += nb_octets_par_pixel;
     }
   }
}

//______________________________________________________________________________
void alx_image_32::Fixer_couleur_transparente_mtd_1(const int r, const int v, const int b, const int seuil)
{ if( nb_octets_par_pixel < 4 ) return;

  unsigned int pos = 0;
  unsigned char *T = (unsigned char *)tempon;

  for(int y=0; y < H(); y++)
   {for(int x=0; x < L(); x++)
     {if( nb_octets_par_pixel >= 4
        &&(int)T[pos+0] > (r - seuil) && (int)T[pos+0] < (r + seuil)
        &&(int)T[pos+1] > (v - seuil) && (int)T[pos+1] < (v + seuil)
        &&(int)T[pos+2] > (b - seuil) && (int)T[pos+2] < (b + seuil)
        ) {T[pos + 3] = 0;} else {T[pos + 3] = 0xFF;}

      pos += nb_octets_par_pixel;
     }
   }
}

//______________________________________________________________________________
const bool alx_image_32::Threaded_maj( const int tx, const int ty
        , const int source_ordre_couleur, const int source_nb_octet_par_pix
        , const int target_ordre_couleur, const int target_nb_octet_par_pix
        , const char *buffer) {
   if(thread_maj != NULL) {
     if(!thread_maj->Has_terminated()) {
        return false;
       }
     delete thread_maj;
    }
   thread_maj = new Thread_maj(this, tx, ty, source_ordre_couleur, source_nb_octet_par_pix, target_ordre_couleur, target_nb_octet_par_pix, buffer);
   thread_is_processing_image = true; image_processed_by_thread = false,
   thread_maj->resume();
   return true;
  }

//______________________________________________________________________________
void alx_image_32::Merge_Tempon_void(void *T_void, const unsigned int nb_T_pix)
{ unsigned int pos = 0, pos_T = 0;
  char *T = (char*) T_void;

  for(int y=0; y < H(); y++)
   {for(int x=0; x < L(); x++)
     {if( T[pos_T+3] ) {tempon[pos]   = T[pos_T];
                        tempon[pos+1] = T[pos_T+1];
                        tempon[pos+2] = T[pos_T+2];
                       }
      pos += nb_octets_par_pixel; pos_T += nb_T_pix;
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

    if(fopen_s(&f, nf, "rb") != 0) return false;

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
const bool alx_image_32::Sauver_dans_fichier(const char *nf)
{if(num_il_img > 0)
  {ilBindImage(num_il_img);
  } else {// Créer une image, charger les données
          ilGenImages(1, &num_il_img);
          ilBindImage(num_il_img);

          ILenum format_il;
          switch(ordonnancement_couleurs)
           {case GL_RGB             : format_il = IL_RGB;             break;
            case GL_BGR             : format_il = IL_BGR;             break;
            case GL_RGBA            : format_il = IL_RGBA;            break;
            case GL_BGRA            : format_il = IL_BGRA;            break;
            case GL_LUMINANCE       : format_il = IL_LUMINANCE;       break;
            case GL_LUMINANCE_ALPHA : format_il = IL_LUMINANCE_ALPHA; break;
            case GL_COLOR_INDEX     : format_il = IL_COLOR_INDEX;     break;
          //     case IL_COLOUR_INDEX: ordonnancement_couleurs = GL_COLOUR_INDEX; break;
            default:
              printf("Au secours !!! Pb de sauvegarde de %s", nf);
            break;
           }
          ilEnable(IL_FILE_OVERWRITE);

          ilTexImage( L()                 // ILuint Width
                    , H()                 // ILuint Height
                    , 0                   // ILuint Depth
                    , nb_octets_par_pixel // ILubyte Bpp
                    , format_il           // ILenum Format
                    , IL_UNSIGNED_BYTE    // ILenum Type
                    , NULL              // ILvoid *Data
                    );
         }

 ilSetData( tempon );
 wchar_t WC_nf[1024]; size_t size_converted;
 mbstowcs_s(&size_converted, WC_nf, nf, 1024);

 return ilSaveImage(WC_nf) == 0;
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
   wchar_t WC_nf[1024]; size_t size_converted;
   mbstowcs_s(&size_converted, WC_nf, f_name.Texte(), 1024);
   ilLoadImage( WC_nf );
   ILenum il_b = ilGetError();
   if ( il_b != IL_NO_ERROR )
    {printf("ERREUR DE CHARGEMENT DE L'IMAGE %s\n", f_name.Texte());
     switch(il_b) {
		 case IL_COULD_NOT_OPEN_FILE:
			 printf("- The file pointed to by FileName could not be opened. Either the file does not exist or is in use by another process.\n");
			 break;
		 case IL_ILLEGAL_OPERATION:
			 printf("- There is currently no image bound. Use ilGenImages and ilBindImage before calling this function.\n");
			 break;
		 case IL_INVALID_EXTENSION:
			 printf("- The file could not be loaded based on extension or header.\n");
			 break;
		 case IL_INVALID_PARAM:
			 printf("- FileName was not valid. It was most likely NULL.\n");
			 break;
		 default:
			 printf("Unknown error %c\n", il_b);
		}
     tempon=NULL;
     l = h = nb_octets_par_pixel = taille_tempon = 0;
     mutex_tempon->unlock();
     return false;
    }

   ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE);

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
//    unsigned char intensite;

    if(fopen_s(&f, n, "rb") != 0) return false;
    strcpy_s(nom_image, n);
    Inverser_y(false);

     // c'est partie!
     fsetpos(f, &p_l); fread(&p, 1, 1, f); fread(&g, 1, 1, f); l = 256*g + p;
     fsetpos(f, &p_h); fread(&p, 1, 1, f); fread(&g, 1, 1, f); h = 256*g + p;
     fsetpos(f, &p_t);
     int /*i, */t=l*h;

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
{maj_transfo(tx, ty, ordre_couleur, nb_octet_par_pix, ordre_couleur, nb_octet_par_pix, buffer);}

//______________________________________________________________________________
void alx_image_32::maj_transfo( const int tx, const int ty
                      , const int source_ordre_couleur, const int source_nb_octet_par_pix
                      , const int target_ordre_couleur, const int target_nb_octet_par_pix
                      , const char *buffer)
{if(target_nb_octet_par_pix == 0 || target_nb_octet_par_pix < source_nb_octet_par_pix) {printf("ERROR : trying to copy an image with too much byte per pixel (%d compared to %d)\n", source_nb_octet_par_pix, target_nb_octet_par_pix);}
 unsigned int t = tx*ty*target_nb_octet_par_pix/*nb_octet_par_pix*/;
 mutex_tempon->lock();
 if (t>taille_tempon) {
   if(tempon) delete[] tempon;
   taille_tempon = target_nb_octet_par_pix/*nb_octet_par_pix*/ * tx * ty;
   tempon = new char[taille_tempon];
  }

 if(buffer && tempon)
  {if(source_nb_octet_par_pix == target_nb_octet_par_pix)
    {memcpy(tempon, buffer, t);
    } else {const char *src_pos = buffer;
                  char *tgt_pos = tempon;
            for(int pos=0; pos < tx*ty; pos++)
             {for(int j=0; j<source_nb_octet_par_pix; j++) *(tgt_pos++) = *(src_pos++);
              tgt_pos += target_nb_octet_par_pix-source_nb_octet_par_pix;
             }
           }
  }

 Ordonnancement_couleurs (target_ordre_couleur);
 Nb_octets_par_pixel     (target_nb_octet_par_pix);
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

