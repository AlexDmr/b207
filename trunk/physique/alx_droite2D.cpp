#include "alx_droite2D.h"

/******************************************************************************/
/***************************** Les constructeurs ******************************/
/******************************************************************************/
void alx_droite2D::init()
{ref       = new alx_point2D();
 directeur = new alx_vecteur2D();}

alx_droite2D::alx_droite2D()
{init();}

alx_droite2D::alx_droite2D(const alx_droite2D &d)
{ref       = new alx_point2D( *(d.ref) );
 directeur = new alx_vecteur2D( *(d.directeur) );}

void alx_droite2D::maj(const alx_point2D &d, const alx_point2D &f)
{ref->maj(d);
 directeur->maj(d, f);}

alx_droite2D::alx_droite2D(const alx_point2D &a, const alx_point2D &b)
{ref       = new alx_point2D(a);
 directeur = new alx_vecteur2D(a, b);}

void alx_droite2D::maj(const alx_point2D &p, const alx_vecteur2D &v)
{ref->maj(p);
 directeur->maj(v);}

alx_droite2D::alx_droite2D(const alx_point2D &p, const alx_vecteur2D &v)
{ref       = new alx_point2D(p);
 directeur = new alx_vecteur2D(v);}

alx_droite2D::~alx_droite2D()
{delete ref;
 delete directeur;}

void alx_droite2D::alpha_beta_gamma(double &alpha, double &beta, double &gamma) const
{alpha =-directeur->dy;
 beta  = directeur->dx;
 gamma = -(alpha*ref->x + beta*ref->y);}

/******************************************************************************/
/****************************** Les méthodes **********************************/
/******************************************************************************/
void alx_droite2D::Pivoter(const double angle, const alx_point2D &pivot)
{ref->Pivoter(angle, pivot);
 directeur->Pivoter(angle);}

alx_droite2D& alx_droite2D::pivoter(const double angle, const alx_point2D &pivot) const
{alx_droite2D *rep = new alx_droite2D(*this);
 rep->Pivoter(angle, pivot);
 return *rep;}

void alx_droite2D::Etirer(const double x, const double y)
{ref->Etirer(x, y);
 directeur->Etirer(x, y);}

void alx_droite2D::Etirer(const alx_vecteur2D &v)
{ref->Etirer(v);
 directeur->Etirer(v);}

alx_droite2D& alx_droite2D::etirer(const alx_vecteur2D &v) const
{alx_droite2D *rep = new alx_droite2D(*this);

 rep->Etirer(v);
 return *rep;}

/******************************************************************************/
void alx_droite2D::Symetrique(const alx_point2D &pivot)
{ref->Symetrique(pivot);}

alx_droite2D& alx_droite2D::symetrique(const alx_point2D &pivot) const
{alx_droite2D *rep = new alx_droite2D(*this);

 rep->Symetrique(pivot);
 return *rep;}

/******************************************************************************/
void alx_droite2D::Symetrique(const alx_droite2D &axe)
{ref->Symetrique(axe);
 directeur->Symetrique(axe);}

alx_droite2D& alx_droite2D::symetrique(const alx_droite2D &axe) const
{alx_droite2D *rep = new alx_droite2D(*this);

 rep->Symetrique(axe);
 return *rep;}

/******************************************************************************/
void alx_droite2D::Perpendiculaire(const alx_point2D &pt)
{directeur->Perpendiculaire_g();
 *ref = pt;}

alx_droite2D& alx_droite2D::perpendiculaire(const alx_point2D &pt) const
{alx_droite2D *rep = new alx_droite2D(*this);

 rep->Perpendiculaire(pt);
 return *rep;}

/******************************************************************************/

// si vrai alors res est l'intersection
bool alx_droite2D::intersection(const alx_droite2D &dr, alx_point2D &res) const
{double alpha
      , det = directeur->dx * dr.directeur->dy
            - directeur->dy * dr.directeur->dx;

 if(det == 0) return false;
 alpha = ( (ref->y * dr.directeur->dx) - (ref->x * dr.directeur->dy)
         + (dr.ref->x * dr.directeur->dy) - (dr.ref->y * dr.directeur->dx) )
       / det;

 res.x = ref->x + alpha*directeur->dx;
 res.y = ref->y + alpha*directeur->dy;
 return true;
}

/******************************************************************************/
/********************** Les surcharges d'opérateurs ***************************/
/******************************************************************************/
void alx_droite2D::maj(const alx_droite2D &dr)
{ref->maj( *(dr.ref) );
 directeur->maj( *(dr.directeur) );}

alx_droite2D &alx_droite2D::operator =(const alx_droite2D &dr)  // Surcharge du =
{alx_droite2D tmp(dr);

 maj(tmp);
 return *this;}

