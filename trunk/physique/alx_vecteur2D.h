#ifndef __ALXVECTEUR__
#define __ALXVECTEUR__

#include "alx_primitive2D.h"
#include "..\utilitaires\alx_chaine_char.h"

class alx_point2D;
class alx_droite2D;
class alx_segment2D;

class alx_vecteur2D : public alx_primitive2D
{
 friend const alx_vecteur2D& operator+(const alx_vecteur2D &, const alx_vecteur2D &);
 friend const alx_vecteur2D& operator-(const alx_vecteur2D &, const alx_vecteur2D &);
 friend const double         operator*(const alx_vecteur2D &, const alx_vecteur2D &);
 friend const alx_vecteur2D& operator*(const double e, const alx_vecteur2D &);
 friend const alx_vecteur2D& operator*(const alx_vecteur2D &, const double e);

 protected:
   mutable alx_chaine_char serialisation;

 public:
   double dx, dy;

   inline const double Dx() const {return dx;}
   inline const double Dy() const {return dy;}
   inline void Dx(const double v) {dx=v;}
   inline void Dy(const double v) {dy=v;}

 // Les constructeurs
  alx_vecteur2D();
  alx_vecteur2D(const alx_vecteur2D &v);
  alx_vecteur2D(const double x, const double y);
  alx_vecteur2D(const alx_point2D &a, const alx_point2D &b);
  alx_vecteur2D(const alx_point2D &);
  //~alx_vecteur2D();

 // La sérialisation
  virtual const alx_chaine_char& Serialiser_contenu() const;
  virtual void Deserialiser_contenu(const alx_chaine_char &txt, unsigned int &pos);

 // Les méthodes qui s'appliquent à l'objet lui même:
  void Oppose();
  void Pivoter(const double angle);
  void Perpendiculaire_g();
  void Perpendiculaire_d();
  void Symetrique(const alx_vecteur2D &axe);
  void Symetrique(const alx_droite2D &axe);
  void Ajouter(const alx_vecteur2D &);
  void Soustraire(const alx_vecteur2D &);
  void Ajouter(const alx_point2D &);
  void Soustraire(const alx_point2D &);
  void Ajouter(const double x, const double y);
  void Soustraire(const double x, const double y);
  void Alonger(const double );
  void Pivoter    (const double a, const alx_point2D &p) {Pivoter(a);}
  void Symetrique (const alx_point2D &p) {}
  void Etirer(const alx_vecteur2D &);
  void Etirer(const double x, const double y);

 // Les méthodes qui renvoient un pointeur sur objet:
  alx_vecteur2D& oppose() const;
  alx_vecteur2D& pivoter(const double angle) const;
  inline alx_vecteur2D& pivoter(const double angle, const alx_point2D &pt) const {return pivoter(angle);}
  alx_vecteur2D& perpendiculaire_g() const;
  alx_vecteur2D& perpendiculaire_d() const;
  alx_vecteur2D& symetrique(const alx_vecteur2D &axe) const;
  alx_vecteur2D& symetrique(const alx_droite2D &axe) const;
  alx_vecteur2D& alonger(const double) const;
  virtual alx_vecteur2D& symetrique(const alx_point2D &pivot) const
   {return *(new alx_vecteur2D(*this));}
  virtual alx_vecteur2D& etirer(const alx_vecteur2D &) const;
  bool   equivalent(const alx_vecteur2D &p, double marge) const;

  void maj(const double dx, const double dy);
  void maj(const alx_point2D &);
  void maj(const alx_point2D &, const alx_point2D &);
  void maj(const alx_vecteur2D&);
  void maj(const alx_segment2D&);
  const alx_vecteur2D &operator =(const alx_vecteur2D &);  // Surcharge du  =
  const alx_vecteur2D &operator+=(const alx_vecteur2D &);  // Surcharge du +=
  const alx_vecteur2D &operator-=(const alx_vecteur2D &);  // Surcharge du -=
  const alx_vecteur2D &operator*=(const double e);         // Surcharge du *=

  double produit_scalaire(const alx_vecteur2D &) const;
  void Vecteur_norme();
  const alx_vecteur2D& vecteur_norme() const ;
  const alx_vecteur2D& normer(const double n) const;
  void           Normer(const double n);

  double angle_trigo(const alx_vecteur2D &v) const;
  double Norme() const;
};


#endif
