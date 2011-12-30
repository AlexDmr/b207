#include "alx_simulateur_points_contact.h"
#include "../utilitaires/fonctions_listes.h"
#include "constantes.h"

/******************************************************************************/
/*************************** Les constructeurs ********************************/
/******************************************************************************/
alx_simulateur_points_contact::alx_simulateur_points_contact() : alx_classe_base_liant()
{modele_indesirable = NULL;}

/******************************************************************************/
alx_simulateur_points_contact::~alx_simulateur_points_contact()
{alx_element_liste<alx_ensemble_contact*> *it     = L_ens_contact.Premier()
                                        , *it_fin = L_ens_contact.Fin();

 for(; it!=it_fin; it=it->svt)
   delete (it->E());
}

/******************************************************************************/
/****************************** Les méthodes **********************************/
/******************************************************************************/
alx_ensemble_contact* alx_simulateur_points_contact::Ajouter_ensemble_contact()
{alx_ensemble_contact *np;

 np = new alx_ensemble_contact();
 L_ens_contact.Ajouter_au_debut(np);

 return np;
}

/******************************************************************************/
bool alx_simulateur_points_contact::Retirer_ensemble_contact(alx_ensemble_contact *ens)
{bool rep = L_ens_contact_actifs.Retirer( ens );
 rep |= L_ens_contact.Retirer( ens );

return rep;
/*
  alx_element_liste<alx_ensemble_contact*> *it     = L_ens_contact.Premier()
                                        , *it_fin = L_ens_contact.Fin();

 for(; (it!=it_fin)&&(ens!=it->E()); it=it->svt);

 if(it!=it_fin) {L_ens_contact.Retirer(it); return true;}

 return false;*/
}

/******************************************************************************/
alx_modele_contact* alx_simulateur_points_contact::Ajouter_modele( alx_ensemble_contact *ens
                                                  , alx_model_simulation_physique1 *modele
                                                  , alx_noeud_scene *noeud_modele
                                                  , int capacite )
{alx_modele_contact *nmc = new alx_modele_contact();

 modele->abonner_a(this, (int)(void*)modele);

 nmc->capacite        = capacite;
 nmc->modele_physique = modele;
 nmc->noeud_du_modele = noeud_modele;
 ens->noeud_du_modele = noeud_modele;

 ens->L_modele_contact.Ajouter_au_debut(nmc);
 return nmc;
}

/******************************************************************************/
bool alx_simulateur_points_contact::Fixer_point_contact_translation( alx_ensemble_contact *ens
                                       , alx_modele_contact *mc
                                       , alx_point2D *pt
                                       , alx_point_contact &pt_contact )
{// Commençons par vérifier que ce modèle fait bien parti de l'ensemble
 if( !Appartient_a(ens->L_modele_contact, mc) ) return false;

// On fait le liens
 pt_contact.actif = true;
 pt_contact.pt    = pt;
 pt_contact.mc    = mc;
 A_pt_tmp.maj( *(pt_contact.PT()) ); //alx_point2D pt_trans( *(pt_contact.PT()) );
 pt_contact.Valeur_prcdt( A_pt_tmp );//pt_trans );
 mc->noeud_du_modele->Changer_coordonnees_inverse( A_pt_tmp );//pt_trans );
 ens->correction.maj(0,0);
 pt_contact.Valeur_prcdt_ds_rep_pere( A_pt_tmp );//pt_trans );

 dernier_point_contact_ajoute = &pt_contact;

 if(!ens->pt_rot.actif)
   ens->repere0.maj( *(mc->noeud_du_modele) );
 ens->vect_position0.maj( A_pt_tmp );//pt_trans );

// Attention à la rotation aussi, on refixe le même point qu'avant mais
// cela permet de mettre à jour les calculs!
 if(ens->pt_rot.actif)
   Fixer_point_contact_rotation(ens, ens->pt_rot.mc, ens->pt_rot.pt, pt_contact, ens->pt_rot);
  else if(ens->pt_rot_defaut.actif)
         Fixer_point_contact_rotation(ens, ens->pt_rot_defaut.mc, ens->pt_rot_defaut.pt, pt_contact, ens->pt_rot_defaut);

 return true;
}

