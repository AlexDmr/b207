#include "alx_vecteur2D.h"

#include "alx_segment2D.h"
#include "alx_droite2D.h"
#include "alx_point2D.h"
#include "alx_primitive2D.h"
#include "math_alex.cpp"
#include "alx_geometrie2D.h"

/******************************************************************************/
/*************************** Les constructeurs ********************************/
/******************************************************************************/
alx_vecteur2D::alx_vecteur2D()
{dx = 0; dy = 0;}

alx_vecteur2D::alx_vecteur2D(const alx_vecteur2D &v)
{dx = v.dx; dy = v.dy;}

alx_vecteur2D::alx_vecteur2D(const double x, const double y)
{dx = x; dy = y;}

alx_vecteur2D::alx_vecteur2D(const alx_point2D &a, const alx_point2D &b)
{dx = b.x - a.x;
 dy = b.y - a.y;}

alx_vecteur2D::alx_vecteur2D(const alx_point2D &p)
{dx = p.x; dy = p.y;}

void alx_vecteur2D::maj(const alx_segment2D& s)
{dx = s.b.x - s.a.x;
 dy = s.b.y - s.a.y;}

//______________________________________________________________________________
//_____________________________ La sérialisation _______________________________
//______________________________________________________________________________
const alx_chaine_char& alx_vecteur2D::Serialiser_contenu() const
{serialisation.Maj("(",0,1);
 serialisation += dx;
 serialisation.Ajouter(";",0,1);
 serialisation += dy;
 serialisation.Ajouter(")",0,1);
 return serialisation;}

//______________________________________________________________________________
void alx_vecteur2D::Deserialiser_contenu(const alx_chaine_char &txt, unsigned int &pos)
{pos++; // on passe la (
 dx = txt.Lire_double(pos);
 pos++; // on passe le ;
 dy = txt.Lire_double(pos);
 pos++; // on passe la )
}

/******************************************************************************/
/********************** Les surcharges d'opérateurs ***************************/
/******************************************************************************/
bool alx_vecteur2D::equivalent(const alx_vecteur2D &v, double marge) const
{return ( absd(dx-v.dx) <= marge )
      &&( absd(dy-v.dy) <= marge );}

void alx_vecteur2D::maj(const alx_point2D &p)
{dx = p.x; dy = p.y;}

void alx_vecteur2D::maj(const alx_point2D &deb, const alx_point2D &fin)
{dx = fin.x - deb.x;
 dy = fin.y - deb.y;}

void alx_vecteur2D::Ajouter(const alx_point2D &p)
{dx += p.x;
 dy += p.y;}

void alx_vecteur2D::Soustraire(const alx_point2D &p)
{dx -= p.x;
 dy -= p.y;}


alx_vecteur2D& alx_vecteur2D::symetrique(const alx_vecteur2D &axe) const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);

 rep->Symetrique(axe);
 return *rep;}

void alx_vecteur2D::Symetrique(const alx_droite2D &axe)
{Symetrique( *(axe.directeur) );}

alx_vecteur2D& alx_vecteur2D::symetrique(const alx_droite2D &axe) const
{return symetrique( *(axe.directeur) );}

alx_point2D AV_pt_tmp;
void alx_vecteur2D::Pivoter(const double angle)
{AV_pt_tmp.maj(dx, dy); //alx_point2D rep(this->dx, this->dy);
 /*rep*/AV_pt_tmp.Pivoter(angle, Pt_nul());
 maj(AV_pt_tmp/*rep*/);}

void alx_vecteur2D::Etirer(const double x, const double y)
{dx *= x;
 dy *= y;}

void alx_vecteur2D::Etirer(const alx_vecteur2D &v)
{dx *= v.dx;
 dy *= v.dy;}

alx_vecteur2D& alx_vecteur2D::etirer(const alx_vecteur2D &v) const
{alx_vecteur2D *rep = new alx_vecteur2D(v);
 rep->Etirer(v);
 return *rep;}

void alx_vecteur2D::maj(const double dx, const double dy)
{this->dx = dx;
 this->dy = dy;}

void alx_vecteur2D::maj(const alx_vecteur2D& v)
{dx = v.dx;
 dy = v.dy;}

const alx_vecteur2D &alx_vecteur2D::operator =(const alx_vecteur2D &v)  // Surcharge du +=
{dx = v.dx;
 dy = v.dy;
 return *this;}

const alx_vecteur2D &alx_vecteur2D::operator+=(const alx_vecteur2D &v)  // Surcharge du +=
{dx += v.dx;
 dy += v.dy;
 return *this;}

const alx_vecteur2D &alx_vecteur2D::operator-=(const alx_vecteur2D &v)  // Surcharge du -=
{dx -= v.dx;
 dy -= v.dy;
 return *this;}

void alx_vecteur2D::Alonger(const double d)
{dx *= d;
 dy *= d;}

alx_vecteur2D &alx_vecteur2D::alonger(const double d) const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);
 rep->Alonger(d);

 return *rep;}

const alx_vecteur2D &alx_vecteur2D::operator*=(const double d)         // Surcharge du *=
{dx *= d;
 dy *= d;
 return *this;}

