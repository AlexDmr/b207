#ifndef __ALX_ARBRE_CPP__
#define __ALX_ARBRE_CPP__

#include "alx_arbre.h"
#include <stdio.h>

template<class T> alx_arbre<T>::alx_arbre()
{init();}

template<class T> alx_arbre<T>::alx_arbre(T element)
{init(); e = element;}

/******************************************************************************/
/****************************** Les méthodes **********************************/
/******************************************************************************/
template<class T> bool alx_arbre<T>::A_pour_fils(alx_arbre<T> *e)
{alx_element_liste<alx_arbre<T>*> *it     = L_fils.Premier()
                                , *it_fin = L_fils.Fin();
 for(; (it!=it_fin)&&(it->E()!=e); it=it->svt);
 if(it!=it_fin) return true;
  else return false;}

template<class T> void alx_arbre<T>::Ajouter_fils_replique(alx_arbre<T> *e)
{alx_arbre<T> *papa = e->pere;
 L_fils.Ajouter_a_la_fin(e);
 e->L_peres.Ajouter_au_debut( this );
 e->pere = papa;}

//______________________________________________________________________________
template<class T> void alx_arbre<T>::Ajouter_fils(alx_arbre<T> *e)
{L_fils.Ajouter_a_la_fin(e);
 e->L_peres.Ajouter_au_debut( this );
 e->pere = this;}

//______________________________________________________________________________
template<class T> void alx_arbre<T>::Vider_peres()
{alx_element_liste<alx_arbre<T>*> *it;
 pere = NULL;
 while( !L_peres.Vide() )
  {it = L_peres.Premier();
   it->E()->Retirer_fils( this );}
}

//______________________________________________________________________________
template<class T> void alx_arbre<T>::Vider_fils()
{alx_element_liste<alx_arbre<T>*> *it;
 while( !L_fils.Vide() )
  {it = L_fils.Premier();
   it->E()->pere = NULL;
   Retirer_fils( *it );}
}

//______________________________________________________________________________
template<class T> void alx_arbre<T>::Retirer_fils(alx_arbre<T> *e)
{alx_element_liste<alx_arbre<T>*> *it     = L_fils.Premier()
                                , *it_fin = L_fils.Fin();
 for(; (it!=it_fin)&&(it->E()!=e); it=it->svt);
 if(it!=it_fin) return Retirer_fils(*it);}

/******************************************************************************/
template<class T> T* alx_arbre<T>::Rechercher_et_traiter( bool f(T *e, void *info_comp)
                                                        , void *info )
{alx_element_liste<alx_arbre<T>*> *it     = L_fils.Premier()
                                , *it_fin = L_fils.Fin();

 if(f(&e, info))
   return &e;

 T *rep;
 for(; it!=it_fin; it=it->svt)
   if( (rep = (it->E())->Rechercher_et_traiter(f, info)) != NULL )
     return rep;

 return NULL;
}

/******************************************************************************/
template<class T> void alx_arbre<T>::Parcourir_et_traiter ( bool f(T *e, void *info_comp)
                                                          , void *info )
{if( !f(&e, info) ) return; // Pas la peine le traitement est non avenue, on est déja passé par là, on ressort vite avant de boucler!
 alx_element_liste<alx_arbre<T>*> *it     = L_fils.Premier()
                                , *it_fin = L_fils.Fin();
 for(; (it!=it_fin); it=it->svt)
   (it->E())->Parcourir_et_traiter(f, info);
}

#endif
