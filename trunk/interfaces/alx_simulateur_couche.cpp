#include "alx_simulateur_couche.h"
#include "../utilitaires/alx_arbre.cpp"

/************************** Les constructeurs *********************************/
alx_simulateur_couche::alx_simulateur_couche(alx_simulateur_pointeurs *sp)
{sim_pointeur = sp;
 sim_pointeur->abonner_en_dernier_a(this, 1);
}

/******************************************************************************/
/***************************** Les méthodes ***********************************/
/******************************************************************************/
//alx_liste<alx_repere2D*> *L_rep;
void alx_simulateur_couche::Traiter_infos(info_du_contenant *info, alx_evennement_pointeur *evt_ptr)
{
 if(evt_ptr == (alx_evennement_pointeur*) NULL) {evt_ptr = info->EVT();}

     alx_evennement_pointeur evt_pipo ( *evt_ptr ), *evt_svg = evt;
     info_du_contenant       info_pipo(NULL, NULL, &evt_pipo);
     alx_liste<alx_repere2D*> L_rep_tmp, L_rep_not_in_graph;
     evt = &evt_pipo;

     // Prévenir ceux qu'on a quitté...
     if(evt_ptr->Ptr()->L_reperes().Taille() > 0)
      {//info_pipo.noeud = static_cast<alx_noeud_scene*>(evt_ptr->Ptr()->L_reperes().Dernier()->E());
       if(info) {
         info_pipo.noeud = info->noeud;
         info_pipo.mp    = &info_pipo.noeud->MP();
         info_pipo.L_REPERES() = info->L_REPERES();
        } else {info_pipo.noeud = Noeud_puit();
                info_pipo.mp    = Mp_puit();
               }
       evt_pipo.Type_evt( alx_pointeur_disparition );
         L_rep_tmp.maj( evt_ptr->Ptr()->L_reperes() );
         if(info  && evt_ptr->Type_evt() != ALX_pointeur_disparition())
          {L_rep_tmp.Retirer_liste( info->L_REPERES() );
           L_rep_tmp.Retirer( info->noeud );
           if( !L_rep_tmp.Contient( evt_ptr->Ptr()->L_reperes().Dernier_E_const() )
             &&evt_ptr->Ptr()->L_reperes().Dernier_E_const() != info->noeud )
            {L_rep_tmp.Ajouter_a_la_fin( evt_ptr->Ptr()->L_reperes().Dernier_E_const() );}
          } else {
                  if( !L_rep_tmp.Contient( evt_ptr->Ptr()->L_reperes().Dernier_E_const() ) )
                    L_rep_tmp.Ajouter_a_la_fin( evt_ptr->Ptr()->L_reperes().Dernier_E_const() );
                 }
         evt = &evt_pipo;
        // Vider L_rep_tmp des noeuds qui ne sont plus dans le graphe de scène...(possiblement des noeuds détruits...)
         L_rep_not_in_graph = L_rep_tmp;
           Elaguer_noeuds_descendants_de(L_rep_not_in_graph, evt_ptr->Racine() );
           L_rep_tmp.Retirer_liste( L_rep_not_in_graph );

         Reconstruire_rappels(&info_pipo, L_rep_tmp);
         Traiter_evt(&evt_pipo, &info_pipo);
         Delete_L_rappels(&info_pipo);
      }

     // Prévenir ceux chez qui on rentre...
     if(info && evt_ptr->Type_evt() != ALX_pointeur_disparition())
      {//info_pipo.noeud = static_cast<alx_noeud_scene*>(info->L_REPERES().Dernier()->E());
       //info_pipo.mp    = &info_pipo.noeud->MP();
       if(info) {
         info_pipo.noeud = info->noeud;
         info_pipo.mp    = &info_pipo.noeud->MP();
         info_pipo.L_REPERES() = info->L_REPERES();
        } else {info_pipo.noeud = Noeud_puit();
                info_pipo.mp    = Mp_puit();
               }
       evt_pipo.Type_evt( alx_pointeur_apparition );
         L_rep_tmp.maj( info->L_REPERES() );
         if (!L_rep_tmp.Contient(info->noeud) )
           L_rep_tmp.Ajouter_a_la_fin( info->noeud );
         if(evt_ptr->Ptr()->L_reperes().Taille() > 0)
           L_rep_tmp.Retirer_liste( evt_ptr->Ptr()->L_reperes() );
         if( !L_rep_tmp.Contient( evt_ptr->Ptr()->L_reperes().Dernier_E_const() ) && info->L_REPERES().Contient( evt_ptr->Ptr()->L_reperes().Dernier_E_const() )
           &&evt_ptr->Ptr()->L_reperes().Dernier_E_const() != info->noeud )
           {L_rep_tmp.Ajouter_a_la_fin( evt_ptr->Ptr()->L_reperes().Dernier_E_const() );}
         if( !L_rep_tmp.Contient(info->noeud) && evt_ptr->Ptr()->L_reperes().Dernier_E_const() != info->noeud)
           L_rep_tmp.Ajouter_a_la_fin( info->noeud );
         evt = &evt_pipo;
         Reconstruire_rappels(&info_pipo, L_rep_tmp);
         Traiter_evt(&evt_pipo, &info_pipo);
         Delete_L_rappels(&info_pipo);
      }

     // Mise à jour des données
     evt = evt_svg;
     if(info) {
       evt_ptr->Ptr()->L_reperes( info->L_REPERES() );
       evt_ptr->Ptr()->Noeud_sous_pointeur( info->NOEUD() );
       if( !evt_ptr->Ptr()->L_reperes().Contient(info->noeud) )
         evt_ptr->Ptr()->Add_to_L_reperes( info->noeud );
      } else {evt_ptr->Ptr()->Vider_L_reperes();
              evt_ptr->Ptr()->Noeud_sous_pointeur( NULL );
             }

}

