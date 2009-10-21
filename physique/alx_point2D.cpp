#include "alx_point2D.h"
#include "alx_segment2D.h"
#include "alx_deplacement2D.h"
#include "math_alex.cpp"

/******************************************************************************/
/***************************** Les constructeurs ******************************/
/******************************************************************************/
alx_point2D::alx_point2D() // par défaut
{x = 0; y = 0;}

alx_point2D::alx_point2D(const double x, const double y) // pour un point 2D
{this->x = x;
 this->y = y;}

alx_point2D::alx_point2D(const alx_point2D &p) // par rapport à un autre point
{x = p.x;
 y = p.y;}

//______________________________________________________________________________
//_____________________________ La sérialisation _______________________________
//______________________________________________________________________________
const alx_chaine_char& alx_point2D::Serialiser_contenu() const
{serialisation.Maj("(",0,1);
 serialisation += x;
 serialisation.Ajouter(";",0,1);
 serialisation += y;
 serialisation.Ajouter(")",0,1); //  = alx_chaine_char("(")+x+";"+y+")";
 return serialisation;}

//______________________________________________________________________________
void alx_point2D::Deserialiser_contenu(const alx_chaine_char &txt, unsigned int &pos)
{pos++; // on passe la (
 x = txt.Lire_double(pos);
 pos++; // on passe le ;
 y = txt.Lire_double(pos);
 pos++; // on passe la )
}

/******************************************************************************/
/**************************** Les transformations *****************************/
/******************************************************************************/
void alx_point2D::Glisser(const alx_vecteur2D &g)
{const double ax = x
            , ay = y;
 x = ax + ay*g.Dx();
 y = ay + ax*g.Dy();}

void alx_point2D::Glisser(const double gx, const double gy)
{const double ax = x
            , ay = y;
 x = ax + ay*gx;
 y = ay + ax*gy;}

//______________________________________________________________________________
void alx_point2D::Etirer(const alx_vecteur2D &v)
{x *= v.dx;
 y *= v.dy;}

void alx_point2D::Etirer(const double x, const double y)
{this->x *= x;
 this->y *= y;}

alx_point2D& alx_point2D::etirer(const alx_vecteur2D &v)  const
{alx_point2D *rep = new alx_point2D(*this);

 rep->Etirer(v);
 return *rep;}
//______________________________________________________________________________
void alx_point2D::Projection(const alx_droite2D  &d)
{
/* alx_droite2D perp(*this, d.directeur->perpendiculaire_g() );
 d.intersection(perp, *this);*/
 double dx = d.directeur->dx
      , dy = d.directeur->dy
      , X  = d.ref->x
      , Y  = d.ref->y;
 double alpha = (x*dx + y*dy - X*dx - Y*dy)
              / (dy*dy + dx*dx);

 x = X + alpha*dx;
 y = Y + alpha*dy;
}

alx_point2D& alx_point2D::projection(const alx_droite2D  &d) const
{alx_point2D *rep = new alx_point2D(*this);
 rep->Projection(d);
 return *rep;}

/******************************************************************************/
void alx_point2D::Barycentre(const alx_segment2D &seg, const double m1, const double m2)
{double m = m1 + m2;
// if(m == 0.0) m = 1.0;

 x = (seg.a.x*m1 + seg.b.x*m2) / m;
 y = (seg.a.y*m1 + seg.b.y*m2) / m;
}

alx_point2D& alx_point2D::barycentre(const alx_segment2D &seg, const double m1, const double m2) const
{alx_point2D *rep = new alx_point2D();

 rep->Barycentre(seg, m1, m2);
 return *rep;
}

/******************************************************************************/
alx_point2D AP_pt_tmp;
void alx_point2D::Symetrique(const alx_droite2D  &d)
{AP_pt_tmp.maj(*this); //alx_point2D proj(*this);

 /*proj*/AP_pt_tmp.Projection(d);
 x += /*proj*/AP_pt_tmp.x + /*proj*/AP_pt_tmp.x - x - x;
 y += /*proj*/AP_pt_tmp.y + /*proj*/AP_pt_tmp.y - y - y;
}
alx_point2D& alx_point2D::symetrique(const alx_droite2D  &d) const
{alx_point2D *rep = new alx_point2D(*this);

 rep->Symetrique(d);
 return *rep;
}

/******************************************************************************/

void alx_point2D::Symetrique(const alx_point2D   &p)
{x = p.x + p.x - x;
 y = p.y + p.y - y;}

alx_point2D& alx_point2D::symetrique(const alx_point2D   &p) const
{alx_point2D *rep = new alx_point2D(*this);

 rep->Symetrique(p);
 return *rep;}

/******************************************************************************/
void alx_point2D::Translation(const double dx, const double dy)
{x += dx;
 y += dy;}

/******************************************************************************/
void alx_point2D::Translation(const alx_vecteur2D &v)
{x += v.dx;
 y += v.dy;}

alx_point2D& alx_point2D::translation(const alx_vecteur2D &v) const
{alx_point2D *rep = new alx_point2D( x + v.dx
                                   , y + v.dy);
 return *rep;}

/******************************************************************************/
void alx_point2D::Translation(const alx_point2D &p)
{x += p.x;
 y += p.y;}

