if {![info exists noeud_partage]} {source groupe.tcl}
source menu_main.tcl
source ALX_Drawer.tcl
N_i_mere Afficher_souris

# Creation du logiciel de dessin
 set n_root [B_noeud]
   $n_root Position_des_fils_changeable 0
   $noeud_partage Ajouter_fils $n_root
 ALX_Drawer A_drawer
   $n_root Ajouter_fils [A_drawer get_canvas]

# On pose un polygone
 set poly_pour_main [B_polygone]
   $poly_pour_main Ajouter_contour [ProcRect 0 0 200 50]
   #$noeud_partage Ajouter_fils $poly_pour_main
   $n_root Ajouter_fils_au_debut $poly_pour_main
   $poly_pour_main Etirement 1.25 1.25
   B_contact ctc_poly_pour_main "$poly_pour_main 1"

# Génération des items du menu en lien avec le logiciel de dessin
 proc Complete_with {L_name menu multiple} {
   upvar $L_name L
   set nb [expr ($multiple - [llength $L]%$multiple) % $multiple]
   for {set i 0} {$i < $nb} {incr i} {lappend L $menu}
  }
  
 Menu_item menu_item_Draw_Void  "" "" "" [list]
 set L_items ""
 lappend L_items [Menu_item menu_item_Draw_FreeHand  "Free Hand" "A_drawer set_mode Drawing_mode; A_drawer set_drawing_mode FreeHand " "" [list]]
 lappend L_items [Menu_item menu_item_Draw_Rectangle "Rectangle" "A_drawer set_mode Drawing_mode; A_drawer set_drawing_mode Rectangle" "" [list]]
 lappend L_items [Menu_item menu_item_Draw_Oval      "Oval"      "A_drawer set_mode Drawing_mode; A_drawer set_drawing_mode Oval     " "" [list]]
 Complete_with L_items menu_item_Draw_Void 8
 foreach COL [list {White {1 1 1 1}} {Black {0 0 0 1}} {Red {1 0 0 1}} {Green {0 1 0 1}} {Blue {0 0 1 1}} {Yellow {1 1 0 1}} {Violet {1 0 1 1}} {Cyan {0 1 1 1}} ] {
   set col_name [lindex $COL 0]
   set col      [lindex $COL 1] 
   set m [Menu_item menu_item_Color_$col_name  "$col_name" "A_drawer set_mode Edit_mode; A_drawer set_current_color {$col}; A_drawer set_edit_mode Color " "" [list]]; 
   $m set_init_cmd "\$b_pol Couleur $col"
   lappend L_items $m
  }
 Complete_with L_items menu_item_Draw_Void 8
 lappend L_items [Menu_item menu_item_Edit_Copy   "Copy"   "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Copy"   "" [list]]
 lappend L_items [Menu_item menu_item_Edit_Paste  "Paste"  "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Paste"  "" [list]]
 lappend L_items [Menu_item menu_item_Edit_Delete "Delete" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Delete" "" [list]]
 Complete_with L_items menu_item_Draw_Void 8
 lappend L_items [Menu_item menu_item_Edit_GetTexture_1  "Set texture 1"  "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Get_set_texture_1" "" [list]]
 lappend L_items [Menu_item menu_item_Edit_EditTexture_1 "Edit texture 1" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Edit_texture_1" "" [list]]
 
   set L_ss_items ""
   lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_add            "Add"        "A_drawer set_combi_value [GL_add]" "" [list]]
   lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_sub            "Sub"        "A_drawer set_combi_value [GL_sub]" "" [list]]
   lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_add_signed_ARB "Add_signed" "A_drawer set_combi_value [GL_add_signed_ARB]" "" [list]]
  # lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_GL_decal "Decal" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Combi_texture; A_drawer set_combi_tex_value [GL_decal]" "" [list]]
  lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_GL_interpolate_ARB "Interpolate" "A_drawer set_combi_value [GL_interpolate_ARB]" "" [list]]
 #  lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_GL_combine_ARB "Combine"    "A_drawer set_combi_tex_value [GL_combine_ARB]" "" [list]]
   lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_GL_modulate    "Modulate"   "A_drawer set_combi_value [GL_modulate]" "" [list]]
   lappend L_ss_items [Menu_item menu_item_Edit_Combi_rgb_GL_replace     "Replace"    "A_drawer set_combi_value [GL_replace]" "" [list]]
   Complete_with L_ss_items menu_item_Draw_Void 8
 lappend L_items [Menu_item menu_item_Edit_Edit_COLOR_Interp_RGB   "Interpollation Texture 1 RGB"   "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Apply_Combi; A_drawer set_combi_mode couleurs; A_drawer set_combi_tex rgb"   "" $L_ss_items]
 lappend L_items [Menu_item menu_item_Edit_Edit_COLOR_Interp_Alpha "Interpollation Texture 1 Alpha" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Apply_Combi; A_drawer set_combi_mode couleurs; A_drawer set_combi_tex alpha" "" $L_ss_items]
 lappend L_items [Menu_item menu_item_Edit_GetTexture_2  "Set texture 2"  "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Get_set_texture_2" "" [list]]
 lappend L_items [Menu_item menu_item_Edit_EditTexture_2 "Edit texture 2" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Edit_texture_2" "" [list]]
 lappend L_items [Menu_item menu_item_Edit_Edit_ALPHA_Interp_RGB   "Interpollation Texture 2 RGB"   "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Apply_Combi; A_drawer set_combi_mode textures; A_drawer set_combi_tex rgb"   "" $L_ss_items]
 lappend L_items [Menu_item menu_item_Edit_Edit_ALPHA_Interp_Alpha "Interpollation Texture 2 Alpha" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Apply_Combi; A_drawer set_combi_mode textures; A_drawer set_combi_tex alpha" "" $L_ss_items]
 
     set L_ss_ss_items_sources ""
	 lappend L_ss_ss_items_sources  [Menu_item menu_item_Edit_SRC_GL_texture           "GL_texture"           "A_drawer set_SRC_mode [GLenum2UI [GL_texture]]"           "" ""]
	 lappend L_ss_ss_items_sources  [Menu_item menu_item_Edit_SRC_GL_constant_ARB      "GL_constant_ARB"      "A_drawer set_SRC_mode [GLenum2UI [GL_constant_ARB]]"      "" ""]
	 lappend L_ss_ss_items_sources  [Menu_item menu_item_Edit_SRC_GL_primary_color_ARB "GL_primary_color_ARB" "A_drawer set_SRC_mode [GLenum2UI [GL_primary_color_ARB]]" "" ""]
	 lappend L_ss_ss_items_sources  [Menu_item menu_item_Edit_SRC_GL_previous_ARB      "GL_previous_ARB"      "A_drawer set_SRC_mode [GLenum2UI [GL_previous_ARB]]"      "" ""]
     Complete_with L_ss_ss_items_sources menu_item_Draw_Void 8
	 set L_ss_ss_items_operande ""
	 lappend L_ss_ss_items_operande [Menu_item menu_item_Edit_OPE_GL_src_color         "GL_src_color"         "A_drawer set_OPE_mode [GLenum2UI [GL_src_color]]"         "" ""]
	 lappend L_ss_ss_items_operande [Menu_item menu_item_Edit_OPE_GL_1_moins_src_color "GL_1_moins_src_color" "A_drawer set_OPE_mode [GLenum2UI [GL_1_moins_src_color]]" "" ""]
	 lappend L_ss_ss_items_operande [Menu_item menu_item_Edit_OPE_GL_src_alpha         "GL_src_alpha"         "A_drawer set_OPE_mode [GLenum2UI [GL_src_alpha]]"         "" ""]
	 lappend L_ss_ss_items_operande [Menu_item menu_item_Edit_OPE_GL_1_moins_src_alpha "GL_1_moins_src_alpha" "A_drawer set_OPE_mode [GLenum2UI [GL_1_moins_src_alpha]]" "" ""]
	 Complete_with L_ss_ss_items_operande menu_item_Draw_Void 8  

   set L_ss_items ""
   lappend L_ss_items [Menu_item menu_item_Edit_const_combi_colors  "Constante colors"  "" "" [list]]
   lappend L_ss_items [Menu_item menu_item_Edit_const_combi_texture "Constante texture" "" "" [list]]
   Complete_with L_ss_items menu_item_Draw_Void 4
     set L_ss_ss_items_SRC_OPE ""
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_couleurs_source_arg   "Texture 1 RGB source"      "A_drawer set_alpha {}      ; A_drawer set_combi_mode couleurs" "" $L_ss_ss_items_sources ]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_couleurs_operande     "Texture 1 RGB operation"   "A_drawer set_alpha {}      ; A_drawer set_combi_mode couleurs" "" $L_ss_ss_items_operande]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_textures_source_arg   "Texture 2 RGB source"      "A_drawer set_alpha {}      ; A_drawer set_combi_mode textures" "" $L_ss_ss_items_sources ]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_textures_operande     "Texture 2 RGB operation"   "A_drawer set_alpha {}      ; A_drawer set_combi_mode textures" "" $L_ss_ss_items_operande]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_couleurs_source_arg_A "Texture 1 ALPHA source"    "A_drawer set_alpha {_Alpha}; A_drawer set_combi_mode couleurs" "" $L_ss_ss_items_sources ]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_couleurs_operande_A   "Texture 1 ALPHA operation" "A_drawer set_alpha {_Alpha}; A_drawer set_combi_mode couleurs" "" $L_ss_ss_items_operande]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_textures_source_arg_A "Texture 2 ALPHA source"    "A_drawer set_alpha {_Alpha}; A_drawer set_combi_mode textures" "" $L_ss_ss_items_sources ]
     lappend L_ss_ss_items_SRC_OPE [Menu_item menu_item_Edit_Combinaison_textures_operande_A   "Texture 2 ALPHA operation" "A_drawer set_alpha {_Alpha}; A_drawer set_combi_mode textures" "" $L_ss_ss_items_operande]
	 Complete_with L_ss_ss_items_SRC_OPE menu_item_Draw_Void 8
   lappend L_ss_items [Menu_item menu_item_Edit_Tex_ARG_num_0 "Argument 0" "A_drawer set_Tex_ARG_num 0" "" $L_ss_ss_items_SRC_OPE]
   lappend L_ss_items [Menu_item menu_item_Edit_Tex_ARG_num_1 "Argument 1" "A_drawer set_Tex_ARG_num 1" "" $L_ss_ss_items_SRC_OPE]
   lappend L_ss_items [Menu_item menu_item_Edit_Tex_ARG_num_2 "Argument 2" "A_drawer set_Tex_ARG_num 2" "" $L_ss_ss_items_SRC_OPE]   
   Complete_with L_ss_items menu_item_Draw_Void 8
 
 Complete_with L_items menu_item_Draw_Void 8

