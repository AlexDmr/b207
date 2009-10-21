#include "alx_ellipse2D.h"
#include "math_alex.cpp"

/******************************************************************************/
/***************************** Les constructeurs ******************************/
/******************************************************************************/
alx_ellipse2D::alx_ellipse2D() : alx_point2D()// par défaut
{a = b = 0.0;
 axe_abscisse.maj(1, 0);
 axe_ordonnee.maj(0, 1);
}

alx_ellipse2D::alx_ellipse2D( const double x, const double y
                            , const double a, const double b
                            , const alx_vecteur2D &axe_abscisse) : alx_point2D(x, y)
{this->a = a;
 this->b = b;
 this->axe_abscisse.maj(axe_abscisse);
 axe_ordonnee.maj(axe_abscisse); axe_ordonnee.Perpendiculaire_g();
}

alx_ellipse2D::alx_ellipse2D(const alx_ellipse2D &p)
{maj(p);}

bool alx_ellipse2D::intersection(const alx_droite2D &d, alx_point2D &res1, alx_point2D &res2) const
{double alpha, beta, gamma; // Pour la droite
 d.alpha_beta_gamma(alpha, beta, gamma);

 if(beta == 0)
  {if(alpha == 0) return false;
   double x = -gamma/alpha;
   double y = b*sqrt(1-x*x/(a*a));
   res1.maj(x, y);
   res2.maj(x,-y);
   return true;
  }


 double alpha2_sur_beta2 = alpha*alpha/(beta*beta);
 double a2 = a*a
      , b2 = b*b;
 double as = b2+a2*alpha2_sur_beta2;
 if(as == 0) return false;
 double bs = a2*alpha*2*gamma/(beta*beta)
      , cs = alpha2_sur_beta2*gamma*gamma - a2*b2;

 double det = bs*bs-4*as*cs;
 if(det < 0) return false;

 double sqrt_det = sqrt(det);

 a2 = (-bs-sqrt_det)/(2*as);
 b2  = -(alpha*a2+gamma)/beta;
 res1.maj(a2, b2);

 a2 = (-bs+sqrt_det)/(2*as);
 b2 = -(alpha*a2+gamma)/beta;
 res2.maj(a2, b2);

 return true;
}

/******************************************************************************/
/**************************** Les transformations *****************************/
/******************************************************************************/
void alx_ellipse2D::Symetrique (const alx_point2D  &p)
{alx_point2D::Symetrique(p);}

void alx_ellipse2D::Symetrique (const alx_droite2D  &d)
{alx_point2D::Symetrique(d);
 axe_abscisse.Symetrique(d);
 axe_ordonnee.maj(axe_abscisse); axe_ordonnee.Perpendiculaire_g();}

void alx_ellipse2D::Pivoter   (const double angle, const alx_point2D &p)
{alx_point2D::Pivoter(angle, p);
 axe_abscisse.Pivoter(angle);
 axe_ordonnee.maj(axe_abscisse); axe_ordonnee.Perpendiculaire_g();}

alx_ellipse2D& alx_ellipse2D::projection(const alx_droite2D  &d) const
{alx_ellipse2D *rep = new alx_ellipse2D(*this);

 rep->Projection(d);
 return *rep;}

/******************************************************************************/
alx_ellipse2D& alx_ellipse2D::symetrique(const alx_droite2D  &d) const
{alx_ellipse2D *rep = new alx_ellipse2D(*this);

 rep->Symetrique(d);
 return *rep;}

/******************************************************************************/
alx_ellipse2D& alx_ellipse2D::symetrique(const alx_point2D   &p) const
{alx_ellipse2D *rep = new alx_ellipse2D(*this);

 ((alx_point2D*)rep)->Symetrique(p);
 return *rep;}

/******************************************************************************/
alx_ellipse2D& alx_ellipse2D::translation(const alx_vecteur2D &v) const
{alx_ellipse2D *rep = new alx_ellipse2D( x + v.dx
                                       , y + v.dy
                                       , a, b, axe_abscisse);
 return *rep;}

alx_ellipse2D& alx_ellipse2D::translation(const alx_point2D &p) const
{alx_ellipse2D *rep = new alx_ellipse2D( x + p.x
                                       , y + p.y
                                       , a, b, axe_abscisse);
 return *rep;}

/******************************************************************************/
alx_ellipse2D& alx_ellipse2D::pivoter  (const double angle, const alx_point2D &p) const
{alx_ellipse2D *rep = new alx_ellipse2D(*this);

 rep->Pivoter(angle, p);
 return *rep;}

/******************************************************************************/
/***************************** Les mesures ************************************/
/******************************************************************************/
double alx_ellipse2D::aire() const
{return PI*a*b;}

bool alx_ellipse2D::contient(const alx_point2D &p)
{alx_vecteur2D tmp(p);
 double nx = tmp.produit_scalaire(axe_abscisse)
      , ny = tmp.produit_scalaire(axe_ordonnee);
 double a2 = Asqr(a)
      , b2 = Asqr(b);
 return (b2*Asqr(nx) + a2*Asqr(ny) )<= (a2*b2);
}

/******************************************************************************/
/***************************** Les surcharges *********************************/
/******************************************************************************/
alx_ellipse2D &alx_ellipse2D::operator =(const alx_ellipse2D &p)    // Surcharge du  =
{maj(p);
 return *this;}

alx_ellipse2D &alx_ellipse2D::operator+=(const alx_vecteur2D &v)  // Surcharge du +=
{x += v.dx;
 y += v.dy;
 return *this;}

alx_ellipse2D &alx_ellipse2D::operator-=(const alx_vecteur2D &v)  // Surcharge du -=
{x -= v.dx;
 y -= v.dy;
 return *this;}

alx_ellipse2D& operator+(const alx_ellipse2D &p, const alx_vecteur2D &v)
{alx_ellipse2D *rep = new alx_ellipse2D(p);
 return *rep += v;}

alx_ellipse2D& operator+(const alx_vecteur2D &v, const alx_ellipse2D &p)
{alx_ellipse2D *rep = new alx_ellipse2D(p);
 return *rep += v;}

alx_ellipse2D& operator-(const alx_ellipse2D &p, const alx_vecteur2D &v)
{alx_ellipse2D *rep = new alx_ellipse2D(p);
 return *rep -= v;}

alx_ellipse2D& operator-(const alx_vecteur2D &v, const alx_ellipse2D &p)
{alx_ellipse2D *rep = new alx_ellipse2D(p);
 return *rep -= v;}







