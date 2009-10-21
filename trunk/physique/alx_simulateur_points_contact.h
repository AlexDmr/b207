#ifndef __ALX_SIMULATEUR_POINTS_CONTACT_H__
#define __ALX_SIMULATEUR_POINTS_CONTACT_H__

#include "alx_classes_bases.h"
#include "alx_model_simulation_physique1.h"
#include "alx_geometrie2D.h"
#include "..\utilitaires\alx_liste.h"//<list.h>

#include "../interfaces/alx_noeud_scene.h"

const int alx_point_contact_capacite_translation = 1
        , alx_point_contact_capacite_rotation    = 2
        , alx_point_contact_capacite_rapporteur  = 4
        , alx_point_contact_capacite_etirement   = 8;

inline const unsigned int Capa_T() {return alx_point_contact_capacite_translation;}
inline const unsigned int Capa_R() {return alx_point_contact_capacite_rotation;}
inline const unsigned int Capa_E() {return alx_point_contact_capacite_etirement;}
inline const unsigned int Capa_Rapporteur() {return alx_point_contact_capacite_rapporteur;}

class alx_modele_contact
{public:
  // Les constituants
   alx_vecteur2D trans_base, E_base;
   double        rot_base;

   int capacite;
   alx_model_simulation_physique1 *modele_physique;
   alx_noeud_scene                *noeud_du_modele;

  // Les constructeurs
   alx_modele_contact() {capacite=0; modele_physique=NULL; noeud_du_modele=NULL;}
  // Les méthode
  inline const int Capacite() const {return capacite;}
  inline void Capacite(const int c) {capacite = c;}
  inline const alx_noeud_scene*                Noeud() const {return noeud_du_modele;}
  inline const alx_model_simulation_physique1* MP()    const {return modele_physique;}
};
typedef alx_modele_contact* P_alx_modele_contact;

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class alx_point_contact
{private:
  alx_point2D pt_transforme, valeur_prcdt, valeur_prcdt_ds_rep_pere;
  alx_model_simulation_physique1 *modele_pere;

 public :
  // Les constituants
   bool actif;
   int id;
   alx_point2D *pt, pt_init;
   alx_liste<alx_repere2D*> *L_repere;
   alx_modele_contact *mc;

  // Les constructeurs
   alx_point_contact() {actif=false; id=0; pt=NULL; modele_pere=NULL; L_repere=NULL; mc=NULL;}
  ~alx_point_contact() {if(L_repere)delete L_repere;}

  // Les méthodes
   inline void Lier(alx_model_simulation_physique1 *m) {modele_pere=m;}
   inline void Prelier(alx_liste<alx_repere2D*> *L_rep)
    {if(L_repere)delete L_repere;
     if(L_rep)
      {L_repere = new alx_liste<alx_repere2D*>();
       L_repere->maj(*L_rep);
      } else {L_repere = NULL;}
    }

   inline alx_liste<alx_repere2D*>* get_L_repere() {return L_repere;}
   inline void Delier() {modele_pere=NULL; if(L_repere) delete L_repere; L_repere = NULL;}
   inline bool Lie()    const {return modele_pere!=NULL;}
   inline bool PreLie() const {return L_repere!=NULL;}
   alx_point2D* Valeur_prcdt() {return &valeur_prcdt;}
   void Valeur_prcdt(const alx_point2D &p) {valeur_prcdt.maj(p);}
   alx_point2D* Valeur_prcdt_ds_rep_pere() {return &valeur_prcdt_ds_rep_pere;}
   void Valeur_prcdt_ds_rep_pere(const alx_point2D &p) {valeur_prcdt_ds_rep_pere.maj(p);}
   const alx_point2D& Valeur_initiale() const {return pt_init;}
   void Valeur_initiale(const alx_point2D& p) {pt_init.maj(p);}

   inline alx_modele_contact* MC() {return mc;}
   alx_point2D* PT()
    {pt_transforme.maj(*pt);
     if(L_repere)
       Repere_transformation(pt_transforme, *L_repere);
       //XXXRepere_transformation_sauf_dernier(pt_transforme, *L_repere);
     return &pt_transforme;
    }
};
typedef alx_point_contact* P_alx_point_contact;

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
class alx_ensemble_contact
{public:
  // Les constituants
   Liste_de_rappel L_rappel_changement;

   bool rot_defaut, capacite_etirement, repere_pipo_E_actif, pt_rot_defaut_actif, pt_trans_defaut_actif;
   double etirement_originel, etirement_noeud_originel_x, etirement_noeud_originel_y;
   alx_noeud_scene *noeud_du_modele;
   alx_point_contact pt_trans, pt_trans_defaut, pt_trans_E
                   , pt_rot  , pt_rot_defaut;
   alx_vecteur2D vect_position0;
   double rotation0, rotation_prcdt, rotation_effectuee, complement_rot0;
   alx_repere2D repere0, repere_pipo_E;
   alx_point2D centre_rot_export;
   alx_vecteur2D correction;
   alx_liste<alx_modele_contact*> L_modele_contact;

  // Les constructeurs
   alx_ensemble_contact() {repere_pipo_E_actif = false;
                           pt_rot_defaut_actif = pt_trans_defaut_actif = true;}
   ~alx_ensemble_contact()
    {alx_element_liste<alx_modele_contact*> *it     = L_modele_contact.Premier()
                                          , *it_fin = L_modele_contact.Fin();
     for(; it!=it_fin; it=it->svt)
      delete it->E();
    }

//_____________________________Les méthodes_____________________________________
   inline void abonner_a_changement    (alx_methode_rappel *m) {L_rappel_changement.Ajouter_methode_a_la_fin(m);}
   inline void desabonner_de_changement(alx_methode_rappel *m) {L_rappel_changement.Retirer_methode         (m);}

