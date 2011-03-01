#include "alx_image_opengl.h"
#include <glu.h>
#include <time.h>

void alx_image_opengl::init()
{texture_cree = false;
 image_creer  = false;
 lisser = true;
 Couleur_def(true);
 Couleur(1,1,1,1);
 Image_creer_en_interne(true);
 translucidite = false;
 MipMap(false);
 ordre_couleur_texture        = GL_bvra();
 nb_octets_par_pixels_texture = 4;
 forcer_destruction_texture   = false;
}


//______________________________________________________________________________
alx_image_opengl::alx_image_opengl(const bool mipmap) : alx_image_32()
{init();
 MipMap(mipmap);}

//______________________________________________________________________________
alx_image_opengl::alx_image_opengl(const INFOS_TEXTURE info_text, const double taille_x, const double taille_y)
{init();
 Image_creer_en_interne(false);
 texture_cree = true;
 image_creer  = false;
 info_texture.Copier( info_text );
 info_texture.taille_reelle[0] = taille_x;
 info_texture.taille_reelle[1] = taille_y;
 Etirement_x( taille_x );
 Etirement_y( taille_y );
}

//______________________________________________________________________________
void alx_image_opengl::maj(const INFOS_TEXTURE &info)
{Image_creer_en_interne(false);
 texture_cree = true;
 image_creer  = false;
 info_texture.Copier( info );
}

//______________________________________________________________________________
void alx_image_opengl::maj(const alx_image_opengl &img)
{Couleur( img.Couleur() );
 etirement_x  = img.etirement_x;
 etirement_y  = img.etirement_y;
 texture_cree = false;
 image_creer  = false;
 Inverser_x( img.Inverser_x() );
 Inverser_y( img.Inverser_y() );
 couleur_def  = img.couleur_def;
 lisser       = img.lisser;
 MipMap( img.MipMap() );

 alx_image_32::maj( img );
 maj(&img);
 Image_creer_en_interne(true);

 info_texture.Copier( img.Info_texture_const() );
}

//______________________________________________________________________________
alx_image_opengl::alx_image_opengl(const alx_image_opengl &img)
{init();
 maj(img);}

//______________________________________________________________________________
alx_image_opengl::alx_image_opengl(const char *n, const bool mipmap) : alx_image_32(n)
{init();
 MipMap( mipmap );
 texture_cree = false;

 image_creer = true;
 maj_tempon();
 Image_creer_en_interne(true);
 alx_chaine_char cc_tmp;
 cc_tmp  = (unsigned long int)time(NULL);
 cc_tmp += " ";
 cc_tmp += n;
 Info_texture().Info_data_CC( cc_tmp );
}

//______________________________________________________________________________
void alx_image_opengl::maj(const char *n)
{alx_image_32::maj(n);
 image_creer = true;
 maj_tempon();
 Image_creer_en_interne(true);
 alx_chaine_char cc_tmp;
 cc_tmp  = (unsigned long int)time(NULL);
 cc_tmp += " ";
 cc_tmp += n;
 Info_texture().Info_data_CC( cc_tmp );
}

//______________________________________________________________________________
void alx_image_opengl::maj_transfo( const int tx, const int ty
        , const int source_ordre_couleur, const int source_nb_octet_par_pix
        , const int target_ordre_couleur, const int target_nb_octet_par_pix
        , const char *buffer)
{alx_image_32::maj_transfo(tx, ty, source_ordre_couleur, source_nb_octet_par_pix, target_ordre_couleur, target_nb_octet_par_pix, buffer);
 if(buffer)
  {image_creer = true;
   maj_tempon();
   Image_creer_en_interne(true);}
 alx_chaine_char cc_tmp;
 cc_tmp  = (unsigned long int)time(NULL);
 cc_tmp += " ";
 Info_texture().Info_data_CC( cc_tmp );
}

//______________________________________________________________________________
void alx_image_opengl::maj(const int tx, const int ty, const int ordre_couleur, const int nb_octet_par_pix, const char *buffer)
{alx_image_32::maj(tx, ty, ordre_couleur, nb_octet_par_pix, buffer);
 if(buffer)
  {image_creer = true;
   maj_tempon();
   Image_creer_en_interne(true);}
 alx_chaine_char cc_tmp;
 cc_tmp  = (unsigned long int)time(NULL);
 cc_tmp += " ";
 Info_texture().Info_data_CC( cc_tmp );
}

//______________________________________________________________________________
void alx_image_opengl::maj(const alx_image_32 *img_32)
{image_creer = true;
 maj_tempon(img_32);}

