#ifndef __ALXDROITE__
#define __ALXDROITE__

#include "math_alex.h"
#include "alx_point2D.h"
#include "alx_vecteur2D.h"
#include "alx_primitive2D.h"

class alx_point2D;
class alx_vecteur2D;
class alx_segment2D;

class alx_droite2D : public alx_primitive2D
{
 private:
  void init();
 public:
  // Un point, un veteur directeur
  alx_point2D   *ref;
  alx_vecteur2D *directeur;

 // Les constructeurs:
  alx_droite2D();
  alx_droite2D(const alx_droite2D &d);
  alx_droite2D(const alx_point2D &, const alx_point2D &);
  alx_droite2D(const alx_point2D &p, const alx_vecteur2D &v);
  ~alx_droite2D();

 // Les méthodes qui s'appliquent à l'objet:
  void Pivoter(const double angle, const alx_point2D &pivot);
  void Symetrique(const alx_point2D &pivot);
  void Symetrique(const alx_droite2D &axe);
  void Perpendiculaire(const alx_point2D &pt);
  void alpha_beta_gamma(double &alpha, double &beta, double &gamma) const;
  void Etirer(const alx_vecteur2D &v);
  void Etirer(const double x, const double y);

 // Les méthodes qui renvoient des pointeurs sur objets
  alx_droite2D& pivoter(const double angle, const alx_point2D &pivot) const;
  alx_droite2D& symetrique(const alx_point2D &pivot) const;
  alx_droite2D& symetrique(const alx_droite2D &axe) const;
  alx_droite2D& perpendiculaire(const alx_point2D &pt) const;
  alx_droite2D& etirer(const alx_vecteur2D &v) const;

  bool intersection(const alx_droite2D &, alx_point2D &res) const ;
    // si vrai alors res est l'intersection
  void maj(const alx_droite2D &);
  void maj(const alx_point2D &, const alx_point2D &);
  void maj(const alx_point2D &p, const alx_vecteur2D &v);
 // Les surcharges d'opérateurs:
  alx_droite2D &operator =(const alx_droite2D &);  
};

#endif
