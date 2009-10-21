#ifndef __ALX_METADATA_H__
#define __ALX_METADATA_H__

#include "alx_chaine_char.h"

class MetaData : private alx_chaine_char
{private:
   unsigned int nb_elements
              , *tailles, *positions
              , nb_reel;
   mutable alx_chaine_char reponse, cc_tmp;
   void init();
   void Allouer(unsigned int nb);
   const int Chercher(const alx_chaine_char &var, unsigned int pos) const;

 public:
  // Constructors
   MetaData();
  ~MetaData();

  // Methods
   const alx_chaine_char& get_alx_chaine_char() const {return *this;}
   const char* get_texte() const {return Texte();}
   const bool Ajouter(const char *var, const char *val);
   const bool Retirer(const char *var);
   const bool Retirer(const alx_chaine_char &var);
   const char* Val_CC(const alx_chaine_char &var) const;
   const char* Val(const char *var)               const;

   void Vider();

   void maj( const MetaData &m );
};

#endif
