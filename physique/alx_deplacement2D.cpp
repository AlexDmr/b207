#include "alx_deplacement2D.h"
#include "alx_vecteur2D.h"
#include "math_alex.cpp"

alx_deplacement2D::alx_deplacement2D()
{t_deb = 0;
 t_fin = 0;}

/******************************************************************************/
void alx_deplacement2D::maj(const alx_deplacement2D &d)
{seg.maj(d.seg);
 dep.maj(d.dep);
 t_deb = d.t_deb;
 t_fin = d.t_fin;}

alx_deplacement2D::alx_deplacement2D(const alx_deplacement2D &dep)
{maj(dep);}

/******************************************************************************/
void alx_deplacement2D::maj(const alx_vecteur2D &d, const alx_vecteur2D &f, double td, double tf)
{t_deb = td;
 t_fin = tf;

 seg.a.x = d.dx;
 seg.a.y = d.dy;
 seg.b.x = f.dx;
 seg.b.y = f.dy;

 dep.maj(f);
 dep.Soustraire(d);
}

alx_deplacement2D::alx_deplacement2D(const alx_vecteur2D &d, const alx_vecteur2D &f, double td, double tf)
{maj(d, f, td, tf);}

/******************************************************************************/
void alx_deplacement2D::maj(const alx_point2D &d, const alx_point2D &f, double td, double tf)
{
 t_deb = td;
 t_fin = tf;

 seg.a.maj(d);
 seg.b.maj(f);
 dep.dx = f.x - d.x;
 dep.dy = f.y - d.y;
}

alx_deplacement2D::alx_deplacement2D(const alx_point2D &d, const alx_point2D &f, double td, double tf)
{
 maj(d, f, td, tf);
}

/******************************************************************************/
void alx_deplacement2D::maj(const alx_point2D &p, const alx_vecteur2D &v, double td, double tf)
{
 t_deb = td;
 t_fin = tf;

 seg.a.maj(p);
 seg.b.maj(p);
 seg.b.Translation(v);

 dep.maj( v );
}

alx_deplacement2D::alx_deplacement2D(const alx_point2D &p, const alx_vecteur2D &v, double td, double tf)
{
 maj(p, v, td, tf);
}

/******************************************************************************/
void alx_deplacement2D::maj(const alx_segment2D &s, double td, double tf)
{
 t_deb = td;
 t_fin = tf;
 seg.maj(s);
 dep.maj(s);
}

alx_deplacement2D::alx_deplacement2D(const alx_segment2D &s, double td, double tf)
{
 maj(s, td, tf);
}

/*
alx_deplacement2D::~alx_deplacement2D()
{
 delete;
}
*/

/******************************************************************************/
/**************************** Les méthodes ************************************/
/******************************************************************************/
bool alx_deplacement2D::proximite(const double R, const alx_droite2D &d     , double &t) const
{
 return proximite( R
                 , alx_deplacement2D( seg.a.projection(d)
                                    , seg.b.projection(d)
                                    , t_deb
                                    , t_fin )
                 , t);
}

/******************************************************************************/
bool alx_deplacement2D::equ2iemedegree(double a, double b, double c, double &res) const
{
 if(a == 0)
  {
   if(b == 0)
    {
     res = 1000.0;
     return false;
    }
   else
    {
     res = -c/b;
     if( (res >= 0.0)
       ||(res <= 1.0) ) return true;
      else
       {
        res = 1000.0;
        return false;
       }
    }
  }
 else
  {
   double t1, t2, tmin, tmax, delta;

   delta = Asqr(b) - 4*a*c;
   if(delta < 0)
    {
     res = 1000.0;
     return false;
    }
   t1 = (-b - sqrt(delta)) / (2*a);
   t2 = (-b + sqrt(delta)) / (2*a);
   tmin = Amin( t1, t2 );
   tmax = Amax( t1, t2 );


   if( ( (tmax >= 0.0)
       &&(tmax <= 1.0) )
     ||( (tmin >= 0.0)
       &&(tmin <= 1.0) )
     ||( (tmin < 0.0)
       &&(tmax > 1.0) )  )
    {
     res = Amax(tmin, 0.0);
     return true;
    }

   res = 1000.0;
   return false;

/*   if( (tmin > 1.0)
     ||(tmax < 0.0) )
    {
     res = 1000.0;
     return false;
    }

   if(tmin >= 0.0) res = tmin;
   else if(tmax >= 0.0) res = tmax;
        else
         {
          res = 1000.0;
          return false;
         }

   return true;  */
  }
}

/******************************************************************************/
bool alx_deplacement2D::proximite(const double R, const alx_point2D &pt, double &t) const
{
 double x, y, dx, dy, a, b
      , alpha, beta, gamma;

 x  = seg.a.x;
 y  = seg.a.y;
 dx = dep.dx;
 dy = dep.dy;
 a  = pt.x;
 b  = pt.y;

 alpha = Asqr(dx) + Asqr(dy);
 beta  = 2*(dx*(x-a) + dy*(y-b));
 gamma = Asqr(x-a) + Asqr(y-b) - Asqr(R);

// Passage en revue des cas:
 return equ2iemedegree(alpha, beta, gamma, t);
}

