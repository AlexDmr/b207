#ifndef __ALX_LIEN_INTERET_H__
#define __ALX_LIEN_INTERET_H__

//#include "alx_classe_base_liant.h"
class alx_classe_base_liant;

class lien_interet
{
 public:
  // Les constituants:
   alx_classe_base_liant *interessee; // Adresse de l'intéressée
   int num;                           // Numéro identifiant le modèle pour l'intéressé.

  // Les constructeurs:
   inline lien_interet() {interessee = (alx_classe_base_liant*)0;}
   inline lien_interet( alx_classe_base_liant *i
                      , int n )
    {interessee = i;
     num = n;}

  // Les autres méthodes:
   inline void maj(const lien_interet &i)
    {interessee = i.interessee;
     num        = i.num;}

   inline lien_interet &operator =(const lien_interet &i)
    {maj(i);
     return *this;}
};


#endif