   inline alx_point_contact* Pt_contact_trans_actif()
    {if(pt_trans.actif) return &pt_trans;
      else if(pt_trans_defaut.actif) return &pt_trans_defaut;
            else return NULL;
    }

   inline alx_point_contact* Pt_contact_rot_actif()
    {if(pt_rot.actif) return &pt_rot;
      else if(pt_rot_defaut.actif) return &pt_rot_defaut;
            else return NULL;
    }

             //_________________________________//
   void enregistrer_transformation_des_modeles()
    {alx_element_liste<alx_modele_contact*> *it_mc     = L_modele_contact.Premier()
                                          , *it_fin_mc = L_modele_contact.Fin();

     for(; it_mc!=it_fin_mc; it_mc=it_mc->svt)
      {(it_mc->E())->trans_base.maj( (it_mc->E())->modele_physique->position );
       (it_mc->E())->rot_base      = (it_mc->E())->modele_physique->rotation;
      }
    }

   inline bool Fait_intervenir(const alx_point2D *pt) const
    {return( (pt_trans.actif        && pt_trans.pt        == pt)
           ||(pt_rot.actif          && pt_rot.pt          == pt)
           ||(pt_trans_defaut.actif && pt_trans_defaut.pt == pt)
           ||(pt_rot_defaut.actif   && pt_rot_defaut.pt   == pt)
           ||(pt_trans_E.actif      && pt_trans_E.pt      == pt) );
    }
};
typedef alx_ensemble_contact* P_alx_ensemble_contact;


//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
// Lorsqu'on abonne un modèle physique au simulateur de point de contact
// on lui donne comme identifiant son adresse.
class alx_simulateur_points_contact : public alx_classe_base_liant
{private:
   alx_point2D   A_pt_tmp, A_pt_tmp2, A_pt_tmp3, A_pt_tmp4, A_pt_tmp5, pt_tmp_E_1, pt_tmp_E_2;
   alx_vecteur2D A_v_tmp , A_v_tmp2 , A_v_tmp3 , A_v_tmp4 , A_v_tmp5;
   alx_repere2D  A_rep_tmp;
   alx_model_simulation_physique1 *modele_indesirable;
  // Trucs utils pour les mises à jours de position
   alx_point2D *pt_maj; // Lors d'une mise à jour de position, pour se souvenir du point que l'on passe et ne pas le retraiter en boucle
   alx_vecteur2D *vect_maj; // Idem avec ce vecteur.

   alx_liste<alx_ensemble_contact*> L_ens_contact, L_ens_contact_actifs;
   alx_point_contact *dernier_point_contact_ajoute;

   bool Trouver_model( alx_model_simulation_physique1 *mp
                     , alx_liste<alx_repere2D*> *L_rep
                     , alx_modele_contact   **mc
                     , alx_ensemble_contact **ens_c);
   bool Fixer_point_contact_translation( alx_ensemble_contact *ens
                                       , alx_modele_contact *mc
                                       , alx_point2D *pt
                                       , alx_point_contact &pt_contact );
   bool Fixer_point_contact_rotation( alx_ensemble_contact *ens
                                    , alx_modele_contact *mc
                                    , alx_point2D *pt
                                    , alx_point_contact &pt_contact_trans
                                    , alx_point_contact &pt_contact_rot );
   bool Ajouter_point_contact( alx_ensemble_contact *ec
                             , alx_modele_contact   *mc
                             , alx_point2D          *pt
                             , alx_liste<alx_repere2D*>  *L_rep);
 public :
  // Les constructeurs
   alx_simulateur_points_contact();
   ~alx_simulateur_points_contact();

  // Les méthodes
  alx_liste<alx_ensemble_contact*>& L_ENS_CONTACT()        {return L_ens_contact;}
  alx_liste<alx_ensemble_contact*>& L_ENS_CONTACT_ACTIFS() {return L_ens_contact_actifs;}

   alx_ensemble_contact* Ajouter_ensemble_contact();
   bool                  Retirer_ensemble_contact(alx_ensemble_contact *ens);
   alx_modele_contact* Ajouter_modele( alx_ensemble_contact *ens
                                     , alx_model_simulation_physique1 *modele
                                     , alx_noeud_scene *noeud_modele
                                     , int capacite );
   bool Fixer_point_rotation_defaut   ( alx_ensemble_contact *ens
                                      , alx_modele_contact *mc
                                      , alx_point2D *pt );
   bool Fixer_point_translation_defaut( alx_ensemble_contact *ens
                                      , alx_modele_contact *mc
                                      , alx_point2D *pt );

   bool Activer_point_rotation_defaut   ( alx_ensemble_contact *ens
                                        , const bool b);
   bool Activer_point_translation_defaut( alx_ensemble_contact *ens
                                        , const bool b);

  // Les surcharges de la classe base liant
   void simuler();
   void rationnaliser(int num);
   void Changement_dans_le_modele(alx_model_simulation_physique1 *mp);

  // Pour la simulation
   bool Retirer_pt_contact(alx_ensemble_contact *ens, alx_point2D *pt);
   void simuler(alx_ensemble_contact *ens);
   inline alx_point_contact* Dernier_point_contact_ajoute() {return dernier_point_contact_ajoute;}
};

#endif