/******************************************************************************/
// A REFAIRE EN CALCULANT TOUTES LES PROXIMITE ET EN SELECTIONNANT
// LA PREMIERE.
bool alx_deplacement2D::proximite( const double R
                                 , const alx_segment2D &s
                                 , double &t
                                 , alx_type_primitive2D &type) const
{
 bool prox_seg, prox_a, prox_b;
 double t_seg, t_pt_a, t_pt_b;

 alx_droite2D dr(s.a, s.b);
 alx_point2D proj_a(seg.a)
           , proj_b(seg.b);
 proj_a.Projection(dr);
 proj_b.Projection(dr);
 alx_deplacement2D dep_proj( proj_a
                           , proj_b
                           , t_deb, t_fin );

 if( proximite(R, dep_proj, t_seg) )
  {
   double inutil;
   alx_point2D impact(dep_proj.seg.a);
   alx_vecteur2D dep_impact(dep_proj.dep);

   dep_impact.Alonger(t_seg);
   impact.Translation( dep_impact );
   if(s.contient(impact, inutil))
     prox_seg = true;
    else
     {
      prox_seg = false;
      t_seg = 1000.0;
     }
  }
  else
   {
    prox_seg = false;
    t_seg = 1000.0;
   }

 prox_a = proximite( R, s.a, t_pt_a );
 if(!prox_a) t_pt_a = 1000.0;
 prox_b = proximite( R, s.b, t_pt_b );
 if(!prox_b) t_pt_b = 1000.0;

 if(prox_seg||prox_a||prox_b)
  {
   t = Amin(t_seg, Amin(t_pt_a, t_pt_b));
   if(t==t_seg)
     type = Talx_segment2D;
    else if(t==t_pt_a)
           type = Talx_pta_segment2D;
          else if(t==t_pt_b)
                  type = Talx_ptb_segment2D;

   return true;
  }

 return false;
}

bool alx_deplacement2D::choc( const double R
                            , const alx_segment2D &s
                            , double &t
                            , alx_type_primitive2D &type) const
{
 if( !proximite(R, s, t, type) ) return false;

 bool rep = false;
 alx_vecteur2D *axe, *v;
 alx_point2D   *proj, *cour;
 alx_droite2D  *porteuse;

 switch(type)
  {
   case Talx_pta_segment2D:
     // Choc si le prod scal vitesse du dep par le vect point dep-s.a
     // est positif.
     axe = new alx_vecteur2D(seg.a, s.a);

     rep = (axe->produit_scalaire(dep) >= 0.0);
     delete axe;
   break;
   case Talx_ptb_segment2D:
     axe = new alx_vecteur2D(seg.a, s.b);

     rep = (axe->produit_scalaire(dep) >= 0.0);
     delete axe;
   break;
   case Talx_segment2D:
     // On projette le point de départ du dep sur la droite portant le segment.
     // si le produit scalaire de la vitesse du dep avec le vect depart-projection
     // est positif, alors il y a choc.
     cour = new alx_point2D(seg.a);
     v    = new alx_vecteur2D(dep);
     v->Alonger(t);
     cour->Translation(*v);

     porteuse = new alx_droite2D(s.a, s.b);
     proj = new alx_point2D(*cour);
     proj->Projection(*porteuse);
     axe  = new alx_vecteur2D(*cour, *proj);

     rep = (axe->produit_scalaire(dep) >= 0.0);
     delete porteuse;
     delete cour;
     delete proj;
     delete axe;
     delete v;
   break;
  }

 return rep;
}

/******************************************************************************/
bool alx_deplacement2D::choc( const double R, const alx_deplacement2D &d, double &t) const
{ // proximité et rapprochement:
 if( !proximite(R, d, t) ) return false;

 // On se place dans le repère 1D de l'axe des centres
 alx_point2D C1
           , C2;

 C1.Positionner_sur(*this, t);
 C2.Positionner_sur(d, t);

 alx_vecteur2D axe(C1, C2);
 axe.Vecteur_norme();
 double x1 = axe.produit_scalaire( dep )
      , x2 = axe.produit_scalaire( d.dep );

 // On cherche les cas où il n'y a pas choc.
 if( signe(x1) == signe(x2) ) // Condition de base 1 pour qu'il y ait choc.
  {
   return x2 < x1; // Fonctionne dans les deux cas de signes!!!
  }
  else // Les signes sont différent mais on peut encore s'éloigner!
   {
    return !( (x1<0.0)&&(x2>0.0) );
   }

 // Si on en n'a pas trouvé c'est qu'il y a bien choc.
 //return true;
}

bool alx_deplacement2D::proximite( const double R, const alx_deplacement2D &d, double &t) const
{
 alx_point2D p2, p1;
 double a, b, c
      , dx, dy
      , Ax, Ay;


 dx = d.dep.dx - dep.dx;
 dy = d.dep.dy - dep.dy;
 p2 = d.seg.a;
 p1 = seg.a;
 Ax = p2.x - p1.x;
 Ay = p2.y - p1.y;

 a = Asqr(dx) + Asqr(dy);
 b = 2*(dx*Ax + dy*Ay);
 c = Asqr(Ax) + Asqr(Ay) - Asqr(R);

// Passage en revue des cas:
 return equ2iemedegree(a, b, c, t);
}

/******************************************************************************/
/************************** Les surcharges ************************************/
/******************************************************************************/
alx_deplacement2D &alx_deplacement2D::operator =(const alx_deplacement2D &dep)
{
 maj(dep);
 return *this;
}

