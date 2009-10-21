#ifndef __ALX_AFFICHAGE_POINTEURS_H__
#define __ALX_AFFICHAGE_POINTEURS_H__

#include "..\..\utilitaires\alx_liste.h"
#include "..\alx_simulateur_pointeurs.h"

typedef void (*f_affichage)(const double x, const double y, void *param_sup);

class association_affichage_num_pointeur
{
 public:
   int num;
   f_affichage f;
   void *param;

   association_affichage_num_pointeur() {}
   association_affichage_num_pointeur(f_affichage f, void *param, const int num)
    {this->f   = f;
     this->num = num;
     this->param = param;}

     
};

class affichage_pointeurs
{
 private:
   alx_liste<association_affichage_num_pointeur> L_fonctions_affichage;
   alx_simulateur_pointeurs *sim_pointeurs;

 public :
   affichage_pointeurs(alx_simulateur_pointeurs *s);

   bool Associer_fonction_affichage_a_pointeur(f_affichage f, void *param, const int n);
   void Afficher();
};

#endif
 