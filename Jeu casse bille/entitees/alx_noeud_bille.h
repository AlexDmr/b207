#ifndef __ALX_ENTITE_BILLE_H__
#define __ALX_ENTITE_BILLE_H__

#include "..\..\physique\alx_classes_bases.h"
#include "..\..\physique\alx_entite_palet.h"
#include "..\..\opengl\alx_sommet_opengl.h"
#include "..\..\opengl\config_opengl.h"
#include "..\..\interfaces\alx_noeud_scene.h"

class alx_noeud_bille : public alx_noeud_scene
{
 private:
   double r, v, b, a
        , rayon;
   GLuint GL_sphere;
   void init_liste_affichage();

 public :
   alx_entite_palet *palet;

  // Les constructeurs
   alx_noeud_bille();
   alx_noeud_bille( const double rayon, const double masse, const double elasticite
                  , const alx_point2D &depart, const alx_point2D &arrive
                  , const double t_deb, const double t_fin
                  , const double r, const double v, const double b, const double a );

   ~alx_noeud_bille();

  // Les méthodes
   void Afficher();
   
};

#endif
