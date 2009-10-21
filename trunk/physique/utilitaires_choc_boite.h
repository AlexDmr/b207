#include "math_alex.cpp" 

// Construit dans mbm (modèle de boite englobant le mouvement) la boite englobant l'entité
// modélisé par mp1 (modèle physique 1) et mb1 (modèle de boite englobant le modèle
// de choc de l'entité définit dans son repère propre) au cour de son DERNIER déplacement.
void construire_boite_englobant_deplacement( alx_model_simulation_physique1 &mp1
                                           , alx_model_simulation_bidon1 &mb1
                                           , alx_model_simulation_bidon1 &mbm)
{alx_segment2D *der_seg;
 double minx, maxx
      , miny, maxy;
      
 der_seg = &( mp1.L_dep.Dernier()->E().seg );
 mbm.maj(mb1);
 
 minx = Amin( der_seg->a.x, der_seg->b.x);
 maxx = Amax( der_seg->a.x, der_seg->b.x);
 miny = Amin( der_seg->a.y, der_seg->b.y);
 maxy = Amax( der_seg->a.y, der_seg->b.y);

 mbm.x += minx;
 mbm.y += miny;
 mbm.l += maxx - minx;
 mbm.h += maxy - miny;
}

/******************************************************************************/
inline bool comp_boite_x_sup( models_utils_choc_2D *m1
                            , models_utils_choc_2D *m2)
{return m1->mb->x >= m2->mb->x;}
                        /*****************************/
inline bool comp_boite_x_inf( models_utils_choc_2D *m1
                            , models_utils_choc_2D *m2)
{return m1->mb->x <= m2->mb->x;}

                        /*****************************/
inline bool comp_boite_y_sup( models_utils_choc_2D *m1
                            , models_utils_choc_2D *m2)
{return m1->mb->y >= m2->mb->y;}

                        /*****************************/
inline bool comp_boite_y_inf( models_utils_choc_2D *m1
                            , models_utils_choc_2D *m2)
{return m1->mb->y <= m2->mb->y;}
/******************************************************************************/

// Tri par insertion
// UN PROBLEME POTENTIEL ICI !
template <class T> void trier_liste( alx_liste<T> &L, bool(comp)(T, T) )
{alx_element_liste<T> *it_prcdt
                    , *it_cour
                    , *it_deb = L.Premier()
                    , *it_fin = L.Fin();

 it_prcdt = it_deb;
 if(it_prcdt != it_fin)
  {
   it_cour = it_prcdt;
   it_cour=it_cour->svt;
  }
  else
   {it_cour = it_prcdt;}

 while( it_cour != it_fin )
  {
   if( comp(it_cour->E(), it_prcdt->E()) )
    { // On avance
     it_cour  = it_cour->svt;
     it_prcdt = it_prcdt->svt;
    }
    else
     { // on permutte et on recule si on n'est pas déja au début
      L.Ajouter_avant(it_cour->E(), it_prcdt); // *it_cour avant it_prcdt
      L.Retirer( it_cour );
      it_cour = it_prcdt;
      it_prcdt=it_prcdt->prcdt;
      if( it_prcdt != L.Premier() )
       {
        it_cour  = it_cour->prcdt;
        it_prcdt = it_prcdt->prcdt;}
       else
        {it_cour  = it_cour->svt;
         it_prcdt = it_prcdt->svt;}
     }
  }
}

// Fusion simple de 2 listes dans la première.
template <class T> void fusionner( alx_liste<T> &L1
                                 , alx_liste<T> &L2 )
{alx_element_liste<T> *it, *it_fin = L2.Fin();

 for(it=L2.Premier(); it!=it_fin; it=it->svt)
  L1.Ajouter_au_debut( it->E() );
}


// Ces fonctions visent à diviser de façon grossière et rapide un ensemble
// de modèle de boite en plusieurs ensembles de boites qui semblent ne pas
// pouvoir être séparés si on considère leurs projections en x et y.
void degrouper_x( alx_liste< alx_liste<models_utils_choc_2D*> > &LL );
void degrouper_y( alx_liste< alx_liste<models_utils_choc_2D*> > &LL );


