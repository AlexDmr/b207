#include "alx_noeud_entete_sdl_opengl.h"
#include "..\..\opengl\texte.h"

void alx_noeud_entete_sdl_opengl::Afficher()
{
 if( !fenetre_gl ) return;
 // Un rectangle coloré comme il faut avec le texte dedans.
 double etirement_texte;
 const double *tc
             , l = fenetre->Longueur_entete()
             , h = fenetre->Hauteur_entete();
 if( (h==0)||(l==0) )
   return;
 //entete.Affichage_du_polygone2D_par_triangles(GL_TRIANGLE_STRIP, 1);//tracer_polygone_couleur(entete, 4);
 glBegin(GL_QUADS);
   tc = fenetre_gl->Couleur_entete(0); glColor4dv(tc); glVertex3d(0,0,0);
   tc = fenetre_gl->Couleur_entete(1); glColor4dv(tc); glVertex3d(l,0,0);
   tc = fenetre_gl->Couleur_entete(2); glColor4dv(tc); glVertex3d(l,h,0);
   tc = fenetre_gl->Couleur_entete(3); glColor4dv(tc); glVertex3d(0,h,0);
 glEnd();
  // Affichage du texte
 glPushMatrix();
   glColor3dv(couleur);
   etirement_texte = h / fenetre_gl->Fonte_entete()->HAUTEUR();
    glTranslated(0, etirement_texte*fenetre_gl->Fonte_entete()->HAUTEUR()/3, 0);
   glScaled(etirement_texte, etirement_texte, 0);
   int pos;
   fenetre_gl->Fonte_entete()->ecrire(pos, l/etirement_texte, fenetre->Nom());
 glPopMatrix();
}
