#ifndef __ALX_ELLIPSE2D_H__
#define __ALX_ELLIPSE2D_H__

#include "alx_point2D.h"

class alx_ellipse2D : public alx_point2D
{
 friend alx_ellipse2D& operator+(const alx_ellipse2D &, const alx_vecteur2D &);
 friend alx_ellipse2D& operator+(const alx_vecteur2D &, const alx_ellipse2D &);
 friend alx_ellipse2D& operator-(const alx_ellipse2D &, const alx_vecteur2D &);
 friend alx_ellipse2D& operator-(const alx_vecteur2D &, const alx_ellipse2D &);

 private:
   alx_vecteur2D axe_ordonnee;

 public:
   double a, b;
   alx_vecteur2D axe_abscisse;

  // Les constructeurs:
   alx_ellipse2D();                             // par défaut
   alx_ellipse2D( const double x, const double y
                , const double a, const double b
                , const alx_vecteur2D &axe_abscisse); // pour un point 2D
   alx_ellipse2D(const alx_ellipse2D &p);        // par rapport à un autre point
  // Le destructeur:
   //~alx_ellipse2D();

   bool intersection(const alx_droite2D &, alx_point2D &res, alx_point2D &res2) const ;
  // Les transformations appliqués à l'objet lui même:
   virtual void Symetrique (const alx_droite2D  &d);
   virtual void Symetrique (const alx_point2D  &p);
   virtual void Pivoter    (const double, const alx_point2D &);

  // Les transformations qui renvoi un pointeur sur objet:
   virtual alx_ellipse2D& projection (const alx_droite2D  &d) const;
   virtual alx_ellipse2D& symetrique (const alx_droite2D  &d) const;
   virtual alx_ellipse2D& symetrique (const alx_point2D   &p) const;
   virtual alx_ellipse2D& pivoter    (const double angle, const alx_point2D &p) const;
   virtual alx_ellipse2D& translation(const alx_vecteur2D &v) const;
   virtual alx_ellipse2D& translation(const alx_point2D &) const;

  //Trucs utils:
   bool contient(const alx_point2D &);
   inline void maj_axe_ordonnee()
    {
     axe_ordonnee.maj(axe_abscisse);
     axe_ordonnee.Perpendiculaire_g();
    }


  // Les mesures:
   double aire() const;

  // Les surcharges:
   inline void maj( const double x, const double y
                  , const double a, const double b, const alx_vecteur2D &axe_abscisse)
    {
     this->x = x;
     this->y = y;
     this->a = a;
     this->b = b;
     this->axe_abscisse.maj(axe_abscisse);
     maj_axe_ordonnee();
    }
   inline void maj(const alx_ellipse2D &e)
    {
     ((alx_point2D*)this)->maj( (alx_point2D)e );
     a = e.a;
     b = e.b;
     axe_abscisse.maj(e.axe_abscisse);
     axe_ordonnee.maj(e.axe_ordonnee);
    }
   alx_ellipse2D &operator =(const alx_ellipse2D &);   // Surcharge du  =
   alx_ellipse2D &operator+=(const alx_vecteur2D &);  // Surcharge du +=
   alx_ellipse2D &operator-=(const alx_vecteur2D &);  // Surcharge du -=
};

#endif
