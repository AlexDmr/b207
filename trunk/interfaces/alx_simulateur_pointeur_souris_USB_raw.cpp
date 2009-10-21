#include "alx_simulateur_pointeur_souris_USB_raw.h"
#include <stdio.h>

/*******************************************************************************/
alx_simulateur_pointeur_souris_USB_raw::alx_simulateur_pointeur_souris_USB_raw( int *e_x, int *e_y
                                                                , alx_point2D *bg
                                                                , alx_vecteur2D *vh
                                                                , alx_vecteur2D *vv) : alx_simulateur_pointeurs_generique( e_x, e_y
                                                                                                                         , bg
                                                                                                                         , vh
                                                                                                                         , vv )
                                                                                     , derniere_pos(*e_x/2.0, *e_y/2.0)
{available_mice = 0;
 need_scan = false;
 for(unsigned int i=0; i<NB_MAX_MOUSE; i++)
  {for(unsigned int j=0; j<T_ETAT_MOUSE; j++)
    {tab_etats[i][j] = 0;}
  }
}

/*******************************************************************************/
void alx_simulateur_pointeur_souris_USB_raw::simuler()
{ManyMouseEvent event_mm;

 //printf("alx_simulateur_pointeur_souris_USB_raw::simuler\n");
 while(ManyMouse_PollEvent(&event_mm))
  {//if (event_mm.device >= (unsigned int) available_mice)
   //  continue;
   switch (event_mm.type)
    {case MANYMOUSE_EVENT_RELMOTION:
       //printf("Mouse #%u relative motion %s %d\n", event_mm.device, event_mm.item == 0 ? "X" : "Y", event_mm.value);
       unsigned int dx, dy;
       if(event_mm.item == 0) {dx = event_mm.value;
                               dy = 0;
                              } else {dx = 0;
                                      dy = event_mm.value;
                                     }

       tab_etats[event_mm.device][0] += dx;
       tab_etats[event_mm.device][1] += dy;

       // Le déplacement est dans (xrel, yrel).
       //derniere_pos.Translation(event.motion.xrel, event.motion.yrel);

       convertir_coordonnees(dx, dy, pt_dep);
       convertir_coordonnees(tab_etats[event_mm.device][0], tab_etats[event_mm.device][1], pt_arrive);

       A_ic_tmp.coord.x = pt_arrive.x;
       A_ic_tmp.coord.y = pt_arrive.y;
       A_ic_tmp.dep.x = pt_dep.x;
       A_ic_tmp.dep.y = pt_dep.y;
       A_ic_tmp.type_appuie = 0;

       // Balancer la sauce
       A_ep_tmp.maj(event_mm.device, alx_pointeur_mouvement, A_ic_tmp, Noeud_pere());
       distribuer_evennement( A_ep_tmp ); //alx_evennement_pointeur(0, alx_pointeur_mouvement, comp) );

     break;
     case MANYMOUSE_EVENT_ABSMOTION:
       printf("Mouse #%u absolute motion %s %d\n", event_mm.device, event_mm.item == 0 ? "X" : "Y", event_mm.value);
     break;
     case MANYMOUSE_EVENT_BUTTON:
       alx_pointeur_type_evennement t_evt;
       need_scan = true;
       //printf("Mouse #%u button %u %s\n", event_mm.device, event_mm.item, event_mm.value ? "down" : "up");
       int val;
       if(event_mm.value) {t_evt = alx_pointeur_enfonce;
                           tab_etats[event_mm.device][2] |= 1 << event_mm.item;
                           val = tab_etats[event_mm.device][2];
                          } else {t_evt = alx_pointeur_relache;
                                  val = tab_etats[event_mm.device][2];
                                  tab_etats[event_mm.device][2] &= ~(1 << event_mm.item);
                                 }

       if(event_mm.item == 2 || ((val & 3) == 3) )
        {convertir_coordonnees(*ecran_x/2.0, *ecran_y/2.0, pt_tmp);
         A_ic_tmp.coord.x = pt_tmp.x;
         A_ic_tmp.coord.y = pt_tmp.y;
         A_ic_tmp.dep.x = A_ic_tmp.coord.x - tab_etats[event_mm.device][0];
         A_ic_tmp.dep.y = A_ic_tmp.coord.y - tab_etats[event_mm.device][1];
         tab_etats[event_mm.device][0] = *ecran_x/2.0;
         tab_etats[event_mm.device][1] = *ecran_y/2.0;
        // Balancer la sauce
         A_ep_tmp.maj(event_mm.device, alx_pointeur_mouvement, A_ic_tmp, Noeud_pere());
         distribuer_evennement( A_ep_tmp );
         break;
        }
      //info_comp_evennement comp; // Les informations complémentaires.
       A_ic_tmp.type_appuie = event_mm.item;
       // Ne pas oublier de localiser le clique, on considère que le clique à eut
       // lieu avant un déplacement éventuel :
       //alx_point2D pt_clique;
       convertir_coordonnees(tab_etats[event_mm.device][0], tab_etats[event_mm.device][1], pt_clique);
       A_ic_tmp.coord.x = pt_clique.x;
       A_ic_tmp.coord.y = pt_clique.y;

      // Balancer la sauce
       A_ep_tmp.maj(event_mm.device, t_evt, A_ic_tmp, Noeud_pere());
       distribuer_evennement( A_ep_tmp ); //alx_evennement_pointeur(0, t_evt, comp) );
     break;
     case MANYMOUSE_EVENT_SCROLL:
     break;
    }
  }
}
