source x.tcl
B_configure $canvas -Gerer_contacts 0 -Afficher_noeud 0
source B207_Helper.tcl
B207_Helper helper
helper Plug_under $noeud_partage
source Helpers/Helpers_utils.tcl
Load_helper_in Helpers/alx_noeud_fiche_sdl_opengl.B207_helper $output_window
$output_window Nom_IU "Current task window"

set Manip_root [helper get_MANIP_root_for_hands]
$Noeud_scene Ajouter_fils_au_debut $Manip_root

set P [helper get_MANIP_poly_L_hand]
puts "set cc \[$P Serialisation_differences_type\]"

#$P Serialiser_difference_type
