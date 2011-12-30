#include "alx_liant_ptr_contact.h"

alx_liant_ptr_contact::alx_liant_ptr_contact( alx_simulateur_pointeurs      *sp
                                            , alx_simulateur_points_contact *sc)
{sim_pointeurs = sp;
 sim_contact   = sc;
 sim_pointeurs->abonner_a(this, 1);
 //sim_contact->abonner_a(this, 2);
}

/******************************************************************************/
//void alx_liant_ptr_contact::simuler()
//{}

/******************************************************************************/
void alx_liant_ptr_contact::rationnaliser(int num)
{alx_element_liste<alx_evennement_pointeur> *it_evt     = sim_pointeurs->L_evt.Premier()
                                          , *it_fin_evt = sim_pointeurs->L_evt.Fin()
                                          /*, *it_tmp_evt*/;
 alx_element_liste<alx_ensemble_contact*> *it_contact
                                        , *it_contact_fin = sim_contact->L_ENS_CONTACT().Fin();
                                        //ATTENTION, *it_contact_fin = sim_contact->L_ENS_CONTACT_ACTIFS().Fin();
 L_ens_contact_modif.Vider();
 alx_evennement_pointeur *e;
 alx_pointeur_type_evennement type_evt;
 alx_point2D *pt;
 bool evt_traite;

 switch(num)
  {case 1: // Le simulateur de pointeurs qui nous informe.
    // On récupère l'évennement.
//XXX     while(it_evt!=it_fin_evt)
     for(; it_evt!=it_fin_evt; it_evt=it_evt->svt)
      {evt_traite = false;
       e = &( it_evt->E() );
       // Si c'est un simple mouvement on ne fait RIEN
       /*if(e->Type_evt() == alx_pointeur_mouvement)
        {it_tmp_evt = it_evt;
         it_evt = it_evt->svt;
         sim_pointeurs->L_evt.Retirer(it_tmp_evt);
         continue;} */

       pt = &( ( sim_pointeurs->Pointeur(e->Identifiant()) )->pt );
       it_contact = sim_contact->L_ENS_CONTACT().Premier();

//XXX       it_tmp_evt = it_evt;
//XXX       it_evt = it_evt->svt;
       for(; it_contact!=it_contact_fin; it_contact=it_contact->svt) // Voir qui fait intervenir ce point
        {                                             // dans le simulateur de contact.
         if((it_contact->E())->Fait_intervenir(pt)) // Ajouter à l'ensemble
          {
          // Vérifier si ça n'est pas une disparition ou un relachement
           type_evt = e->Type_evt();
           switch(type_evt)
            {case alx_pointeur_relache:
             case alx_pointeur_disparition:
               if(sim_contact->Retirer_pt_contact(it_contact->E(), pt))
                 evt_traite = true;
             break;
             default:
              // Sinon simplement mettre à jour la zone de contact
               L_ens_contact_modif.Ajouter_au_debut(it_contact->E());
               evt_traite = true;
            }
          }
        }
//XXX       if(evt_traite)
//XXX         sim_pointeurs->L_evt.Retirer(it_tmp_evt); // On a traité l'évennement, on l'efface.
      }
     // Simuler le contact sur les éléments de cet ensemble.
      it_contact     = L_ens_contact_modif.Premier();
      it_contact_fin = L_ens_contact_modif.Fin();
      for(; it_contact!=it_contact_fin; it_contact=it_contact->svt)
        sim_contact->simuler(it_contact->E());
   break;
  }
}
