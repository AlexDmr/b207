#ifndef __ALX_CHAINE_CHAR_H__
#define __ALX_CHAINE_CHAR_H__

#include <string.h>
class alx_chaine_char
{
 friend const alx_chaine_char& operator+(const bool, const alx_chaine_char &);
 friend const alx_chaine_char& operator+(const alx_chaine_char &, const bool);
 friend const alx_chaine_char& operator+(const int, const alx_chaine_char &);
 friend const alx_chaine_char& operator+(const alx_chaine_char &, const int);
 friend const alx_chaine_char& operator+(const unsigned int, const alx_chaine_char &);
 friend const alx_chaine_char& operator+(const alx_chaine_char &, const unsigned int);
 friend const alx_chaine_char& operator+(const double, const alx_chaine_char &);
 friend const alx_chaine_char& operator+(const alx_chaine_char &, const double);
 friend const alx_chaine_char& operator+(const char *, const alx_chaine_char &);
 friend const alx_chaine_char& operator+(const alx_chaine_char &, const char *);
 friend const alx_chaine_char& operator+(const alx_chaine_char &, const alx_chaine_char &);
 // Classement alphabétique
 //friend const bool operator<(const alx_chaine_char &, const alx_chaine_char &);
 //friend const bool operator>(const alx_chaine_char &, const alx_chaine_char &);

 private:
   char *texte, *tmp;
   unsigned int taille, taille_reelle, taille_tmp;
   void init();
   void Reallouer_texte(const unsigned int t);
   void Reallouer_tmp  (const unsigned int t);

 protected:

 public:
  // Les constructeurs
   alx_chaine_char();
   alx_chaine_char(const alx_chaine_char &);
   alx_chaine_char(const unsigned int n);
   alx_chaine_char(const char *txt);
   alx_chaine_char(const char *txt, const unsigned int n);
  ~alx_chaine_char();

 // Les méthodes
   const bool Egal_a(const char *txt, const unsigned int premier, const unsigned int dernier) const;
   const bool Prefixe_de(const char *txt) const;
   void Effacer(const unsigned int dep, const unsigned int nb);
   void Inserer(const unsigned int pos, const char *c);
   void Inserer(const unsigned int pos, const alx_chaine_char &c);

   inline const unsigned int Taille() const {return taille;}
   inline const char*        Texte()  const {return texte;}
   inline void               Texte(const char *t)  {*this = t;}

 // Pour sauver dans un fichier
   inline void Ecrire_dans(const alx_chaine_char &nom) {Ecrire_dans(nom.Texte(), Taille()+1);}
   void Ecrire_dans(const char *nom, const unsigned int nb);
   //void Lire_dans(const alx_chaine_char &nom) {Lire_dans(nom.Texte());}
   void Lire_dans(const char *nom);
  // Outils pour le parcours de la chaine
   const bool   Lire_bool(unsigned int & pos)   const;
   const int    Lire_entier(unsigned int & pos) const;
   const double Lire_double(unsigned int & pos) const;
   void         Maj(const char *txt, const unsigned int dep, const unsigned int nb_char);
   void         Maj(const char *txt, const unsigned int nb_char);
   void         Ajouter(const char *txt, const unsigned int dep, const unsigned int nb_char);
   const unsigned int Position_du_prochain(const char c, const unsigned int p) const;
   void Fixer_taille(const unsigned int t);
   const unsigned int Remplacer_char_par(const char c1, const char c2);
   inline const bool Remplacer_char(const unsigned int index, const char c) {if(index >= taille_reelle) return false;
                                                                             texte[index] = c;
                                                                             return true;}
   inline const char Char(const unsigned int index) {if(index >= taille_reelle) return 0;
                                                     return texte[index];}

 // Les surcharges d'opérateurs unaires.
   size_t operator()(const alx_chaine_char &) const {return (size_t)texte;}; // For hash_map 
   const bool operator!=(const alx_chaine_char &) const;  // Surcharge du  !=
   const bool operator==(const alx_chaine_char &) const;  // Surcharge du  ==
   const bool operator!=(const char *) const;  // Surcharge du  ==
   const bool operator==(const char *) const;  // Surcharge du  ==
   const alx_chaine_char &operator =(const alx_chaine_char &);  // Surcharge du  =
   const alx_chaine_char &operator =(const char *);
   const alx_chaine_char &operator =(const bool);
   const alx_chaine_char &operator =(const int);
   const alx_chaine_char &operator =(const unsigned int);
   const alx_chaine_char &operator =(const unsigned long int);
   const alx_chaine_char &operator =(const double);
   const alx_chaine_char &operator+=(const alx_chaine_char &);  // Surcharge du  +=
   const alx_chaine_char &operator+=(const char *);
   const alx_chaine_char &operator+=(const bool);
   const alx_chaine_char &operator+=(const int);
   const alx_chaine_char &operator+=(const unsigned int);
   const alx_chaine_char &operator+=(const unsigned long int);
   const alx_chaine_char &operator+=(const double);

   const bool operator<(const alx_chaine_char& p) const;
   const bool operator>(const alx_chaine_char& p) const;
};

const alx_chaine_char& Chaine_vide();
#endif
