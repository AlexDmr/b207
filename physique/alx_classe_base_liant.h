#ifndef __ALX_CLASSE_BASE_LIANT_H__
#define __ALX_CLASSE_BASE_LIANT_H__

#include "alx_classe_base_modele.h"
class alx_classe_base_model;

class alx_classe_base_liant : public alx_classe_base_model
{
 bool bloquer_rationnalisation;
 
 public:
   alx_classe_base_liant() : alx_classe_base_model() {bloquer_rationnalisation = false;}

   virtual void rationnaliser(int num) {} // Rationnalise à partir du modèle numéro num.
   inline const bool Bloquer_rationnalisation() const {return bloquer_rationnalisation;}
   inline void Bloquer_rationnalisation(const bool b) {bloquer_rationnalisation = b;}
};

#endif
