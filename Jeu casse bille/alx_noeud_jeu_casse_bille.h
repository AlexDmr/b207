#ifndef __ALX_NOEUD_JEU_CASSE_BILLE_H__
#define __ALX_NOEUD_JEU_CASSE_BILLE_H__

#include "../interfaces/alx_noeud_scene.h"
#include "alx_jeu_casse_bille.h"

class alx_noeud_jeu_casse_bille : public alx_noeud_scene
{
 private:

 public :


   void Afficher();
  // Renvoi un pointeur sur le modèle physique qui contient la primitive, NULL si aucun.
   info_du_contenant* Contient(alx_point2D &pt, int action);
   void PreRendre();
};

#endif
 