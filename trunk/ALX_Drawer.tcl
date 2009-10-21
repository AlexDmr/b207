if {![info exists noeud_partage] && ![info exists img_fond]} {source groupe.tcl}
proc etire_fond args {}

  $f_obs Vider_peres
  N_i_mere Afficher_souris

#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method ALX_Drawer constructor {} {
# Shape management
 set this(L_polygones)    ""
 set this(Pool_polygones) ""

# Pointers management
 set this(Pool_Liste_alx_repere2D) ""
 set this(L_Liste_alx_repere2D)    ""
 set this(pt_tmp)                  [B_point]
 set this(Pool_B_rappel)           ""
 
# Modes
 set this(mode)            "Drawing_mode"
 set this(edit_mode)       ""
 set this(drawing_mode)    "FreeHand"
 set this(operation_mode)  "Add_to_canvas"
 set this(current_color)   "1 1 1 1"
 set this(combi_mode)      "couleurs"
 set this(Tex_ARG_num)     0
 set this(OPE_mode)        0
 set this(SRC_mode)        0
 set this(alpha)           ""
 set this(combi_tex)       "rgb"
 set this(combi_value)     [GL_modulate]
 set this(current_image_1) "chat_souris.png"
 set this(current_image_2) "reflet_transparent.png" 

 set this(clipboard)       ""
 
# Operations (Union, Intersection, difference, XOR)
 set this(L_nodes_Operande_1) [list]
 set this(L_nodes_Operande_2) [list]
 set this(node_operation)     ""

# Canvas
 set this(canvas) [B_polygone]
   $this(canvas) Ajouter_contour    [ProcRect 0 0 1024 730]
   $this(canvas) Ajouter_MetaData_T Draw_in_$objName 1
   $this(canvas) Couleur            0 0 0 0.3
   $this(canvas) Position_des_fils_changeable 0
   
# CallBacks to code the automate
 set this(rap_PRESS) [B_rappel [Interp_TCL]]
   $this(rap_PRESS) Texte "$objName Start_Drawing \[$objName get_info_from_rap $this(rap_PRESS)\]"
   $this(canvas) abonner_a_detection_pointeur [$this(rap_PRESS) Rappel] [ALX_pointeur_enfonce]
 
# Callback for texture mapping
 set this(rap_mapping) [B_rappel [Interp_TCL]]
   $this(rap_mapping) Texte "if {\[catch \{$objName Remap_texture $this(rap_mapping)\} err\]} {puts \"Remap error:\n\$err\"}"
   
# Callbacks for the taptap technique
  set this(rap_taptap) [B_rappel [Interp_TCL]]
    $this(rap_taptap) Texte "if {\[catch \{$objName Event_related_to_taptap $this(rap_taptap)\} err\]} {puts \"TapTap error:\n\$err\"}"
}

#_____________________________________________________________________________________________________
method ALX_Drawer dispose {} {
 foreach p $this(L_polygones)    {Detruire $p}
 foreach p $this(Pool_polygones) {Detruire $p}
 foreach l $this(Pool_Liste_alx_repere2D) {Detruire $l}
 foreach l $this(L_Liste_alx_repere2D)    {Detruire $l}
 
 Detruire $this(canvas)
 Detruire $this(pt_tmp)
}

#_____________________________________________________________________________________________________
Generate_accessors ALX_Drawer [list canvas L_nodes_Operande_1 L_nodes_Operande_2 node_operation alpha Tex_ARG_num OPE_mode SRC_mode combi_mode drawing_mode edit_mode mode operation_mode current_color combi_tex combi_value]

#_____________________________________________________________________________________________________
method ALX_Drawer set_mode {m} {
 set this(mode) $m
 if {$m != "Drawing_mode"} {set b 1} else {set b 0}
 puts "this(L_polygones) = {$this(L_polygones)}"
 foreach p $this(L_polygones) {$p Gerer_contacts $b}
}

