#include "config_opengl.h"
#include "../physique/math_alex.h"
//#include <windows.h>
#include <extgl.h>

#include "../physique/alx_methode_rappel.h""

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
INFOS_TEXTURE::INFOS_TEXTURE(const char *n) {
  tx = ty = start_x = start_y = 0;
  taille_reelle[0] = taille_reelle[1] = 0;
  id_texture = -1;
  L_rappel_changement_dim  = new Liste_de_rappel();
  L_rappel_changement_data = new Liste_de_rappel();
  nom = n;
  data_modifiees = false;
}

//______________________________________________________________________________
void INFOS_TEXTURE::Informer_changement_dim()    {L_rappel_changement_dim->Rappeler( this );}
void INFOS_TEXTURE::abonner_a_changement_dim     (alx_methode_rappel *m) {L_rappel_changement_dim->Ajouter_methode_a_la_fin(m);}
void INFOS_TEXTURE::desabonner_de_changement_dim (alx_methode_rappel *m) {L_rappel_changement_dim->Retirer_methode(m);}

//______________________________________________________________________________
void INFOS_TEXTURE::Informer_changement_data()    {Data_modifiees(true); L_rappel_changement_data->Rappeler( this );}
void INFOS_TEXTURE::abonner_a_changement_data     (alx_methode_rappel *m) {L_rappel_changement_data->Ajouter_methode_a_la_fin(m);}
void INFOS_TEXTURE::desabonner_de_changement_data (alx_methode_rappel *m) {L_rappel_changement_data->Retirer_methode(m);}

//______________________________________________________________________________
const INFOS_TEXTURE & INFOS_TEXTURE::operator =(const INFOS_TEXTURE &info)
{//*L_rappel_changement_dim  = *(info.L_rappel_changement_dim);
 //*L_rappel_changement_data = *(info.L_rappel_changement_data);
 tx                       = info.tx;
 ty                       = info.ty;
 taille_reelle[0]         = info.taille_reelle[0];
 taille_reelle[1]         = info.taille_reelle[1];
 id_texture               = info.id_texture;
 Info_data_CC( info.Info_data() );
 return *this;
}

