#ifndef __ALXCERCLEH__
#define __ALXCERCLEH__

#include "alx_point2D.h"

class alx_cercle2D : public alx_point2D
{friend alx_cercle2D& operator+(const alx_cercle2D &, const alx_vecteur2D &);
 friend alx_cercle2D& operator+(const alx_vecteur2D &, const alx_cercle2D &);
 friend alx_cercle2D& operator-(const alx_cercle2D &, const alx_vecteur2D &);
 friend alx_cercle2D& operator-(const alx_vecteur2D &, const alx_cercle2D &);

 public:
   double rayon;

  // Les constructeurs:
   alx_cercle2D();                             // par défaut
   alx_cercle2D(double x, double y, double r); // pour un point 2D
   alx_cercle2D(const alx_cercle2D &p);        // par rapport à un autre point
  // Le destructeur:
   //~alx_cercle2D();

  // Les transformations appliqués à l'objet lui même:

  // Les transformations qui renvoi un pointeur sur objet:
   virtual alx_cercle2D& projection (const alx_droite2D  &d) const;
   virtual alx_cercle2D& symetrique (const alx_droite2D  &d) const;
   virtual alx_cercle2D& symetrique (const alx_point2D   &p) const;
   virtual alx_cercle2D& pivoter    (const double angle, const alx_point2D &p) const;
   virtual alx_cercle2D& translation(const alx_vecteur2D &v) const;
   virtual alx_cercle2D& translation(const alx_point2D &) const;

  //Trucs utils:
   bool contient(const alx_point2D &);

  // Les mesures:
   double aire() const;

  // Les surcharges:
   inline void maj(const double x, const double y, const double r)
    {this->x = x;
     this->y = y;
     rayon   = r;}

   inline void maj(const alx_cercle2D &c)
    {x     = c.x;
     y     = c.y;
     rayon = c.rayon;}
     
   alx_cercle2D &operator =(const alx_cercle2D &);   // Surcharge du  =
   alx_cercle2D &operator+=(const alx_vecteur2D &);  // Surcharge du +=
   alx_cercle2D &operator-=(const alx_vecteur2D &);  // Surcharge du -=
};

#endif
