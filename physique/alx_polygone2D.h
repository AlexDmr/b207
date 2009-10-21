#ifndef __ALX_POLYGONE2D_H__
#define __ALX_POLYGONE2D_H__

#include "..\utilitaires\alx_liste.h" //<list.h>
#include "math_alex.h"
#include "alx_point2D.h"
#include "alx_vecteur2D.h"
#include "alx_droite2D.h"
#include "alx_primitive2D.h"

class alx_point2D;
class alx_vecteur2D;
class alx_droite2D;
class alx_point2D;


class alx_polygone2D : public alx_primitive2D
{
 private:

 public:
  // Les constituants:
   alx_liste<alx_point2D> L_points;

  // Les constructeurs:   
   alx_polygone2D();
   alx_polygone2D(alx_liste<alx_point2D> &L_pt);
   alx_polygone2D(alx_liste<alx_point2D*> &L_ppt);
   alx_polygone2D(alx_polygone2D &p);

  // Les méthodes portant sur l'objet lui même.
   void Translation(const alx_vecteur2D &);
   void Translation(const alx_point2D &);
   void Pivoter    (const double , const alx_point2D &);
   void Symetrique (const alx_point2D &);
   void Symetrique (const alx_droite2D &);
   void Etirer     (const alx_vecteur2D &v);
   void Etirer(const double x, const double y);
   
  // Les méthodes renvoyants un polygone2D transformé. 
/*   alx_polygone2D& translation(const alx_vecteur2D &v) ;
   alx_polygone2D& translation(const alx_point2D &v) ;
   alx_polygone2D& pivoter    (const double angle, const alx_point2D &pivot) ;
   alx_polygone2D& symetrique (const alx_point2D &centre) ;
   alx_polygone2D& symetrique (const alx_droite2D &axe) ;
   alx_polygone2D& etirer     (const alx_vecteur2D &) ; */
   
  // Trucs utils
   bool contient(const alx_point2D &);
   bool Rectangle_encadrant(alx_polygone2D &rep); // Enumère les sommets dans le sens trigo à partir du coin bg.

  // Des infos:
   double longueur();
   void Barycentre(alx_point2D &b);
   alx_point2D& barycentre();

  // Les surcharges d'opérateurs:
   void maj(const alx_polygone2D &);
   void maj(alx_liste<alx_point2D> &L_pt);
   alx_polygone2D &operator =(const alx_polygone2D &);

};

#endif
