#ifndef __ALX_MODEL_SIMULATION_PHYSIQUE_H__
#define __ALX_MODEL_SIMULATION_PHYSIQUE_H__

#include "..\utilitaires\alx_liste.h" //<list.h>
#include "alx_physique2D.h"
#include "alx_classes_bases.h"
#include "alx_modele_mobilite.h"
#include "alx_deplacement2D.h"
#include "alx_polygone2D.h"
#include "alx_repere2D.h"

class alx_repere2D;
/******************************************************************************/
/*********************** alx_model_simulation_physique1 ***********************/
/******************************************************************************/
// Les messages:
//  0 : Mise à jour des positions
//     0 : par translation
//     1 : par rotation
//     2 : par translation et rotation
//     4 : par étirement
//  1 : Mise à jour des composants
//     0 : Un nouveau cercle physique
//     1 : Un nouveau segment 2D
//     2 : Un nouveau polygone 2D
//     3 : Une nouvelle ellipse 2D
//     4 : Une nouvelle droite 2D
//  2 : Contact avec un objet
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//  3 : Rupture de contact avec un objet
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//  4 : Déplacement du contact avec un objet
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//  5 : Changement de l'objet (aspect dynamique / aux mises à jours statiques)
//     1 : Translation
//       *V : pointeur sur vecteur2D
//         *P : pointeur sur point2D, le point de départ e la translation en coordonnées du modèle.
//     2 : Rotation
//       *A : angle trigonométrique (pointeur sur double)
//         *P : pointeur sur point2D
//     3 : Etirement
//       *V : pointeur sur vecteur2D
//         *P : pointeur sur point2D
//  6 : Contact avec un objet positionner par une suite de transformation de repère
//     0 : Un point
//        * : adresse du point
//           * : adresse du modèle physique
//              * : adresse d'une liste de transformation de repère list<alx_repere2D*>*
//  7 : Réajustement de l'objet (à la suite d'un changement)
//     1 : Translation
//       *V : pointeur sur vecteur2D
//         *P : pointeur sur point2D, le point de départ e la translation en coordonnées du modèle.
//     2 : Rotation
//       *A : angle trigonométrique (pointeur sur double)
//         *P : pointeur sur point2D
//     3 : Etirement
//       *V : pointeur sur vecteur2D
//         *P : pointeur sur point2D

class alx_model_simulation_physique1 : public alx_classe_base_model
{
 private:
  const alx_point2D point0;
  alx_point2D   A_pt_tmp;
  alx_vecteur2D A_v_tmp;

 public:
  // Les données:
   #include "ENS_Caracteristiques_physiques.h"
   #include "ENS_primitive2D.h"
   ALX_MODELE_MOBILITE mobilite;
   alx_vecteur2D barycentre // par rapport au model
               , correction;
   bool ne_pas_modifier_la_correction;

  // Les constructeurs:
   alx_model_simulation_physique1();
   alx_model_simulation_physique1(const alx_model_simulation_physique1 &);
   ~alx_model_simulation_physique1();

  // Les méthodes:
   inline void Pivoter(const alx_point2D &centre, const double r)
    {position.dx -= centre.x;
     position.dy -= centre.y;
     Pivoter(r);
     position.dx += centre.x;
     position.dy += centre.y;}

   inline void Pivoter(const double r)
    {rotation += r;
     position.Pivoter(r);}

   inline void Translation(const alx_vecteur2D &v) {position.Ajouter(v);}

   void Ajouter_cercle2D_phys(const alx_cercle2D_physique &, const double masse);
   void Ajouter_segment2D (const alx_point2D &   , const alx_point2D &);
   void Ajouter_polygone2D(const alx_polygone2D &, const double masse);
   void Ajouter_ellipse2D (const alx_ellipse2D & , const double masse);
   void Ajouter_segment2D (const alx_segment2D & );
   void Ajouter_droite2D  (const alx_droite2D &  );

   void Integrer_force_en_vitesse(const alx_vecteur2D &f, double dt);
   void Deplacement_par_loi_dynamique(const double temps_deb, const double dt);
   void Repositionner(double x, double y, double r, bool avertir);
   void Positionner  (double x, double y);
//   void Translater_dynamiquement(double x, double y, bool avertir);

   inline void Appliquer_rotation_a(alx_primitive2D &p)
           {A_pt_tmp.maj(centre_rot); //alx_point2D centre( centre_rot );
            Appliquer_translation_a( A_pt_tmp ); //centre );
            p.Pivoter(rotation, A_pt_tmp ); //centre);
           }
   inline void Appliquer_rotation_inverse_a(alx_primitive2D &p)
           {A_pt_tmp.maj(centre_rot); //alx_point2D centre( centre_rot );
            Appliquer_translation_a( A_pt_tmp ); //centre );
            p.Pivoter(-rotation, A_pt_tmp ); //centre);
           }
   inline void Appliquer_translation_a(alx_primitive2D &p)
           { p.Translation(position); }
   inline void Appliquer_translation_inverse_a(alx_primitive2D &p)
           {A_v_tmp.maj( position ); // alx_vecteur2D tmp(position);
            A_v_tmp.Oppose(); // tmp.Oppose();
            p.Translation(A_v_tmp); //tmp);
           }
   inline void Appliquer_deplacement_a(alx_primitive2D &p)
           {p.Pivoter(rotation, centre_rot );
            p.Translation(position);}
   inline void Appliquer_deplacement_inverse_a(alx_primitive2D &p)
           {A_v_tmp.maj( position ); // alx_vecteur2D tmp(position);
            A_v_tmp.Oppose(); // tmp.Oppose();
            p.Translation(A_v_tmp); //tmp);
            p.Pivoter(-rotation, centre_rot );
           }

   bool Contient(const alx_point2D &p);

   void Point_contact_detecte      ( alx_point2D *pt, alx_classe_base_liant *liant_demandeur);
   void Point_contact_detecte      ( alx_point2D *pt, alx_classe_base_liant *liant_demandeur
                                   , alx_liste<alx_repere2D*> *L_rep);
   void Rupture_contact_detecte    ( alx_point2D *pt, alx_classe_base_liant *liant_demandeur
                                   , alx_liste<alx_repere2D*> *L_rep);
   void Deplacement_contact_detecte( alx_point2D *pt, alx_classe_base_liant *liant_demandeur
                                   , alx_liste<alx_repere2D*> *L_rep);

   void Informer_au_sujet_de_translation(alx_vecteur2D *v, alx_point2D *p);
   void Informer_au_sujet_de_rotation(double *angle, alx_point2D *pt);
   void Informer_au_sujet_de_etirement(alx_vecteur2D *v, alx_point2D *pt);

   void Informer_Reajustement_translation(alx_vecteur2D *v, alx_point2D *p);
   void Informer_Reajustement_rotation(double *angle, alx_point2D *pt);
   void Informer_Reajustement_etirement(alx_vecteur2D *v, alx_point2D *pt);

   void maj(const alx_model_simulation_physique1 &);
   alx_model_simulation_physique1 &operator =(const alx_model_simulation_physique1 &);    // Surcharge du  =
};

#endif
