#ifndef __ALX_PRINCIPAL_H__
#define __ALX_PRINCIPAL_H__

#include "..\interfaces\sdl_opengl\alx_interface_sdl_opengl_1.h"
#include "..\CamNote\CamNote.h"

#include <windows.h>

#include <stdio.h>
#include <SDL.h>

#include "alx_image.h"
#include "texte.h"
#include "config_opengl.h"

#include "..\physique\alx_geometrie2D.h"

#include "..\interfaces\sdl_opengl\alx_interface_sdl_opengl_1.h"
//#include "..\Jeu casse bille\alx_jeu_casse_bille.h"
#include "..\Comet Olfa\olfa_comet.h"
#include <tcl.h>


class Route66;
class principal
{private:
   SDL_Surface *ecran;
   Route66 *route66;
   int ecran_x, ecran_y;
   alx_interface_sdl_opengl_1 *interface_1;

   alx_chaine_char cc_nom_exec;
 //  alx_jeu_casse_bille        *jeu_casse_brique;
 //  alx_simulateur_choc_2D     *sim_choc2D;

   alx_image_32 *img;
   //gl_fonte *fonte_arial;
   double angle_camera;

   //cogitant::Environment env_GC;
   alx_interpreteur_tcl *environnement_tcl;

   void Initialisation();
/*
   int pointeur_x, pointeur_y;
   int id_souris;

   alx_point2D coin_bg;
   alx_vecteur2D vect_h, vect_v;

   alx_simulateur_pointeurs_souris *sim_souris;
   alx_simulateur_pointeurs        *sim_pointeurs;
   alx_simulateur_couche           *sim_couche;
   alx_liant_ptr_contact           *liant_ptr_contact;
   alx_simulateur_points_contact   *sim_contact; */

 public :
   principal(const char *nom_exec);
   principal(const char *nom_exec, alx_interpreteur_tcl *environnement_tcl);
   ~principal();

   int demarrer( Uint32 propriete
               , unsigned int tx = 1024
               , unsigned int ty = 768);

  // Méthodes d'accès:
   inline alx_interface_sdl_opengl_1* Interface_BIGre()   {return interface_1;}
   inline Route66*                    Route_66()          {return route66;}
   inline alx_interpreteur_tcl*       Environnement_TCL() {return environnement_tcl;}

};

#endif
