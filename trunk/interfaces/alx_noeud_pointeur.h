#ifndef __ALX_NOEUD_POINTEUR_H__
#define __ALX_NOEUD_POINTEUR_H__

#include "alx_noeud_scene.h"

class alx_pointeur;

class alx_noeud_pointeur : public alx_noeud_scene
{private:
   const alx_pointeur *ptr;
   bool affichage_default;

 public :
   alx_noeud_pointeur(const alx_pointeur *p);

   void Affichage_default(const bool b) {affichage_default = b;}
   const bool Affichage_default() const {return affichage_default;}

  // Les surcharges de la classe noeud:
   virtual void Afficher();
   virtual void Dessin_noeud();
   virtual info_du_contenant* Contient(alx_point2D &pt, int action) {return (info_du_contenant*)NULL;}
};
alx_noeud_pointeur* VoidVersPointeur(void *p) {return (alx_noeud_pointeur*)p;}

#endif
