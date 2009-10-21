#ifndef __ALX_JEU_CASSE_BILLE_H__
#define __ALX_JEU_CASSE_BILLE_H__

#include "..\physique\alx_geometrie2D.h"

#include "..\physique\alx_entite_palet.h"
#include "..\physique\alx_entite_bloc.h"

#include "..\physique\alx_simulateur_boite_rectangulaire.h"
#include "..\physique\alx_simulateur_physique_classique_champs_uniforme.h"
#include "..\physique\alx_simulateur_physique_initialisation.h"
#include "..\physique\alx_simulateur_mouvement_newton.h"
#include "..\physique\alx_simulateur_choc_2D.h"

#include "..\alx_evennement_pointeur.h"
#include "..\alx_liant_ptr_contact.h"
#include "..\alx_simulateur_couche.h"
#include "..\alx_simulateur_pointeurs.h"
#include "..\alx_simulateur_pointeurs_souris.h"

#include "entitees\alx_noeud_bille.h"
#include "entitees\alx_noeud_tapis.h"

#include "..\opengl\texte.h"
#include "..\opengl\alx_image.h"

#include "..\opengl\config_opengl.h"

class alx_jeu_casse_bille : public alx_classe_base_liant
{
 private:
   unsigned int num_texture_fond;
   alx_image_32 *texture_fond;
   gl_fonte *fonte_arial;

   int ecran_x, ecran_y;
   double angle_camera;

   int pointeur_x, pointeur_y;
   int id_souris;

   alx_point2D coin_bg;
   alx_vecteur2D vect_h, vect_v;
   alx_vecteur2D trans;
   double rotation;

   alx_simulateur_pointeurs_souris *sim_ptr_souris;
   alx_simulateur_pointeurs        *sim_pointeurs;
   alx_liant_ptr_contact           *liant_ptr_contact;
   alx_simulateur_couche           *sim_couche;

   alx_simulateur_boite_rectangulaire      sim_cadre;
   alx_simulateur_physique_champs_uniforme sim_champs;
   alx_simulateur_physique_initialisation  sim_init_newton;
   alx_simulateur_mouvement_newton         sim_mouvement_newton;
   alx_simulateur_choc_2D                 *sim_choc;

   double t_cour;

  // Les variables du jeu
   alx_noeud_tapis *tapis;

  // Les fonctions du jeu.
   void Initialiser_jeu();
   void Lacher_bille(alx_point2D &pt);
   
 public :
   alx_simulateur_points_contact   *sim_contact;
   alx_noeud_scene A_scene;

  // Les constructeurs 
   alx_jeu_casse_bille( alx_simulateur_choc_2D *sc
                      , int ecran_x, int ecran_y, double angle
                      , alx_simulateur_pointeurs *sp
                      , alx_simulateur_couche    *scouche
                      , alx_simulateur_points_contact    *pc);
   ~alx_jeu_casse_bille();

  // Les méthodes:
   void Translation(const alx_vecteur2D &t);
   void Rotation(const double r);

   void Simuler(double dt);
   void Afficher();

   void rationnaliser(int num);
};

#endif
 