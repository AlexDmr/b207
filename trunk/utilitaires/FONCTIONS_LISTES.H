#ifndef __FONCTIONS_LISTES_H__
#define __FONCTIONS_LISTES_H__

template <class T> bool Appartient_a(alx_liste<T> &L, T e)
{alx_element_liste<T> *Lit     = L.Premier()
                    , *Lit_fin = L.Fin();

 while( ( Lit != Lit_fin )
      &&( Lit->E() != e) )
   Lit = Lit->svt;

 return Lit != Lit_fin;
}

template <class T> bool Ajouter_dans_ensemble(alx_liste<T> &L, T e)
{alx_element_liste<T> *Lit     = L.Premier()
                    , *Lit_fin = L.Fin();

 while( ( Lit != Lit_fin )
      &&( Lit->E() != e) )
   Lit = Lit->svt;

 if(Lit == Lit_fin)
  {L.Ajouter_au_debut(e);
   return true;}

 return false;
}


#endif
 