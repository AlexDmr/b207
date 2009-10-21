#include "alx_segment2D.h"

/******************************************************************************/
/******************************** Les privés **********************************/
/******************************************************************************/
void alx_segment2D::trier()
{if(a.x < b.x)
  {tri_x[0] = a.x;
   tri_x[1] = b.x;}
  else
   {tri_x[0] = b.x;
    tri_x[1] = a.x;}

 if(a.y < b.y)
  {tri_y[0] = a.y;
   tri_y[1] = b.y;}
  else
   {tri_y[0] = b.y;
    tri_y[1] = a.y;}
}

/******************************************************************************/
/***************************** Les constructeurs ******************************/
/******************************************************************************/
alx_segment2D::alx_segment2D()
{a.x = 0.0;
 b.x = 1.0;
 a.y = 0.0;
 b.y = 1.0;
 //trier();
 modif = false;}

alx_segment2D::alx_segment2D(const alx_segment2D &s)
{a.maj(s.a);
 b.maj(s.b);
 //trier();
 modif = false;}

alx_segment2D::alx_segment2D(const alx_point2D &a, const alx_point2D &b)
{this->a.maj(a);
 this->b.maj(b);
 //trier();
 modif = false;}

alx_segment2D::alx_segment2D(const alx_point2D &a, const alx_vecteur2D &v)
{this->a.maj(a);
 this->b.maj(a);
 this->b.Translation(v);
 //trier();
 modif = false;}

void alx_segment2D::Etirer(const double x, const double y)
{a.Etirer(x, y);
 b.Etirer(x, y);}

void alx_segment2D::Etirer     (const alx_vecteur2D &v)
{a.Etirer(v);
 b.Etirer(v);
 modif = true;}

alx_segment2D& alx_segment2D::etirer     (const alx_vecteur2D &v) const
{alx_segment2D *rep = new alx_segment2D(*this);

 rep->Etirer(v);
 return *rep;}

/******************************************************************************/
/********************************** Les accès *********************************/
/******************************************************************************/
alx_vecteur2D& alx_segment2D::vecteur() const
{return *(new alx_vecteur2D(a, b));}

/******************************************************************************/
/******************************** Les méthodes ********************************/
/******************************************************************************/
void alx_segment2D::Translation(const alx_vecteur2D &v)
{a.Translation(v);
 b.Translation(v);}

alx_segment2D& alx_segment2D::translation(const alx_vecteur2D &v) const
{alx_segment2D *rep = new alx_segment2D(*this);

 rep->Translation(v);
 return *rep;}
/******************************************************************************/
void alx_segment2D::Translation(const alx_point2D &p)
{a.Translation(p);
 b.Translation(p);}

alx_segment2D& alx_segment2D::translation(const alx_point2D &p) const
{alx_segment2D *rep = new alx_segment2D(*this);

 rep->Translation(p);
 return *rep;}

/******************************************************************************/
void alx_segment2D::Pivoter   (const double angle, const alx_point2D &pivot)
{a.Pivoter(angle, pivot);
 b.Pivoter(angle, pivot);
 //trier();
 modif = true;}

alx_segment2D& alx_segment2D::pivoter(const double angle, const alx_point2D &pivot) const
{alx_segment2D *rep = new alx_segment2D(*this);

 rep->Pivoter(angle, pivot);
 return *rep;}
/******************************************************************************/
void alx_segment2D::Symetrique (const alx_point2D &centre)
{a.Symetrique(centre);
 b.Symetrique(centre);
 //trier();
 modif = true;}

alx_segment2D& alx_segment2D::symetrique(const alx_point2D &centre) const
{alx_segment2D *rep = new alx_segment2D(*this);

 rep->Symetrique(centre);
 return *rep;}

/******************************************************************************/
void alx_segment2D::Symetrique (const alx_droite2D &axe)
{a.Symetrique(axe);
 b.Symetrique(axe);
 //trier();
 modif = true;}

alx_segment2D& alx_segment2D::symetrique(const alx_droite2D &axe) const
{alx_segment2D *rep = new alx_segment2D(*this);

 rep->Symetrique(axe);
 return *rep;}

/******************************************************************************/
/********************************* Trucs utiles *******************************/
/******************************************************************************/
bool alx_segment2D::intersection(alx_segment2D &s, alx_point2D &rep)
{
 trier();
 s.trier();
 modif = false;

 if( (tri_x[1] < s.tri_x[0])
   ||(tri_x[0] > s.tri_x[1])
   ||(tri_y[1] < s.tri_y[0])
   ||(tri_y[0] > s.tri_y[1]) )
  return false;

 // Test plus poussé:
 alx_droite2D d1(a, b)
            , d2(s.a, s.b);
 if( !d1.intersection(d2, rep) )
  return false;

 if( (rep.x >= s.tri_x[0] - EPSILON)
   &&(rep.x <= s.tri_x[1] + EPSILON)
   &&(rep.y >= s.tri_y[0] - EPSILON)
   &&(rep.y <= s.tri_y[1] + EPSILON)
   &&(rep.x >= tri_x[0] - EPSILON)
   &&(rep.x <= tri_x[1] + EPSILON)
   &&(rep.y >= tri_y[0] - EPSILON)
   &&(rep.y <= tri_y[1] + EPSILON) )
  return true;

 return false;
}

/******************************************************************************/
// Attention on suppose ici que le point est déja sur la droite porteuse du segment!
bool alx_segment2D::contient(const alx_point2D &p, double t) const
{
 double det = b.x - a.x;

 if(det != 0.0)
  {t = (p.x - a.x) / det;}
  else
   {det = b.y - a.y;
    if(det != 0.0)
     {t = (p.y - a.y) / det;}
     else
      {t = 0.0;
       return ( (a.x == p.x)&&(a.y == p.y) );}
   }
 if( (t >= 0.0)
   &&(t <= 1.0) ) return true;
 return false;
}

/******************************************************************************/
/*********************************** Des infos ********************************/
/******************************************************************************/
double alx_segment2D::longueur()
{return a.distance(b);}

void alx_segment2D::droite(alx_droite2D &d) const
{d.maj(a, b);}

alx_droite2D& alx_segment2D::droite() const
{return *(new alx_droite2D(a, b));}

/******************************************************************************/
/************************ Les surcharges d'opérateurs *************************/
/******************************************************************************/
void alx_segment2D::maj(const alx_point2D &pt_deb, const alx_point2D &pt_fin)
{a.maj(pt_deb);
 b.maj(pt_fin);
 modif = true;}

void alx_segment2D::maj(const alx_segment2D &s)
{a.maj(s.a);
 b.maj(s.b);
 modif = true;}

alx_segment2D &alx_segment2D::operator =(const alx_segment2D &s)
{maj(s);
 return *this;}

