#ifndef __ALX_CLASSE_BASE_ENTITE_H__
#define __ALX_CLASSE_BASE_ENTITE_H__

#include "stdio.h"

class alx_classe_base_entite
{
 public:
  alx_classe_base_entite() {}
  virtual void rationnaliser(int num_modele) {/*printf("maj(%d) foireuse ", num_modele);*/}
  virtual void agir() {}

};

#endif
 