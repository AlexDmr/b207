#ifndef __ALXELEMENTGENERIQUE__
#define __ALXELEMENTGENERIQUE__

template <class T>
class element_generique
{
 public:
  element_generique *prcdt, *svt;
  T elmt;

 // Les constructeurs
  element_generique();
  element_generique(T *e);
  ~element_generique();

 // Les méthodes
  void lier(element_generique *p, element_generique *s);
};

#endif