/******************************************************************************/
bool alx_simulateur_points_contact::Fixer_point_contact_rotation( alx_ensemble_contact *ens
                                    , alx_modele_contact *mc
                                    , alx_point2D *pt
                                    , alx_point_contact &pt_contact_trans
                                    , alx_point_contact &pt_contact_rot )
{// Commençons par vérifier que ce modèle fait bien parti de l'ensemble
 if( !Appartient_a(ens->L_modele_contact, mc) ) return false;

// On fait le liens
 pt_contact_rot.actif = true;
 pt_contact_rot.pt    = pt;
 pt_contact_rot.mc    = mc;
 dernier_point_contact_ajoute = &pt_contact_rot;

 A_pt_tmp.maj( *(pt_contact_rot.PT()) ); //alx_point2D pt_rot( *(pt_contact_rot.PT()) );
 pt_contact_rot.Valeur_prcdt( A_pt_tmp ); //pt_rot );
 mc->noeud_du_modele->Changer_coordonnees_inverse( A_pt_tmp );//pt_rot );
 pt_contact_rot.Valeur_prcdt_ds_rep_pere( A_pt_tmp );//pt_rot );

// Calcul de l'angle
 if(!ens->pt_trans.actif)
   ens->repere0.maj( *(mc->noeud_du_modele) );

 A_pt_tmp2.maj( *(pt_contact_trans.PT()) ); //alx_point2D pt_trans( *(pt_contact_trans.PT()) );
// Passage dans les coordonnées du repère du dessus (pour les deux points) :
// Le point de rotation a déja été traité (voir au dessus).
 ens->noeud_du_modele->Changer_coordonnees_inverse( A_pt_tmp2 );//pt_trans );

// Calcul de l'angle de rotation de départ
 alx_vecteur2D v_pt(A_pt_tmp, A_pt_tmp2); //pt_rot, pt_trans);
 ens->rotation0 = Vect_h().angle_trigo(v_pt);
 ens->complement_rot0 = (double)0;

 return true;
}

/******************************************************************************/
bool alx_simulateur_points_contact::Fixer_point_rotation_defaut( alx_ensemble_contact *ens
                                                               , alx_modele_contact *mc
                                                               , alx_point2D *pt )
{if(!ens->pt_trans_defaut.actif) return false;
 ens->rot_defaut = true;
 return Fixer_point_contact_rotation( ens, mc, pt
                                    , ens->pt_trans_defaut
                                    , ens->pt_rot_defaut);
}

/******************************************************************************/
bool alx_simulateur_points_contact::Fixer_point_translation_defaut( alx_ensemble_contact *ens
                                                                  , alx_modele_contact *mc
                                                                  , alx_point2D *pt )
{return Fixer_point_contact_translation( ens, mc, pt
                                       , ens->pt_trans_defaut);
}

/******************************************************************************/
bool alx_simulateur_points_contact::Activer_point_rotation_defaut   ( alx_ensemble_contact *ens, const bool b)
{ens->pt_rot_defaut_actif = b;
 return b;
}

/******************************************************************************/
bool alx_simulateur_points_contact::Activer_point_translation_defaut( alx_ensemble_contact *ens, const bool b)
{ens->pt_trans_defaut_actif = b;
 return b;
}

/******************************************************************************/
/******************* Les surcharges de la classe base liant *******************/
/******************************************************************************/
void alx_simulateur_points_contact::simuler()
{alx_element_liste<alx_ensemble_contact*> *it     = L_ens_contact_actifs.Premier() //L_ens_contact
                                        , *it_fin = L_ens_contact_actifs.Fin();

 for(; it!=it_fin; it=it->svt)
  {simuler(it->E());}
}

