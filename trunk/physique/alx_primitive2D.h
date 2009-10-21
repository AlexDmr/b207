#ifndef __ALXPRIMITIVE2DH__
#define __ALXPRIMITIVE2DH__

//#include "alx_point2D.h"
//#include "alx_vecteur2D.h"
//#include "alx_droite2D.h"

class alx_vecteur2D;
class alx_point2D;
class alx_droite2D;

class alx_primitive2D
{
 public:
 // Les méthodes appliquées à l'objet lui même:
  virtual void Translation(const alx_vecteur2D &) {}
  virtual void Pivoter    (const double angle, const alx_point2D &p) {}
  virtual void Symetrique (const alx_point2D &) {}
  virtual void Symetrique (const alx_droite2D &) {}
  virtual void Etirer     (const alx_vecteur2D &) {}
  virtual void Etirer     (const double x, const double y) {}

 // les méthodes renvoyant un pointeur sur un nouvel objet
  virtual alx_primitive2D& rotation(const double angle, const alx_point2D &pivot) const
   {return *(new alx_primitive2D);}
  virtual alx_primitive2D& symetrique(const alx_point2D &pivot) const
   {return *(new alx_primitive2D);}
  virtual alx_primitive2D& symetrique(const alx_droite2D &axe) const
   {return *(new alx_primitive2D);}
  virtual alx_primitive2D& etirer(const alx_vecteur2D &) const
   {return *(new alx_primitive2D);}
};

#endif