//______________________________________________________________________________
void alx_simulateur_couche::Elaguer_noeuds_descendants_de(alx_liste<alx_repere2D*> &L_noeuds, alx_noeud_scene *noeud_racine)
{if(noeud_racine->Val_MetaData("alx_simulateur_couche::Elaguer_noeuds_non_descendants_de") !=  (char*)NULL) {return;}
 noeud_racine->Ajouter_MetaData_T("alx_simulateur_couche::Elaguer_noeuds_non_descendants_de", "1");

   L_noeuds.Retirer( static_cast<alx_repere2D*>(noeud_racine) );
   alx_element_liste<alx_arbre<alx_noeud_scene*>*> *it = noeud_racine->Init_parcours_fils();
   alx_noeud_scene *n;
   while( !noeud_racine->Est_parcours_fils_fini(it) )
    {n  = it->E()->e;
     Elaguer_noeuds_descendants_de(L_noeuds, n);
     it = noeud_racine->Suivant_dans_parcours_fils(it);
    }
   noeud_racine->Terminer_parcours_fils(it);

 noeud_racine->Retirer_MetaData_T("alx_simulateur_couche::Elaguer_noeuds_non_descendants_de");
}

//______________________________________________________________________________
bool alx_simulateur_couche::Zone_pointee(alx_noeud_scene *ns, void *info_sup)
{alx_noeud_scene *noeud              = ns;
 alx_evennement_pointeur *evt_ptr    = ((alx_evennement_pointeur*)info_sup);
 alx_point2D     *pt                 = evt_ptr->ptr->P_Point();
// alx_model_simulation_physique1 *mp;
 info_du_contenant *info = (info_du_contenant*)NULL;

 // On recalle le pointeur pour tenir compte que ses coordonnées sont exprimées dans noeud
  //pt_tmp.maj( *pt );
  //noeud->Changer_coordonnees_inverse( *pt );
 int op = noeud->operation_si_passage;
 bool rep = false;
   evt=evt_ptr;
   switch( evt_ptr->Type_evt() )
    {case alx_pointeur_enfonce:   op = noeud->operation_si_appuie;  break;
     case alx_pointeur_relache:   op = noeud->operation_si_relache; break;
     case alx_pointeur_mouvement: op = noeud->operation_si_passage; break;
    }

 info = noeud->Contenu_dans_fils(*pt, op);
 if( (info == NULL)
   ||( info != NULL && (!info->L_REPERES().Est_prefixe_de( evt_ptr->Ptr()->L_reperes() )
                       ||info->L_REPERES().Taille() >= evt_ptr->Ptr()->L_reperes().Taille()
                       ||info->NOEUD() != evt_ptr->Ptr()->L_reperes().Dernier_E_const()
                       )
     )
   ||(evt_ptr->Type_evt() == ALX_pointeur_disparition()) ) // Doit-on prévenir d'une entrée ou sortie?
  {this->Traiter_infos(info, evt_ptr);
  }

 if(info) // Doit-on prévenir des détections de contact?
  {evt = evt_ptr;
   Traiter_evt(evt_ptr, info);
   rep = true;
  }

/*
if( (info = noeud->Contenu_dans_fils(*pt, op)) != NULL )
 {// Prévenir le noeud précédent que le pointeur n'est plus au dessus, si c'est le cas
  alx_pointeur *ptr = evt_ptr->Ptr();
  if ( evt_ptr->Noeud_sous_pointeur()!=(alx_noeud_scene*)NULL
     && ptr->Noeud_sous_pointeur() != info->NOEUD()
     && evt_ptr->Type_evt() != alx_pointeur_apparition
     && evt_ptr->Type_evt() != alx_pointeur_disparition )
   {alx_evennement_pointeur evt_pipo( *evt_ptr );

    // Générer un évennement de disparition pour l'ancien
      evt_pipo.Type_evt( alx_pointeur_disparition );
      Zone_pointee(noeud, &evt_pipo);

    // Générer un évennement d'apparition pour le nouveau
      evt_pipo.Type_evt( alx_pointeur_apparition );
      Zone_pointee(noeud, &evt_pipo);
   }
  // Mise à jour du noeud sous le pointeur
  evt_ptr->Noeud_sous_pointeur( info->NOEUD() );
  Traiter_evt(evt_ptr, info);
  rep = true;
 }
*/
//______________
 if( (info)
   &&(info != Rep_puit()) )
  {delete info;}

 return rep;
}