/******************************************************************************/
// D'abord simuler la translation puis la rotation.
void alx_simulateur_points_contact::simuler(alx_ensemble_contact *ens)
{if(ens->L_modele_contact.Vide()) return;
 if( (!ens->pt_rot.actif)
   &&(!ens->pt_trans.actif) ) return;

 alx_point_contact *pt_contact_trans;
 if(ens->pt_trans.actif) pt_contact_trans = &(ens->pt_trans);
  else if(ens->pt_trans_defaut_actif && ens->pt_trans_defaut.actif) pt_contact_trans = &(ens->pt_trans_defaut);
        else return;

// Etirement remis à la valeur d'origine
 //ens->noeud_du_modele->Etirement( ens->repere0.Etirement() );
 if(ens->repere_pipo_E_actif)
   ens->noeud_du_modele->maj( ens->repere_pipo_E );

// On détermine s'il y a lieu de faire une rotation et si oui quel en est le point central
 alx_point_contact *pt_contact_rot;
 bool faire_rot = true;
 if(ens->pt_rot.actif) pt_contact_rot = &(ens->pt_rot);
  else if(ens->pt_rot_defaut_actif && ens->pt_rot_defaut.actif) pt_contact_rot = &(ens->pt_rot_defaut);
        else faire_rot = false;

// Dans tous les cas on détermine le point de translation.
 double rotation;
 A_pt_tmp.maj( *(pt_contact_trans->PT()) );
   // Décalage pour tenir compte d'un éventuel changement de l'étirement ou du glissement
    A_pt_tmp5.maj( A_pt_tmp );
// On se place dans les coordonnées du repère d'avant
 ens->noeud_du_modele->Changer_coordonnees_inverse( A_pt_tmp );
    A_rep_tmp.maj( ens->repere0 );
      A_rep_tmp.Origine( ens->noeud_du_modele->Origine() );
      A_rep_tmp.Rotation( ens->noeud_du_modele->Rotation() );
    A_rep_tmp.Changer_coordonnees_inverse( A_pt_tmp5 );
    A_v_tmp5.maj(A_pt_tmp, A_pt_tmp5);
   //____________________________________________________________________________________
// Calcul de la translation par rapport au repère au moment de l'ancrage.
 A_v_tmp.maj( A_pt_tmp );
// On soustrait la position initiale
 A_v_tmp.Soustraire( ens->vect_position0 );

      A_v_tmp.Ajouter( A_v_tmp5 ); // CORRECTION POUR LES CAS DE CHANGEMENT D'ETIREMENT OU DE GLISSEMENT !!!!!

// On détermine la rotation s'il y a lieu.
 double etirement=1;
 if(faire_rot)
  {A_pt_tmp2.maj( *(pt_contact_rot->PT()) );
     // Décalage pour tenir compte d'un éventuel changement de l'étirement ou du glissement
      //A_pt_tmp5.maj( A_pt_tmp2 );
  // On se place dans les coordonnées du repère d'avant
   ens->noeud_du_modele->Changer_coordonnees_inverse( A_pt_tmp2 );
      //A_rep_tmp.Changer_coordonnees_inverse( A_pt_tmp5 );
      //A_v_tmp5.maj(A_pt_tmp, A_pt_tmp5);
     //____________________________________________________________________________________
   A_v_tmp2.maj(A_pt_tmp2, A_pt_tmp);
      A_v_tmp2.Ajouter( A_v_tmp5 ); // CORRECTION POUR LES CAS DE CHANGEMENT D'ETIREMENT OU DE GLISSEMENT !!!!!
   rotation = Vect_h().angle_trigo( A_v_tmp2 )
            - ens->rotation0;
   if( ens->capacite_etirement
     &&(abs(ens->etirement_originel) > 0.0000001) )
     etirement = pt_contact_trans->PT()->distance( *(pt_contact_rot->PT()) )/ens->etirement_originel;
  }
  else rotation = 0;

 if(faire_rot && (rotation != 0)) // C'est le bordel, il faut modifier la translation
  {// On va faire une rotation, le point de départ de la translation va s'en trouver
   // changer implicitement lors du changement de repère. La translation pour maintenir
   // le point d'appuie à la même place sur le modèle physique doit donc être modifiée.
   A_pt_tmp3.maj(ens->vect_position0.dx, ens->vect_position0.dy );
   A_pt_tmp3.Translation_inverse( ens->repere0.origine );
   A_pt_tmp4.maj( A_pt_tmp3 );
   A_pt_tmp3.Pivoter(rotation);

   A_v_tmp3.maj(A_pt_tmp3, A_pt_tmp4);

   A_v_tmp.Ajouter(A_v_tmp3);
  }

// Appliquer la transformation à tous les modèles de l'ensemble...
 alx_element_liste<alx_modele_contact*> *it     = ens->L_modele_contact.Premier()
                                      , *it_fin = ens->L_modele_contact.Fin();

// Mise à jour du repère
 alx_repere2D *rep = ens->noeud_du_modele;
 ens->rotation_prcdt = rep->rotation;
 rep->Origine( ens->repere0.origine );
 if(faire_rot)
   rep->rotation = ens->repere0.rotation;

 rep->origine.Translation(A_v_tmp); //trans);
 if(faire_rot)
   rep->rotation += rotation;
 ens->complement_rot0 = rotation;

 ens->rotation_effectuee = rep->rotation - ens->rotation_prcdt;

 if(faire_rot)
   ens->centre_rot_export.maj( *(pt_contact_trans->PT()) );

// Calcul du point de départ de la translation et de la translation elle même.
 A_pt_tmp3.maj( *(pt_contact_trans->Valeur_prcdt_ds_rep_pere()) );
 A_pt_tmp4.maj( A_pt_tmp );
 A_v_tmp3.maj(A_pt_tmp3, A_pt_tmp4);

 ens->noeud_du_modele->Changer_coordonnees_prim( &A_v_tmp3 );
 ens->noeud_du_modele->Changer_coordonnees( A_pt_tmp4 );

 A_v_tmp3.Ajouter( ens->correction );
 A_v_tmp4.maj(ens->correction);

// La mise à jour des valeurs précédentes des points de contact
 pt_contact_trans->Valeur_prcdt(A_pt_tmp4);
 pt_contact_trans->Valeur_prcdt_ds_rep_pere(A_pt_tmp);
 if(faire_rot)
   pt_contact_rot->Valeur_prcdt(A_pt_tmp2);

// Gestion de l'étirement
 ens->repere_pipo_E_actif = false;
 if( (ens->pt_trans.actif || ens->pt_rot.actif)
   &&( (ens->pt_rot.actif && ens->pt_rot.mc && (ens->pt_rot.mc->Capacite() & Capa_E()))
     ||(ens->pt_trans_E.actif && ens->pt_trans_E.mc && (ens->pt_trans_E.mc->Capacite() & Capa_E()))
     ||( ens->pt_trans.mc && (ens->pt_trans.actif && ens->pt_trans.mc->Capacite() & Capa_E())
       &&(ens->pt_rot.actif || ens->pt_trans_E.actif)
       )
     )
   ) {if(ens->pt_trans.actif) {
        alx_point_contact *pt_E;
        // Sauver le repère dans pipo repère
        ens->repere_pipo_E.maj( *(ens->noeud_du_modele) );
          ens->repere_pipo_E_actif = true;
        // Trouver l'autre point pour calculer l'étiremment
        if(ens->pt_rot.actif) {pt_E = &ens->pt_rot;} else {pt_E = &ens->pt_trans_E;}
        // pos_avant : Coordonnées du point trans dans le repère père sans étirement
        pt_tmp_E_1.maj( ens->pt_trans.Valeur_initiale() );
        ens->noeud_du_modele->Changer_coordonnees_inverse( pt_tmp_E_1 );
        // Etirement
        double E_dx, E_dy
             , det_E_dx = pt_E->Valeur_initiale().X() - ens->pt_trans.Valeur_initiale().X()
             , det_E_dy = pt_E->Valeur_initiale().Y() - ens->pt_trans.Valeur_initiale().Y();
        if(det_E_dx != 0) {E_dx = (pt_E->PT()->X() - ens->pt_trans.PT()->X())/det_E_dx;} else {E_dx = 1;}
        if(det_E_dy != 0) {E_dy = (pt_E->PT()->Y() - ens->pt_trans.PT()->Y())/det_E_dy;} else {E_dy = 1;}
        if(det_E_dx == 0) {E_dx = E_dy;}
        if(det_E_dy == 0) {E_dy = E_dx;}
        ens->noeud_du_modele->Etirement( ens->repere0.Ex() * E_dx
                                       , ens->repere0.Ey() * E_dy);
        // pos_apres : Coordonnées du point trans dans le repère père avec étirement
        pt_tmp_E_2.maj( ens->pt_trans.Valeur_initiale() );
        ens->noeud_du_modele->Changer_coordonnees_inverse( pt_tmp_E_2 );
        // Déplacement de pos_avant - pos_apres
        //ens->noeud_du_modele->Translation( -50, -50 );
        //printf("(%f; %f) -> (%f; %f)\n", pt_tmp_E_1.X(), pt_tmp_E_1.Y(), pt_tmp_E_2.X(), pt_tmp_E_2.Y());
        ens->noeud_du_modele->Translation(  pt_tmp_E_1.X(),  pt_tmp_E_1.Y() );
        ens->noeud_du_modele->Translation( -pt_tmp_E_2.X(), -pt_tmp_E_2.Y() );
      } else {// On a un point de rotation seulement, mais avec de l'étirement
              alx_point_contact *pt_E;
              ens->repere_pipo_E.maj( *(ens->noeud_du_modele) );
                ens->repere_pipo_E_actif = true;
              // Trouver l'autre point pour calculer l'étiremment
              pt_E = &ens->pt_rot;
              // pos_avant : Coordonnées du point trans dans le repère père sans étirement
              pt_tmp_E_1.maj( *(ens->pt_rot_defaut.PT()) );
              ens->noeud_du_modele->Changer_coordonnees_inverse( pt_tmp_E_1 );
              // Etirement
              double E_dx, E_dy
                   , det_E_dx = pt_E->Valeur_initiale().X() - ens->pt_rot_defaut.PT()->X()
                   , det_E_dy = pt_E->Valeur_initiale().Y() - ens->pt_rot_defaut.PT()->Y();
              if(det_E_dx != 0) {E_dx = (pt_E->PT()->X() - ens->pt_rot_defaut.PT()->X())/det_E_dx;} else {E_dx = 1;}
              if(det_E_dy != 0) {E_dy = (pt_E->PT()->Y() - ens->pt_rot_defaut.PT()->Y())/det_E_dy;} else {E_dy = 1;}
              if(det_E_dx == 0) {E_dx = E_dy;}
              if(det_E_dy == 0) {E_dy = E_dx;}
              ens->noeud_du_modele->Etirement( ens->repere0.Ex() * E_dx
                                             , ens->repere0.Ey() * E_dy);
              // pos_apres : Coordonnées du point trans dans le repère père avec étirement
              pt_tmp_E_2.maj( *(ens->pt_rot_defaut.PT()) );
              ens->noeud_du_modele->Changer_coordonnees_inverse( pt_tmp_E_2 );
              // Déplacement de pos_avant - pos_apres
              ens->noeud_du_modele->Translation(  pt_tmp_E_1.X(),  pt_tmp_E_1.Y() );
              ens->noeud_du_modele->Translation( -pt_tmp_E_2.X(), -pt_tmp_E_2.Y() );
             }
     }

 A_pt_tmp3.maj( A_pt_tmp4 );
 A_pt_tmp3.Translation_inverse( A_v_tmp3 );

// Information des intéressés :
 for(; it!=it_fin; it=it->svt)
  {
   if( (it->E())->capacite & alx_point_contact_capacite_rapporteur)
    {
     modele_indesirable = (it->E())->modele_physique;
     if( faire_rot && (rotation != (double)0) )
      {
       modele_indesirable->correction.maj( A_v_tmp4 ); //correction_svg );
       modele_indesirable->Informer_au_sujet_de_rotation( &(ens->rotation_effectuee), &(ens->centre_rot_export));
      }
     if( (/*deplacement_pt_trans*/A_v_tmp3.dx != 0)||(/*deplacement_pt_trans*/A_v_tmp3.dy != 0) )
      {
       modele_indesirable->correction.maj( A_v_tmp4 ); //correction_svg );
       modele_indesirable->Informer_au_sujet_de_translation(&A_v_tmp3/*deplacement_pt_trans*/, &A_pt_tmp3/*point_contact_au_depart*/); // Pour celui ci on informe les autres mais pas nous (on ne veut pas passer par rationnalise
      }
//     vect_maj = NULL;
//     modele_indesirable = NULL;
//     rationnaliser( (int)(void*)((it->E())->modele_physique) ); // Pour remmettre les choses en ordre après l'intervention du Repositionnement
    }
  }
 modele_indesirable = NULL; //Bloquer_rationnalisation(false);
 ens->correction.maj(0,0);

 ens->L_rappel_changement.Rappeler( this );
}


