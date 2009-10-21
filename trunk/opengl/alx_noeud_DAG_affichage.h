#ifndef __ALX_NOEUD_DAG_AFFICHAGE__
#define __ALX_NOEUD_DAG_AFFICHAGE__

#include <list.h>

class alx_noeud_DAG_affichage
{
 public:
   double translucidite_courante;
   list<alx_noeud_DAG_affichage*> Lperes;
   list<alx_noeud_DAG_affichage*> Lfils;


   alx_noeud_DAG_affichage()
    {
     translucidite_courante = 1.0f;
    }

   virtual void afficher();
   virtual void afficher_fils()
    {
     list<alx_noeud_DAG_affichage*>::iterator it, it_fin = Lfils.end();

     for(it=Lfils.begin(); it!=it_fin; it++)
       (*it)->afficher();
    }

   virtual void afficher_peres()
    {
     list<alx_noeud_DAG_affichage*>::iterator it, it_fin = Lperes.end();

     for(it=Lperes.begin(); it!=it_fin; it++)
       (*it)->afficher();
    }
};

#endif
 