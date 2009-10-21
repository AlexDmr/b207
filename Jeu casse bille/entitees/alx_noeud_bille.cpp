#include "alx_noeud_bille.h"
#include <glut.h>

//------------------------------------------------------------------------------
//--------------------------- Les constructeurs --------------------------------
//------------------------------------------------------------------------------
void alx_noeud_bille::init_liste_affichage()
{
 GL_sphere = glGenLists(1);
 glNewList(GL_sphere, GL_COMPILE);
   glColor4d(r, v, b, a);
   glutSolidSphere(rayon, 20, 20);
 glEndList();
}

alx_noeud_bille::alx_noeud_bille()
{
 palet = new alx_entite_palet;
 r = v = b = a = 1.0;
 init_liste_affichage();
}

alx_noeud_bille::alx_noeud_bille( const double rayon, const double masse, const double elasticite
                                  , const alx_point2D &depart, const alx_point2D &arrive
                                  , const double t_deb, const double t_fin
                                  , const double r, const double v, const double b, const double a )
{palet = new alx_entite_palet( rayon, masse, elasticite
                             , depart, arrive
                             , t_deb, t_fin);
 this->r = r;
 this->v = v;
 this->b = b;
 this->a = a;
 this->rayon = rayon;
 init_liste_affichage();
}

//------------------------------------------------------------------------------
alx_noeud_bille::~alx_noeud_bille()
{delete palet;}


//------------------------------------------------------------------------------
//--------------------------------- Les méthodes -------------------------------
//------------------------------------------------------------------------------
void alx_noeud_bille::Afficher()
{
 alx_model_simulation_physique1 *mp_palet = &( palet->model_classique_et_choc );
 //alx_cercle2D c( mp_palet->L_cercle2D_physique.front() );
 //mp_palet->Appliquer_deplacement_a( c );

 glPushMatrix();
   Positionner( mp_palet->position
              , mp_palet->rotation
              , mp_palet->centre_rot );
   glCallList( GL_sphere );
 glPopMatrix();
// alx_noeud_scene::Afficher_fils(0);
}