/******************************************************************************/
void alx_simulateur_couche::Delete_L_rappels(info_du_contenant *info)
{
  if(info->LL_RAPPEL())
   {alx_liste<Liste_de_rappel*> *LR = info->LL_RAPPEL();
    alx_element_liste<Liste_de_rappel*> *it     = LR->Premier()
                                      , *it_fin = LR->Fin();
    for(;it!=it_fin;it=it->svt)
     {it->E()->Vider();
      //Not a copy... delete it->E();
     }

    LR->Vider();
   }
 delete info->LL_RAPPEL();
 info->LL_RAPPEL( NULL );
}

/******************************************************************************/
void alx_simulateur_couche::Traiter_evt(alx_evennement_pointeur *evt_ptr, info_du_contenant *info)
{alx_point2D *pt = evt_ptr->ptr->P_Point();
  // On préviens les gents maintenant!
  alx_liste<alx_repere2D*> L_repere( info->L_reperes );
  info->EVT(evt_ptr);
  if(info->noeud && info->mp && !info->noeud->Est_une_replique())
    switch ( evt_ptr->Type_evt() )
     {case alx_pointeur_enfonce  : info->mp->Point_contact_detecte(pt, this, &L_repere)      ; break;
      case alx_pointeur_relache  : info->mp->Rupture_contact_detecte(pt, this, &L_repere)    ; break;
      case alx_pointeur_mouvement: info->mp->Deplacement_contact_detecte(pt, this, &L_repere); break;
     }
  if(info->LL_RAPPEL())
   {alx_liste<Liste_de_rappel*> *LR = info->LL_RAPPEL();
    alx_element_liste<Liste_de_rappel*> *it     = LR->Premier()
                                      , *it_fin = LR->Fin();
    for(;it!=it_fin;it=it->svt)
     {it->E()->Param( info );
      it->E()->Rappeler();
     }
   }
}

/******************************************************************************/
void alx_simulateur_couche::simuler()
{}

/******************************************************************************/
bool Mise_a_appartenance_non_testee(alx_noeud_scene **e, void *info_comp)
{if( !(*e)->Appartenance_deja_testee() ) return false;
 (*e)->Appartenance_deja_testee( false );
 return true;
}

void alx_simulateur_couche::rationnaliser(int num)
{alx_element_liste<alx_evennement_pointeur> *it     = sim_pointeur->L_evt.Premier();
// alx_point2D *pt; 
 alx_evennement_pointeur *evt_ptr;

 switch(num)
  {case 1: // Un message du simulateur de pointeurs, on va voir les évennements
     evt_ptr = &( it->E() );
     if( (evt_ptr->Racine() != NULL)
       &&Zone_pointee(evt_ptr->Racine()/*A_racine*/, evt_ptr) ) // On a traité, on efface
       {sim_pointeur->L_evt.Retirer(it);}
   break;
  }
}



