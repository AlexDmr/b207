#include "alx_noeud_zone_rendu_sdl_opengl.h"
#include "..\..\physique\math_alex.cpp"
#include "alx_prerendeur.h"
#include "../choses_communes.h"
#include <math.h>

extern "C" {
#include <glu.h>
#include <glut.h>
}


alx_noeud_zone_rendu_sdl_opengl::alx_noeud_zone_rendu_sdl_opengl( alx_prerendeur *prerendeur_zone
                                                                , double nb_pixels_par_unite
                                                                , HDC hdc, HGLRC hglrc
                                                                , ALXGL_TYPE_RENDU type_rendu ) : alx_noeud_scene(false), nb_zooms(1)
{if (prerendeur_zone == NULL)
  {prerendeur_zone = Prerendeur_defaut();}

 alx_noeud_scene::Prerendeur(prerendeur_zone);

 Couleur(0, 0, 0, 0.6);

 this->type_rendu = type_rendu;
 this->nb_pixels_par_unite = nb_pixels_par_unite;
 this->hdc   = hdc;
 this->hglrc = hglrc;
 if (prerendeur_zone)
  {this->angle_camera = prerendeur_zone->Angle_camera();
  } else {this->angle_camera = 90;}

 texture_deja_cree = tempon_deja_cree = false;

// prerendeur_zone->Prendre_en_charge( (alx_noeud_scene*)this, 1, ACTION_DU_RENDU_rendre_toujours, 'p');
 prerendeur_pour_la_zone = prerendeur_zone;
 if (prerendeur_pour_la_zone)
  {prerendeur_local = new alx_prerendeur( this
                                        , prerendeur_pour_la_zone->Ecran_x()
                                        , prerendeur_pour_la_zone->Ecran_y()
                                        , prerendeur_pour_la_zone->Angle_camera() );
  } else {prerendeur_local = new alx_prerendeur( this, 256, 256, 90);
         }

 prerendeur_local->Vider();

 nb_unite_x = nb_unite_y = 0;
 init();
}

alx_noeud_zone_rendu_sdl_opengl::~alx_noeud_zone_rendu_sdl_opengl()
{if(!tempon_deja_cree) return;
 for(unsigned int i=0;i<Nb_zooms();i++)
   glDeleteTextures(1, (unsigned int *)(&(infos_texture_tempon[i].id_texture)) );
}

