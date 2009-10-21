#include "alx_simulateur_pointeurs_souris.h"


/*******************************************************************************/
alx_simulateur_pointeurs_souris::alx_simulateur_pointeurs_souris( int *e_x, int *e_y
                                                                , alx_point2D *bg
                                                                , alx_vecteur2D *vh
                                                                , alx_vecteur2D *vv) : alx_simulateur_pointeurs_generique( e_x, e_y
                                                                                                                         , bg
                                                                                                                         , vh
                                                                                                                         , vv )
                                                                                     , derniere_pos(*e_x/2.0, *e_y/2.0)
{}

/*******************************************************************************/
void alx_simulateur_pointeurs_souris::simuler()
{cour = 1-cour;

// Mise à jour des coordonnées.
// int b = SDL_GetMouseState( &(souris_x[cour]), &(souris_y[cour]) );
// SDL_Event  event;

// while( SDL_PollEvent( &event ) )
 switch( event.type )
  {case SDL_MOUSEBUTTONUP:
   case SDL_MOUSEBUTTONDOWN:
  // Mise à jour des coordonnées et mise en attente d'évennements pour les boutons...
    { // Placer un évennement
     //if(event.button.button == 3) // DEBUG 2008 11
     //  break;
     alx_pointeur_type_evennement t_evt;    // Son type.
     if(event.type==SDL_MOUSEBUTTONUP) t_evt = alx_pointeur_relache;
      else t_evt = alx_pointeur_enfonce;

     if( (t_evt == alx_pointeur_relache)
       &&(event.button.button == SDL_BUTTON_MIDDLE) )
      {convertir_coordonnees(*ecran_x/2.0, *ecran_y/2.0, pt_tmp);
       A_ic_tmp.coord.x = pt_tmp.x;
       A_ic_tmp.coord.y = pt_tmp.y;
       A_ic_tmp.dep.x = A_ic_tmp.coord.x - derniere_pos.x;
       A_ic_tmp.dep.y = A_ic_tmp.coord.y - derniere_pos.y;
       derniere_pos.maj(*ecran_x/2.0, *ecran_y/2.0);

      // Balancer la sauce
       A_ep_tmp.maj(0, alx_pointeur_mouvement, A_ic_tmp, Noeud_pere());
       distribuer_evennement( A_ep_tmp ); //alx_evennement_pointeur(0, alx_pointeur_mouvement, comp) );
       break;}

      //XXX event.button.button = SDL_BUTTON_WHEELDOWN ou SDL_BUTTON_WHEELUP
      if( (event.button.button == SDL_BUTTON_MIDDLE)
          ||(event.button.button > 3)
        &&(event.button.button != SDL_BUTTON_WHEELDOWN)
        &&(event.button.button != SDL_BUTTON_WHEELUP)
        )
        break;

     //info_comp_evennement comp; // Les informations complémentaires.
      if( event.button.button == SDL_BUTTON_WHEELDOWN
        ||event.button.button == SDL_BUTTON_WHEELUP ) {A_ic_tmp.type_appuie  = 0;
                                                      } else {A_ic_tmp.type_appuie  = event.button.button;
                                                             }
       //_________________________________________________________
       //if(A_ic_tmp.type_appuie==3) A_ic_tmp.type_appuie=4;
       A_ic_tmp.type_appuie &= 3;
       if(event.button.button == SDL_BUTTON_WHEELDOWN) {A_ic_tmp.type_appuie |= 4;}
       if(event.button.button == SDL_BUTTON_WHEELUP  ) {A_ic_tmp.type_appuie |= 8;}
       //_________________________________________________________

     // Ne pas oublier de localiser le clique, on considère que le clique à eut
     // lieu avant un déplacement éventuel :
     //alx_point2D pt_clique;
     convertir_coordonnees(derniere_pos.x, derniere_pos.y, pt_clique);
     A_ic_tmp.coord.x = pt_clique.x;
     A_ic_tmp.coord.y = pt_clique.y;

     // Balancer la sauce
     A_ep_tmp.maj(0, t_evt, A_ic_tmp, Noeud_pere());
     distribuer_evennement( A_ep_tmp ); //alx_evennement_pointeur(0, t_evt, comp) );

     if (A_ic_tmp.type_appuie&4 || A_ic_tmp.type_appuie&8 ) {
       A_ep_tmp.maj(0, alx_pointeur_relache, A_ic_tmp, Noeud_pere());
       distribuer_evennement( A_ep_tmp );
      }
    }
   break;
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
   case SDL_MOUSEMOTION :
    {//info_comp_evennement comp; // Les informations complémentaires.
     //alx_point2D pt_arrive, dep;

     // Le déplacement est dans (xrel, yrel).
     derniere_pos.Translation(event.motion.xrel, event.motion.yrel);

     convertir_coordonnees(event.motion.xrel, event.motion.yrel, pt_dep);
     convertir_coordonnees(derniere_pos.x   , derniere_pos.y   , pt_arrive);

     A_ic_tmp.coord.x = pt_arrive.x;
     A_ic_tmp.coord.y = pt_arrive.y;
     A_ic_tmp.dep.x = pt_dep.x;
     A_ic_tmp.dep.y = pt_dep.y;
     A_ic_tmp.type_appuie = 0;

     // Balancer la sauce
     A_ep_tmp.maj(0, alx_pointeur_mouvement, A_ic_tmp, Noeud_pere());
     distribuer_evennement( A_ep_tmp ); //alx_evennement_pointeur(0, alx_pointeur_mouvement, comp) );
    }
   break;
  }
}


