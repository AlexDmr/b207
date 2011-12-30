#ifndef __ALX_TABLE_HACHAGE_H__
#define __ALX_TABLE_HACHAGE_H__

#include "..\utilitaires\alx_liste.h"//<list.h>

// Quelques fonctions pour l'insertions:
//typedef bool pfonc(int, int);

bool identique(int a, int b);
bool different(int a, int b);
bool tjs_vrai(int , int );
bool tjs_faux(int , int );

template <class T> class table_hachage
{private:
   template <class T2> class conteneur
    {public:
      T2  e;
      int id;

      conteneur() {}
      conteneur(const T2 e1, const int id1)
       {e  = e1;
        id = id1;}

      const conteneur &operator =(const conteneur &c)
       {e  = c.e;
        id = c.id;
        return *this;}
    };

   int nb_hachage;
   alx_liste< conteneur<T> > **tab;

   void creer_tab(int n)
    {tab = new alx_liste< conteneur<T> >*[n];
     for(int i=0; i<n; i++)
       tab[i] = new alx_liste< conteneur<T> >; }

 public:
 /***********************************************/
  // Les constructeurs:
   table_hachage()
    {nb_hachage = 211;
     creer_tab(nb_hachage);}

   table_hachage(int nb)
    {nb_hachage = nb;
     creer_tab(nb_hachage);}

   ~table_hachage()
    {for(int i=0; i<nb_hachage; i++)
       delete tab[i];
     delete[] tab;
    }

 /***********************************************/
  // Les méthodes:
   const int Nb_hachage() const {return nb_hachage;}
   alx_liste< conteneur<T> >** Tab() {return tab;}
   alx_liste< conteneur<T> >** Const_Tab() const {return tab;}

 /***********************************************/
   void vider()
    {for(int i=0;i<nb_hachage;i++)
       tab[i]->Vider();
    }

 /***********************************************/
   void construire_liste(alx_liste<T> &L)
    {alx_element_liste< conteneur<T> > *it, *it_fin;
     int i;

     for(i=0;i<nb_hachage;i++)
      {it     = tab[i]->Premier();
       it_fin = tab[i]->Fin();
       for(; it!=it_fin; it=it->svt)
         L.Ajouter_a_la_fin( (it->E()).e );
      }
    }

 /***********************************************/
   void inserer(const T e, const int id)
    {int pos = id % nb_hachage;
     conteneur<T> temp(e, id);

     tab[pos]->Ajouter_a_la_fin(temp);
    }

 /***********************************************/
//   bool inserer(T e, int id, pfonc fct)
   bool inserer(const T e, const int id, bool fct(int, int))
    {int pos = id % nb_hachage;
     alx_element_liste< conteneur<T> > *it     = tab[pos]->Premier()
                                     , *it_fin = tab[pos]->Fin();
     while( (it != it_fin)
          &&( fct(id, (it->E()).id)) )
       it=it->svt;

     if(it == it_fin)
      {conteneur<T> temp(e, id);
       tab[pos]->Ajouter_a_la_fin(temp);
       return true;}
     return false;
    }

 /***********************************************/
   bool trouver(const int id, T **e)
    {int pos = id % nb_hachage;
     alx_element_liste< conteneur<T> > *it     = tab[pos]->Premier()
                                     , *it_fin = tab[pos]->Fin();

     while( (it != it_fin)
          &&( (it->E()).id != id ) )
       it=it->svt;

     if(it != it_fin)
      {*e = &((it->E()).e);
       return true;}

     return false;
    }

 /***********************************************/
   bool trouver(const int id, T **e, bool f(T *modele, T *cour), T *modele)
    {int pos = id % nb_hachage;
     alx_element_liste< conteneur<T> > *it     = tab[pos]->Premier()
                                     , *it_fin = tab[pos]->Fin();

     while( (it != it_fin)
          &&( !f(modele, &((it->E()).e)) ))
       it=it->svt;

     if(it != it_fin)
      {*e = &((it->E()).e);
       return true;}

     return false;
    }

 /***********************************************/
   bool oter   (const int id)
    {int pos = id % nb_hachage;
     alx_element_liste< conteneur<T> > *it     = tab[pos]->Premier()
                                     , *it_fin = tab[pos]->Fin();

     while( (it != it_fin)
          &&( (it->E()).id != id ) )
       it=it->svt;

     if(it != it_fin)
      {tab[pos]->Retirer(it);
       return true;}

     return false;
    }

   const table_hachage<T> &operator =(const table_hachage<T> &t)
    {for(int i=0; i<nb_hachage; i++)
       delete tab[i];
     delete[] tab;

     nb_hachage = t.Nb_hachage();
     tab = new alx_liste< conteneur<T> >*[nb_hachage];
     for(int i=0; i<nb_hachage; i++)
      {tab[i] = new alx_liste< conteneur<T> >;
       tab[i]->maj( *(t.Const_Tab()[i]) );}

     return *this;
    }

};

#endif
