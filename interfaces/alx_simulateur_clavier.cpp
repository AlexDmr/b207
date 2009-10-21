#include "alx_simulateur_clavier.h"
//#include <mem.h>

void alx_simulateur_clavier::init()
{etat_clavier =  SDL_GetKeyState(&NB_CAR);
 etat_clavier_prcdt = new Uint8[NB_CAR];
 memcpy(etat_clavier_prcdt, etat_clavier, NB_CAR);

 SDL_EnableUNICODE( 1 );
 SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);}

/*******************************************************************************/
void alx_simulateur_clavier::distribuer_evennement(alx_evennement_dispositif_saisie &evt)
{alx_element_liste<alx_ds_rapporteur> *it, *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin); it=it->svt)
  {evt.Identifiant( (it->E()).id );
   (it->E()).sim_ds->L_evt.Ajouter_a_la_fin(evt);
  }
}

/*******************************************************************************/
alx_simulateur_clavier::alx_simulateur_clavier()
{init();}

/*******************************************************************************/
int alx_simulateur_clavier::Ajouter_rapporteur(alx_simulateur_dispositif_saisie *r)
{alx_element_liste<alx_ds_rapporteur> *it, *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin)&&((it->E()).sim_ds!=r); it=it->svt) ;

 if(it!=it_fin) return false; // r est déja rapporteur

 alx_ds_rapporteur rapporteur;
  rapporteur.sim_ds = r;
  rapporteur.id = r->Ajouter_dispositif_saisie();

 L_rapporteur.Ajouter_a_la_fin( rapporteur );
 return rapporteur.id;}

/*******************************************************************************/
bool alx_simulateur_clavier::Retirer_rapporteur(alx_simulateur_dispositif_saisie *r)
{alx_element_liste<alx_ds_rapporteur> *it, *it_fin = L_rapporteur.Fin();

 for(it=L_rapporteur.Premier(); (it!=it_fin)&&((it->E()).sim_ds!=r); it=it->svt);

 if(it==it_fin) return false; // r n'est pas un rapporteur
 L_rapporteur.Retirer(it);
 return true;
}


/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
void alx_simulateur_clavier::simuler()
{
// Mise à jour des coordonnées.
 etat_clavier = SDL_GetKeyState(NULL);

 SDL_keysym ks;
 info_comp_evennement_dispositif_saisie comp;

// while( SDL_PollEvent( &event ) )
    {
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type ){
      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN:
       {
        comp.caractere_saisie = 0;
       }
      break;
      case SDL_KEYDOWN:case SDL_KEYUP:
        ks = event.key.keysym;
        if(event.type == SDL_KEYDOWN) comp.type_appuie = alx_dispositif_saisie_appuie;
         else comp.type_appuie = alx_dispositif_saisie_relache;

      // mettre à jour l'état connu du clavier
        //etat_clavier_prcdt[ks.scancode] = etat_clavier[ks.scancode];
        if(ks.unicode == 0)
         {
          switch(ks.sym)
           {
            case SDLK_DELETE:
              comp.est_un_caractere = true;
              comp.caractere_saisie = ks.sym;
            break;
            default:
              comp.est_un_caractere = false;
              comp.caractere_saisie = ks.sym;
           }
         }
         else
          {comp.est_un_caractere = true;
           comp.caractere_saisie = ks.unicode;}

      // Balancer la sauce
        distribuer_evennement( alx_evennement_dispositif_saisie( 0
                                                               , (alx_dispositif_saisie_type_evennement)comp.type_appuie
                                                               , comp) );
        break;
      default:
        break;
    }
  }
}