# Menu operation and more
 lappend L_items [Menu_item menu_item_Edit_Select_for_op1 "Select for operande 1" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode _Un_Select_for_Operande_1" "" ""]
 lappend L_items [Menu_item menu_item_Apply_oper_on_nodes "Apply operation" "A_drawer Apply_operation_on_nodes" "" ""]
 lappend L_items [Menu_item menu_item_Apply_unselect_op_1 "Unselect Operande 1" "A_drawer Unselect_Operande_N 1" "" ""]
 lappend L_items [Menu_item menu_item_Edit_Edit_More "More" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode Update_texture" "" $L_ss_items]
 Complete_with L_items menu_item_Draw_Void 4
 lappend L_items [Menu_item menu_item_Edit_Select_for_op2 "Select for operande 2" "A_drawer set_mode Edit_mode; A_drawer set_edit_mode _Un_Select_for_Operande_2" "" ""]
   set L_ss_items_nodes_operations ""
   lappend L_ss_items_nodes_operations [Menu_item menu_item_Op_node_Union "Union"        "A_drawer set_node_operation Union"        "" ""]
   lappend L_ss_items_nodes_operations [Menu_item menu_item_Op_node_Inter "Intersection" "A_drawer set_node_operation Intersection" "" ""]
   Complete_with L_ss_items_nodes_operations menu_item_Draw_Void 4
   lappend L_ss_items_nodes_operations [Menu_item menu_item_Op_node_Diff  "Difference"   "A_drawer set_node_operation Difference"   "" ""]
   lappend L_ss_items_nodes_operations [Menu_item menu_item_Op_node_XOR   "XOR"          "A_drawer set_node_operation XOR"          "" ""]
   Complete_with L_ss_items_nodes_operations menu_item_Draw_Void 8
 lappend L_items [Menu_item menu_item_Select_oper_on_nodes "Select operation (\[A_drawer get_node_operation\])" "" "" $L_ss_items_nodes_operations]
 lappend L_items [Menu_item menu_item_Apply_unselect_op_2 "Unselect Operande 2" "A_drawer Unselect_Operande_N 2" "" ""]
 lappend L_items [Menu_item menu_item_Save           "Save" "A_drawer Save_drawings" "" ""]
  
 Complete_with L_items menu_item_Draw_Void 8
   
 puts "L_items : {$L_items}"
 Menu_item menu_item_root "MenuRoot" "" "Menu root" $L_items
 
 #set L_SubMenus [list ]
 #for {set i 1} {$i <= 64} {incr i} {
 #  Menu_item menu_item_$i "SubMenu_$i" "puts $i" "Sub menu $i" [list]
 #  lappend L_SubMenus menu_item_$i
 # }
 #
 #Menu_item menu_item_root "MenuRoot" "puts MenuRoot" "Menu root" $L_SubMenus
 
# Génération du menu
Automate_Menu_Main automate_main $poly_pour_main menu_item_root
  automate_main Load_polygone ./menus/menu_main_8.poly

if {![gmlObject info exists object Liant_mutable]} {
  source test_mutable.tcl
  Liant_Mutable_sim_ptr Liant_mutable; Liant_mutable startListening 8910
  #source sim_mutable.tcl
  #Sim_MuTable sim_table localhost 8910
  #sim_table Send_infos 20
 }
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
source Pointer_catcher.tcl

#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
#___________________________________________________________________________________________________________________________________________
$poly_pour_main Vider
Pointer_catcher pointeur_catcher $noeud_fen $poly_pour_main automate_main
pointeur_catcher set_area_min 32
