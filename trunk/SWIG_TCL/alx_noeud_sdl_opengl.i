/* File : example.i */
%module alx_noeud_sdl_opengl

#include "C:\FMOD\api\inc\fmod.h"

%{
#include "C:\FMOD\api\inc\fmod.h"
#include "..\\opengl\\config_opengl.h"
#include "..\\utilitaires\\alx_chaine_char.h"
#include "..\\utilitaires\\alx_arbre.h"
#include "..\\utilitaires\\MetaData.h"
#include "..\\opengl\\alx_image.h"
#include "..\\opengl\\alx_image_opengl.h"
#include "..\\interfaces\\alx_noeud_scene.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_fenetre_sdl_opengl.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_sdl_opengl.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_zoom_sdl_opengl.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_ovale_sdl_opengl.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_ovale_zoom_sdl_opengl.h"
#include "..\\Comet Olfa\\olfa_comet.h"
#include "..\\Comet Olfa\\Creation_objet_dans_tcl.h"
#include "..\\tcl\\alx_interpreteur_tcl.h"
#include "..\\interfaces\\sdl_opengl\\alx_interface_sdl_opengl_1.h"
#include "..\\Route 66\\route66\\includes\\route66.h"
#include "..\\horloge\\alx_horloge_vectorielle.h"
#include "..\\tcl\\alx_interpreteur_tcl.h"
#include "..\\physique\\alx_simulateur_points_contact.h"
#include "..\\physique\\alx_repere2D_lentille.h"
#include "..\\interfaces\\fontes.h"
#include "..\\interfaces\\choses_communes.h"
#include "..\\interfaces\\alx_simulateur_pointeurs.h"
#include "..\\interfaces\\alx_simulateur_pointeurs_generique.h"
#include "..\\interfaces\\alx_noeud_pointeur.h"
#include "..\\interfaces\\alx_simulateur_couche.h"
#include "..\\interfaces\\alx_evennement_pointeur.h"
#include "..\\interfaces\\alx_evennement_dispositif_saisie.h"
#include "..\\interfaces\\alx_simulateur_dispositif_saisie.h"
#include "..\\interfaces\\alx_simulateur_pointeur_souris_USB_raw.h"
#include "..\\interfaces\\alx_simulateur_pointeurs_generique.h"
#include "..\\interfaces\\alx_simulateur_pointeurs_TCL.h"
#include "..\\interfaces\\alx_simulateur_pointeurs_souris.h"

#include "..\\interfaces\\sdl_opengl\\alx_noeud_repere_lentille.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_polygone_sdl_opengl.h"
#include "..\\interfaces\\sdl_opengl\\alx_noeud_potentiometre_sdl_opengl.h"
#include "..\\utilitaires\\alx_liste.h"
#include "..\\interfaces\\sdl_opengl\\tableau_de_double.h"

#include "..\\physique\\alx_methode_rappel.h"
#include "..\\Route 66\\slimlib\\slimcore\\includes\\slimsocket.h"
#include "..\\utilitaires\\Server_TCP_TCL.h"

//#include "..\\..\\Video\\FFMPEG_for_TCL\\ffmpeg_for_tcl\\infos_audio.h"

//#include "C:\FMOD\api\inc\fmod.h"
%}

/* Let's just grab the original header file here */
%include "C:\FMOD\api\inc\fmod.h"

%include "..\\opengl\\config_opengl.h"
  %template(Couleur_RVBAf) Couleur_RVBA<float>;
  %template(Couleur_RVBAd) Couleur_RVBA<double>;
%include "..\\interfaces\\fontes.h"
%include "..\\interfaces\\choses_communes.h"
%include "..\\horloge\\alx_horloge_vectorielle.h"
%include "..\\Route 66\\route66\\includes\\route66.h"
%include "..\\tcl\\alx_interpreteur_tcl.h"
%include "..\\utilitaires\\alx_arbre.h"
  %template(noeud) alx_arbre<alx_noeud_scene*>;
%include "..\\utilitaires\\alx_chaine_char.h"
%include "..\\physique\\alx_point2D.h"
%include "..\\physique\\alx_vecteur2D.h"
%include "..\\physique\\alx_repere2D.h"
%include "..\\physique\\alx_droite2D.h"
%include "..\\physique\\alx_segment2D.h"
%include "..\\physique\\alx_cercle2D.h"
%include "..\\physique\\alx_ellipse2D.h"
%include "..\\physique\\alx_classe_base_modele.h"
%include "..\\physique\\alx_classe_base_liant.h"
%include "..\\interfaces\\alx_noeud_scene.h"
%include "..\\interfaces\\alx_simulateur_couche.h"
%include "..\\interfaces\\alx_evennement_pointeur.h"
%include "..\\interfaces\\alx_evennement_dispositif_saisie.h"
%include "..\\interfaces\\alx_simulateur_dispositif_saisie.h"
%include "..\\interfaces\\alx_simulateur_pointeurs_generique.h"
%include "..\\interfaces\\alx_simulateur_pointeurs_souris.h"
%include "..\\interfaces\\alx_simulateur_pointeurs_TCL.h"

