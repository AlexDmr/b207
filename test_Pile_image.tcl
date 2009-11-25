source x_server.tcl
$noeud_partage Vider_fils
source B207_Pile_image.tcl
  set L_image_names [glob "wmplayer 2008-07-08*"]
  Pile_image pile Resize [llength $L_image_names]
  set pos 0; foreach img [pile get_L_images] {$img maj [lindex $L_image_names $pos]; $img Calculer_boites; incr pos}
    [pile get_root] Calculer_boites
  $noeud_partage Ajouter_fils [pile get_root]
  
 source sim_mutable.tcl
   Sim_MuTable sim_table localhost 8910
   #sim_table Send_infos 20
   
 source test_mutable.tcl
   Liant_Mutable_sim_ptr Liant_mutable; Liant_mutable startListening 8910
   
 B_contact ctc_pile "[pile get_root] 0"
   ctc_pile add "[lindex [pile get_L_images] 0] 11"

 source Helpers/Helpers_utils.tcl
 source B207_Helper.tcl
   Load_helper_in Helpers/alx_noeud_image_sdl_opengl.B207_helper [lindex [pile get_L_images] 0]
   [lindex [pile get_L_images] 0] Nom_IU "Pile of images"
   B207_Helper helper
   helper Plug_under $Noeud_scene
   
  
 Load_helper_in Helpers/alx_noeud_fiche_sdl_opengl.B207_helper $output_window
 $output_window Nom_IU "Current task window"
 
 proc #text args ""

 