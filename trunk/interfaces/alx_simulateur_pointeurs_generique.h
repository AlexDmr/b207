#ifndef __ALX_SIMULATEUR_POINTEURS_GENERIQUE_H__
#define __ALX_SIMULATEUR_POINTEURS_GENERIQUE_H__

#include "../physique/alx_classes_bases.h"
#include "alx_simulateur_pointeurs.h"
#include "..\utilitaires\alx_liste.h"
#include "..\utilitaires\alx_chaine_char.h"
#include "..\utilitaires\MetaData.h"
#include "../physique/alx_geometrie2D.h"  // point, vecteur, droite, segment...
#include <SDL.h>

class alx_noeud_scene;
class alx_simulateur_pointeurs_generique : public alx_classe_base_liant
{protected:
  // Optimisation
   alx_vecteur2D A_v_tmp;
   MetaData metadata;

  // Reste
   alx_liste<alx_pointeur_rapporteur> L_rapporteur;
   int *ecran_x, *ecran_y;
   alx_vecteur2D *vect_h, *vect_v;
   alx_point2D   *coin_bg;
   int souris_x[2], souris_y[2], bouton[3][2];
   int cour;

   void init();

  // Lien vers le noeud de rattachement dans le graphe de scène
   alx_noeud_scene *noeud_pere;
 public :
   SDL_Event  event;

   alx_simulateur_pointeurs_generique( int *e_x, int *e_y
                                     , alx_point2D *bg
                                     , alx_vecteur2D *vh, alx_vecteur2D *vv);

   virtual int  Ajouter_rapporteur(alx_simulateur_pointeurs *r, alx_noeud_scene *pere, const unsigned int nb = 1);
   virtual bool Retirer_rapporteur(alx_simulateur_pointeurs *r);

   inline const int Ecran_X() const {return *ecran_x;}
   inline const int Ecran_Y() const {return *ecran_y;}
   inline virtual void Noeud_pere(alx_noeud_scene *n) {noeud_pere = n;}
   inline virtual alx_noeud_scene* Noeud_pere()       {return noeud_pere;}
   virtual void simuler();

   void convertir_coordonnees(const double x, const double y, alx_point2D &res);
   void distribuer_evennement(alx_evennement_pointeur &evt);

   MetaData& get_MetaData() {return metadata;}
};

#endif

