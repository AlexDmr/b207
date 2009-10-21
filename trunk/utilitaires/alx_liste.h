#ifndef __ALX_LISTE_H__
#define __ALX_LISTE_H__

//#define DEBUG_LISTE 1
#ifdef DEBUG_LISTE
 #include <stdio.h>
#endif
template <class T> class alx_element_liste
{
 private:
   T e;
   inline void init() {prcdt = svt = (alx_element_liste<T> *)0;}
 public:
   mutable alx_element_liste<T> *prcdt, *svt;
   alx_element_liste()           {init();}
   alx_element_liste(const T &e) {init(); this->e = e;}

   inline T& E() {return e;}
   inline const T& Const_E() const {return e;}
   //inline T &operator *() {return e;}  // Surcharge du  *
//   friend bool operator == (const alx_element_liste<T> e1, const alx_element_liste<T> e2) {return e1.e == e2.e;};
//   friend bool operator != (const alx_element_liste<T> e1, const alx_element_liste<T> e2) {return e1.e != e2.e;};
   inline bool operator==(alx_element_liste<T> element) {return e == element.e;}  // Surcharge du  ==
   inline bool operator!=(alx_element_liste<T> element) {return e != element.e;}  // Surcharge du  ==
};

template <class T> class alx_liste
{private:
   unsigned int nb;
   mutable alx_element_liste<T> *cour, *n;
   #ifdef DEBUG_LISTE
     mutable alx_element_liste<T> *tmp_debug;
   #endif

   void init()
    {nb=0;
     debut = new alx_element_liste<T>();
     fin   = new alx_element_liste<T>();
     debut->svt = fin;
     fin->prcdt = debut;}

   mutable alx_element_liste<T> *debut, *fin;

 public :
   alx_liste() {init();}
   alx_liste(const alx_liste &l) {init(); maj(l);}
  ~alx_liste() {Vider();
                delete debut; debut = (alx_element_liste<T> *)0;
                delete fin;   fin   = (alx_element_liste<T> *)0;}
   inline const unsigned int Taille() const {return nb;}

  // Opérations d'apartenances
   const bool Contient(const T element_recherche) const
   {alx_element_liste<T> *it     = Premier()
                       , *it_fin = Fin();
    for(;it!=it_fin;it=it->svt)
      if(it->E() == element_recherche) return true;

     return false;
    }

//____________________________________________________________________________
  // Pour un parcours plus facile...
   inline alx_element_liste<T>* Init_parcours()                    const {alx_element_liste<T> *it = debut->svt; return it;}
   inline const bool Est_parcours_fini(alx_element_liste<T> *it)   const {return it == fin;}
   inline const bool Parcours_continue(alx_element_liste<T> *it)   const {return it != fin;}
   inline const T  Courant_dans_parcours(alx_element_liste<T> *it) const {return it->E();}
   inline alx_element_liste<T>* Suivant_dans_parcours(alx_element_liste<T> *it) const {return it->svt; }
   inline void Terminer_parcours(alx_element_liste<T> *it)         const {}

   inline alx_element_liste<T>* Init_parcours_inverse_fils()                    const {alx_element_liste<T> *it = fin->prcdt; return it;}
   inline const bool Est_parcours_inverse_fils_fini(alx_element_liste<T> *it)   const {return it==debut;}
   inline const bool Parcours_inverse_fils_continue(alx_element_liste<T> *it)   const {return it!=debut;}
   inline const T  Courant_dans_parcours_inverse_fils(alx_element_liste<T> *it) const {return it->E();}
   inline alx_element_liste<T>* Suivant_dans_parcours_inverse_fils(alx_element_liste<T> *it)     const {return it->prcdt;}
   inline void Terminer_parcours_inverse(alx_element_liste<T> *it)              const {}

//____________________________________________________________________________
  // Gestion des fils...
   inline alx_element_liste<T>* Ajouter_a_pos(const T &e, const unsigned int pos)
    {alx_element_liste<T> *it_a_pos;
     if(pos >= Taille()) {it_a_pos = Fin();} else {it_a_pos = It(pos);}
     return Ajouter_avant(e, it_a_pos);
    }
   inline alx_element_liste<T>* Ajouter_avant(const T &e, alx_element_liste<T> *p)
    {n = new alx_element_liste<T>(e);
     n->svt   = p;
     n->prcdt = p->prcdt;
     p->prcdt->svt = n;
     p->prcdt = n;
     nb++;
     return n;}
   inline alx_element_liste<T>* Ajouter_apres(const T &e, alx_element_liste<T> *p)
    {n = new alx_element_liste<T>(e);
     n->prcdt = p;
     n->svt   = p->svt;
     p->svt->prcdt = n;
     p->svt = n;
     nb++;
     return n;}
   inline alx_element_liste<T>* Ajouter_au_debut(const T &e)
    {return Ajouter_apres(e, debut);}
   inline alx_element_liste<T>* Ajouter_a_la_fin(const T &e)
    {return Ajouter_avant(e, fin);}

   inline const bool Retirer_premier() {return Retirer(Premier());}
   inline const bool Retirer_dernier() {return Retirer(Dernier());}
   inline const bool Retirer(alx_element_liste<T> *p)
    {
     #ifdef DEBUG_LISTE
      // p fait il partit de la liste????
      for(tmp_debug = debut->svt; tmp_debug != fin; tmp_debug=tmp_debug->svt)
        {if(tmp_debug==p) break;}
      if( (tmp_debug == fin)
        ||(p == fin)
        ||(p == debut) )
        printf("DEBUG");
    #endif

     p->prcdt->svt = p->svt;
     p->svt->prcdt = p->prcdt;
     delete p;
     nb--;
     return true;
    }
   const bool Retirer(const T &e)
    {alx_element_liste<T> *it = Premier();
     while( (it != fin)
          &&(it->E() != e) )
       it = it->svt;
     if(it != fin)
      {return Retirer(it);}
     return false;
    }
   const bool Retirer_index(const unsigned int index)
    {alx_element_liste<T> *it = Premier();
     unsigned int pos = 0;
     while( (it != fin)
          &&(pos++ != index) )
       it = it->svt;
     if(it != fin)
      {return Retirer(it);}
     return false;
    }

//____________________________________________________________________________
   const unsigned int Index(const T &e                   ) const {cour = Premier();
                                                            unsigned int pos = 0;
                                                            while( (cour != fin)
                                                                 &&(cour->E() != e) )
                                                             {pos++; cour = cour->svt;}
                                                            if(cour != fin)
                                                              return pos;
                                                            return -1;
                                                           }
   const unsigned int Index(const alx_element_liste<T> *e) const {cour = Premier();
                                                            unsigned int pos = 0;
                                                            while( (cour != fin)
                                                                 &&(cour != e) )
                                                             {pos++; cour = cour->svt;}
                                                            if(cour != fin)
                                                              return pos;
                                                            return -1;
                                                           }

   alx_element_liste<T>* It(const T &element) const {
     alx_element_liste<T> *it = Premier();
     while( (it != fin)
          &&(it->E() != element) ) it = it->svt;
     return it;
    }
   alx_element_liste<T>* It(const unsigned int pos) const {if(pos >= nb) return Fin();
                                                           alx_element_liste<T> *it = Premier();
                                                           for(unsigned int i=0;i<pos;i++) it=it->svt;
                                                           return it;
                                                          }

//____________________________________________________________________________
   T  Copie_de_E(const unsigned int pos) const {return It(pos)->E();}
   T& E(const unsigned int pos) {return It(pos)->E();}
   T& E(alx_element_liste<T> *it) {return it->E();}
   T& Premier_E() {return E(Premier());}
   T& Dernier_E() {return E(Dernier());}

   const T& Premier_E_const() const {return E(Premier());}
   const T& Dernier_E_const() const {return E(Dernier());}

//____________________________________________________________________________
   inline const bool Vide() const {return nb==0;}
   void Vider()
    {cour = Premier();
     while(cour != Fin())
      {n = cour->svt;
       delete cour;
       cour = n;}
     nb = 0;
     debut->svt = fin;
     fin->prcdt = debut;
    }
  //____________________________________________________________________________
   void maj(const alx_liste &l)
    {Vider();
     alx_element_liste<T> *it     = l.Premier()
                        , *it_fin = l.Fin();
     for(;it!=it_fin;it=it->svt)
      {Ajouter_a_la_fin( it->E() );}
    }

  //____________________________________________________________________________
 // Sous listes
  inline alx_liste<T>& Sous_liste_It(alx_liste<T> &L_rep, const alx_element_liste<T> *it_premier, const alx_element_liste<T> *it_dernier) const
   {L_rep.Vider();
    if(Taille() > 0)
     {const alx_element_liste<T> *it_p, *it_d;
      if(it_premier == Fin()) {it_p = Dernier();} else {it_p = it_premier;}
      if(it_dernier == Fin()) {it_d = Dernier();} else {it_d = it_dernier;}
      L_rep.Ajouter_a_la_fin(*this, it_p, it_d->svt);
     }
    return L_rep;
   }

//____________________________________________________________________________
  inline alx_liste<T>& Sous_liste_E(alx_liste<T> &L_rep, const T &premier, const T &dernier) const
   {return Sous_liste_It(L_rep, It(premier), It(dernier));
   }

//____________________________________________________________________________
  inline alx_liste<T>& Sous_liste_index(alx_liste<T> &L_rep, const unsigned int i_premier, const unsigned int i_dernier) const
   {L_rep.Vider();
    if(Taille() > 0)
     {const unsigned int i_p = i_premier<Taille()?i_premier:(Taille()-1)
                       , i_d = i_dernier<Taille()?i_dernier:(Taille()-1);
      L_rep.Ajouter_a_la_fin(*this, It(i_p), It(i_d)->svt);
     }
    return L_rep;
   }

//____________________________________________________________________________
 // Les manips avec d'autres listes :
  inline void Ajouter_a_la_fin(const alx_liste<T> &l)
   {Ajouter_avant(l, Fin());}
   //____________________________________________________________________________
  void Ajouter_au_debut(const alx_liste<T> &l)
   {Ajouter_apres(l, Debut());}
   //____________________________________________________________________________
  void Ajouter_avant(const alx_liste<T> &l, alx_element_liste<T> *p)
   {alx_element_liste<T> *it     = l.Premier()
                       , *it_fin = l.Fin();
    for(;it!=it_fin;it=it->svt)
      Ajouter_avant(it->E(), p);
   }
   //____________________________________________________________________________
  void Ajouter_apres(const alx_liste<T> &l, alx_element_liste<T> *p)
   {alx_element_liste<T> *it     = l.Dernier()
                       , *it_deb = l.Debut();
    for(;it!=it_deb;it=it->prcdt)
      Ajouter_apres(it->E(), p);
   }
   //____________________________________________________________________________
 // Les manips avec des sous listes
  inline void Ajouter_a_la_fin(const alx_liste<T> &l, const alx_element_liste<T> *deb, const alx_element_liste<T> *fin)
   {Ajouter_apres(l, Dernier(), deb, fin);}
   //____________________________________________________________________________
  void Ajouter_au_debut(const alx_liste<T> &l, const alx_element_liste<T> *deb, const alx_element_liste<T> *fin)
   {Ajouter_avant(l, Premier(), deb, fin);}
   //____________________________________________________________________________
  void Ajouter_avant(const alx_liste<T> &l, alx_element_liste<T> *p, const alx_element_liste<T> *it_deb, const alx_element_liste<T> *it_fin)
   {const alx_element_liste<T> *it = it_deb;
    for(;it!=it_fin && it!=l.Fin(); it=it->svt)
      Ajouter_avant(it->E(), p);
   }
   //____________________________________________________________________________
  void Ajouter_apres(const alx_liste<T> &l, alx_element_liste<T> *p, const alx_element_liste<T> *it_deb, const alx_element_liste<T> *it_fin)
   {if(it_deb==it_fin) return;
    alx_element_liste<T> *it= it_fin->prcdt;
    for(;it!=it_deb && it!=l.Debut(); it=it->prcdt)
      Ajouter_apres(it->E(), p);
    if(it==it_deb) Ajouter_apres(it->E(), p);
   }

//____________________________________________________________________________
 // Les opérations complexes
  void Retirer_liste (const alx_liste<T> &L)
   {alx_element_liste<T> *it     = L.Premier()
                       , *it_fin = L.Fin();
    for(;it!=it_fin;it=it->svt)
     {this->Retirer( it->E() );
     }
   }

//____________________________________________________________________________
  const unsigned int Intersection_avec (const alx_liste<T> &L, alx_liste<T> &L_res)
   {alx_element_liste<T> *it     = L.Premier()
                       , *it_fin = L.Fin();
    L_res.Vider();
    for(;it!=it_fin;it=it->svt)
     {if(this->Contient( it->E() ) ) {L_res.Ajouter_a_la_fin( it->E() );}
     }
    return L_res.Taille();
   }

//____________________________________________________________________________
 // Les accesseurs
  inline alx_element_liste<T>* Premier() const {return debut->svt;}
  inline alx_element_liste<T>* Dernier() const {return fin->prcdt;}
  inline alx_element_liste<T>* Debut()   const {return debut;}
  inline alx_element_liste<T>* Fin()     const {return fin;}

//____________________________________________________________________________
 // Les opérateurs
  alx_liste<T> &operator =(const alx_liste &l) {maj(l); return *this;}
  friend bool operator==(const alx_liste<T> &L1, const alx_liste<T> &L2) {
    if (L1.Taille() != L2.Taille()) {return false;}
    alx_element_liste<T> *it     = L1.Premier()
                       , *it_fin = L1.Fin()
                       , *it_L   = L2.Premier();
    for(;it!=it_fin;it=it->svt)
     {if(it->E() != it_L->E()) {return false;}
      it_L = it_L->svt;
     }
    return true;
   }
   //____________________________________________________________________________
  friend bool operator!=(const alx_liste<T> &L1, const alx_liste<T> &L2) {return !(L1 == L2);}

//____________________________________________________________________________
  const bool Est_prefixe_de(const alx_liste<T> &L) const
   {if(Taille() > L.Taille()) {return false;}
    alx_element_liste<T> *it     = Premier()
                       , *it_fin = Fin()
                       , *it_L   = L.Premier();
    for(;it!=it_fin;it=it->svt)
     {if(it->E() != it_L->E()) {return false;}
      it_L = it_L->svt;
     }
    return true;
   }

  //____________________________________________________________________________
  const bool Est_suffixe_de(const alx_liste<T> &L) const
   {if(Taille() > L.Taille()) {return false;}
    alx_element_liste<T> *it     = Dernier()
                       , *it_deb = Debut()
                       , *it_L   = L.Dernier();
    for(;it!=it_deb;it=it->prcdt)
     {if(it->E() != it_L->E()) {return false;}
      it_L = it_L->prcdt;
     }
    return true;
   }

};



#endif
