#ifndef __ALX_UTILITAIRES_LISTES_H__
#define __ALX_UTILITAIRES_LISTES_H__

//#include <list.h>

/* TOUT A REVERIFIER ICI !!!!! */

template <class T1, class T2> bool Dans_liste(T1 &e, alx_liste<T2> &L, bool reco(T1 &, T2 &) )
{
 alx_element_liste<T2> *Lit     = L.Premier()
                     , *Lit_fin = L.Fin();

 while( ( Lit != Lit_fin   )
      &&( !reco(e, (Lit->E())) )
      ) Lit=Lit->svt;

 return Lit != Lit_fin;
}

template <class T> void inserer_dans_liste( alx_liste<T> &L
                                          , const T &e
                                          , bool comp(const T&, const T&)
                                          , alx_element_liste<T> **it )
{
 alx_element_liste<T> *it_fin = L.Fin();

 *it = L.Premier();
 while( (*it != it_fin)
      &&(!comp(e, ((*it)->E())) )  ) (*it)=(*it)->svt;

 if((*it)==it_fin)
  {(*it)=(*it)->prcdt;
   L.Ajouter_a_la_fin(e);
   (*it)=(*it)->svt;
  }
  else
   {L.Ajouter_avant(e, (*it));
    (*it)=(*it)->prcdt;
   }
}

template <class T> void inserer_dans_liste( alx_liste<T> &L
                                          , const T e
                                          , bool comp(const T, const T)
                                          , alx_element_liste<T> **it )
{
 alx_element_liste<T> *it_fin = L.Fin();

 (*it) = L.Premier();
 while( ((*it) != it_fin)
      &&(!comp(e, ((*it)->E())) )  ) (*it)=(*it)->svt;

 if((*it)==it_fin)
  {(*it)=(*it)->prcdt;
   L.Ajouter_a_la_fin(e);
   (*it)=(*it)->svt;
  }
  else
   {L.Ajouter_avant(e, *it);}
}

#endif
