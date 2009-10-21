#include "alx_cercle2D.h"
#include "math_alex.cpp"


/******************************************************************************/
/***************************** Les constructeurs ******************************/
/******************************************************************************/
alx_cercle2D::alx_cercle2D() : alx_point2D()// par défaut
{rayon = 0.0;}

alx_cercle2D::alx_cercle2D(const double x, const double y, const double r) : alx_point2D(x, y)
{rayon = r;}

alx_cercle2D::alx_cercle2D(const alx_cercle2D &p) : alx_point2D(p.x, p.y)
{rayon = p.rayon;}

/******************************************************************************/
/**************************** Les transformations *****************************/
/******************************************************************************/
alx_cercle2D& alx_cercle2D::projection(const alx_droite2D  &d) const
{alx_cercle2D *rep = new alx_cercle2D(*this);

 rep->Projection(d);
 return *rep;}

/******************************************************************************/
alx_cercle2D& alx_cercle2D::symetrique(const alx_droite2D  &d) const
{alx_cercle2D *rep = new alx_cercle2D(*this);

 rep->Symetrique(d);
 return *rep;}

/******************************************************************************/
alx_cercle2D& alx_cercle2D::symetrique(const alx_point2D   &p) const
{alx_cercle2D *rep = new alx_cercle2D(*this);

 rep->Symetrique(p);
 return *rep;}

/******************************************************************************/
alx_cercle2D& alx_cercle2D::translation(const alx_vecteur2D &v) const
{alx_cercle2D *rep = new alx_cercle2D( x + v.dx
                                     , y + v.dy
                                     , rayon);
 return *rep;}

alx_cercle2D& alx_cercle2D::translation(const alx_point2D &p) const
{alx_cercle2D *rep = new alx_cercle2D( x + p.x
                                     , y + p.y
                                     , rayon);
 return *rep;}

/******************************************************************************/
alx_cercle2D& alx_cercle2D::pivoter  (const double angle, const alx_point2D &p) const
{alx_cercle2D *rep = new alx_cercle2D(*this);

 rep->Pivoter(angle, p);
 return *rep;}

/******************************************************************************/
/***************************** Les mesures ************************************/
/******************************************************************************/
double alx_cercle2D::aire() const
{return PI*Asqr(rayon);}

bool alx_cercle2D::contient(const alx_point2D &p)
{return p.distance_carre((alx_point2D)(*this)) <= Asqr(rayon);}

/******************************************************************************/
/***************************** Les surcharges *********************************/
/******************************************************************************/

alx_cercle2D &alx_cercle2D::operator =(const alx_cercle2D &p)    // Surcharge du  =
{maj(p);
 return *this;}

alx_cercle2D &alx_cercle2D::operator+=(const alx_vecteur2D &v)  // Surcharge du +=
{x += v.dx;
 y += v.dy;
 return *this;}

alx_cercle2D &alx_cercle2D::operator-=(const alx_vecteur2D &v)  // Surcharge du -=
{x -= v.dx;
 y -= v.dy;
 return *this;}

alx_cercle2D& operator+(const alx_cercle2D &p, const alx_vecteur2D &v)
{alx_cercle2D *rep = new alx_cercle2D(p);
 return *rep += v;}

alx_cercle2D& operator+(const alx_vecteur2D &v, const alx_cercle2D &p)
{alx_cercle2D *rep = new alx_cercle2D(p);
 return *rep += v;}

alx_cercle2D& operator-(const alx_cercle2D &p, const alx_vecteur2D &v)
{alx_cercle2D *rep = new alx_cercle2D(p);
 return *rep -= v;}

alx_cercle2D& operator-(const alx_vecteur2D &v, const alx_cercle2D &p)
{alx_cercle2D *rep = new alx_cercle2D(p);
 return *rep -= v;}