alx_point2D& alx_point2D::translation(const alx_point2D &p) const
{alx_point2D *rep = new alx_point2D( x + p.x
                                   , y + p.y);
 return *rep;}

/******************************************************************************/
void alx_point2D::Translation_inverse(const double dx, const double dy)
{x -= dx;
 y -= dy;}

/******************************************************************************/
void alx_point2D::Translation_inverse(const alx_vecteur2D &v)
{x -= v.dx;
 y -= v.dy;}

alx_point2D& alx_point2D::translation_inverse(const alx_vecteur2D &v) const
{alx_point2D *rep = new alx_point2D( x - v.dx
                                   , y - v.dy);
 return *rep;}

/******************************************************************************/
void alx_point2D::Translation_inverse(const alx_point2D &p)
{x -= p.x;
 y -= p.y;}

alx_point2D& alx_point2D::translation_inverse(const alx_point2D &p) const
{alx_point2D *rep = new alx_point2D( x - p.x
                                   , y - p.y);
 return *rep;
}

/******************************************************************************/
void alx_point2D::Pivoter  (const double angle)
{double dx = x
      , dy = y;
 dernier_cos = cos(angle);
 dernier_sin = sin(angle);
 x = dx*dernier_cos - dy*dernier_sin;
 y = dx*dernier_sin + dy*dernier_cos;}

//______________________________________________________________________________
void alx_point2D:: Inverser()
{x = -x;
 y = -y;}

//______________________________________________________________________________
void alx_point2D::Pivoter  (const double angle, const alx_point2D &p)
{// Calcul des coordonnées du point cible:
 double cos_angle = cos(angle)
      , sin_angle = sin(angle)
      , dx = x - p.x
      , dy = y - p.y;
 x = p.x + dx*cos_angle - dy*sin_angle; //p.x + d*cos(alpha+angle);
 y = p.y + dx*sin_angle + dy*cos_angle; //p.y + d*sin(alpha+angle);
}

//______________________________________________________________________________
void alx_point2D::Pivoter(const double angle, const double px, const double py)
{double dx = x - px
      , dy = y - py;
 dernier_cos = cos(angle);
 dernier_sin = sin(angle);
 x = px + dx*dernier_cos - dy*dernier_sin; //p.x + d*cos(alpha+angle);
 y = py + dx*dernier_sin + dy*dernier_cos; //p.y + d*sin(alpha+angle);
}

//______________________________________________________________________________
alx_point2D& alx_point2D::pivoter  (const double angle, const alx_point2D &p) const
{alx_point2D *rep = new alx_point2D(*this);

 rep->Pivoter(angle, p);
 return *rep;
}

/******************************************************************************/
alx_vecteur2D AP_v_tmp;
void alx_point2D::Positionner_sur(const alx_deplacement2D &d, double t)
{AP_v_tmp.maj(d.dep); //alx_vecteur2D tmp(d.dep);

 /*tmp*/AP_v_tmp.Alonger(t);
 maj(d.seg.a);
 Translation(AP_v_tmp); //tmp);
}

/******************************************************************************/
/***************************** Les mesures ************************************/
/******************************************************************************/
double alx_point2D::distance(const alx_droite2D &d) const
{AP_pt_tmp.maj(*this);// alx_point2D proj(*this);

 /*proj*/AP_pt_tmp.Projection(d);
 return distance(AP_pt_tmp/*proj*/);}

double alx_point2D::distance_carre(const alx_point2D  &p) const
{return Asqr(x - p.x) + Asqr(y - p.y);}

double alx_point2D::distance(const alx_point2D  &p) const
{return sqrt( distance_carre(p) );}

bool alx_point2D::equivalent(const alx_point2D &p, double marge) const
{return ( absd(x-p.x) <= marge )
      &&( absd(y-p.y) <= marge );}

/******************************************************************************/
/***************************** Les surcharges *********************************/
/******************************************************************************/
void alx_point2D::maj(const alx_point2D &p)
{x = p.x;
 y = p.y;}

void alx_point2D::maj(const double x, const double y)
{this->x = x;
 this->y = y;}

void alx_point2D::maj(const double *T)
{this->x = T[0];
 this->y = T[1];}

//______________________________________________________________________________
const alx_point2D &alx_point2D::operator =(const alx_point2D &p)    // Surcharge du  =
{x = p.x;
 y = p.y;
 return *this;}

const alx_point2D &alx_point2D::operator+=(const alx_vecteur2D &v)  // Surcharge du +=
{x += v.dx;
 y += v.dy;
 return *this;}

const alx_point2D &alx_point2D::operator-=(const alx_vecteur2D &v)  // Surcharge du -=
{x -= v.dx;
 y -= v.dy;
 return *this;}

const alx_point2D& operator+(const alx_point2D &p, const alx_vecteur2D &v)
{alx_point2D *rep = new alx_point2D(p);
 return *rep += v;}

const alx_point2D& operator+(const alx_vecteur2D &v, const alx_point2D &p)
{alx_point2D *rep = new alx_point2D(p);
 return *rep += v;}

const alx_point2D& operator-(const alx_point2D &p, const alx_vecteur2D &v)
{alx_point2D *rep = new alx_point2D(p);
 return *rep -= v;}

const alx_point2D& operator-(const alx_vecteur2D &v, const alx_point2D &p)
{alx_point2D *rep = new alx_point2D(p);
 return *rep -= v;}