%include "..\\utilitaires\\MetaData.h"
%include "..\\opengl\\alx_image.h"
%include "..\\opengl\\alx_image_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_interface_sdl_opengl_1.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_bouton_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_editeur_olfa_comet.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_entete_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_fenetre_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_ovale_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_zoom_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_fiche_ovale_zoom_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_image_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_polygone_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_potentiometre_sdl_opengl.h"
%include "..\\tcl\\alx_interpreteur_tcl.h"
%include "..\\interfaces\\alx_evennement_pointeur.h"
%include "..\\interfaces\\alx_simulateur_couche.h"

/* %include "..\\interfaces\\sdl_opengl\\alx_noeud_transformations_sdl_opengl.h" */

%include "..\\interfaces\\sdl_opengl\\alx_noeud_visualisation_camera.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_zone_rendu_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_zone_texte_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_zone_texte_interpreteur_tcl_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_prerendeur.h"
%include "..\\interfaces\\sdl_opengl\\deformations_dynamiques.h"
  %template(deformation_dynamique_int)    deformation_dynamique_scalaire<int>;
  %template(deformation_dynamique_uint)   deformation_dynamique_scalaire<unsigned int>;
  %template(deformation_dynamique_double) deformation_dynamique_scalaire<double>;

%include "..\\interfaces\\classiques\\alx_fenetre.h"
%include "..\\interfaces\\classiques\\alx_fenetre_opengl.h"
%include "..\\interfaces\\classiques\\alx_fiche.h"
%include "..\\interfaces\\classiques\\alx_fiche_opengl.h"
%include "..\\interfaces\\classiques\\alx_fiche_ovale.h"
%include "..\\interfaces\\classiques\\alx_fiche_ovale_opengl.h"
%include "..\\interfaces\\classiques\\alx_zone_texte.h"
%include "..\\interfaces\\classiques\\alx_zone_texte_opengl.h"

%include "..\\physique\\alx_simulateur_points_contact.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_potentiometre_sdl_opengl.h"

%include "..\\Comet Olfa\\olfa_comet.h"
%include "..\\Comet Olfa\\Creation_objet_dans_tcl.h"
%include "..\\CamNote\\alx_telec_camnote_cpp.h"
%include "..\\CamNote\\alx_visu_image_camnote_cpp.h"
%include "..\\CamNote\\alx_telec_camnote_cpp_bigre.h"
%include "..\\CamNote\\alx_visu_image_camnote_cpp_bigre.h"
%include "..\\CamNote\\CamNote.h"
%include "..\\interfaces\\alx_simulateur_pointeurs.h"
%include "..\\interfaces\\alx_simulateur_pointeurs_generique.h"
%include "..\\interfaces\\alx_simulateur_pointeurs_TCL.h"
%include "..\\interfaces\\alx_simulateur_pointeur_souris_USB_raw.h"
%include "..\\interfaces\\alx_noeud_pointeur.h"
%include "..\\physique\\alx_repere2D_lentille.h"
%include "..\\interfaces\\sdl_opengl\\tableau_de_double.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_polygone_sdl_opengl.h"
%include "..\\interfaces\\sdl_opengl\\alx_noeud_repere_lentille.h"

%include "..\\Route 66\\slimlib\\slimcore\\includes\\slimsocket.h"
%include "..\\utilitaires\\Server_TCP_TCL.h"
%include "..\\physique\\alx_methode_rappel.h"

//%include "..\\..\\Video\\FFMPEG_for_TCL\\ffmpeg_for_tcl\\infos_audio.h"

//%include "C:\FMOD\api\inc\fmod.h"

%include "..\\utilitaires\\alx_liste.h"
  %template(Liste_info_du_contenant) alx_liste<info_du_contenant*>;
  %template(Liste_alx_repere2D)      alx_liste<alx_repere2D*>;
  %template(Liste_alx_noeud_scene)   alx_liste<alx_noeud_scene*>;
  %template(Liste_Liste_de_rappel)   alx_liste<Liste_de_rappel*>;

