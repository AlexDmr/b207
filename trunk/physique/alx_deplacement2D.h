#ifndef __ALXDEPLACEMENT__
#define __ALXDEPLACEMENT__

#include "alx_point2D.h"
#include "alx_vecteur2D.h"
#include "alx_droite2D.h"
#include "alx_segment2D.h"
#include "alx_type.h"

/****************************TOUT A REVOIR ICI ********************************/

class alx_point2D;
class alx_vecteur2D;
class alx_droite2D;
class alx_segment2D;

class alx_deplacement2D
{
 protected:
  bool equ2iemedegree(double a, double b, double x, double &res) const;
  
 public:
  alx_segment2D seg;
  alx_vecteur2D dep;
  double t_deb, t_fin;

 // Les affectations
  void maj(const alx_deplacement2D &);
  void maj(const alx_point2D &, const alx_point2D &, double td, double tf);
  void maj(const alx_vecteur2D &, const alx_vecteur2D &, double td, double tf);
  void maj(const alx_point2D &, const alx_vecteur2D &, double td, double tf);
  void maj(const alx_segment2D &s, double td, double tf);

 // Les constructeurs
  alx_deplacement2D();
  alx_deplacement2D(const alx_deplacement2D &);
  alx_deplacement2D(const alx_point2D &, const alx_point2D &, double td, double tf);
  alx_deplacement2D(const alx_vecteur2D &, const alx_vecteur2D &, double td, double tf);
  alx_deplacement2D(const alx_point2D &, const alx_vecteur2D &, double td, double tf);
  alx_deplacement2D(const alx_segment2D &s, double td, double tf);

 // Les méthodes
  bool proximite(const double R, const alx_point2D &      , double &t) const;

  bool proximite(const double R, const alx_droite2D &     , double &t) const;

  bool choc     (const double R, const alx_deplacement2D &, double &t) const;
  bool proximite(const double R, const alx_deplacement2D &, double &t) const;

  bool choc     ( const double R
                , const alx_segment2D &
                , double &
                , alx_type_primitive2D &) const;
  bool proximite( const double R
                , const alx_segment2D &
                , double &
                , alx_type_primitive2D &) const;

 // Les surcharges d'opérateurs
  alx_deplacement2D &operator =(const alx_deplacement2D &);
};

#endif
