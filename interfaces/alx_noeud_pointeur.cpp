#include "alx_noeud_pointeur.h"
#include "alx_simulateur_pointeurs.h"
#include <glew.h>

//______________________________________________________________________________
void Pointeur_classique(const double x, const double y, void *param_sup)
{glPushMatrix();
   glTranslated(x, y, 0);
   
   for(int i=2;i>=0;i-=2)
    {glColor3f(i/2.0,i/2.0,i/2.0);
     glBegin(GL_TRIANGLE_STRIP);
	 glVertex2i(  i+6, -i-16);
     glVertex2i(   -i,     i);
     glVertex2i(  i+3,  -i-3);
     glVertex2i( i+16,  -i-6);
     glEnd();
    }
   
 glPopMatrix();
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
alx_noeud_pointeur::alx_noeud_pointeur(const alx_pointeur *p) : alx_noeud_scene()
{ptr = p;
 affichage_default = true;
 alx_chaine_char cc_ptr_nom_tmp ("Pointeur ");
 cc_ptr_nom_tmp += p->Nom();
 Nom_IU(cc_ptr_nom_tmp);}

//______________________________________________________________________________
void alx_noeud_pointeur::Afficher()
{const alx_point2D &pt = ptr->Point();
 Origine(pt);
// if(affichage_default)
//   Pointeur_classique(pt.X(), pt.Y(), NULL);
 alx_noeud_scene::Afficher();
}

//______________________________________________________________________________
void alx_noeud_pointeur::Dessin_noeud()
{Pointeur_classique(0, 0, NULL);
}
