#include "alx_simulateur_pointeurs_joystick.h"

/*******************************************************************************/
alx_simulateur_pointeurs_joystick::alx_simulateur_pointeurs_joystick( int *e_x, int *e_y
                                                                    , alx_point2D *bg
                                                                    , alx_vecteur2D *vh
                                                                    , alx_vecteur2D *vv
                                                                    , int nb_joysticks) : alx_simulateur_pointeurs_generique( e_x, e_y
                                                                                                                            , bg
                                                                                                                            , vh
                                                                                                                            , vv )
{this->nb_joysticks = nb_joysticks;
 etat_joysticks = new Etat_joystick[nb_joysticks];
 for(int i=0;i<nb_joysticks;i++)
  {
   etat_joysticks[i].x       = 0;
   etat_joysticks[i].y       = 0;
   etat_joysticks[i].boutons = 0;
  }
}

/*******************************************************************************/
/*int  alx_simulateur_pointeurs_joystick::Ajouter_rapporteur(alx_simulateur_pointeurs *r)
{
 alx_element_liste<alx_pointeur_rapporteur> *it, *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin)&&((it->E()).sim_ptr!=r); it=it->svt) ;

 if(it!=it_fin) return false; // r est déja rapporteur

 alx_pointeur_rapporteur rapporteur;
  rapporteur.sim_ptr = r;
  rapporteur.id = r->Ajouter_pointeur("");

 L_rapporteur.Ajouter_au_debut( rapporteur );
 return rapporteur.id;
}

//______________________________________________________________________________
bool alx_simulateur_pointeurs_joystick::Retirer_rapporteur(alx_simulateur_pointeurs *r)
{
 alx_element_liste<alx_pointeur_rapporteur> *it, *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin)&&((it->E()).sim_ptr!=r); it=it->svt) ;

 if(it==it_fin) return false; // r n'est pas un rapporteur
 L_rapporteur.Retirer(it);
 return true;
}
      */
/*******************************************************************************/
void alx_simulateur_pointeurs_joystick::simuler()
{
 cour = 1-cour;
 int num_joystick;
 
 switch( event.type )
  {
   case SDL_JOYAXISMOTION:
   case SDL_JOYBUTTONDOWN:
     num_joystick = event.jaxis.which;
   break;
   case SDL_JOYBUTTONUP:
     num_joystick = event.jbutton.which;
   break;
   default: return;
  }

 int sens_x = 0
   , sens_y = 0
   , sens;
 switch( event.type )
  {
   case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
     if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
      {
       if(event.jaxis.value < -3200)
         sens = -10;
        else sens = 10;
         
       if( event.jaxis.axis == 0)
        {
         etat_joysticks[num_joystick].x += sens;
         sens_x = sens;
        } 
        else if( event.jaxis.axis == 1)
              {
               etat_joysticks[num_joystick].y += sens;
               sens_y = sens;
              } 

       info_comp_evennement comp; // Les informations complémentaires.
       alx_point2D pt_arrive, dep;
       convertir_coordonnees(sens_x, sens_y, dep);
       convertir_coordonnees(etat_joysticks[num_joystick].x   , etat_joysticks[num_joystick].y   , pt_arrive);

       comp.coord.x = pt_arrive.x;
       comp.coord.y = pt_arrive.y;
       comp.dep.x = dep.x;
       comp.dep.y = dep.y;

      // Balancer la sauce
       distribuer_evennement( alx_evennement_pointeur(num_joystick, alx_pointeur_mouvement, comp, Noeud_pere()) );
      }
    break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
     {// Placer un évennement
      alx_pointeur_type_evennement t_evt;    // Son type.

      info_comp_evennement comp; // Les informations complémentaires.
      comp.type_appuie = 1<<(event.jbutton.button);

      if(event.type==SDL_JOYBUTTONUP)
       {
        t_evt = alx_pointeur_relache;
        etat_joysticks[num_joystick].boutons &= ~(comp.type_appuie);
       }
       else
       {
        t_evt = alx_pointeur_enfonce;
        etat_joysticks[num_joystick].boutons |= comp.type_appuie;
       }

      
     // Ne pas oublier de localiser le clique, on considère que le clique à eut
     // lieu avant un déplacement éventuel :
      alx_point2D pt_clique;
      convertir_coordonnees(etat_joysticks[num_joystick].x, etat_joysticks[num_joystick].y, pt_clique);
      comp.coord.x = pt_clique.x;
      comp.coord.y = pt_clique.y;

     // Balancer la sauce
      distribuer_evennement( alx_evennement_pointeur(num_joystick, t_evt, comp, Noeud_pere()) );
     }
    break;
  }
}


 