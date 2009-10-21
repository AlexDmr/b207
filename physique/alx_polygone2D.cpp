#include "alx_polygone2D.h"
#include "alx_segment2D.h"
#include "math_alex.cpp"

/******************************************************************************/
/***************************** Les constructeurs ******************************/
/******************************************************************************/
alx_polygone2D::alx_polygone2D()
{};

alx_polygone2D::alx_polygone2D(alx_liste<alx_point2D> &L_pt)
{L_points.maj(L_pt);}

void alx_polygone2D::maj(alx_liste<alx_point2D> &L_pt)
{L_points.maj(L_pt);}

alx_polygone2D::alx_polygone2D(alx_liste<alx_point2D*> &L_ppt)
{alx_element_liste<alx_point2D*> *it, *it_fin = L_ppt.Fin();

 for(it=L_ppt.Premier(); it!=it_fin; it=it->svt)
   L_points.Ajouter_a_la_fin( *(it->E()) );
}

alx_polygone2D::alx_polygone2D(alx_polygone2D &p)
{maj(p);}

void alx_polygone2D::Translation(const alx_vecteur2D &v)
{
 alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Translation(v);
}

void alx_polygone2D::Translation(const alx_point2D &p)
{
 alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Translation(p);
}

void alx_polygone2D::Pivoter   (const double angle, const alx_point2D &pivot)
{alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Pivoter(angle, pivot);
}

void alx_polygone2D::Symetrique (const alx_point2D &p)
{alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Symetrique(p);
}

void alx_polygone2D::Symetrique (const alx_droite2D &d)
{alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Symetrique(d);
}

/******************************************************************************/
/*************** Les méthodes renvoyants un polygone2D transformé *************/
/******************************************************************************/
void alx_polygone2D::Etirer(const double x, const double y)
{alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Etirer(x, y);
}

void alx_polygone2D::Etirer     (const alx_vecteur2D &v)
{alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
   (it->E()).Etirer(v);
}
/*
alx_polygone2D& alx_polygone2D::etirer     (const alx_vecteur2D &v)
{alx_polygone2D *rep = new alx_polygone2D(*this);
 rep->Etirer(v);
 return *rep;}

alx_polygone2D& alx_polygone2D::translation(const alx_vecteur2D &v)
{alx_polygone2D *rep = new alx_polygone2D(*this);
 rep->Translation(v);
 return *rep;}

alx_polygone2D& alx_polygone2D::translation(const alx_point2D &v)
{alx_polygone2D *rep = new alx_polygone2D(*this);
 rep->Translation(v);
 return *rep;}

alx_polygone2D& alx_polygone2D::pivoter   (const double angle, const alx_point2D &pivot)
{alx_polygone2D *rep = new alx_polygone2D(*this);
 rep->Pivoter(angle, pivot);
 return *rep;}

alx_polygone2D& alx_polygone2D::symetrique (const alx_point2D &centre)
{alx_polygone2D *rep = new alx_polygone2D(*this);
 rep->Symetrique(centre);
 return *rep;}

alx_polygone2D& alx_polygone2D::symetrique (const alx_droite2D &axe) 
{alx_polygone2D *rep = new alx_polygone2D(*this);
 rep->Symetrique(axe);
 return *rep;}
*/
/******************************************************************************/
/********************************** Trucs utils *******************************/
/******************************************************************************/
bool alx_polygone2D::Rectangle_encadrant(alx_polygone2D &rep)
{
 if(L_points.Vide()) return false;

 alx_element_liste<alx_point2D> *it = L_points.Premier(), *it_fin = L_points.Fin();
 double x_min = (it->E()).x
      , y_min = (it->E()).y;
 double x_max = x_min
      , y_max = y_min;
 double x, y;

 for(it=it->svt; it!=it_fin; it=it->svt)
  {
   x = (it->E()).x;
   y = (it->E()).y;
   x_min = Amin(x_min, x); y_min = Amin(y_min, y);
   x_max = Amax(x_max, x); y_max = Amax(y_max, y);
  }

 rep.L_points.Vider();
 rep.L_points.Ajouter_a_la_fin( alx_point2D(x_min, y_max) );
 rep.L_points.Ajouter_a_la_fin( alx_point2D(x_max, y_max) );
 rep.L_points.Ajouter_a_la_fin( alx_point2D(x_max, y_min) );
 rep.L_points.Ajouter_a_la_fin( alx_point2D(x_min, y_min) );

 return true;
}

