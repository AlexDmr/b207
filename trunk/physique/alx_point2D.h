#ifndef __ALXPOINT__
#define __ALXPOINT__

#include "alx_droite2D.h"
#include "alx_vecteur2D.h"
#include "alx_primitive2D.h"
#include "..\utilitaires\alx_chaine_char.h"

class alx_droite2D;
class alx_vecteur2D;
class alx_segment2D;
class alx_deplacement2D;

/* Définit un point dans l'espace 2D     */
/* Donne des méthodes de transformations */
class alx_point2D : public alx_primitive2D
{
 friend const alx_point2D& operator+(const alx_point2D &, const alx_vecteur2D &);
 friend const alx_point2D& operator+(const alx_vecteur2D &, const alx_point2D &);
 friend const alx_point2D& operator-(const alx_point2D &, const alx_vecteur2D &);
 friend const alx_point2D& operator-(const alx_vecteur2D &, const alx_point2D &);

 protected:
   mutable alx_chaine_char serialisation;
   double dernier_cos, dernier_sin;

 public:
  // Les données en publique comme un gros sale....mais au moins l'accès est rapide.
   double x, y;

   inline const double X() const {return x;}
   inline const double Y() const {return y;}
   inline void X(const double v) {x=v;}
   inline void Y(const double v) {y=v;}
   inline const double Der_cos() {return dernier_cos;}
   inline const double Der_sin() {return dernier_sin;}
  // Les constructeurs:
   alx_point2D();                             // par défaut
   alx_point2D(const double x, const double y);// pour un point 2D
   alx_point2D(const alx_point2D &p);         // par rapport à un autre point
  // Le destructeur:
   //~alx_point2D();

 // La sérialisation
  virtual const alx_chaine_char& Serialiser_contenu() const;
  virtual void Deserialiser_contenu(const alx_chaine_char &txt, unsigned int &pos);

  // Les transformations appliqués à l'objet lui même:
   void Projection(const alx_droite2D  &d);
   void Symetrique(const alx_droite2D  &d);
   void Symetrique(const alx_point2D   &p);
   void Pivoter   (const double angle);
   void Pivoter   (const double angle, const alx_point2D &p);
   void Pivoter   (const double angle, const double px, const double py);
   void Translation(const alx_vecteur2D &);
   void Translation(const alx_point2D &);
   void Translation(const double x, const double y);
   void Translation_inverse(const alx_vecteur2D &);
   void Translation_inverse(const alx_point2D &);
   void Translation_inverse(const double x, const double y);
   void Barycentre(const alx_segment2D &, const double a, const double b);
   void Positionner_sur(const alx_deplacement2D&, const double t);
   void Etirer(const alx_vecteur2D &);
   void Etirer(const double x, const double y);
   void Glisser(const alx_vecteur2D &);
   void Glisser(const double x, const double y);
   void Inverser();

  // Les transformations qui renvoi un pointeur sur objet:
   alx_point2D& projection(const alx_droite2D  &d) const;
   alx_point2D& barycentre(const alx_segment2D &, const double a, const double b) const;
   virtual alx_point2D& symetrique(const alx_droite2D  &d) const;
   virtual alx_point2D& symetrique(const alx_point2D   &p) const;
   virtual alx_point2D& pivoter   (const double angle, const alx_point2D &p) const;
   virtual alx_point2D& translation(const alx_vecteur2D &) const;
   virtual alx_point2D& translation(const alx_point2D &) const;
   virtual alx_point2D& translation_inverse(const alx_vecteur2D &) const;
   virtual alx_point2D& translation_inverse(const alx_point2D &) const;
   virtual alx_point2D& etirer(const alx_vecteur2D &) const;

  // Les mesures:
   double distance(const alx_droite2D &d) const;
   double distance(const alx_point2D  &p) const;
   double distance_carre(const alx_point2D  &p) const;
   bool   equivalent(const alx_point2D &p, double marge) const;

  // Les surcharges:
   void maj(const double x, const double y);
   void maj(const double *T);
   void maj(const alx_point2D &);
   const alx_point2D &operator =(const alx_point2D &);    // Surcharge du  =
   const alx_point2D &operator+=(const alx_vecteur2D &);  // Surcharge du +=
   const alx_point2D &operator-=(const alx_vecteur2D &);  // Surcharge du -=

};

typedef alx_point2D* P_alx_point2D;

#endif
