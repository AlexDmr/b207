#ifndef __ALX_ARBRE_H__
#define __ALX_ARBRE_H__

#include "../physique/alx_classes_bases.h"
#include "alx_liste.h"
//#include <list.h>

template <class T> class alx_arbre// : public alx_classe_base_model
{
 private:
   void init() { pere = (alx_arbre<T>*)0; }

 public :
   T e;
   alx_arbre<T> *pere;
   alx_liste<alx_arbre<T>*> L_peres;
   alx_liste<alx_arbre<T>*> L_fils;

  // Les constructeurs
   alx_arbre();
   alx_arbre(T element);

  // Les méthodes
   inline T&            E()             {return e;}
   inline const T&      Const_E() const {return e;}
   inline alx_arbre<T>* Pere() {return pere;}
   void Vider_peres();
   void Vider_fils();
   inline const unsigned int Nb_fils () const {return L_fils.Taille();}
   inline const unsigned int Nb_Peres() const {return L_peres.Taille();}
   alx_arbre<T>* Fils_numero(const unsigned int pos) {return L_fils.E(pos);}
   bool A_pour_fils(alx_arbre<T> *e);
   void Ajouter_fils_replique(alx_arbre<T> *e);
   void Ajouter_fils(alx_arbre<T> *e);
   inline void Ajouter_fils_au_debut(alx_arbre<T> *e) {L_fils.Ajouter_au_debut(e); e->L_peres.Ajouter_au_debut( this );}
   inline void Ajouter_fils_a_la_fin(alx_arbre<T> *e) {L_fils.Ajouter_a_la_fin(e); e->L_peres.Ajouter_au_debut( this );}
   inline void Ajouter_fils_a_pos(alx_arbre<T> *e, const unsigned int pos) {L_fils.Ajouter_a_pos(e, pos); e->L_peres.Ajouter_au_debut( this );}
   inline void Ajouter_fils(alx_arbre<T> *e, alx_element_liste<alx_arbre<T>*> &it)
    {L_fils.Ajouter_avant(e, &it);
     it.prcdt->E()->pere = this;
     it.prcdt->E()->L_peres.Ajouter_au_debut( this );}

   void Retirer_fils(alx_arbre<T> *e);
   inline void Retirer_fils(alx_element_liste<alx_arbre<T>*> &it)
    {it.E()->L_peres.Retirer( this );
     //pere = NULL;
     L_fils.Retirer(&it);}

   T*   Rechercher_et_traiter( bool f(T *e, void *info_comp), void *info );
   void Parcourir_et_traiter ( bool f(T *e, void *info_comp), void *info );
};

#endif