void alx_noeud_zone_rendu_sdl_opengl::init()
{top_classos = false;
 effacer_fond = true; //false;
 Triangle_control(false);

 noeud_a_rendre = NULL;

 infos_texture_tempon = new INFOS_TEXTURE[Nb_zooms()];
 //glBindTexture(GL_TEXTURE_2D, infos_texture_tempon.id_texture);
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_zone_rendu_sdl_opengl::maj()
{
 switch(type_rendu)
  {case ALXGL_RENDU_DANS_TEXTURE:                  maj_texture_rendu();  break;
   case ALXGL_RENDU_ECRAN_PUIS_COPIE_DANS_TEXTURE: maj_texture_tempon(); break;
   case ALXGL_RENDU_STENCIL:                                             break;
  }
}

//______________________________________________________________________________
void alx_noeud_zone_rendu_sdl_opengl::Nb_zooms(const unsigned int n)
{if( Nb_zooms()==n) return; //rien à faire.
 delete[] infos_texture_tempon;
 for(unsigned int i=0;i<Nb_zooms();i++)
   glDeleteTextures(1, (unsigned int *)(&(infos_texture_tempon[i].id_texture)) );
 nb_zooms = n;
 tempon_deja_cree = false;
 infos_texture_tempon = new INFOS_TEXTURE[Nb_zooms()];
}

//______________________________________________________________________________
void alx_noeud_zone_rendu_sdl_opengl::maj_texture_tempon()
{texture_recree = false;
// Mise à jour de la texture tempon.
 int tx = (int)(Nb_unite_x() * nb_pixels_par_unite)
   , ty = (int)(Nb_unite_y() * nb_pixels_par_unite)
   , txp2 = 64
   , typ2 = 64;
 unsigned int i;

 if(tx < Nb_unite_x() * nb_pixels_par_unite) {tx++; tx = (tx>>1)<<1;}
 if(ty < Nb_unite_y() * nb_pixels_par_unite) {ty++; ty = (ty>>1)<<1;}

 while(txp2 < tx) txp2 = txp2<<1;
 while(typ2 < ty) typ2 = typ2<<1;

 if(tempon_deja_cree)
  {if( (txp2 == infos_texture_tempon[0].tx)
     &&(typ2 == infos_texture_tempon[0].ty) ) return; // Quitter....MAIS QUITTEZ DONC!!!
   for(i=0;i<Nb_zooms();i++)
     glDeleteTextures(1, (unsigned int *)(&(infos_texture_tempon[i].id_texture)) );
  }
 texture_recree = true;
 for(i=0;i<Nb_zooms();i++)
   CreerTexture(&(infos_texture_tempon[i].id_texture), txp2, typ2, 4, GL_BGRA, &(infos_texture_tempon[i]));
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 tempon_deja_cree = true;
}

//______________________________________________________________________________
void alx_noeud_zone_rendu_sdl_opengl::maj_texture_rendu()
{texture_recree = false;
// Mise à jour de la texture de rendu.
 int tx = (int)(Nb_unite_x() * nb_pixels_par_unite)
   , ty = (int)(Nb_unite_y() * nb_pixels_par_unite)
   , txp2 = 64
   , typ2 = 64;

 if(tx < Nb_unite_x() * nb_pixels_par_unite) {tx++; tx = (tx>>1)<<1;}
 if(ty < Nb_unite_y() * nb_pixels_par_unite) {ty++; ty = (ty>>1)<<1;}

 while(txp2 < tx) txp2 = txp2<<1;
 while(typ2 < ty) typ2 = typ2<<1;

 if(texture_deja_cree)
  {
   if( (txp2 == texture_rendu_fiche.Largeur())
     &&(typ2 == texture_rendu_fiche.Hauteur()) ) return;
   glDeleteTextures(1, &id_text);
  }
 texture_recree = true;
 texture_rendu_fiche.Initialiser(txp2, typ2);
 wglMakeCurrent(texture_rendu_fiche.Hdc(), texture_rendu_fiche.Hglrc());
 glDisable(GL_BLEND);
 glDisable(GL_STENCIL_TEST);
 glDisable(GL_DEPTH_TEST);
 glClearColor(0, 0, 1, 0);
 glMatrixMode(GL_PROJECTION); // on rend active la matrice de projection
 glLoadIdentity(); // on reinitialise la matrice de projection
 gluPerspective(200.0/3.0,(double)(texture_rendu_fiche.Largeur())/(double)(texture_rendu_fiche.Hauteur()),0.0,10000.0);

 glMatrixMode(GL_MODELVIEW);
// On place la caméra
/*
 camera_pour_ecran( alx_point2D &coin_bg, alx_vecteur2D &v_largeur
                  , double hauteur, double angle_degre)
*/

// On revient à la fenètre windows:
 wglMakeCurrent(hdc, hglrc);
 glGenTextures(1, &id_text);
 glBindTexture(GL_TEXTURE_2D, id_text);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

 texture_deja_cree = true;
}


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_zone_rendu_sdl_opengl::PreRendre()
{maj();
 if(noeud_a_rendre)
   {
    unsigned int marqueur = noeud_a_rendre->GET_a_new_marqueur();
    noeud_a_rendre->Parcourir_et_traiter(Prise_en_compte_des_reperes_ou_pas, &marqueur);
    noeud_a_rendre->PreRendre_fils();
    } else {unsigned int marqueur = this->GET_a_new_marqueur();
            this->Parcourir_et_traiter(Prise_en_compte_des_reperes_ou_pas, &marqueur);
            PreRendre_fils();
           }

// Préparation puis affichage du corp:
 int taille_x = (int)(Nb_unite_x()*nb_pixels_par_unite)
   , taille_y = (int)(Nb_unite_y()*nb_pixels_par_unite);

 if(taille_x < Nb_unite_x() * nb_pixels_par_unite) {taille_x++; taille_x = (taille_x>>1)<<1;}
 if(taille_y < Nb_unite_y() * nb_pixels_par_unite) {taille_y++; taille_y = (taille_y>>1)<<1;}

 glPushAttrib(GL_ENABLE_BIT);
 glDisable(GL_BLEND);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_TEXTURE_2D);
 glEnable(GL_SCISSOR_TEST);

 if(taille_y<=0 || taille_x<=0)
  {glPopAttrib();
   return;}

 unsigned int tempon = Tempon();
 Tempon(0);

 glViewport(0, 0, taille_x, taille_y);
 glMatrixMode(GL_PROJECTION); // on rend active la matrice de projection
 glLoadIdentity();           // on reinitialise la matrice de projection
 gluPerspective(angle_camera,(double)(taille_x)/(double)(taille_y),0.0,10000.0);
 glMatrixMode(GL_MODELVIEW);

 if(Triangle_control()) angle++; // Pour ne pas tourner n fois si on fait des zoommap

 const float val = 10;
 for(unsigned int i=0; i<Nb_zooms(); i++)
  {if(Effacer_fond())
    {Activer_stencil(GL_always(), GL_zero(), 0);
     glPushAttrib(GL_ENABLE_BIT);
     glDisable(GL_ALPHA_TEST);
     glPushMatrix();
       glLoadIdentity();
       glPushAttrib(GL_COLOR_BUFFER_BIT);
         glColor4dv( Couleur() );
         glTranslatef(0.001, 0.002, -0.5);
         glBegin(GL_TRIANGLE_STRIP);
           glVertex2f(-val, -val);
           glVertex2f( val, -val);
           glVertex2f(-val,  val);
           glVertex2f( val,  val);
         glEnd();
       glPopAttrib();
     glPopMatrix();
     glPopAttrib();
     Depiler_stencil();
    }

   glPushMatrix();
     glLoadIdentity();
     //glTranslatef(-0.5, -0.2, 0);
     camera_pour_ecran_relatif((double)taille_x/(double)(nb_pixels_par_unite*(i+1)), angle_camera, (double)taille_y/(double)taille_x);
     glTranslated(-(double)taille_x/(double)(nb_pixels_par_unite*(i+1)*2.0)
                 ,-(double)taille_y/(double)(nb_pixels_par_unite*(i+1)*2.0)
                 , 0.0);
//     camera_pour_ecran_relatif(Nb_unite_x()/(i+1), angle_camera, Nb_unite_y()/Nb_unite_x());
//     glTranslated(-Nb_unite_x()/((i+1)*2.0)
//                 ,-Nb_unite_y()/((i+1)*2.0)
//                 , 0.0);

    if(Triangle_control())
     {glPushMatrix();
       glLoadIdentity();
       glTranslatef(0,0,-3);
       angle %= 360;
       glColor3f(1, 0.8, 0.6);
       glRotatef(angle, 1, 1, 1);
       glBegin(GL_TRIANGLES);
             glColor3f(1,1,0); glVertex3f(-1, -1, 0);
             glColor3f(1,0,1); glVertex3f( 1, -1, 0);
             glColor3f(0,1,1); glVertex3f( 0,  1, 0);
       glEnd();
      glPopMatrix();
     }

  // On fait le rendu à l'écran
   Changement_repere( *((alx_repere2D*)(alx_noeud_scene*)this) );
   if(noeud_a_rendre)
     noeud_a_rendre->Afficher_fils(0);
    else
      Afficher_fils(0); // On affiche en augmentant mettant le val stencil des fils au courant+1
   glPopMatrix(); // Pour la [fonction] camera_pour_ecran.
   if( infos_texture_tempon[i].taille_reelle[0] != taille_x
     ||infos_texture_tempon[i].taille_reelle[1] != taille_y )
     {infos_texture_tempon[i].taille_reelle[0] = taille_x;
      infos_texture_tempon[i].taille_reelle[1] = taille_y;
      infos_texture_tempon[i].Informer_changement_dim();
     }

 // TOP CLASSOS--------------------------------------------------------------
 if( top_classos )
 {// Affichage de la texture par dessus en translucide
  const unsigned int v = 10;
  double coord_g = (v*nb_pixels_par_unite)/infos_texture_tempon[i].tx
       , coord_d = ((Nb_unite_x()-v)*nb_pixels_par_unite)/infos_texture_tempon[i].tx
       , coord_b = (v*nb_pixels_par_unite)/infos_texture_tempon[i].ty
       , coord_h = ((Nb_unite_y()-v)*nb_pixels_par_unite)/infos_texture_tempon[i].ty;

  glPushMatrix();
  glLoadIdentity();
  //glTranslatef(-0.5, -0.2, 0);
  camera_pour_ecran_relatif(Nb_unite_x()/(i+1), angle_camera, Nb_unite_y()/Nb_unite_x());
  glTranslated(-Nb_unite_x()/((i+1)*2.0)
              ,-Nb_unite_y()/((i+1)*2.0)
              , 0.0);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, infos_texture_tempon[i].id_texture);
  glEnable(GL_BLEND);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
      glColor4f(1, 1, 1, 0.99);
      glTexCoord2f(coord_g, coord_b); glVertex3f(0, 0, 0);
      glTexCoord2f(coord_d, coord_b); glVertex3f(Nb_unite_x(), 0, 0);
      glTexCoord2f(coord_d, coord_h); glVertex3f(Nb_unite_x(), Nb_unite_y(), 0);
      glTexCoord2f(coord_g, coord_h); glVertex3f(0, Nb_unite_y(), 0);
    glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
 } // fin top_classos

  // On copie le résultat dans la texture du niveau ed détail en cours.
   glBindTexture(GL_TEXTURE_2D, infos_texture_tempon[i].id_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR);
   glCopyTexSubImage2D( GL_TEXTURE_2D, 0
                      , 0, 0 // Pas de décalage de la destination par rapport au coin bas gauche de la texture
                      , 0, 0 // On copie à partir du coin bas gauche du tempon image
                      , taille_x, taille_y ); // Taille du rectangle pour la copie.
  } //_Fin de la génération des textures détaillées_____________________________
 glPopAttrib();

 Tempon(tempon);
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
void alx_noeud_zone_rendu_sdl_opengl::Afficher()
{}

