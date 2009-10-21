#ifndef __ALX_CERCLE2D_PHYSIQUE_H__
#define __ALX_CERCLE2D_PHYSIQUE_H__

#include "alx_cercle2D.h"

class alx_cercle2D_physique : public alx_cercle2D
{
 public:
  // les extensions pour un modèle physique statique:
  double masse;
  double elasticite;

  alx_cercle2D_physique() : alx_cercle2D()
   {
    masse      = 1.0;
    elasticite = 1.0;
   }

  alx_cercle2D_physique(double m, double e, double rayon) : alx_cercle2D(0.0, 0.0, rayon)
   {
    masse      = m;
    elasticite = e;
   }

  alx_cercle2D_physique(double x, double y, double m, double e, double rayon) : alx_cercle2D(x, y, rayon)
   {
    masse      = m;
    elasticite = e;
   }

  void maj(const alx_cercle2D_physique &c)
   {
    //alx_cercle2D::maj( (alx_cercle2D)(c) );
    alx_cercle2D::maj( c );
    this->masse      = c.masse;
    this->elasticite = c.elasticite;
   }
   
  alx_cercle2D_physique &operator =(const alx_cercle2D_physique &c)    // Surcharge du  =
   {
    maj(c);
    return *this;
   }

};

#endif
