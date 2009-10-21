#ifndef __ALX_CAMNOTE_H__
#define __ALX_CAMNOTE_H__

// Route 66 nous voici!
#include <winsock.h>

//#include "..\interfaces\interface.h"
class alx_interface_sdl_opengl_1;
class Route66;

/*
#include "sdl_opengl\alx_noeud_fiche_ovale_sdl_opengl.h"
#include "sdl_opengl\alx_noeud_bouton_sdl_opengl.h"
#include "sdl_opengl\alx_noeud_image_sdl_opengl.h"
#include "sdl_opengl\alx_noeud_zone_texte_sdl_opengl.h"
#include "sdl_opengl\alx_noeud_visualisation_camera.h"
#include "sdl_opengl\alx_noeud_transformations_sdl_opengl.h"
*/
#include <memory.h>
//#include "tcputil.h"
//#include "..\..\src\SDL_thread.h"

#include "alx_visu_image_camnote_cpp_bigre.h"
#include "alx_telec_camnote_cpp_bigre.h"
#include "..\Comet Olfa\Creation_objet_dans_tcl.h"

class CamNote : public alx_classe_base_liant, public olfa_comet, public alx_noeud_scene
{
 private:
   //SDL_Thread *thread_serveur;
   Uint16 port_telecommande;
   Route66 *route66;

/*
   inline void rationnaliser_fiche(const int *infos)
    {
     if(infos[0] == 0)
      {
       (this->*(T_rationnalisation_fiche[infos[0]][infos[1]]))(infos);
       return;
      }
     if(infos[0] == 1)
      {
       interface_mere->Sim_dispo_saisi()->Prendre_evennements_lies_a( (alx_point2D*)(void*)(infos[2])
                                                                     , (alx_classe_base_liant*)this
                                                                     , 4 );
      }

    }
*/
   alx_chaine_char nom_presentation; unsigned int num_transparent;
   alx_interface_sdl_opengl_1 *interface_mere;
   alx_telec_camnote_cpp_bigre      *telecommande;
   alx_visu_image_camnote_cpp_bigre *visualisateur;

   bool Telec_translucide, Telec_invisible;
   double Translucidite_telec, temps_transition;
   double translucidite_deb, translucidite_fin;
   alx_repere2D repere_svg_telec;
   deformation_dynamique_scalaire<double>  *deformation_telec_translucidite;
   deformation_dynamique_repere_et_dim     *deformation_telec_pos_et_dim;
   deformation_dynamique_etirement_fenetre *deformation_telec;

   //cogitant::Environment *env_GC;
   alx_interpreteur_tcl  *interp_tcl;

   void Rationnaliser_nom_racine_trans(void *param);
   void Rationnaliser_num_trans(void *param);
   void Rationnaliser_intensite_pix_mirroir(void *param);
   void Rationnaliser_activation_pix_mirroir(void *param);

   void Rationnaliser_bouton_transfo(void *param);
   void Rationnaliser_bouton_retour(void *param);

   void Reception_message_telec             (unsigned int argc, char **argv);
   void Reception_message_telec_pix_mirroirs(unsigned int argc, char **argv);
   void Demande_reemission                  (unsigned int argc, char **argv);
   alx_noeud_bouton_sdl_opengl        *bouton_transfo, *bouton_retour;
   unsigned int nouveau_num_transparent[2];
   float nouvelle_translucidite;
   bool info_pix_mirroirs, info_transparents, nouvelle_activation_pix_mirroirs;
   alx_noeud_visualisation_camera *visu_cam;

 public :
   bool mise_a_jour_a_faire;

   CamNote( alx_interface_sdl_opengl_1 *interface_m, alx_noeud_visualisation_camera *cam
          , alx_interpreteur_tcl  *interp );
   ~CamNote();

  // Les projections
   alx_classe_base_liant* Classe_base_liant() {return (alx_classe_base_liant*)this;}
   olfa_comet*            Olfa_comet()        {return (olfa_comet*)this;}
   alx_noeud_scene*       Noeud_scene()       {return (alx_noeud_scene*)this;}

  // Les méthodes
   void Nom_presentation(const alx_chaine_char n) {nom_presentation = n;}
   inline const alx_chaine_char& Nom_presentation() const {return nom_presentation;}
   void Num_transparent(const unsigned int n) {num_transparent = n;}
   const unsigned int Num_transparent() const {return num_transparent;}
   void Demarrer();
   void Quitter();

   void Emission_66(const char *m);

  // Les surcharges
   void rationnaliser(int num);
   void Afficher();
   void PreRendre();
};

typedef CamNote* P_CamNote;

#endif