inline void degrouper( alx_liste< alx_liste<models_utils_choc_2D*> > &LL )
{degrouper_x( LL );}

void degrouper_x( alx_liste< alx_liste<models_utils_choc_2D*> > &LL )
{
 alx_element_liste< alx_liste<models_utils_choc_2D*> > *itl, *itl_tmp;
 alx_liste<models_utils_choc_2D*> *PL_temp;
 alx_element_liste<models_utils_choc_2D*> *itb, *itb_fin;
 double M;
 int taille, i;

 taille = LL.Taille();
 itl = LL.Premier();

 for(i=0;i<taille;i++)
  {
   // trier la liste en cour suivant x.
   trier_liste(itl->E(), comp_boite_x_sup);

   // Découper la liste en cour, placer les morceaux à la fin de LL.
   itb     = (itl->E()).Premier();
   itb_fin = (itl->E()).Fin();
   M = (itb->E())->mb->x + (itb->E())->mb->l;
   PL_temp = new alx_liste<models_utils_choc_2D*>;

   for(; itb != itb_fin; itb=itb->svt)
    {
     if( (itb->E())->mb->x > M )
      { // On peut diviser la liste
       LL.Ajouter_a_la_fin( *PL_temp );
       PL_temp = new alx_liste<models_utils_choc_2D*>;
      }
     M = Amax( (itb->E())->mb->x + (itb->E())->mb->l, M);
     PL_temp->Ajouter_a_la_fin( itb->E() );
    }

   LL.Ajouter_a_la_fin( *PL_temp );

   // Effacer la liste considérée.
   itl_tmp = itl;
   itl = itl->svt;
   LL.Retirer( itl_tmp );
  }

 // On coupe en y;
 degrouper_y(LL);
}


void degrouper_y( alx_liste< alx_liste<models_utils_choc_2D*> > &LL )
{
 alx_liste< alx_liste<models_utils_choc_2D*> > PLL_temp;
 alx_element_liste< alx_liste<models_utils_choc_2D*> > *itl, *itl_tmp;
 alx_liste<models_utils_choc_2D*> *PL_temp;
 alx_element_liste<models_utils_choc_2D*> *itb, *itb_fin;
 bool degroupe;
 double M;
 int taille, i, pos_y;

 taille = LL.Taille();
 itl = LL.Premier();

 for(i=0;i<taille;i++)
  {
   // trier la liste en cour suivant y.
   trier_liste(itl->E(), comp_boite_y_sup);

   // Découper la liste en cour, placer les morceaux à la fin de LL.
   itb     = (itl->E()).Premier();
   itb_fin = (itl->E()).Fin();
   M = (itb->E())->mb->y + (itb->E())->mb->h;
   PL_temp = new alx_liste<models_utils_choc_2D*>;
   degroupe = false;

   for(; itb != itb_fin; itb=itb->svt)
    {
     pos_y = (itb->E())->mb->y;
     if( pos_y > M )
      { // On peut diviser la liste
       degroupe = true;
       PLL_temp.Ajouter_a_la_fin( *PL_temp );
       PL_temp = new alx_liste<models_utils_choc_2D*>;
      }
     M = Amax( (itb->E())->mb->y + (itb->E())->mb->h, M);
     PL_temp->Ajouter_a_la_fin( itb->E() );
    }

   if( degroupe )
    {PLL_temp.Ajouter_a_la_fin( *PL_temp );
     // Effacer la liste considérée.
     itl_tmp = itl;
     itl = itl->svt;
     LL.Retirer( itl_tmp );
    }
    else
     {itl = itl->svt;
      delete PL_temp;
     } 
  }

 // On coupe en y;
 if( PLL_temp.Taille() != 0)
  {
/*   int taille_LL = LL.size()
     , taille_PLL_temp = PLL_temp.size(); */

   degrouper_x( PLL_temp );
   fusionner(LL, PLL_temp);
   /*taille_LL = LL.size();
   taille_PLL_temp = PLL_temp.size();

   taille = taille_LL;*/
  }
}