#_____________________________________________________________________________________________________
method ALX_Drawer Update_texture {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != "" && $node != "NULL"} {
   $node Combinaison_$this(combi_mode)_operande$this(Tex_ARG_num)$this(alpha)   $this(OPE_mode)
   $node Combinaison_$this(combi_mode)_source_arg$this(Tex_ARG_num)$this(alpha) $this(SRC_mode)
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Save_drawings {} {
 set txt ""
 foreach p [Lister_fils_de $this(canvas)] {
   set cc [$p Serialiser_type] 
   append txt [$cc Texte] "\n"
  }
 set t [clock seconds]
 set f [open ${objName}_${t}.B207_DRAWING a]
   puts $f $txt
 close $f
}

#_____________________________________________________________________________________________________
method ALX_Drawer get_info_from_rap {rap} {
 set p [$rap Param]
 return [Void_vers_info $p]
}

#_____________________________________________________________________________________________________
method ALX_Drawer Start_Drawing {infos} {
 set node [Real_class [$infos NOEUD]]
 if {[$node Val_MetaData Draw_in_$objName] == ""} {return}
 if {[catch "$objName $this(mode) $infos" err]} {
   puts "ERROR in mode (method Start_Drawing) :\n$err"
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Edit_mode {infos} {
 if {[catch "$objName $this(edit_mode) $infos" err]} {
   puts "ERROR in Edit_mode:\n$err"
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer _Un_Select_for_Operande_1 {infos} {return [this _Un_Select_for_Operande_N $infos 1]}
method ALX_Drawer _Un_Select_for_Operande_2 {infos} {return [this _Un_Select_for_Operande_N $infos 2]}

#_____________________________________________________________________________________________________
method ALX_Drawer _Un_Select_for_Operande_N {infos N} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   set var ${objName}_selected_as_operande_$N
   if {[$node Val_MetaData $var] == ""} {
     $node Ajouter_MetaData_T $var 1
	 this Select_Operande_N_L_nodes $N $node
	 lappend this(L_nodes_Operande_$N) $node
	} else {$node Retirer_MetaData_T $var
	        set this(L_nodes_Operande_$N) [lremove $this(L_nodes_Operande_$N) $node]
           }
  }
 puts "L_op_1 : {$this(L_nodes_Operande_1)}"
 puts "L_op_2 : {$this(L_nodes_Operande_2)}"
}

#_____________________________________________________________________________________________________
method ALX_Drawer Apply_operation_on_nodes {} {
 foreach n_op1 $this(L_nodes_Operande_1) {
   foreach n_op2 $this(L_nodes_Operande_2) {
     $n_op1 $this(node_operation) $n_op2
    }
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Unselect_Operande_N {N} {
 this Unselect_Operande_N_L_nodes $N $this(L_nodes_Operande_$N)
}

#_____________________________________________________________________________________________________
method ALX_Drawer Select_Operande_N_L_nodes {N L_nodes} {
 if {$N == 1} {
   foreach node $L_nodes {
     $node Mode_texture 1
	 $node Couleur_masque_texture 0.5 0.5 0.5 0.85
	 if {[lsearch $this(L_nodes_Operande_1) $node] == -1} {lappend this(L_nodes_Operande_1) $node}
	} 
  } else {foreach node $L_nodes {
            $node Mode_texture 1
			$node Couleur_masque_texture 0.5 0.5 0.5 0.5
			if {[lsearch $this(L_nodes_Operande_2) $node] == -1} {lappend this(L_nodes_Operande_2) $node}
		   }
         }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Unselect_Operande_N_L_nodes {N L_nodes} {
 foreach node $L_nodes {
   $node Mode_texture 0
   $node Couleur_masque_texture 1 1 1 1
   set this(L_nodes_Operande_1) [lremove $this(L_nodes_Operande_1) $node]
   set this(L_nodes_Operande_2) [lremove $this(L_nodes_Operande_2) $node]
  } 
}

#_____________________________________________________________________________________________________
method ALX_Drawer Apply_Combi {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   $node Combinaison_$this(combi_mode)_$this(combi_tex) [GLenum2UI [this get_combi_value]]
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Color {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   eval "$node Couleur $this(current_color)"
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Destroy {node rap ptr} {
 puts "$objName Destroy $node"
 if {[$node Val_MetaData Under_destruction] == 1} {return}
 $node Ajouter_MetaData_T Under_destruction 1
 puts Gogo_destroy; 
 after 1000 "puts -nonewline Destruction; Detruire $node; puts { done}; puts -nonewline {B_rappel release}; $objName Release_B_rappel $rap; puts { done}"

 puts "$ptr desabonner_de_changement \[$rap Rappel\]"
 $ptr desabonner_de_changement [$rap Rappel]
 puts "Destruction triggered"
}

#_____________________________________________________________________________________________________
method ALX_Drawer Delete {infos} {
 puts "$objName Delete"
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   set ptr [$infos Ptr]
   $node Gerer_contacts 0; $node Afficher_noeud 0; ctc_$node dispose
   set rap [this Get_a_B_rappel]; $rap Texte "if {\[$ptr Appuie\] == 0} {$objName Destroy $node $rap $ptr}"
   $ptr abonner_a_changement [$rap Rappel]
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Copy {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   if {$this(clipboard) == ""} {set cc [B_cc]} else {set cc [lindex $this(clipboard) 0]}
   $cc = [$node Serialiser_type]
   $this(pt_tmp) maj [$infos X_au_contact] [$infos Y_au_contact]
   #$node Changer_coordonnees $this(pt_tmp)
   set this(clipboard) [list $cc [$this(pt_tmp) X] [$this(pt_tmp) Y] [$node Val_MetaData Texture_1_image_file_name] [$node Val_MetaData Texture_2_image_file_name]]
   puts "$objName Copy of $node done..."
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Paste {infos} {
 #puts "$objName Paste"
 if {$this(clipboard) != ""} {
   set node [Real_class [$infos NOEUD]]
   puts "\n   __________\nOriginal contact on $node at <[$infos X_au_contact] ; [$infos Y_au_contact]>"
   set new_node [B_polygone]
     $new_node Deserialiser_type [lindex $this(clipboard) 0]
	 set x_copied [lindex $this(clipboard) 1]; set y_copied [lindex $this(clipboard) 2]
	# Express the infos contact point with respect to the canvas
	 $this(pt_tmp) maj [$infos Point_au_contact];
	 set L_reperes [this Get_a_Liste_alx_repere2D];	 
	 set L_rep_tmp [this Get_a_Liste_alx_repere2D]; 
	 
	 Lister_partie_noeud [$infos L_REPERES] $this(canvas) $node $L_rep_tmp
	   Ajouter_noeud_en_fin_de_liste_rep $L_rep_tmp $node
	   Lister_partie_index $L_rep_tmp 1 9999 $L_reperes
	 puts "L_reperes : $L_reperes\nL_reperes.Taille() == [$L_reperes Taille] / [[$infos L_REPERES] Taille] in infos"
	 Repere_transformation_inverse $this(pt_tmp) $L_reperes
	 set x_canvas [$this(pt_tmp) X]; set y_canvas [$this(pt_tmp) Y]
	 puts "Pasted point on $node at <$x_canvas ; $y_canvas>"
	 
	 this Release_Liste_alx_repere2D $L_reperes
	 this Release_Liste_alx_repere2D $L_rep_tmp
	 
	# Replace the Pasted polygone
	 $new_node Origine $x_canvas $y_canvas
	 $new_node Translation_interne [expr -$x_copied*[$new_node Ex]] [expr -$y_copied*[$new_node Ey]]
	 
	# Copy Textures informations
	 set tmp $this(current_image_1); set this(current_image_1) [lindex $this(clipboard) 3]; this Get_set_texture_1_node $new_node; set this(current_image_1) $tmp
	 set tmp $this(current_image_2); set this(current_image_2) [lindex $this(clipboard) 4]; this Get_set_texture_2_node $new_node; set this(current_image_2) $tmp

   this Register_drawed_polygone $new_node
  } 
}

#_____________________________________________________________________________________________________
#_________________________________________ Texture Edition ___________________________________________
#_____________________________________________________________________________________________________
method ALX_Drawer Fit_into {mother daughter} {
 $mother   Calculer_boites; set boite_mother   [$mother   Boite_noeud]
 $daughter Calculer_boites; set boite_daughter [$daughter Boite_noeud]
 set ex [expr [$boite_mother Tx]/[$boite_daughter Tx]]
 set ey [expr [$boite_mother Ty]/[$boite_daughter Ty]]
 $daughter Origine   [$boite_mother BG_X] [$boite_mother BG_Y]
 $daughter Rotation  0 
 $daughter Etirement $ex $ey
}

#_____________________________________________________________________________________________________
method ALX_Drawer Enable_img_texture {img} {
 $img Gerer_contacts 1
 $img Afficher_noeud 1
 $img Ne_pas_pre_rendre 0
}

#_____________________________________________________________________________________________________
method ALX_Drawer Disable_img_texture {img} {
 $img Gerer_contacts 0
 $img Afficher_noeud 0
 $img Ne_pas_pre_rendre 1
}

#_____________________________________________________________________________________________________
method ALX_Drawer Has_moved {n} {
 set e 0.00001
 set px [$n Px]; set py [$n Py]
 set r  [$n Rotation]
 set ex [$n Ex]; set ey [$n Ey]
 set coords [$n Val_MetaData prev_coords]
 if {$coords == ""} {$n Ajouter_MetaData_T prev_coords "$px $py $r $ex $ey"; return 1}
 
 if { abs([lindex $coords 0] - $px) > $e \
    ||abs([lindex $coords 1] - $py) > $e \
	||abs([lindex $coords 2] - $r ) > $e \
	||abs([lindex $coords 3] - $ex) > $e \
	||abs([lindex $coords 4] - $ey) > $e } {$n Ajouter_MetaData_T prev_coords "$px $py $r $ex $ey"; return 1} else {return 0}
}

#_____________________________________________________________________________________________________
method ALX_Drawer Remap {poly nb img} {
 set tab_texture [$img Val_MetaData tab_texture]; set tab [$tab_texture Tab]
 set boite [$poly Boite_noeud]
# Convert coordinates of the poly bounding box into the space of the image
 $tab_texture Ajouter 0 [$boite BG_X] [$boite BG_Y] [$boite HD_X] [$boite BG_Y]
 $tab_texture Ajouter 4 [$boite HD_X] [$boite HD_Y] [$boite BG_X] [$boite HD_Y]
 $img Changer_coordonnees $tab 4
   set info_texture [$img Info_texture];
   set L [TabDouble2list $tab_texture];  set nL ""; set tx [$info_texture Tx].0; set ty [$info_texture Ty].0
   foreach {x y} $L {
     lappend nL [expr $x/$tx] [expr $y/$ty]
    }
   ProcTabDouble $nL $tab_texture
   set tab [$tab_texture Tab]
 $poly Coords_textures$nb $tab
 $poly maj_tess
}

#_____________________________________________________________________________________________________
method ALX_Drawer Remap_texture {rap} {
 set img  [Void_vers_noeud [$rap Param]]
 set img  [Real_class $img]
 set L_infos_mapping [$img Val_MetaData L_infos_mapping]

 if {[this Has_moved $img]} {this Remap [lindex $L_infos_mapping 0] [lindex $L_infos_mapping 1] $img}
}

#_____________________________________________________________________________________________________
method ALX_Drawer Process_L_taptap_events {L_events_name ms dt} {
 upvar $L_events_name L_events
 set   new_L_events   ""
 
# Eliminate old events
 foreach e $L_events {
   set t [lindex $e 2]
   if {($ms - $t) <= $dt} {lappend new_L_events $e}
  }
 set L_events $new_L_events
 
# Is there a pattern Ptr1.press Ptr1.release Ptr2.press Ptr2.release ?
 set L_press  ""
 set nb_clics 0
 foreach e $L_events {
   if {[lindex $e 1] == "PRESS"} {lappend L_press [lindex $e 0]
                                 } else {if {[lsearch $L_press [lindex $e 0]] != -1} {incr nb_clics
								                                                      set  L_press  ""
																		             }
								        }
  }
  
 return [expr $nb_clics >= 2]
}

#_____________________________________________________________________________________________________
# Close if double tap on almost the same point
method ALX_Drawer Event_related_to_taptap {rap} {
 set infos [this get_info_from_rap $rap]
 set ptr   [$infos Ptr]
 set evt   [B_sim_couche Evt_courant]
 set node  [$infos NOEUD] 
 if {[$evt Type_evt] == [ALX_pointeur_enfonce]} {
   set L_events [$node Val_MetaData L_taptap_events]
   lappend L_events [list $ptr PRESS [N_i_mere ms] [$infos X_au_contact] [$infos Y_au_contact]]
   $node Ajouter_MetaData_T L_taptap_events $L_events
  } else {if {[$evt Type_evt] == [ALX_pointeur_relache]} {
            set L_events [$node Val_MetaData L_taptap_events]
			set ms       [N_i_mere ms]
			lappend L_events [list $ptr RELEASE $ms [$infos X_au_contact] [$infos Y_au_contact]]
			if {[this Process_L_taptap_events L_events $ms 500]} {
			  this Disable_img_texture $node
			 }
			$node Ajouter_MetaData_T L_taptap_events $L_events
           }
         }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Subscribe_to_taptap_to_close {node} {
 puts "$objName Subscribe_to_taptap_to_close $node"
 $node abonner_a_detection_pointeur [$this(rap_taptap) Rappel] [expr [ALX_pointeur_enfonce] + [ALX_pointeur_relache]]
 $node Ajouter_MetaData_T rap_taptap_to_close $this(rap_taptap)
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_set_texture_1 {infos} {
 puts "$objName Get_set_texture_1 $infos"
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   this Get_set_texture_1_node $node
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_set_texture_1_node {node} {
   if {$this(current_image_1) == ""} {return}
   set img_texture_1 [$node Val_MetaData img_texture_1]
   if {$img_texture_1 == ""} {
     set img_texture_1 [B_image $this(current_image_1)]; $node Ajouter_MetaData_T Texture_1_image_file_name $this(current_image_1)
	   $img_texture_1 Afficher_boite_noeud 1
	   $node Ajouter_fils $img_texture_1; 
	   this Disable_img_texture $img_texture_1 
	   set resizer_BG [B_polygone]; $resizer_BG Ajouter_contour [ProcRect -20 -20 20 20]; $resizer_BG Retirer_contour [ProcRect 0 0 20 20]
	     $img_texture_1 Ajouter_fils $resizer_BG
	   set resizer_HD [B_polygone]; $resizer_HD Ajouter_contour [ProcRect -20 -20 20 20]; $resizer_HD Retirer_contour [ProcRect -20 -20 0 0]
	     $img_texture_1 Ajouter_fils $resizer_HD
		 $resizer_HD Origine [$img_texture_1 Lg] [$img_texture_1 Ht]
	   $img_texture_1 Calculer_boites; B_contact ctc_$img_texture_1 "$img_texture_1 11" -add "$resizer_BG 9" -add "$resizer_HD 9"
	 $node Ajouter_MetaData_T img_texture_1 $img_texture_1
	 this Fit_into $node $img_texture_1; this Subscribe_to_taptap_to_close $img_texture_1
	 $img_texture_1 Ajouter_MetaData_T L_infos_mapping [list $node ""]
	 $img_texture_1 Translucidite 1; $img_texture_1 Couleur 3 0.75
	 $img_texture_1 abonner_a_LR_parcours [$img_texture_1 LR_Av_pre_rendu] [$this(rap_mapping) Rappel]
	 set tab_texture [B_TabDouble]; $tab_texture Reserver_place 8; $img_texture_1 Ajouter_MetaData_T tab_texture $tab_texture
    }
   $node Info_texture [$img_texture_1 Info_texture]
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_set_texture_2 {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   this Get_set_texture_2_node $node
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_set_texture_2_node {node} {
   if {$this(current_image_2) == ""} {return}
   set img_texture_2 [$node Val_MetaData img_texture_2]
   if {$img_texture_2 == ""} {
     set img_texture_2 [B_image $this(current_image_2)]; $node Ajouter_MetaData_T Texture_2_image_file_name $this(current_image_2)
	   $img_texture_2 Afficher_boite_noeud 1
	   $node Ajouter_fils $img_texture_2; 
	   this Disable_img_texture $img_texture_2 
	   set resizer_BG [B_polygone]; $resizer_BG Ajouter_contour [ProcRect -20 -20 20 20]; $resizer_BG Retirer_contour [ProcRect 0 0 20 20]
	     $img_texture_2 Ajouter_fils $resizer_BG
	   set resizer_HD [B_polygone]; $resizer_HD Ajouter_contour [ProcRect -20 -20 20 20]; $resizer_HD Retirer_contour [ProcRect -20 -20 0 0]
	     $img_texture_2 Ajouter_fils $resizer_HD
		 $resizer_HD Origine [$img_texture_2 Lg] [$img_texture_2 Ht]
	   $img_texture_2 Calculer_boites; B_contact ctc_$img_texture_2 "$img_texture_2 11" -add "$resizer_BG 9" -add "$resizer_HD 9"
	 $node Ajouter_MetaData_T img_texture_2 $img_texture_2
	 this Fit_into $node $img_texture_2; this Subscribe_to_taptap_to_close $img_texture_2
	 $img_texture_2 Ajouter_MetaData_T L_infos_mapping [list $node 2]
	 $img_texture_2 Translucidite 1; $img_texture_2 Couleur 3 0.75
	 $img_texture_2 abonner_a_LR_parcours [$img_texture_2 LR_Av_pre_rendu] [$this(rap_mapping) Rappel]
	 set tab_texture [B_TabDouble]; $tab_texture Reserver_place 8; $img_texture_2 Ajouter_MetaData_T tab_texture $tab_texture
    }
   $node Info_texture2 [$img_texture_2 Info_texture]
}

#_____________________________________________________________________________________________________
method ALX_Drawer Edit_texture_1 {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   set img_texture_1 [$node Val_MetaData img_texture_1]
   if {$img_texture_1 != ""} {
     this Enable_img_texture $img_texture_1
    }
  }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Edit_texture_2 {infos} {
 set node [Real_class [$infos NOEUD]]
 if {$node != $this(canvas)} {
   set img_texture_2 [$node Val_MetaData img_texture_2]
   if {$img_texture_2 != ""} {
     this Enable_img_texture $img_texture_2
    }
  }
}

#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method ALX_Drawer Drawing_mode {infos} {
 set ptr [$infos Ptr]
 set this(ptr,$ptr) $ptr

# Set up callbacks
 set this(rap,$ptr) [this Get_a_B_rappel "$objName Pointer_has_changed $ptr"]
   $ptr abonner_a_changement [$this(rap,$ptr) Rappel]
   
# Set up a new polygone  
 set this(pol,$ptr)  [this Get_a_polygone]
   eval "$this(pol,$ptr) Couleur $this(current_color)"
   this Register_drawed_polygone $this(pol,$ptr)
   
 set this(Lrep,$ptr) [this Get_a_Liste_alx_repere2D]
   $this(Lrep,$ptr) maj [$infos L_REPERES]
 set this(L_pt,$ptr) [list [$infos X_au_contact] [$infos Y_au_contact]]
 
# Keep trace of the drawing mode
 set this(draw_mode,$ptr) $this(drawing_mode)
}

#_____________________________________________________________________________________________________
method ALX_Drawer Register_drawed_polygone {p} {
 B_contact ctc_$p "$p 11"
 if {![$this(canvas) A_pour_fils $p]} {$this(canvas) Ajouter_fils_au_debut $p}
 $p Ajouter_MetaData_T Draw_in_$objName 1
}

#_____________________________________________________________________________________________________
method ALX_Drawer Pointer_has_changed {ptr} {
 if {[$ptr Appuie] == 0} {
   this Stop_Drawing $ptr
  } else {this Drawing $ptr
          $this(pol,$ptr) Calculer_boites
		  ctc_$this(pol,$ptr) ref "$this(pol,$ptr) 11"
         }
}

#_____________________________________________________________________________________________________
method ALX_Drawer Stop_Drawing {ptr} {
# this Apply_Drawing    $ptr
 this Forget_about_ptr $ptr
}

#_____________________________________________________________________________________________________
method ALX_Drawer Forget_about_ptr {ptr} {
 $ptr desabonner_de_changement [$this(rap,$ptr) Rappel]
 unset this(ptr,$ptr)
 this Release_Liste_alx_repere2D $this(Lrep,$ptr) 
   unset this(Lrep,$ptr)
 unset this(L_pt,$ptr)
}

#_____________________________________________________________________________________________________
method ALX_Drawer Drawing {ptr} {
 $this(pt_tmp) maj [$ptr X] [$ptr Y]
   Repere_transformation $this(pt_tmp) $this(Lrep,$ptr)
   this $this(draw_mode,$ptr) $ptr [$this(pt_tmp) X] [$this(pt_tmp) Y]
   #puts "([$ptr X] [$ptr Y]) => ([$this(pt_tmp) X] [$this(pt_tmp) Y])"
}

#_____________________________________________________________________________________________________
method ALX_Drawer Cancel {} {
 set L [array get this ptr,*]
 foreach {i ptr} $L {
   this Release_a_polygone $this(pol,$ptr)
   this Forget_about_ptr $ptr
  }
}


#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#______________________________________________ Drawing ______________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method ALX_Drawer FreeHand {ptr x y} {
 lappend this(L_pt,$ptr) $x $y
 $this(pol,$ptr) Vider
 set contour [ProcTabDouble $this(L_pt,$ptr)]
   $this(pol,$ptr) Ajouter_contour $contour
 Detruire $contour
}

#_____________________________________________________________________________________________________
method ALX_Drawer Rectangle {ptr x y} {
 $this(pol,$ptr) Vider
 set contour [eval ProcRect "$this(L_pt,$ptr) $x $y"]
   $this(pol,$ptr) Ajouter_contour $contour
 Detruire $contour
}

#_____________________________________________________________________________________________________
method ALX_Drawer Oval {ptr x y} {
 $this(pol,$ptr) Vider
 set x1 [lindex $this(L_pt,$ptr) 0]; set y1 [lindex $this(L_pt,$ptr) 1]
 set cx [expr ($x+$x1)/2.0]        ; set cy [expr ($y+$y1)/2.0]; 
 set tx [expr abs($x-$x1)/2.0]     ; set ty [expr abs($y-$y1)/2.0]

 set contour [ProcOvale $cx $cy $tx $ty 60]
   $this(pol,$ptr) Ajouter_contour $contour
 Detruire $contour
}

#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
#_________________________________________ Technical stuff ___________________________________________
#_____________________________________________________________________________________________________
#_____________________________________________________________________________________________________
method ALX_Drawer Release_Liste_alx_repere2D {l} {
 lappend this(Pool_Liste_alx_repere2D)  $l
 set this(L_Liste_alx_repere2D) [lremove $this(L_Liste_alx_repere2D) $l]
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_a_Liste_alx_repere2D {} {
 if {[llength $this(Pool_Liste_alx_repere2D)]} {
   set rep [lindex $this(Pool_Liste_alx_repere2D) 0]
   set this(Pool_Liste_alx_repere2D) [lrange $this(Pool_Liste_alx_repere2D) 1 end]
  } else {set rep [Liste_alx_repere2D]}
  
 lappend $this(L_Liste_alx_repere2D) $rep
 return $rep
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_a_polygone {} {
 if {[llength $this(Pool_polygones)]} {
   set rep [lindex $this(Pool_polygones) 0]
   set this(Pool_polygones) [lrange $this(Pool_polygones) 1 end]
  } else {set rep [B_polygone]}
 
 $rep Combinaison_couleurs_source_arg2 [GLenum2UI [GL_texture]]
 $rep Combinaison_couleurs_operande2   [GLenum2UI [GL_src_color]]
 $rep Combinaison_textures_source_arg2 [GLenum2UI [GL_texture]]
 $rep Combinaison_textures_operande2   [GLenum2UI [GL_src_alpha]]
 $rep Combinaison_textures_alpha       [GLenum2UI [GL_add]]
 $rep Combinaison_textures_rgb         [GLenum2UI [GL_interpolate_ARB]]
 
 lappend this(L_polygones) $rep
 $rep Couleur 1 1 1 1
 return $rep
}

#_____________________________________________________________________________________________________
method ALX_Drawer Release_a_polygone {p} {
 B_configure $p -Vider_peres -Vider
 lappend this(Pool_polygones) $p
 set this(L_polygones) [lremove $this(L_polygones) $p]
}

#_____________________________________________________________________________________________________
method ALX_Drawer Get_a_B_rappel {{txt ""}} {
 if {[llength $this(Pool_B_rappel)]} {
   set rep [lindex $this(Pool_B_rappel) 0]
   set this(Pool_B_rappel) [lrange $this(Pool_B_rappel) 1 end]
  } else {set rep [B_rappel [Interp_TCL]]}
 
 $rep Texte $txt
 return $rep
}

#_____________________________________________________________________________________________________
method ALX_Drawer Release_B_rappel {r} {
 $r Texte ""
 lappend this(Pool_B_rappel) $r
}

