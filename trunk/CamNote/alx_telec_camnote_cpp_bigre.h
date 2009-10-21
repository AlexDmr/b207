#ifndef __TELECOMMANDE_CAMNOTE_CPP_BIGRE_H__
#define __TELECOMMANDE_CAMNOTE_CPP_BIGRE_H__

#include "..\interfaces\interface.h"
#include "alx_telec_camnote_cpp.h"
#include "..\Comet Olfa\Creation_objet_dans_tcl.h"

const int connection_local  = 1;
const int connection_reseau = 2;
class alx_telec_camnote_cpp_bigre : public alx_telec_camnote_cpp, public olfa_comet, public alx_noeud_fiche_sdl_opengl
{
 private:
  // Optimisation :
   alx_point2D pt_tmp;
  // Le reste
   alx_noeud_fiche_sdl_opengl         *fiche_num_diapo;
   alx_zone_texte_opengl              *z_texte, *z_texte_nom_diapo, *z_texte_numero;
   alx_noeud_zone_texte_sdl_opengl    *zone_texte, *zone_texte_nom_diapo, *zone_texte_numero;
   alx_interface_sdl_opengl_1         *interface_mere;
   alx_noeud_potentiometre_sdl_opengl *potentiometre_translucidite;
   alx_noeud_bouton_sdl_opengl        *bouton_svt, *bouton_prcdt, *bouton_aller_a, *bouton_pix_mirroir;
   alx_noeud_scene                     translation_visu_cam;
   alx_noeud_scene                     etirement_telec;
   alx_noeud_image_sdl_opengl         *visu_cam;
   INFOS_TEXTURE                       texture_camera;

   void Rationnaliser_activation_pixels_mirroirs(void *param);
   void Rationnaliser_translucidite_pixels_mirroirs(void *param);
   void Rationnaliser_bouton_prcdt(void *param);
   void Rationnaliser_bouton_svt(void *param);
   void Rationnaliser_bouton_aller_a(void *param);
   void Rationnaliser_clavier(void *param);

   alx_methode_rappel *fct_rappel_clavier;

 public:
   alx_telec_camnote_cpp_bigre( char *nom
                            //, cogitant::Environment *e
                            , alx_interpreteur_tcl  *interp
                            , alx_interface_sdl_opengl_1 *i_mere
                            , alx_noeud_scene *n_pere
                            , const INFOS_TEXTURE *text_cam
                            , const unsigned int nb_octets_par_pix = 4
                            , const int ordre_couleur = GL_BGRA );

   inline const INFOS_TEXTURE& Texture_camera() const {return texture_camera;}
   inline void Maj_bt_prcdt()
    {bouton_prcdt->Mettre_a_jour();}

   inline void Mettre_a_jour()
    {//potentiometre_translucidite->Mettre_a_jour();
     alx_noeud_fiche_sdl_opengl::Mettre_a_jour();}
   virtual void Numero_transparent(const unsigned int num);
   const unsigned int Numero_transparent() const {return alx_telec_cpp::Numero_transparent();}
   inline void Maj_nom_de_transparent() {Nom_de_transparent( z_texte_nom_diapo->Chaine_char() );}

   inline alx_noeud_fiche_sdl_opengl* Noeud_fenetre_sdl_opengl() {return (alx_noeud_fiche_sdl_opengl*)this;}
   inline virtual const float Intensite_pixels_miroirs() const {return alx_telec_camnote_cpp::Intensite_pixels_miroirs();}
   inline virtual void Intensite_pixels_miroirs(const float i)
    {if( alx_telec_camnote_cpp::Intensite_pixels_miroirs() == i) return;
     alx_telec_camnote_cpp::Intensite_pixels_miroirs(i);
     potentiometre_translucidite->Valeur( alx_telec_camnote_cpp::Intensite_pixels_miroirs()*potentiometre_translucidite->Valeur_max() );
    }

  // Les fonctions pour le pilotage au clavier
   inline void Desactiver_pilotage_clavier()
    {interface_mere->Sim_dispo_saisi()->desabonner_a_tout_evennement_courant( fct_rappel_clavier );}
   inline void Activer_pilotage_clavier()
    {interface_mere->Sim_dispo_saisi()->abonner_a_tout_evennement_courant( fct_rappel_clavier );}

  //
  // Les surcharges de la classe alx_noeud_scene
   //virtual info_du_contenant* Contient(alx_point2D &pt, int action);
   virtual void Afficher();

  // Surcharges du changement de nom des transparents
   inline void Nom_de_transparent(const alx_chaine_char &n) {zone_texte_nom_diapo->Maj_texte(n); alx_telec_camnote_cpp::Nom_de_transparent(n);}
   inline const alx_chaine_char& Nom_de_transparent()       {Nom_de_transparent(zone_texte_nom_diapo->Chaine_char()); return alx_telec_camnote_cpp::Nom_de_transparent();}
};

typedef alx_telec_camnote_cpp_bigre* P_alx_telec_camnote_cpp_bigre;

#endif