//______________________________________________________________________________
void alx_image_opengl::maj_tempon(const alx_image_32 *img_32)
{if(!image_creer) return;
 if (img_32->Tempon_const() == NULL) {return;}

 alx_chaine_char cc_tmp;
 cc_tmp  = (unsigned long int)time(NULL);
 cc_tmp += " ";
 Info_texture().Info_data_CC( cc_tmp );

 glPushAttrib(GL_ENABLE_BIT);
 glEnable(GL_TEXTURE_2D);
 if(texture_cree &&(info_texture.Tx() < img_32->L() || info_texture.Ty() < img_32->H() || forcer_destruction_texture) )
  {glDeleteTextures(1, &id_text);
   id_text = -1;
  }
  else
   {Etirement_x( img_32->L() );
    Etirement_y( img_32->H() );
    texture_cree = true;}

 if(MipMap())
  {glGenTextures(1, (unsigned int*)(&(info_texture.id_texture)));
   glBindTexture(GL_TEXTURE_2D, info_texture.id_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    if( gluBuild2DMipmaps(GL_TEXTURE_2D, Nb_octets_par_pixel(), img_32->L(), img_32->H(), img_32->Ordonnancement_couleurs(), GL_UNSIGNED_BYTE, img_32->Tempon_const()) )
     {return;}
   info_texture.tx = puissance_de_2_superieur(L(), 64);
   info_texture.ty = puissance_de_2_superieur(H(), 64);
   info_texture.taille_reelle[0] = L();
   info_texture.taille_reelle[1] = H();
  } else
     {if( info_texture.Tx() < img_32->L()
        ||info_texture.Ty() < img_32->H()
        ||forcer_destruction_texture )
       {//CreerTexture((int*)(&id_text), img_32->L(), img_32->H(), 4, GL_BGRA, &info_texture );
        CreerTexture((int*)(&id_text), img_32->L(), img_32->H(), nb_octets_par_pixels_texture, ordre_couleur_texture, &info_texture );
       } else {id_text =  info_texture.Id_texture();}
      glBindTexture(GL_TEXTURE_2D, id_text);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      glTexSubImage2D( GL_TEXTURE_2D, 0
                     , 0, 0 // décalage
                     , img_32->L(), img_32->H(), Ordonnancement_couleurs(), GL_UNSIGNED_BYTE, img_32->Tempon_const());
      info_texture.taille_reelle[0] = img_32->L();
      info_texture.taille_reelle[1] = img_32->H();}

 glPopAttrib();
}

//______________________________________________________________________________
void alx_image_opengl::maj_tempon()
{if(!image_creer) return;

 glPushAttrib(GL_ENABLE_BIT);
 glEnable(GL_TEXTURE_2D);
 if( texture_cree
   &&(info_texture.Tx() < L() || info_texture.Ty() < H() || forcer_destruction_texture) )
  {glDeleteTextures(1, &id_text);}
  else
   {Etirement_x( L() );
    Etirement_y( H() );
    texture_cree = true;}

 Lock_mutex_tempon();
 if(MipMap())
  {glGenTextures(1, (unsigned int*)(&(info_texture.id_texture)));
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glBindTexture(GL_TEXTURE_2D, info_texture.id_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   if( gluBuild2DMipmaps(GL_TEXTURE_2D, Nb_octets_par_pixel(), L(), H(), Ordonnancement_couleurs(), GL_UNSIGNED_BYTE, TEMPON()) )
     {return;}
   info_texture.tx = L();
   info_texture.ty = H();
  } else
     {//CreerTexture((int*)(&id_text), L(), H(), 4, GL_BGRA, &info_texture);
      if(info_texture.Tx() < L() || info_texture.Ty() < H() || forcer_destruction_texture)
       {CreerTexture((int*)(&id_text), L(), H(), nb_octets_par_pixels_texture, ordre_couleur_texture, &info_texture);
       } else {id_text = info_texture.Id_texture();}
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glBindTexture(GL_TEXTURE_2D, id_text);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      glTexSubImage2D( GL_TEXTURE_2D, 0
                     , 0, 0 // décalage
                     , L(), H(), Ordonnancement_couleurs(), GL_UNSIGNED_BYTE, TEMPON());
     }
 UnLock_mutex_tempon();
 info_texture.taille_reelle[0] = L();
 info_texture.taille_reelle[1] = H();
/* VRAIMENT VIEUX
 if( Ordonnancement_couleurs() == GL_RGB
   ||Ordonnancement_couleurs() == GL_BGR )
  {
   unsigned char *tab_alpha = new unsigned char[L()*H()];
   memset(tab_alpha, 0xFF, L()*H());
   glTexSubImage2D( GL_TEXTURE_2D, 0
                  , 0, 0 // décalage
                  , L(), H(), GL_ALPHA, GL_UNSIGNED_BYTE, tab_alpha);
   delete tab_alpha;
  }*/

 glPopAttrib();
}

//______________________________________________________________________________
void alx_image_opengl::afficher()
{glPushAttrib(GL_ENABLE_BIT);
   double text_x1, text_x2
        , text_y1, text_y2
        , tx = (double)info_texture.Taille_reelle_x()/(double)info_texture.Tx()
        , ty = (double)info_texture.Taille_reelle_y()/(double)info_texture.Ty()
        , Ox = (double)info_texture.Start_x()        /(double)info_texture.Tx()
        , Oy = (double)info_texture.Start_y()        /(double)info_texture.Ty();
   if(Inverser_x()) {text_x1=Ox+tx; text_x2=Ox;}
    else {text_x1=Ox; text_x2=Ox+tx;}
   if(Inverser_y()) {text_y1=Oy+ty; text_y2=Oy;}
    else {text_y1=Oy; text_y2=Oy+ty;}

   if(couleur_def)
    {glPushAttrib(GL_COLOR_BUFFER_BIT);
     glColor4dv(couleur);}
   if( Translucidite())
      {if(!couleur_def)
         glPushAttrib(GL_COLOR_BUFFER_BIT);
       glEnable(GL_BLEND);
       glEnable(GL_ALPHA_TEST);
       glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
       glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      } else {glDisable(GL_ALPHA_TEST);
              glDisable(GL_BLEND);
             }

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, info_texture.id_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   if( Lisser() )
    {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     if(MipMap()) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else
       {glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if(MipMap()) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
         else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       }
   glBegin(GL_QUADS);
     glTexCoord2d(text_x1, text_y1); glVertex2d(0   , 0);
     glTexCoord2d(text_x2, text_y1); glVertex2d(Lg(), 0);
     glTexCoord2d(text_x2, text_y2); glVertex2d(Lg(), Ht());
     glTexCoord2d(text_x1, text_y2); glVertex2d(0   , Ht());
   glEnd();

 if( couleur_def
   ||Translucidite() )
   glPopAttrib();
 glPopAttrib();
}

