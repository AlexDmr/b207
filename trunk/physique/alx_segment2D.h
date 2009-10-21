#ifndef __ALX_SEGMENT_2D_H__
#define __ALX_SEGMENT_2D_H__

#include "math_alex.h"
#include "alx_point2D.h"
#include "alx_vecteur2D.h"
#include "alx_droite2D.h"
#include "alx_primitive2D.h"

class alx_point2D;
class alx_vecteur2D;
class alx_droite2D;
class alx_point2D;

class alx_segment2D : public alx_primitive2D
{
 private:
  double tri_x[2]
       , tri_y[2];
  void trier();
  bool modif;

 public:
  alx_point2D a, b;

 // Les constructeurs:
  alx_segment2D();
  alx_segment2D(const alx_segment2D &);
  alx_segment2D(const alx_point2D &a, const alx_point2D &b);
  alx_segment2D(const alx_point2D &a, const alx_vecteur2D &v);

 // Les méthodes
  alx_vecteur2D& vecteur() const;
  /*alx_point2D&   depart()  ;
  alx_point2D&   arrive()  ;*/

  void Translation(const alx_vecteur2D &);
  void Translation(const alx_point2D &);
  void Pivoter    (const double , const alx_point2D &);
  void Symetrique (const alx_point2D &);
  void Symetrique (const alx_droite2D &);
  void Etirer     (const alx_vecteur2D &v);
  void Etirer(const double x, const double y);

  alx_segment2D& translation(const alx_vecteur2D &v) const;
  alx_segment2D& translation(const alx_point2D &v) const;
  alx_segment2D& pivoter   (const double angle, const alx_point2D &pivot) const;
  alx_segment2D& symetrique (const alx_point2D &centre) const;
  alx_segment2D& symetrique (const alx_droite2D &axe) const;
  alx_segment2D& etirer     (const alx_vecteur2D &v) const;

 // Trucs utils
  bool intersection(alx_segment2D &s, alx_point2D &rep) ;
  bool contient(const alx_point2D &, double t) const ;

 // Des infos:
  double longueur();
  alx_droite2D& droite() const;
  void droite(alx_droite2D &d) const;

 // Les surcharges d'opérateurs:
  void maj(const alx_segment2D &);
  void maj(const alx_point2D &, const alx_point2D &);
  alx_segment2D &operator =(const alx_segment2D &);
};

#endif
