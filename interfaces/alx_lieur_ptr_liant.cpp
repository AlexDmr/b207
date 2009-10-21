#include "alx_lieur_ptr_liant.h"


void alx_lieur_ptr_liant::rationnaliser(int num)
{if(num == 0)
  {//C'est bien le simulateur de pointeurs qui parle
  // Il faudra voir si des gars sont intéressées par l'évennement du pointeur
  // c'est à dire si il sont intéressés par le point correspondant.
   if( !sim_pointeurs->L_evt.Vide() )
    {alx_evennement_pointeur *evt_ptr = &( (sim_pointeurs->L_evt.Premier())->E() );
     correspondance_ptr_liant *cpl = trouver_cpl( sim_pointeurs->Pointeur(evt_ptr->Identifiant()) );
     if(evt_ptr->Type_evt() != alx_pointeur_mouvement)
       if(cpl) cpl->prevenir( evt_ptr->Type_evt() );
     //if( (cpl!=NULL)&&cpl->prevenir( evt_ptr->Type_evt() ) )
     //  sim_pointeurs->L_evt.pop_front();  // on a traité, on efface.
    }
  }
  else
   { // On reçoit un message, il faut voir si on peut y répondre.
     // Les messages auquels on peut répondre ici sont ceux d'appels
     // à l'aide pour avoir des infos sur un pointeur.
     // Ils devront avoir une certaine forme à définir.
    alx_classe_base_liant *liant_demandeur = (alx_classe_base_liant*)(void*)num;
    int *info_comp_demandeur = liant_demandeur->infos_comp;

    if( (liant_demandeur->nb_infos_comp == 5)
      &&(info_comp_demandeur[0] == -1)
      &&(info_comp_demandeur[1] ==  0) )
     {
      alx_point2D   *pt = (alx_point2D*)(void*)info_comp_demandeur[3];
      int infos_voulues = info_comp_demandeur[2]
        , code          = info_comp_demandeur[4];

     // On regarde si ce point est celui d'un pointeur:
      alx_pointeur *ptr = sim_pointeurs->Pointeur(pt);
      if( ptr==(alx_pointeur*)NULL ) return;
      Trouver_ou_creer_correspondant( ptr, pt, infos_voulues, code, liant_demandeur);
     }

    if( (liant_demandeur->nb_infos_comp == 5)
      &&(info_comp_demandeur[0] == -2)
      &&(info_comp_demandeur[1] ==  0) )
     {
      alx_point2D   *pt = (alx_point2D*)(void*)info_comp_demandeur[3];
      int infos_plus_voulues = info_comp_demandeur[2]
        , code               = info_comp_demandeur[4];

      alx_pointeur *ptr = sim_pointeurs->Pointeur(pt);
      if(ptr==NULL) return;
      Supprimer_interet( ptr, infos_plus_voulues, code, liant_demandeur);
     }

   }
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool fct_cpl(correspondance_ptr_liant *modele, correspondance_ptr_liant *cour)
{if( cour->ptr == modele->ptr) return true;
 return false;
}

//------------------------------------------------------------------------------
correspondance_ptr_liant* alx_lieur_ptr_liant::trouver_cpl( alx_pointeur *ptr )
{
 correspondance_ptr_liant *cpl_trouve = NULL
                        , modele(ptr, (alx_classe_base_liant*)this) ;

 if( !table_correspondance.trouver( (int)(void*)ptr
                                  , &cpl_trouve
                                  , fct_cpl
                                  , &modele) ) return NULL;
 return cpl_trouve;
}

//------------------------------------------------------------------------------
int alx_lieur_ptr_liant::Trouver_ou_creer_correspondant( alx_pointeur *ptr
                                                       , alx_point2D *pt
                                                       , int interet
                                                       , int code
                                                       , alx_classe_base_liant *l_demandeur) // 0:trouvé, 1:créé.
{
 correspondance_ptr_liant *cpl_trouve
                        , modele(ptr, (alx_classe_base_liant*)this) ;
 modele.pt = pt;

 if( !table_correspondance.trouver( (int)(void*)ptr
                                  , &cpl_trouve
                                  , fct_cpl
                                  , &modele) )
  {// On en crée un.
   table_correspondance.inserer(modele, (int)(void*)ptr);
   Trouver_ou_creer_correspondant(ptr, pt, interet, code, l_demandeur); // cette fois on le trouvera...ahahah
   return 1;
  }
  else
   {// on ajoute un interet_liant au correspondant trouvé
    cpl_trouve->L_interet.Ajouter_au_debut( interet_liant( l_demandeur
                                                         , (alx_classe_base_liant*)this
                                                         , code, interet, pt) );
    return 0;
   }
}

//------------------------------------------------------------------------------
bool alx_lieur_ptr_liant::Supprimer_interet( alx_pointeur *ptr
                                           , int interet
                                           , int code
                                           , alx_classe_base_liant *l_demandeur )
{
 correspondance_ptr_liant *cpl_trouve
                        , modele(ptr, (alx_classe_base_liant*)this) ;

 if( table_correspondance.trouver( (int)(void*)ptr
                                 , &cpl_trouve
                                 , fct_cpl
                                 , &modele) )
  {alx_element_liste<interet_liant> *it     = cpl_trouve->L_interet.Premier()
                                  , *it_fin = cpl_trouve->L_interet.Fin()
                                  , *it_tmp;
   unsigned int taille = cpl_trouve->L_interet.Taille();

   while(it!=it_fin)
    {
     it_tmp = it;
     it_tmp = it_tmp->svt;
     if( ((it->E()).liant      == l_demandeur)
       &&((it->E()).code_appel == code) )
      {
       (it->E()).interet_porte &= ~interet;
       if( (it->E()).interet_porte == 0 )
         cpl_trouve->L_interet.Retirer(it);
      }
     it = it_tmp;
    }
   return taille != cpl_trouve->L_interet.Taille();
  }

 return false;
}