void alx_vecteur2D::Ajouter(const alx_vecteur2D &v)
{dx += v.dx;
 dy += v.dy;}

void alx_vecteur2D::Ajouter(const double x, const double y)
{dx += x;
 dy += y;}

void alx_vecteur2D::Soustraire(const double x, const double y)
{dx -= x;
 dy -= y;}

const alx_vecteur2D& operator +(const alx_vecteur2D &v1, const alx_vecteur2D &v2)
{alx_vecteur2D *rep = new alx_vecteur2D(v1);
 rep->Ajouter(v2);
 return *rep;
}

void alx_vecteur2D::Soustraire(const alx_vecteur2D &v)
{dx -= v.dx;
 dy -= v.dy;}

const alx_vecteur2D& operator -(const alx_vecteur2D &v1, const alx_vecteur2D &v2)
{alx_vecteur2D *rep = new alx_vecteur2D(v1);
 rep->Soustraire(v2);
 return *rep;}

const double operator *(const alx_vecteur2D &v1, const alx_vecteur2D &v2)
{return (v1.dx * v2.dx)
      + (v1.dy * v2.dy);}

const alx_vecteur2D& operator *(const double d, const alx_vecteur2D &v)
{alx_vecteur2D *rep = new alx_vecteur2D(v);
 rep->Alonger(d);
 return *rep;}

const alx_vecteur2D& operator *(const alx_vecteur2D &v, const double d)
{alx_vecteur2D *rep = new alx_vecteur2D(v);
 rep->Alonger(d);
 return *rep;}

/******************************************************************************/
/******************************** Les méthodes ********************************/
/******************************************************************************/
void alx_vecteur2D::Oppose()
{dx = -dx;
 dy = -dy;}

alx_vecteur2D& alx_vecteur2D::oppose() const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);
 rep->Oppose();
 return *rep;}

alx_vecteur2D& alx_vecteur2D::pivoter(const double angle) const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);
 rep->Pivoter(angle);
 return *rep;}

void alx_vecteur2D::Perpendiculaire_g()
{double tmp = dx;
 dx = -dy;
 dy =  tmp;}

alx_vecteur2D& alx_vecteur2D::perpendiculaire_g() const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);

 rep->Perpendiculaire_g();
 return *rep;}

void alx_vecteur2D::Perpendiculaire_d()
{double tmp = dx;
 dx =  dy;
 dy = -tmp;}

alx_vecteur2D& alx_vecteur2D::perpendiculaire_d() const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);

 rep->Perpendiculaire_d();
 return *rep;}

// Attention, on ne veut pas de vecteur nulle ici!!!
void alx_vecteur2D::Symetrique(const alx_vecteur2D &axe)
{if(dx==0 && dy==0) return;
 double alpha;

 alpha = 2*(axe.dy * dy + axe.dx * dx)
       / (Asqr(axe.dx) + Asqr(axe.dy));

 dx = alpha*axe.dx - dx;
 dy = alpha*axe.dy - dy;
}

void alx_vecteur2D::Vecteur_norme()
{const double norme = Norme();
 if(norme < 0.0000001) {dx=1; dy=0; return;}
 double a_norme = 1/norme;

 dx *= a_norme;
 dy *= a_norme;
}

const alx_vecteur2D& alx_vecteur2D::vecteur_norme() const
{alx_vecteur2D *rep = new alx_vecteur2D(*this);

 rep->Vecteur_norme();
 return *rep;}

void          alx_vecteur2D::Normer(const double n)
{const double norme = Norme();
 if(norme < 0.0000001) {dx=n; dy=0; return;}
 double a_norme = n/norme;

 dx *= a_norme;
 dy *= a_norme;}


const alx_vecteur2D& alx_vecteur2D::normer(const double n) const
{if(dx==0 && dy==0)
  {alx_vecteur2D *rep = new alx_vecteur2D(n, 0);
   return *rep;}

 alx_vecteur2D *rep = new alx_vecteur2D(dx, dy);
   rep->Normer(n);
 return *rep;}

alx_vecteur2D AV_v_tmp, AV_v_tmp2, AV_v_tmp3;
double alx_vecteur2D::angle_trigo(const alx_vecteur2D &v) const
{//alx_vecteur2D tmp1(*this)
 //            , tmp2(v);
 AV_v_tmp.maj(*this);
 AV_v_tmp2.maj(v);

 AV_v_tmp.Vecteur_norme();
 AV_v_tmp2.Vecteur_norme();

 double argument = AV_v_tmp*AV_v_tmp2;
 if(argument<=-1)
   return PI;
 if(argument>= 1)
   return 0;
 double rep = acos( argument );
 AV_v_tmp3.maj(v); //alx_vecteur2D v_ortho(v);
 /*v_ortho*/AV_v_tmp3.Perpendiculaire_g();

 if(AV_v_tmp3*AV_v_tmp > 0) rep = PI+PI-rep;

 return rep;}

double alx_vecteur2D::Norme() const
{return sqrt( Asqr(dx)
            + Asqr(dy) );}

double alx_vecteur2D::produit_scalaire(const alx_vecteur2D &v) const
{return dx * v.dx
      + dy * v.dy;}