//______________________________________________________________________________
//XXX A MODIFIER IL FAUT AUTRE CHOSE !!!XXX
bool alx_simulateur_points_contact::Trouver_model( alx_model_simulation_physique1 *mp
                                                 , alx_liste<alx_repere2D*> *L_rep
                                                 , alx_modele_contact   **mc
                                                 , alx_ensemble_contact **ens_c)
{alx_element_liste<alx_ensemble_contact*> *it_ec     = L_ens_contact.Premier()
                                        , *it_fin_ec = L_ens_contact.Fin();
 alx_element_liste<alx_modele_contact*> *it_mc, *it_fin_mc;
// alx_ensemble_contact *ec;
 //bool possible = false;
 for(; it_ec!=it_fin_ec; it_ec=it_ec->svt)
  {it_mc     = (it_ec->E())->L_modele_contact.Premier();
   it_fin_mc = (it_ec->E())->L_modele_contact.Fin();

// Ajouter ici comme condition que le chemin d'accès au MP doit être tel qu'il contienne le noeud référent de l'ensemble de contact
// En effet un même noeud peut être présent dans plusieurs ensemble de contact différents selon le chemin d'accès à ce noeud.
   if(mp == &(it_ec->E()->noeud_du_modele->MP()))
    {*ens_c = it_ec->E();
     *mc    = NULL;
     for(; (it_mc!=it_fin_mc); it_mc=it_mc->svt)
      {if(it_mc->E()->modele_physique == mp)
        {*mc = it_mc->E();
         break;}
      }
     return true;
    }
   for(; (it_mc!=it_fin_mc); it_mc=it_mc->svt)
    {if((it_mc->E())->modele_physique == mp)
      {// Vérifier ici si retrouve ou pas le noeud référant de l'ensemble de contact dans la liste de repères
       if(L_rep == NULL)
        {*ens_c = it_ec->E();
         *mc    = it_mc->E();
         return true;
        }
       alx_repere2D *repere = it_ec->E()->noeud_du_modele;
       //possible = true;
       *ens_c = it_ec->E();
       *mc    = it_mc->E();
       if(L_rep->Contient( repere ))
        {
         return true;
        }
      }
    }
  }

// Pour le cas des fenêtres. En effet il y a alors plusieurs MP pour un même noeud...
//if(possible)
// {return true;}

 return false;
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
// Le numéro identifiant est ici l'adresse du modèle physique considéré.
void alx_simulateur_points_contact::Changement_dans_le_modele(alx_model_simulation_physique1 *mp)
{double        angle;
 alx_point2D   /**pt,*/ tmp;
 alx_vecteur2D vect;
 alx_modele_contact   *mc;
 alx_ensemble_contact *ec;

 if(!Trouver_model(mp, NULL, &mc, &ec)) return;

// alx_point_contact *pt_trans = ec->Pt_contact_trans_actif()
//                 , *pt_rot   = ec->Pt_contact_rot_actif();
 switch(mp->infos_comp[1])
  {case 1: // Translation
    {vect.maj( *((alx_vecteur2D*)(void*)(mp->infos_comp[2])) ); // en coordonnés du modèle.

     if(!mp->ne_pas_modifier_la_correction)
       ec->correction.Soustraire( vect );
     ec->noeud_du_modele->Changer_coordonnees_prim_inverse( vect );
     ec->repere0.origine.Translation( vect );

    // Cas où le point de translation est celui par défaut, donc lié au noeud:
     alx_point_contact *ptc_trans_tmp = ec->Pt_contact_trans_actif();
     if(!ptc_trans_tmp) return;

     alx_point2D pt_trans_tmp( *(ptc_trans_tmp->PT()) );
     ec->noeud_du_modele->Changer_coordonnees_inverse( pt_trans_tmp );
     if( !(ec->pt_trans.actif) )
       ec->vect_position0.maj( pt_trans_tmp );

    // Calcul de l'angle de base
     alx_point_contact *ptc_rot_tmp = ec->Pt_contact_rot_actif();
     if(!ptc_rot_tmp) return;
     alx_point2D pt_rot_tmp( *(ptc_rot_tmp->PT()) );
     ec->noeud_du_modele->Changer_coordonnees_inverse( pt_rot_tmp );
     alx_vecteur2D v_pt(pt_rot_tmp, pt_trans_tmp);
     ec->rotation0 = ec->rotation_prcdt = Vect_h().angle_trigo(v_pt) - ec->complement_rot0;
    }
   break;

   case 2: // Rotation
    {angle = *( (double*)(void*)(mp->infos_comp[2]) );
     alx_point2D point( *((alx_point2D*)(void*)(mp->infos_comp[3])) );

    // Rotation du repère de base
     ec->noeud_du_modele->Changer_coordonnees_inverse( point );
     ec->repere0.origine.Pivoter(angle, point);

    // Remettons d'abord la correction précédente à la bonne valeur,
    // Elle doit en effet être la même du point de vue du repère père.
     ec->correction.Pivoter( -angle );
    // Il faut ensuite mettre cette correction à jour par rapport au déplacement
    // induit par la rotation pour le point de contact de translation du modèle
     alx_point_contact *ptc_trans_tmp = ec->Pt_contact_trans_actif();
       if(!ptc_trans_tmp) return;
     alx_point2D pt_trans_tmp( *(ptc_trans_tmp->PT()) );
       ec->repere0.Changer_coordonnees_inverse( pt_trans_tmp );
     alx_point2D pt_trans_tmp_prcdt( pt_trans_tmp );
     pt_trans_tmp_prcdt.Pivoter(-angle, point);
     alx_vecteur2D correction_cour(pt_trans_tmp, pt_trans_tmp_prcdt);
     ec->repere0.Changer_coordonnees_prim( &correction_cour );
    // et enfin la mise à jour:
     ec->correction.Ajouter( correction_cour );
    }
   break;

   case 3: // Etirement
    /*
     vect.maj( *((alx_vecteur2D*)(void*)(mp->infos_comp[2])) );
     pt = (alx_point2D*)(void*)(mp->infos_comp[3]);
     tmp.maj(ec->vect_position0.dx, ec->vect_position0.dy);
     tmp.Translation_inverse(*pt);
     tmp.x *= vect.dx; tmp.y *= vect.dy;
     tmp.Translation(*pt);
     ec->vect_position0.maj(tmp);
    */
   break;
  }
}

//______________________________________________________________________________
void alx_simulateur_points_contact::rationnaliser(int num)
{alx_model_simulation_physique1 *ptr = (alx_model_simulation_physique1*)(void*)num;
 dernier_point_contact_ajoute = NULL;

// if(modele_indesirable == ptr)
//   return;

// Cas d'un changement dynamique dans le modèle, on veut en être informé:
 if( (ptr->nb_infos_comp>=2)
   &&(ptr->infos_comp[0]==7) )
  {if(vect_maj == (alx_vecteur2D*)(void*)(ptr->infos_comp[2]))
     return;
   Changement_dans_le_modele(ptr);
   return;
  }

// Vérifier que le modèle hurle bien à la mort pour cause de détection
// d'un point de contact.
 if( /*(ptr->nb_infos_comp != 4)
   ||*/( (ptr->infos_comp[0]!=2)&&(ptr->infos_comp[0]!=6) )
   ||(ptr->infos_comp[1]!=0) ) return; // Ce message n'est pas le bon

// On pointe le point(ahahah)
 alx_point2D *pt = (alx_point2D*)(void*)ptr->infos_comp[2];

// Voir si le modèle n'a pas déja des points de contacts
 alx_modele_contact   *mc;
 alx_ensemble_contact *ec;

 if(Trouver_model(ptr, (alx_liste<alx_repere2D*>*)(void*)(ptr->infos_comp[4]), &mc, &ec))
  {if(ptr->infos_comp[0]!=6)
     Ajouter_point_contact(ec, mc, pt, NULL);
    else
     {alx_liste<alx_repere2D*> L_rep;
      L_rep.maj( *((alx_liste<alx_repere2D*>*)(void*)(ptr->infos_comp[4])) );
      // Ajouter le noeud courant à L_rep, qui décidément ne contient pas tout ce qu'il faut
      if(mc->noeud_du_modele)
       {// On a spécifié un noeud qui sera mis à jour lors des manipulations du modèle.
        // Il y a 2 cas;
        //  - soit le noeud est dans la liste, auquel cas on enlève tous les éléments suivants de la liste
        //  - soit il n'y est pas et dans ce cas on l'ajoute à la fin de la liste
        alx_element_liste<alx_repere2D*> *it     = L_rep.Premier()
                                       , *it_fin = L_rep.Fin();
        while( it != it_fin
             &&(alx_noeud_scene*)it->E() != mc->noeud_du_modele)
         {it = it->svt;
         }
        if(it == it_fin)
         {L_rep.Ajouter_a_la_fin( mc->noeud_du_modele );
         } else {alx_element_liste<alx_repere2D*> *it_modele = it;
                 while(it_modele->svt != it_fin)
                  {L_rep.Retirer(it_modele->svt);}
                }
       }
      Ajouter_point_contact(ec, mc, pt, &L_rep);
     }
  }
}

//______________________________________________________________________________
bool alx_simulateur_points_contact::Ajouter_point_contact( alx_ensemble_contact *ec
                                                         , alx_modele_contact   *mc
                                                         , alx_point2D          *pt
                                                         , alx_liste<alx_repere2D*>  *L_rep)
{ec->capacite_etirement = false;
// if(L_rep == NULL)
//   printf("AU SECOURS!!!");
 if( (mc->capacite&alx_point_contact_capacite_translation)
   &&(!ec->pt_trans.actif) )
  {// On sélectionne un modèle connu comme capable de translation
   // ET PUIS le point de contact pour la translation n'est pas encore défini
   ec->pt_rot_defaut.actif = (mc->capacite&alx_point_contact_capacite_rotation)
                           &&(!ec->pt_rot.actif);

   //DEBUG
   bool b_tmp = ec->pt_rot.actif;
   alx_point2D        *pt_rot_tmp;
   alx_modele_contact *mc_rot_tmp;
   alx_liste<alx_repere2D*>  L_rep_rot_tmp;
   if(b_tmp) {pt_rot_tmp = ec->pt_rot.pt;
              mc_rot_tmp = ec->pt_rot.mc;
              Retirer_pt_contact(ec, ec->pt_rot.pt);
              L_rep_rot_tmp.maj( *(ec->pt_rot.L_repere) );
             }


   if(L_rep) ec->pt_trans.Prelier(L_rep);
   /* XXX DEBUG
   bool b_tmp = ec->pt_rot.actif;
   ec->pt_rot.actif = false;*/
   Fixer_point_contact_translation(ec, mc, pt, ec->pt_trans);
   /* XXX DEBUG
   ec->pt_rot.actif = b_tmp;
   if(ec->pt_rot.actif)
    {Fixer_point_contact_rotation(ec, ec->pt_rot.mc, ec->pt_rot.pt, ec->pt_trans, ec->pt_rot);
     ec->pt_rot.Valeur_initiale( *ec->pt_rot.PT() );
    } else L_ens_contact_actifs.Ajouter_au_debut( ec );*/
   ec->pt_trans.Valeur_initiale( *ec->pt_trans.PT() );

   //DEBUG
   if(b_tmp) {Ajouter_point_contact(ec, mc_rot_tmp, pt_rot_tmp, &L_rep_rot_tmp);}

   return true;
  }

// On ne peut avoir le même point pour la translation et la rotation
// if(ec->pt_trans.PT() == pt)
//   return false;

 if( (mc->capacite&alx_point_contact_capacite_rotation)
   &&(!ec->pt_rot.actif) )
  {// On sélectionne un modèle connu comme capable de rotation
   // ET PUIS le point de contact pour la rotation n'est pas encore défini
   if(ec->pt_trans.actif)
    {Fixer_point_contact_translation(ec, ec->pt_trans.mc, ec->pt_trans.pt, ec->pt_trans);
     if(L_rep) ec->pt_rot.Prelier(L_rep);
     ec->pt_trans.Valeur_initiale( *ec->pt_trans.PT() );
     bool rep = Fixer_point_contact_rotation(ec, mc, pt, ec->pt_trans, ec->pt_rot);
       ec->pt_rot.Valeur_initiale( *ec->pt_rot.PT() );
     return rep;
    }
   else {L_ens_contact_actifs.Ajouter_au_debut(ec);
         if(ec->pt_trans_defaut.actif)
          {Fixer_point_contact_translation(ec, ec->pt_trans_defaut.mc, ec->pt_trans_defaut.pt, ec->pt_trans_defaut);
           if(L_rep) ec->pt_rot.Prelier(L_rep);
           bool rep = Fixer_point_contact_rotation(ec, mc, pt, ec->pt_trans_defaut, ec->pt_rot);
             ec->pt_rot.Valeur_initiale( *ec->pt_rot.PT() );
           return rep;
          }
        }
  }

// OK si le modèle de contact de ce point ou du point de translation (ou translation/roation) déja enregistré à une capacité d'étirement
// CAS restant : on clique sur un modèle de translation avec étirement et on a déja un point de translation...
 if( ec->pt_trans.actif
   &&!ec->pt_trans_E.actif
   &&!ec->pt_rot.actif
   &&((mc->capacite | ec->pt_trans.mc->capacite) & Capa_E())
   )
  {ec->pt_trans_E.actif = true;
   ec->pt_trans_E.pt = pt;
   ec->pt_trans_E.mc = mc;
   A_pt_tmp.maj( *pt );
   ec->pt_trans_E.Valeur_prcdt( A_pt_tmp );
   mc->noeud_du_modele->Changer_coordonnees_inverse( A_pt_tmp );
   ec->pt_trans_E.Valeur_prcdt_ds_rep_pere( A_pt_tmp );
   dernier_point_contact_ajoute = &ec->pt_trans_E;
   if(L_rep) ec->pt_trans_E.Prelier( L_rep );
     ec->pt_trans_E.Valeur_initiale( *ec->pt_trans_E.PT() );
   return true;
  }

 return false;
}

//______________________________________________________________________________
bool alx_simulateur_points_contact::Retirer_pt_contact( alx_ensemble_contact *ens
                                                      , alx_point2D *pt)
{bool rep = false;
 ens->capacite_etirement = false;
// Cas 1 :
 if(ens->pt_trans.actif && (ens->pt_trans.pt==pt))
  {
   ens->pt_trans.mc->modele_physique->Rupture_contact_detecte(pt, (alx_classe_base_liant*)this, NULL);  // On informe de la rupture

   if(ens->rot_defaut) ens->pt_rot_defaut.actif = true;
   ens->pt_trans.actif = rep = false;
   ens->repere_pipo_E_actif = false;

   if(ens->pt_trans_defaut.actif)
    {
     Fixer_point_contact_translation(ens, ens->pt_trans_defaut.mc, ens->pt_trans_defaut.pt, ens->pt_trans_defaut);
     if(ens->pt_rot.actif)
      {if(ens->pt_rot.MC()->Capacite() & Capa_T())
        {ens->pt_rot.actif = false;
         this->Ajouter_point_contact(ens, ens->pt_rot.MC(), ens->pt_rot.pt, ens->pt_rot.get_L_repere() );
        } else {Fixer_point_contact_rotation(ens, ens->pt_rot.mc, ens->pt_rot.pt, ens->pt_trans_defaut, ens->pt_rot);}
      }
      else {if(ens->pt_rot_defaut.actif)
             Fixer_point_contact_rotation(ens, ens->pt_rot_defaut.mc, ens->pt_rot_defaut.pt, ens->pt_trans_defaut, ens->pt_rot_defaut);
            if(ens->pt_trans_E.actif) {
              ens->pt_trans_E.actif = false;
              this->Ajouter_point_contact( ens
                                         , ens->pt_trans_E.MC()
                                         , ens->pt_trans_E.pt
                                         , ens->pt_trans_E.get_L_repere());
             } else {L_ens_contact_actifs.Retirer( ens );}
           }
    } else {if(ens->pt_trans_E.actif) {
              ens->pt_trans_E.actif = false;
              this->Ajouter_point_contact( ens
                                         , ens->pt_trans_E.MC()
                                         , ens->pt_trans_E.pt
                                         , ens->pt_trans_E.get_L_repere());
             } else {L_ens_contact_actifs.Retirer( ens );}
           }
  }

 if(ens->pt_rot.actif && (ens->pt_rot.pt==pt))
  {
   ens->pt_rot.mc->modele_physique->Rupture_contact_detecte(pt, (alx_classe_base_liant*)this, NULL);  // On informe de la rupture

   ens->pt_rot.actif = rep = false;
   ens->pt_rot_defaut.actif = true;
   ens->repere_pipo_E_actif = false;
   if(ens->pt_rot_defaut.actif)
    {
     //double r = ens->
     if(ens->pt_trans.actif)
      {
       Fixer_point_contact_translation(ens, ens->pt_trans.mc, ens->pt_trans.pt, ens->pt_trans);
       if(ens->pt_trans.mc->capacite & alx_point_contact_capacite_rotation)
         Fixer_point_contact_rotation(ens, ens->pt_rot_defaut.mc, ens->pt_rot_defaut.pt, ens->pt_trans, ens->pt_rot_defaut);
        else
          ens->pt_rot_defaut.actif = false;
      }
      else {L_ens_contact_actifs.Retirer( ens );
            if(ens->pt_trans_defaut.actif)
             {Fixer_point_contact_translation(ens, ens->pt_trans_defaut.mc, ens->pt_trans_defaut.pt, ens->pt_trans_defaut);
              Fixer_point_contact_rotation(ens, ens->pt_rot_defaut.mc, ens->pt_rot_defaut.pt, ens->pt_trans_defaut, ens->pt_rot_defaut);}
           }
    } else if(ens->pt_trans.actif == false) L_ens_contact_actifs.Retirer( ens );
  }
 if(ens->pt_trans_defaut.actif && (ens->pt_trans_defaut.pt==pt))
  {
   ens->pt_trans_defaut.mc->modele_physique->Rupture_contact_detecte(pt, (alx_classe_base_liant*)this, NULL);  // On informe de la rupture
   ens->pt_trans_defaut.actif = rep = false;
  }

 if(ens->pt_rot_defaut.actif && (ens->pt_rot_defaut.pt==pt))
  {
   ens->pt_rot_defaut.mc->modele_physique->Rupture_contact_detecte(pt, (alx_classe_base_liant*)this, NULL);  // On informe de la rupture
   ens->rot_defaut = false;
   ens->pt_rot_defaut.actif = rep = false;
  }

 if( ens->pt_trans_E.actif && (ens->pt_trans_E.pt == pt) )
  {ens->pt_trans_E.actif = false;
   ens->repere_pipo_E_actif = false;
   if(ens->pt_trans.actif)
     Fixer_point_contact_translation(ens, ens->pt_trans.mc, ens->pt_trans.pt, ens->pt_trans);
  }
 return rep;
}
