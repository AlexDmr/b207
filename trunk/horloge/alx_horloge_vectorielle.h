#ifndef __ALX_HORLOGE_VECTORIELLE_H__
#define __ALX_HORLOGE_VECTORIELLE_H__

#include "..\utilitaires\alx_chaine_char.h"
#include "..\utilitaires\alx_liste.h"

class alx_horloge_vectorielle
{// Les opérateurs associés
 friend const bool operator<=(const alx_horloge_vectorielle &, const alx_horloge_vectorielle &);
 friend const bool operator>=(const alx_horloge_vectorielle &, const alx_horloge_vectorielle &);
 friend const bool operator< (const alx_horloge_vectorielle &, const alx_horloge_vectorielle &);
 friend const bool operator> (const alx_horloge_vectorielle &, const alx_horloge_vectorielle &);
 friend const bool operator| (const alx_horloge_vectorielle &, const alx_horloge_vectorielle &);

 private:
 // Pour optimisation
  alx_chaine_char A_cc_tmp1, A_cc_tmp2, A_cc_tmp3;

 // Le reste
  mutable alx_chaine_char serialisation, serialisation_IPs;
  unsigned int ordre;
  int proprio;
  unsigned int *H;
  alx_chaine_char *TabIP;
  alx_liste<alx_chaine_char> *Tab_connaissance;
  void init();

 public :
  // Les constructeurs
   alx_horloge_vectorielle();
   alx_horloge_vectorielle(const unsigned int n);
   alx_horloge_vectorielle(const unsigned int n, const int p);
  ~alx_horloge_vectorielle();

 // Les méthodes d'accès
  inline const unsigned int Horloge(const unsigned int i) const {return H[i];}
  inline const unsigned int* Horloge() const {return H;}
  inline void Proprietaire(const unsigned int p) {proprio = p;}
  inline const unsigned int Proprietaire() const {return proprio;}
  inline const unsigned int Ordre() const {return ordre;}
  void Ordre(const unsigned int o);
  inline const alx_chaine_char& IP(const unsigned int i) const   {return TabIP[i];}
  inline void IP(const unsigned int i, const alx_chaine_char &n) {TabIP[i] = n;}
  inline void IP(const unsigned int i, const char *n)            {TabIP[i] = n;}
  const int Position_dans_horloge(const alx_chaine_char &ip) const;

 // Les méthodes de mise à jour
  // D'un éléments
  const unsigned int Inserer(const unsigned int v, const alx_chaine_char &ip);
  const unsigned int Inserer(const unsigned int v, const unsigned int p, const alx_chaine_char &ip);
  void Supprimer(const unsigned int p);
  void Supprimer(const alx_chaine_char &ip);
  const unsigned int Incrementer();
  // De l'horloge entière
  void Maj(const alx_horloge_vectorielle &h);
  void Prendre_en_compte(const alx_horloge_vectorielle &h);
  void Prendre_en_compte(const alx_chaine_char &h_seri);
  void Reduire_a(const alx_horloge_vectorielle &h);
  //void Prendre_en_compte_horloge_differente(const alx_horloge_vectorielle &h);

 // La connaissance sur la strucuture des autres horloges
  void Connaissance(const unsigned int pos, const char *L_IP);
  const bool Connaissance(const alx_chaine_char &ip, const char *L_IP);
  const alx_liste<alx_chaine_char>& Connaissance(const unsigned int i) const {return Tab_connaissance[i];}
  const bool Inferieur_dans_ordre_total(const alx_horloge_vectorielle &h);
 // Les surcharges d'opérateurs
  const alx_horloge_vectorielle &operator=(const alx_horloge_vectorielle &h);
  const bool operator==(const alx_horloge_vectorielle &h) const;
  inline const bool operator!=(const alx_horloge_vectorielle &h) const {return !(*this==h);}

 // La sérialisation/désérialisation
  const alx_chaine_char& Serialiser_IPs() const;
  const alx_chaine_char& Serialiser() const;
  void Deserialiser(const alx_chaine_char &cc, unsigned int &pos);
};

#endif
