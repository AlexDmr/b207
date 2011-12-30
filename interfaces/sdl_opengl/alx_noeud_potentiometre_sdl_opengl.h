#ifndef __ALX_NOEUD_POTENTIOMETRE_H__
#define __ALX_NOEUD_POTENTIOMETRE_H__

class alx_interface_sdl_opengl_1;
#include "alx_noeud_bouton_sdl_opengl.h"
#include "../../physique/alx_simulateur_points_contact.h"

// Rationnalisation vers l'extérieur:
// 0 : Changement de valeur

class alx_noeud_potentiometre_sdl_opengl : public alx_noeud_scene, public alx_classe_base_liant
{
 private:
  // Les fonctions de rappel
   alx_point2D pt2;
   Liste_de_rappel L_rappel_valeur;

   //alx_repere2D repere0;
   alx_noeud_scene noeud_zone_reglage;
   double longueur, hauteur, longueur_boutons_controles, decalage_initiale;
   int valeur_min, valeur_max, valeur, valeur_saut;
   alx_noeud_bouton_sdl_opengl *bouton_reculer, *bouton_avancer;
   double couleur_fond[4], couleur_zone_reglage[4];

   alx_sommet_opengl gl_zone_reglage[4], gl_zone_fond[4];
   alx_model_simulation_physique1 mp_zone_reglage, mp_zone_fond;

   alx_interface_sdl_opengl_1 *interface_mere;
   P_alx_ensemble_contact ens_contact_zone_redim
                        , ens_contact_zone_fond;
   alx_modele_contact *mod_contact_zone_reglage, *mod_contact_zone_fond;

   void init();
   void maj_interne();

  // Les éléments pour une rationnalisation rapide :
   static const unsigned int NB_ELEMENTS;
   typedef void (alx_noeud_potentiometre_sdl_opengl::* pf)(const int *infos);
   P_alx_classe_base_model *T_modeles_bases;
   pf *T_rationnalisation_global;
   void Rationnaliser_Bouton_prcdt(const int *infos);
   void Rationnaliser_Bouton_svt(const int *infos);
   void Rationnaliser_zone_fond(const int *infos);
   void Rationnaliser_zone_reglage(const int *infos);

 public:
   alx_noeud_potentiometre_sdl_opengl(alx_interface_sdl_opengl_1 *interface_mere, const double lg, const double ht, const double lg_boutons_controles = -1, const unsigned int type_bouton = 1);
   ~alx_noeud_potentiometre_sdl_opengl();

   virtual const char* Real_class_cmd() {return "Void_vers_potentiometre";}

   inline void Mettre_a_jour() {maj_interne();}
   inline double Longueur() const {return longueur;}
   inline double Hauteur()  const {return hauteur;}
   inline int Valeur_min()  const {return valeur_min;}
   inline int Valeur_max()  const {return valeur_max;}
   inline int Valeur()      const {return valeur;}
   inline int Valeur_saut() const {return valeur_saut;}
   inline int NbDivisions() const {return Valeur_max() - Valeur_min();}

   inline void Longueur   (const double l) {longueur    = l;}
   inline void Hauteur    (const double h) {hauteur     = h;}
   inline void Valeur_min (const int vmin) {valeur_min  = vmin;}
   inline void Valeur_max (const int vmax) {valeur_max  = vmax;}
   inline void Valeur     (const int v)    {valeur      = v; Changement_valeur();}
   inline void Valeur_saut(const int v)    {valeur_saut = v;}

   inline double Longueur_boutons_controles() const {return longueur_boutons_controles;}
   inline void   Longueur_boutons_controles(const double l) {longueur_boutons_controles = l;}

   inline const double* Couleur_fond()       const {return couleur_fond;}
   inline void Couleur_fond(const double c[4])
    {Copie_couleur(c, couleur_fond);
     for(int i=0;i<4;i++) Copie_couleur(c, gl_zone_fond[i].coord_couleur);}
   inline void Couleur_fond(const double r, const double v, const double b, const double a)
    {couleur_fond[0]=r; couleur_fond[1]=v; couleur_fond[2]=b; couleur_fond[3]=a;
     for(int i=0;i<4;i++) Copie_couleur(couleur_fond, gl_zone_fond[i].coord_couleur);}

   inline const double* Couleur_zone_reglage()       const {return couleur_zone_reglage;}
   inline void Couleur_zone_reglage(const double c[4])
    {Copie_couleur(c, couleur_zone_reglage);
     for(int i=0;i<4;i++) Copie_couleur(c, gl_zone_reglage[i].coord_couleur);}
   inline void Couleur_zone_reglage(const double r, const double v, const double b, const double a)
    {couleur_zone_reglage[0]=r; couleur_zone_reglage[1]=v; couleur_zone_reglage[2]=b; couleur_zone_reglage[3]=a;
     for(int i=0;i<4;i++) Copie_couleur(couleur_zone_reglage, gl_zone_reglage[i].coord_couleur);}

   inline void Epaisseur_bordure(const double e)
    {
     bouton_reculer->Epaisseur_bordure(e);
     bouton_avancer->Epaisseur_bordure(e);
     maj_interne();
    }

   void Dessin_noeud();
   virtual info_du_contenant* Noeud_contient(const alx_point2D &pt, int action);

   inline void rationnaliser(int num)
    {(this->*(T_rationnalisation_global[num]))(T_modeles_bases[num]->infos_comp);}

   inline void abonner_a_valeur    (alx_methode_rappel *m) {L_rappel_valeur.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_valeur(alx_methode_rappel *m) {L_rappel_valeur.Retirer_methode(m);}
   inline void Changement_valeur()
    {
     L_rappel_valeur.Rappeler(this);
     alx_classe_base_model::nb_infos_comp = 1;
     alx_classe_base_model::infos_comp[0] = 0;
     alx_classe_base_model::Oyez_oyez();
    }
};

typedef alx_noeud_potentiometre_sdl_opengl* P_alx_noeud_potentiometre_sdl_opengl;
inline alx_noeud_potentiometre_sdl_opengl* Void_vers_potentiometre(void *p) {return (alx_noeud_potentiometre_sdl_opengl*)p;}

#endif