//______________________________________________________________________________
void INFOS_TEXTURE::Copier     (const INFOS_TEXTURE &info)
{*this = info;
 Nom_CC( info.Nom() );
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
unsigned int tempon;
void Tempon(const unsigned int t)   {tempon = t;}
const unsigned int Tempon()         {return tempon;}
void Initialiser_tempon()           {tempon = 0;}
const unsigned int Nouveau_tempon() {return ++tempon;}
const unsigned int Incr_tempon()    {return ++tempon;}
const unsigned int Decr_tempon()    {return --tempon;}

int puissance_de_2_superieur(const int nb, int puissance_min)
{while(puissance_min < nb) puissance_min = puissance_min<<1;
 return puissance_min;}

// De préférence mode=GL_RGBA8
static const int taille_buffer = 2048*2048*4;
unsigned char *pTexture = new unsigned char[taille_buffer];
void CreerTexture(int *id_texture, const int tx, const int ty, const unsigned int nb_octet_par_pixels, const int mode_video, INFOS_TEXTURE *info_texture)
{
// Un pointeur pour la mémoire


// We need to create a blank texture to render our dynamic texture too.
// To do this, we just create an array to hold the data and then give this
// array to OpenGL.  The texture is stored on the video card so we can
// destroy the array immediately afterwards.
// This function takes the texture array to store the texture, the
// size of the texture for width and the heigth, the channels (1, 3 or 4),
// the type (LUMINANCE, RGB, RGBA, etc..) and the texture ID to assign it too.

// Allocate and init memory for the image array and point to it from pTexture
 int taille_x = puissance_de_2_superieur(tx, 64)
   , taille_y = puissance_de_2_superieur(ty, 64);

 if(taille_x*taille_y*4 > taille_buffer)
  {delete[] pTexture;
   pTexture = new unsigned char[taille_x*taille_y*4];
  }

// Register the texture with OpenGL and bind it to the texture ID
 glGenTextures(1, (unsigned int*)id_texture);
 glBindTexture(GL_TEXTURE_2D, *id_texture);
 info_texture->id_texture = *id_texture;

// Create the texture and store it on the video card
// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, taille_x, taille_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexture);
 //XXX glTexImage2D(GL_TEXTURE_2D, 0, nb_octet_par_pixels, taille_x, taille_y, 0, mode_video, GL_UNSIGNED_BYTE, pTexture);
 glTexImage2D(GL_TEXTURE_2D, 0, nb_octet_par_pixels, taille_x, taille_y, 0, mode_video, GL_UNSIGNED_BYTE, NULL);

// Set the texture quality
/* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/

 info_texture->tx = taille_x;
 info_texture->ty = taille_y;
}

void camera_pour_ecran_relatif(const double ex, const double angle_degre, const double rapport_hauteur_largeur)
{glLoadIdentity();
 if(rapport_hauteur_largeur*ex == 0)
   return;
 double etirement = 1/(rapport_hauteur_largeur*ex);
 glTranslated(0,0,-0.5);
 glScaled(etirement, etirement, 1);
}

void camera_pour_ecran(const double ex, const double angle_degre, const double rapport_hauteur_largeur)
{glMatrixMode(GL_MODELVIEW);
 glPushMatrix();

 camera_pour_ecran_relatif(ex, angle_degre, rapport_hauteur_largeur);
}

alx_point2D CO_pt_tmp;
alx_vecteur2D CO_v_tmp;
void camera_pour_ecran( const alx_point2D &coin_bg, const alx_vecteur2D &v_largeur
                      , const double hauteur, const double angle_degre, const double rapport_hauteur_largeur)
{double largeur = v_largeur.Norme();
 camera_pour_ecran(largeur, angle_degre, rapport_hauteur_largeur);

 double angle_trigo = v_largeur.angle_trigo( Vect_h() ) //alx_vecteur2D(1.0, 0.0) )
      , l = largeur/2
      , h = hauteur/2;
 glTranslated(-coin_bg.x, -coin_bg.y, 0.0);
 glRotated(angle_trigo*180/PI, 0.0, 0.0, 1.0);

 CO_pt_tmp.maj(-l, -h); //alx_point2D ancien_centre(-l, -h);
 /*ancien_centre*/CO_pt_tmp.Pivoter(-angle_trigo, Pt_nul() ); //alx_point2D(0.0, 0.0) );

 glTranslated(CO_pt_tmp.x, CO_pt_tmp.y, 0.0); //ancien_centre.x, ancien_centre.y, 0.0);
}

void Positionner( const alx_vecteur2D &position
                , const double angle_radian
                , const alx_point2D &centre_rotation )
{CO_v_tmp.maj(position); // alx_vecteur2D trans2;
                        // trans2.maj( position );
 /*trans2*/CO_v_tmp.Pivoter( -angle_radian );

 glTranslated(centre_rotation.x, centre_rotation.y, 0.0);
 glRotated(angle_radian*180/PI, 0, 0, 1);
 glTranslated(-centre_rotation.x, -centre_rotation.y, 0.0);

 glTranslated(CO_v_tmp.dx, CO_v_tmp.dy, 0.0); //trans2.dx, trans2.dy, 0.0);
}


//______________________________________________________________________________
void Etirer(const alx_vecteur2D &e)
{glScaled(e.dx, e.dy, 1);}

//______________________________________________________________________________
void Changement_repere(const alx_repere2D &rep)
{if( rep.Identite()
   ||!rep.Prendre_en_compte_repere() ) return;
 glTranslated(rep.origine.x, rep.origine.y, 0.0);
 if(rep.rotation != 0) glRotated(rep.rotation*180/PI, 0, 0, 1);
// if
 if ( rep.etirement.dx != 1
    ||rep.etirement.dy != 1 ) {glScaled(rep.etirement.dx, rep.etirement.dy, 1);
                              }
 rep.maj_etirement_courant();
// Le glissement
 if( rep.glissement.dx !=0
   ||rep.glissement.dy !=0 )
  {GLfloat mat[16] = {1,0,0,0
                     ,0,1,0,0
                     ,0,0,1,0
                     ,0,0,0,1};
   mat[4] = rep.glissement.dx;
   mat[1] = rep.glissement.dy;
   glMultMatrixf( mat );
  }
}

//______________________________________________________________________________
void Changement_repere_ordre_inverse(const alx_repere2D &rep)
{if( rep.Identite()
   ||!rep.Prendre_en_compte_repere() ) return;
// Le glissement
 if( rep.glissement.dx !=0
   ||rep.glissement.dy !=0 )
  {GLfloat mat[16] = {1,0,0,0
                     ,0,1,0,0
                     ,0,0,1,0
                     ,0,0,0,1};
   mat[4] = rep.glissement.dx;
   mat[1] = rep.glissement.dy;
   glMultMatrixf( mat );
  }
 if ( rep.etirement.dx != 1
    ||rep.etirement.dy != 1 ) {glScaled(rep.etirement.dx, rep.etirement.dy, 1);
                              }
 rep.maj_etirement_courant();
 if(rep.rotation != 0) glRotated(rep.rotation*180/PI, 0, 0, 1);
 glTranslated(rep.origine.x, rep.origine.y, 0.0);
}

