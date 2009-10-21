#include <stdio.h>
#include "element_generique.h"

// Les constructeurs
template <class T> element_generique<T>::element_generique ()
{
 prcdt = NULL;
 svt   = NULL;
}

template <class T> element_generique<T>::element_generique(T *e)
{
 prcdt = NULL;
 svt   = NULL.
 elmt  = *e;
}

template <class T> element_generique<T>::~element_generique()
{}

 // Les méthodes
template <class T> void element_generique<T>::lier(element_generique *p, element_generique *s)
{
 prcdt = p;
 svt   = s;
 p->svt   = this;
 s->prcdt = this;
}
