#ifndef __ALXGL_PRE_RENDEUR_H__
#define __ALXGL_PRE_RENDEUR_H__

#include "../../physique/alx_classe_base_liant.h"
#include "..\..\utilitaires\alx_liste.h" //<list.h>

class alx_noeud_scene;
// A pour but d'effectuer les rendus des fenêtres ou des interacteurs avant l'affichage
// de l'interface. Ceci doit permettre de copier le résultat des rendu dans une texture
// qu'on plaquera sur un polygone lors du rendu de l'interface.
// FONCTIONNEMENT:
// Les noeud de scene s'enregistre.
// On appelle les méthodes PreRendre() de chaque noeud enregistré.

typedef enum
{
 ACTION_DU_RENDU_rendre_une_fois,
 ACTION_DU_RENDU_rendre_toujours
} action_du_rendu;

class pre_rendu
{private:
   void init();

 public :
   float           priorite, accumulateur; // entre 0 et 1.
   alx_noeud_scene *noeud;
   action_du_rendu action;

   pre_rendu();
   pre_rendu(alx_noeud_scene *noeud, const float priorite, const action_du_rendu action);
   void maj(const pre_rendu &);
   pre_rendu &operator =(const pre_rendu &);  // Surcharge du  =
};

class alx_prerendeur : alx_classe_base_liant
{private:
  alx_liste<pre_rendu> L_pre_rendu;
  unsigned int ecran_x, ecran_y;
  double angle_camera;
  alx_noeud_scene *noeud_a_rendre;
  
 public :
  // Les constructeurs
   alx_prerendeur( alx_noeud_scene *noeud_a_rendre
                 , unsigned int ecran_x, unsigned int ecran_y, double angle_camera );

  // Les méthodes
   //void Prendre_en_charge(alx_noeud_scene *noeud, const float priorite, const action_du_rendu action, const char pos); //'d':dernier ; 'p':premier
   //bool Se_decharger_de  (alx_noeud_scene *noeud);

   void Multiplier_priorite_par(const float f);
   void Vider();
   
   void simuler();
   void maj_info_ecran(unsigned int ecran_x, unsigned int ecran_y, double angle_camera);

   inline unsigned int Ecran_x() {return ecran_x;}
   inline unsigned int Ecran_y() {return ecran_y;}
   inline double  Angle_camera() {return angle_camera;}

   inline unsigned int Nb_abonnes() {return L_pre_rendu.Taille();}
};

#endif
 