/******************************************************************************/
// Vrai si en le reliant à un point à l'infini, le segment obtenu
// coupe un nombre impaire de fois le polygone.
// Attention à ne considérer les sommet que comme UN SEUL point si les
// segments ne sont pas du même coté du segment de controle, comme 2 sinon!
// Attention aussi au cas où le premier point est le même que le dernier.
bool alx_polygone2D::contient(const alx_point2D &p)
{
 if(L_points.Taille() <= 2) return false;
 alx_segment2D seg_controle(p, alx_point2D(p.X(), 999999999.9))
             , seg_cour;
 alx_vecteur2D normale_seg_controle(p, seg_controle.b);
 normale_seg_controle.Perpendiculaire_g();

 bool sauter = false;
 
 int nb_coupures = 0;
 alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin(), *it_svt;
 alx_point2D *prcdt = &( L_points.Premier()->E() )
           , *cour, *svt
           , intersect;

 it = L_points.Premier(); it=it->svt;
 for(;it!=it_fin;it=it->svt)
  {
   if(sauter) // On passe tout de suite au suivant.
    {
     prcdt = &(it->E());
     sauter = false;
     continue;
    }
    
   cour = &(it->E());
   seg_cour.maj(*prcdt, *cour);
   if( seg_controle.intersection(seg_cour, intersect) )
    {
     nb_coupures++;
     if( intersect.equivalent(seg_cour.b, EPSILON) )
      {
       // Sélectionner le sommet suivant:
       it_svt =it; it_svt++;
       if(it_svt == it_fin) svt = &( L_points.Premier()->E() );
        else svt = &(it_svt->E());
       // Voir si le précédent et le suivant sont du même coté que seg_controle
       // et si oui alors considérer qu'on a 2 intersections ET ne pas tester les
       // intersections avec le segment suivant.
       if( signe( normale_seg_controle.produit_scalaire( alx_vecteur2D(p, seg_cour.a)) )
         ==signe( normale_seg_controle.produit_scalaire( alx_vecteur2D(p, *svt)) )  )
        {
         nb_coupures++;
         sauter = true;
        }
      }
    }
   prcdt = cour;
  }

 if(!sauter) // Tester le segment dernier_point-premier_point
  {
   it = L_points.Premier();
   cour = &(it->E());
   it=it->svt; svt = &(it->E());
   seg_cour.maj(*prcdt, *cour);
   if( seg_controle.intersection(seg_cour, intersect) )
    {
     nb_coupures++;
     if( intersect.equivalent(*cour, EPSILON)
       &&( signe( normale_seg_controle.produit_scalaire( alx_vecteur2D(p, *prcdt)) )
         !=signe( normale_seg_controle.produit_scalaire( alx_vecteur2D(p, *svt)) )  ) )
      nb_coupures--;
    }
  }

 return (nb_coupures&1) == 1;
}

/******************************************************************************/
/*********************************** Des infos ********************************/
/******************************************************************************/
double alx_polygone2D::longueur()
{
 double rep = 0.0;
 alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();
 alx_point2D *prcdt = &( L_points.Premier()->E() );

 it = L_points.Premier(); if(it!=it_fin) it=it->svt; else return 0.0;
 for(;it!=it_fin;it=it->svt)
  {
   rep += prcdt->distance( (it->E()) );
   prcdt = &(it->E());
  }

 return rep;
}

void alx_polygone2D::Barycentre(alx_point2D &b)
{
 double x = 0, y = 0;
 int    nb = 0;
 alx_element_liste<alx_point2D> *it, *it_fin = L_points.Fin();
 alx_point2D *cour;

 for(it=L_points.Premier(); it!=it_fin; it=it->svt)
  {
   cour = &(it->E());
   x += cour->x;
   y += cour->y;
   nb++;
  }

 b.x = x/nb;
 b.y = y/nb;
}

alx_point2D& alx_polygone2D::barycentre()
{alx_point2D *rep = new alx_point2D;
 Barycentre( *rep );
 return *rep;}

/******************************************************************************/
/************************ Les surcharges d'opérateurs *************************/
/******************************************************************************/
void alx_polygone2D::maj(const alx_polygone2D &pol)
{L_points.maj(pol.L_points);}

alx_polygone2D &alx_polygone2D::operator =(const alx_polygone2D &pol)
{maj(pol);
 return *this;}


