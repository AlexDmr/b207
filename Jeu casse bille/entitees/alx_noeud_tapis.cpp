#include "alx_noeud_tapis.h"

//------------------------------------------------------------------------------
//----------------------------- Les constructeurs ------------------------------
//------------------------------------------------------------------------------
void alx_noeud_tapis::redim_zone(const double lg, const double ht)
{
 alx_element_liste<alx_point2D> *it_pt = mp_tapis.L_polygone2D.Premier_E().L_points.Premier();

 it_pt = it_pt->svt; // On chope  le deuxième point
 ((it_pt->E())).maj(lg, 0.0); it_pt = it_pt->svt;
 ((it_pt->E())).maj(lg, ht); it_pt = it_pt->svt;
 ((it_pt->E())).maj(0.0, ht);
}
//------------------------------------------------------------------------------
alx_noeud_tapis::alx_noeud_tapis(const double lg, const double ht) 
{
 operation_si_appuie = ALX_NOEUD_OPERATION_SI_CONTIENT_ALORS_RIEN;

 alx_liste<alx_point2D> L_pt;
 for(int i=0;i<4;i++)
   L_pt.Ajouter_a_la_fin( alx_point2D(0.0, 0.0) );
 mp_tapis.Ajouter_polygone2D(L_pt, 1.0);

 redim_zone(lg, ht);

 mp_tapis.abonner_a( (alx_classe_base_liant*)this, 1);
}

//------------------------------------------------------------------------------
//---------------------------- les méthodes ------------------------------------
//------------------------------------------------------------------------------
void alx_noeud_tapis::Afficher()
{
 Afficher_fils(0);
}

//------------------------------------------------------------------------------
info_du_contenant* alx_noeud_tapis::Contient(alx_point2D &pt, int action)
{
 if( mp_tapis.Contient( pt ) )
  {
   info_du_contenant *rep = new info_du_contenant;
   rep->mp    = &mp_tapis;
   rep->noeud = (alx_noeud_scene*)this;
   return rep;
  }
  else return NULL;
}

//------------------------------------------------------------------------------
void alx_noeud_tapis::rationnaliser(int num)
{
 int *infos;
 switch(num)
  {
   case 1: // Message du modèle physique
     infos = mp_tapis.infos_comp;
     if( (mp_tapis.nb_infos_comp == 5)
       &&(infos[0] == 6)
       &&(infos[1] == 0) )
      {
       alx_point2D pt( *((alx_point2D*)(void*)(infos[2])) );
       // on a un point exprimé dans le repère d'origne de l'arbre d'affichage,
       // il faut lui appliquer toutes les transformations qui ont mené jusqu'ici.
       Changer_coordonnees( pt );
       Point_contact_detecte( &pt );
      }
   break;
  }
}

//------------------------------------------------------------------------------
void alx_noeud_tapis::Point_contact_detecte(alx_point2D *pt)
{
 nb_infos_comp = 3;
 infos_comp[0] = 0;
 infos_comp[1] = 0;
 infos_comp[2] = (int)(void*)pt;
 Oyez_oyez();
}


